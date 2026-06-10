/******************************************************************************
 * ApkDownloader.cpp
 * 应用内 APK 下载器实现
 *
 * 本文件实现从网络下载 APK 文件并在 Android 设备上安装的完整流程。
 * 包含一个重要的竞态条件修复（readyRead + finished 信号问题）。
 *
 * 核心调用链：
 *   startDownload("https://.../app.apk")
 *     → QNetworkAccessManager::get(request)      // 发起 HTTP GET 请求
 *     → 下载进度中：onDownloadProgress() 更新进度条
 *     → 下载完成：onFinished() 一次性读取数据写入文件
 *     → installApk() 通过 JNI 调用 Android 系统安装器
 ******************************************************************************/

#include "ApkDownloader.h"
#include <QNetworkRequest>       // QNetworkRequest：构造 HTTP 请求头、URL 等
#include <QUrl>                  // QUrl：解析和构造 URL，处理特殊字符编码
#include <QStandardPaths>        // QStandardPaths：获取系统标准目录路径
#include <QDir>                  // QDir：目录操作（创建、遍历、判断存在性）
#include <QFileInfo>             // QFileInfo：获取文件元信息（大小、类型、修改时间等）
#include <QDebug>                // QDebug：qDebug() 输出调试信息到控制台
#include <QThread>               // QThread：线程操作，这里用 QThread::msleep() 短暂延时
#include <QJniObject>            // QJniObject：Qt 封装的 JNI 调用工具，Java Native Interface

// =============================================================================
// 辅助函数：将字节数格式化为人类可读的文件大小字符串
// =============================================================================

/**
 * @brief 格式化文件大小
 * @param size 字节数，qint64 是有符号 64 位整数（最大 9.22EB）
 * @return 格式化后的字符串，如 "1.23 MB"
 *
 * 为什么用 qint64？
 *   QNetworkReply::downloadProgress() 的信号参数类型就是 qint64，
 *   所以这里也用同样的类型，避免类型转换。
 *   qint64 的极值：9,223,372,036,854,775,807（约 9.22 × 10¹⁸ 字节）
 *   对于 APK 文件（通常 1MB~500MB）完全够用。
 *
 * 为什么用 static 函数？
 *   static 函数只在当前编译单元（.cpp 文件）内可见，
 *   不会污染命名空间，也不需要声明在头文件中。
 *   这是 C++ 文件内部工具函数的经典写法。
 */
static QString formatFileSize(qint64 size)
{
    // 小于 1KB（1024 字节）的场景
    // 1024 = 2^10，计算机二进制计数单位
    // 此处用 < 1024 而非 <= 1024，严谨的边界处理
    if (size < 1024)
        // QString::arg() 是 Qt 的字符串格式化方法
        // "%1" 是占位符，会被 arg() 的参数替换
        // 例如：size = 500 → "500 B"
        return QString("%1 B").arg(size);

    // 大于等于 1KB 但小于 1MB（1024KB）的场景
    else if (size < 1024 * 1024)
        // QString::arg(number, fieldWidth, format, precision)
        //   参数 2："fieldWidth" = 0，不指定最小宽度
        //   参数 3："f" = 定点数格式（fixed-point）
        //   参数 4：2 = 保留两位小数
        // 为什么用 1024.0 不是 1024？
        //   如果写 1024，整数除法就会截断小数部分
        //   写成 1024.0 触发浮点除法，得到 12.34 而不是 12
        return QString("%1 KB").arg(size / 1024.0, 0, 'f', 2);

    // 大于等于 1MB 的场景
    else
        // 1024.0 * 1024.0 = 1048576.0，将字节转为兆字节
        // 类似的计算模式可以在 Windows 文件属性、Chrome 下载管理器中看到
        return QString("%1 MB").arg(size / (1024.0 * 1024.0), 0, 'f', 2);
}

// =============================================================================
// 构造函数
// =============================================================================

/**
 * @brief 构造函数
 * @param parent 父窗口指针，QWidget* 类型——注意不是 QObject*
 *
 * 为什么参数是 QWidget* 而不是 QObject*？
 *   QProgressDialog 的构造函数需要 QWidget* 作为父窗口，
 *   用于对话框居中显示和模态属性。
 *   如果传 nullptr，进度对话框会在屏幕正中央弹出。
 */
ApkDownloader::ApkDownloader(QWidget *parent)
    // 初始化列表（Initializer List）：调用父类构造函数并初始化成员变量
    // 初始化列表在构造函数体执行之前完成，效率比在函数体内赋值更高
    : QObject(parent)                                  // 调用 QObject(parent) 将 parent 作为父对象
    , networkManager(new QNetworkAccessManager(this))  // 创建网络管理器，并将 this 设为父对象
    , currentReply(nullptr)                            // 初始化指针为 nullptr，表示尚未发起请求
    , apkFile(nullptr)                                 // 初始化指针为 nullptr，表示尚未打开文件
    , progressDialog(nullptr)                          // 初始化指针为 nullptr，后面再 new
    , parentWidget(parent)                             // 保存父窗口指针，供后续创建对话框和使用
{
    // ==============================================
    // 创建统一风格进度对话框
    // ==============================================

    /**
     * 创建 StyledProgressDialog 替代旧的 QProgressDialog。
     * StyledProgressDialog 采用与 UpdateDialog 一致的视觉风格：
     *   - 无边框圆角窗口
     *   - 圆角白色背景 + 顶部渐变装饰条
     *   - QProgressBar 进度条
     *   - 取消按钮
     *   - 窗口固定（不可拖动）
     */
    progressDialog = new StyledProgressDialog(parentWidget);

    /**
     * setProgressText() 设置进度描述文字
     * 下载开始前显示"正在下载新版本..."，
     * 下载过程中会被 onDownloadProgress 更新为"已下载 5MB / 50MB"等
     */
    progressDialog->setProgressText("正在下载新版本...");

    /**
     * setProgress(0) 设置进度条的初始位置
     * 为什么要设置为 0？
     *   刚创建时，StyledProgressDialog 的默认值是 0，
     *   但显式写出来是为了强调：还没开始下载，进度为 0%
     *   如果忘记了 setProgress(0)，恰巧之前有其他地方用过这个对话框，
     *   它的内部值可能还停留在上一次的数值（比如 100）
     *   这就导致了"一打开对话框就显示 100% 完成"的 BUG
     */
    progressDialog->setProgress(0);

    // ==============================================
    // 连接进度对话框的"取消"信号
    // ==============================================

    /**
     * 连接 StyledProgressDialog::canceled 信号到 Lambda 表达式
     * 当用户点击"取消"按钮时，StyledProgressDialog 发射 canceled() 信号
     *
     * Lambda 表达式 [this]() { ... } 详解：
     *   [this]   ：捕获列表（Capture List），捕获 this 指针供 Lambda 内部使用
     *              如果不捕获 this，Lambda 内部无法访问 currentReply 等成员变量
     *   ()       ：参数列表，这里没有参数因为 canceled() 信号不带参数
     *   { ... }  ：函数体
     *
     * currentReply->abort() 的作用：
     *   abort() 中断当前网络请求，并触发 QNetworkReply::finished 信号
     *   在 onFinished() 中我们会检测 QNetworkReply::OperationCanceledError，
     *   然后不会安装 APK，只做清理工作。
     *
     * 为什么要在 Lambda 里判断 if (currentReply)？
     *   currentReply 是 QNetworkReply* 指针，可能为 nullptr 的情况：
     *     1. 还没调用过 startDownload()，用户就点了取消（虽然不可能）
     *     2. 网络请求已经完成，currentReply 已被置为 nullptr
     *   对 nullptr 调用 abort() 会导致程序崩溃（Segmentation Fault）
     *   所以每次使用指针前必须判空
     */
    connect(progressDialog, &StyledProgressDialog::canceled, this, [this]() {
        // 防止重复 abort：只有 currentReply 非空时才调用 abort
        if (currentReply)
            currentReply->abort();  // abort() 内部会断开连接并触发 finished 信号
    });
}

// =============================================================================
// 析构函数
// =============================================================================

/**
 * @brief 析构函数
 *
 * 析构函数在对象被 delete 时自动调用。
 * 负责清理所有动态分配的资源，防止内存泄漏。
 *
 * 为什么需要手动清理？
 *   C++ 没有垃圾回收（GC）机制，new 出来的对象必须手动 delete。
 *   虽然 Qt 的父子对象机制会在一级一级销毁时自动清理子对象，
 *   但有些资源（如文件、回复）需要在析构前立即释放，不能等到事件循环处理。
 */
ApkDownloader::~ApkDownloader()
{
    // 如果还有正在进行的网络请求，立即中断
    if (currentReply) {
        currentReply->abort();          // 中断请求，不等待服务器响应
        currentReply->deleteLater();    // 安全删除，延迟到事件循环处理
    }

    // 如果还有打开的文件，立即关闭
    if (apkFile) {
        apkFile->close();               // close() 会刷出缓冲区数据到磁盘
        delete apkFile;                  // 释放文件对象内存
        // 注意：我们没有 delete apkFile 后置 nullptr
        // 因为析构函数执行完后对象就不存在了，不需要再置空
    }

    // 关闭进度对话框（如果还开着）
    if (progressDialog)
        progressDialog->close();         // close() 会触发 WA_DeleteOnClose 自动删除
}

// =============================================================================
// 公共方法：开始下载 APK
// =============================================================================

void ApkDownloader::startDownload(const QString& url)
{
    // ==============================================
    // 第一步：参数校验
    // ==============================================

    /**
     * url.isEmpty() 判断字符串是否为空
     * QString::isEmpty() 与 isNull() 的区别：
     *   isEmpty()：长度为 0 的字符串返回 true，包括空字符串 "" 和 null 字符串
     *   isNull() ：仅 null 字符串返回 true，空字符串 "" 返回 false
     *   对于 QString，"" 和 QString() 的区别：
     *     QString()        → isNull()=true,  isEmpty()=true
     *     QString("")      → isNull()=false, isEmpty()=true
     *   这里用 isEmpty() 更安全，因为两种空字符串都能检测到
     */
    if (url.isEmpty()) {
        // emit 发射信号，通知调用方下载失败
        // 信号的参数 downloadFailed 的调用方会收到这个错误字符串
        emit downloadFailed("下载链接为空");
        return;  // return 立即返回，不执行后续代码
    }

    // ==============================================
    // 第二步：确定文件保存路径
    // ==============================================

    /**
     * QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
     * 获取应用程序的数据目录路径。
     *
     * 各平台的实际路径：
     *   Android  → /data/data/<package_name>/files  （对应 getFilesDir()）
     *   Windows  → C:/Users/<用户名>/AppData/Local/<应用名>
     *   Linux    → ~/.local/share/<应用名>
     *   macOS    → ~/Library/Application Support/<应用名>
     *
     * 为什么选 AppDataLocation 而不是其他位置？
     *   QStandardPaths 有多个标准路径：
     *     - TempLocation      ：临时目录，内容可能随时被系统清理
     *     - CacheLocation     ：缓存目录，系统可能在空间不足时自动删除
     *     - AppDataLocation   ：持久化数据目录，不会自动删除
     *   APK 文件下载后需要在安装完成前保持存在，
     *   所以必须放在持久化目录中。
     *
     * 为什么 Android 上要用这个路径？
     *   因为 qtprovider_paths.xml 中配置的 <files-path> 正是覆盖这个目录。
     *   FileProvider 可以生成这个目录下文件的 content:// URI，
     *   从而让系统安装器能够访问到这个 APK 文件。
     *   如果放在其他目录（如 CacheLocation），FileProvider 无法访问，
     *   安装时会报"解析包时出现问题"。
     */
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // QDir 是目录操作类，相当于一个"目录对象"
    QDir dir(dataDir);

    /**
     * dir.exists() 判断目录是否存在
     * !dir.exists() 在目录不存在时进入 if 分支
     */
    if (!dir.exists())
        /**
         * dir.mkpath(".") 递归创建目录
         * 参数 "." 表示创建 dataDir 路径上的所有不存在的目录
         * 例如：/data/data/com.example/files 中 files 不存在，
         *       它会把 data、com.example、files 全部创建出来
         * 为什么不用 mkdir？
         *   mkdir(".")         → 只创建 dataDir 这一级目录，父目录必须存在
         *   mkpath(".")        → 递归创建整个路径的目录，父目录不存在也能创建
         * 用 mkpath 更安全，即使父目录被意外删除也能恢复
         */
        dir.mkpath(".");

    /**
     * 拼接完整的 APK 文件路径
     * dataDir + "/update.apk" 就是类似：
     *   /data/data/com.example.calculationtools/files/update.apk
     *
     * 为什么文件名固定为 update.apk？
     *   每次下载都会覆盖旧文件，用固定文件名省去管理多个版本文件的麻烦。
     *   如果下载失败，下次重试时先删除旧文件再下载。
     */
    apkFilePath = dataDir + "/update.apk";

    // ==============================================
    // 第三步：删除旧的 APK 文件
    // ==============================================

    // QFile::exists() 是静态方法，不需要创建 QFile 对象即可判断
    // 如果上次下载留下了一个损坏或不完整的文件，先删除
    if (QFile::exists(apkFilePath))
        // QFile::remove() 也是静态方法，直接删除文件
        // 删除失败时返回 false，但这里我们忽略失败——后面打开文件时会覆盖检查
        QFile::remove(apkFilePath);

    // ==============================================
    // 第四步：创建并打开本地文件
    // ==============================================

    /**
     * new QFile(apkFilePath) 创建文件对象
     * 注意这里用了 new（堆分配），而不是栈对象
     * 原因：这个文件需要在 startDownload() 返回后仍然存在，
     *       直到 onFinished() 使用完才关闭。
     *       如果定义为栈局部变量，函数返回后文件对象就销毁了。
     * 所以用成员变量 QFile* apkFile 来持有文件对象的生命周期。
     */
    apkFile = new QFile(apkFilePath);

    /**
     * apkFile->open(QIODevice::WriteOnly) 以只写模式打开文件
     * QIODevice::WriteOnly 是打开模式枚举值：
     *   ReadOnly    ：只读，不能写入
     *   WriteOnly   ：只写，会截断（truncate）已有内容
     *   ReadWrite   ：读写
     *   Append      ：追加写入，保留已有内容
     *   Truncate    ：截断文件到 0 字节
     *   等等，可以用按位或组合：QIODevice::WriteOnly | QIODevice::Truncate
     *
     * WriteOnly 模式下，如果文件已存在，默认会清空文件内容（truncate 行为）。
     * 因为我们前面删除了旧文件，这里打开的是一个全新的空文件。
     *
     * 返回值：
     *   true  ：打开成功
     *   false ：打开失败（权限不足、磁盘满、路径不存在等）
     */
    if (!apkFile->open(QIODevice::WriteOnly)) {
        // 打开失败时，先发射失败信号通知调用方
        emit downloadFailed("无法创建临时文件");
        // 释放文件对象内存，因为文件无法使用
        delete apkFile;
        // 将指针置为 nullptr，防止析构函数或其他地方误用已经 delete 的指针
        // 如果不置 nullptr，apkFile 就成了"悬空指针"（Dangling Pointer），
        // 后续对它的任何操作都会导致未定义行为（Undefined Behavior）
        apkFile = nullptr;
        return;  // 提前返回，不继续执行后续代码
    }

    // ==============================================
    // 第五步：初始化进度对话框
    // ==============================================

    /**
     * setProgress(0) 将进度条重置到 0% 位置
     * 为什么这里还要再设一次 0？
     *   构造函数中我们已经 setProgress(0) 了，看起来似乎是重复的。
     *   但是考虑这个场景：
     *     用户第一次下载 → 下载到 50% → 取消
     *     用户再次下载 → 调用 startDownload()
     *   此时 progressDialog 内部的进度值还停留在 50%。
     *   如果不重置为 0，对话框一出现就显示 50%，这是不对的。
     * 所以这里的 setProgress(0) 是为了"重置"进度，不是多余的。
     */
    progressDialog->setProgress(0);

    /**
     * setProgressText() 更新提示文字
     * 这里设为"正在下载新版本..."，提示用户当前状态。
     * 下载过程中，onDownloadProgress 会更新这个文字为具体进度。
     */
    progressDialog->setProgressText("正在下载新版本...");

    /**
     * show() 让对话框显示出来
     *
     * 为什么在这里 show 而不是在构造函数中 show？
     *   如果在构造函数中 show，对象刚创建对话框就弹出来了。
     *   但用户可能只是创建 ApkDownloader 对象，还没想好什么时候开始下载。
     *   所以只在真正开始下载时才显示。
     *
     * show() 与 setVisible(true) 等效。
     * 对于模态对话框，show() 后不会阻塞，用户可以边下载边做其他事。
     */
    progressDialog->show();

    // ==============================================
    // 第六步：构造网络请求
    // ==============================================

    /**
     * QUrl 类的作用：
     *   解析和构造 URL，处理编码问题。
     *   例如 URL 中包含中文或特殊字符时，QUrl 会自动进行百分号编码：
     *     "https://gitee.com/下载/app.apk"
     *     会被编码为 "https://gitee.com/%E4%B8%8B%E8%BD%BD/app.apk"
     *   如果不用 QUrl 而是直接传字符串，特殊字符可能导致请求失败。
     */
    QUrl downloadUrl(url);

    /**
     * QNetworkRequest 是网络请求配置类
     * 相当于一个"请求信封"，里面装着 URL、请求头、Cookie 等信息
     * 参数传 QUrl 对象，不能直接传字符串
     */
    QNetworkRequest request(downloadUrl);

    // ==============================================
    // 第七步：设置请求头，模拟真实浏览器
    // ==============================================

    /**
     * setRawHeader() 设置原始 HTTP 请求头的键值对
     *
     * 为什么需要设置 User-Agent？
     *   很多服务器会检查 User-Agent 头：
     *     - 如果 User-Agent 为空或是 Qt 默认的 "Qt/5.15.2"，可能被拒绝
     *     - 服务器可能把这些请求当作爬虫或恶意请求处理
     *   所以我们模拟 Chrome 浏览器的 User-Agent，让服务器以为是正常访问
     *
     * 注意区分 setRawHeader 和 setHeader：
     *   setHeader()     ：只支持 Qt 预定义的几个头（Content-Type、Content-Length 等）
     *   setRawHeader()  ：可以设置任意的原始 HTTP 头（包括自定义的）
     *   对于 User-Agent，两种都可以，但 setRawHeader 更通用
     */
    request.setRawHeader("User-Agent",
        "Mozilla/5.0 (Linux; Android 14; Pixel 8) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/120.0.6099.43 Mobile Safari/537.36");

    /**
     * Accept 头告诉服务器我们期望接收的响应格式
     * application/vnd.android.package-archive：APK 标准的 MIME 类型
     * application/octet-stream：通用二进制流，服务器不确定格式时常用
     * ：兜底，任何格式都可以
     * 按优先级从高到低排列，逗号分隔
     */
    request.setRawHeader("Accept",
        "application/vnd.android.package-archive, application/octet-stream, */*");

    /**
     * Accept-Language 头告诉服务器优先接收的语言
     * zh-CN：简体中文（中国大陆）
     * zh：中文通用
     * en：英语
     * q=0.9 和 q=0.8 是"质量因子"（Quality Factor），表示优先级权重
     * 取值范围 0~1，省略时默认为 1
     */
    request.setRawHeader("Accept-Language", "zh-CN,zh;q=0.9,en;q=0.8");

    /**
     * Referer 头告诉服务器请求是从哪个页面发起的
     * 有些服务器（包括 Gitee）会检查 Referer 头，
     * 防止文件被第三方网站直接引用（防盗链）。
     * 如果 Referer 不在白名单内，服务器可能返回 403 Forbidden。
     * 设为正确的 Referer 可以绕过这个检查。
     */
    request.setRawHeader("Referer", "https://gitee.com/");

    // ==============================================
    // 第八步：设置 URL 重定向策略
    // ==============================================

    /**
     * RedirectPolicyAttribute 控制 Qt 是否自动跟随 HTTP 重定向
     * NoLessSafeRedirectPolicy 的意思是：
     *   只跟随"同级或更安全的"重定向
     *   安全级别：https > http
     *   所以：
     *     https → https：跟随（安全级别相同） ✓
     *     http  → https：跟随（更安全）       ✓
     *     https → http ：不跟随（降级到不安全） ✗
     *     http  → http ：跟随（相同）          ✓
     *
     * 为什么需要自动重定向？
     *   Gitee 的 raw 下载链接可能会 302 跳转到 CDN 加速地址，
     *   如果不跟随重定向，我们只会得到 302 响应体（几乎没有内容），
     *   而不是真正的 APK 文件数据。
     *
     * 其他可选策略：
     *   ManualRedirectPolicy     ：手动处理，不自动跟随
     *   NoLessSafeRedirectPolicy ：只跟随同级别或更安全的（默认）
     *   SameOriginRedirectPolicy ：只跟随同域名重定向
     *   UserVerifiedRedirectPolicy：需要用户确认后才跟随
     */
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::NoLessSafeRedirectPolicy);

    // ==============================================
    // 第九步：发起 HTTP GET 请求
    // ==============================================

    /**
     * networkManager->get(request) 发起 GET 请求
     *
     * QNetworkAccessManager 支持的方法：
     *   get()    ：GET  请求，获取资源
     *   post()   ：POST 请求，提交数据给服务器
     *   put()    ：PUT  请求，上传资源
     *   delete() ：DELETE 请求，删除资源
     *   head()   ：HEAD 请求，只获取响应头，不获取响应体
     *
     * 返回值 QNetworkReply*：
     *   这个对象"代表"这次 HTTP 请求，后续通过它获取响应数据。
     *   QNetworkReply 继承自 QIODevice，所以可以像读文件一样读取数据。
     *
     * 异步特性（最重要）：
     *   get() 是异步的，立即返回，不会阻塞主线程。
     *   实际的数据传输在后台线程进行。
     *   完成后通过信号（finished）通知。
     *   如果使用同步（阻塞）方式，界面会卡死无法操作，
     *   在 Android 上甚至会触发 ANR（Application Not Responding）对话框。
     */
    currentReply = networkManager->get(request);

    // ==============================================
    // 第十步：连接信号槽
    // ==============================================

    /**
     * 连接 QNetworkReply::downloadProgress 信号
     *
     * 这个信号由 QNetworkReply 在接收数据时持续发射：
     *   每次从网络读取到一段数据后都会发射
     *   参数：
     *     bytesReceived：已接收的字节数（不断累加）
     *     bytesTotal   ：总字节数（根据 Content-Length 头确定）
     *                   如果服务器没有返回 Content-Length，bytesTotal = -1
     *
     * 为什么会有多个信号？
     *   下载 50MB 的文件，可能触发几千次 downloadProgress 信号。
     *   但 QProgressDialog 的 setValue 在值没有变化时不会重绘，
     *   所以不会造成界面卡顿。
     */
    connect(currentReply, &QNetworkReply::downloadProgress,
            this, &ApkDownloader::onDownloadProgress);

    /**
     * 关于 onReadyRead 的设计决策：
     *
     * 常规做法：在 readyRead 信号里增量写入
     *   connect(reply, &QNetworkIODevice::readyRead, this, &MyClass::onReadyRead);
     *   然后：file->write(reply->readAll());
     *
     * 为什么我们这里不连接 readyRead？
     *   因为存在竞态条件（Race Condition）：
     *     1. 最后一段数据到达，readyRead 被发射 → 加入事件队列
     *     2. finished 信号也被发射 → 加入事件队列（排在 readyRead 之后）
     *     3. 事件循环依次处理：先处理 readyRead，写入最后一段数据
     *     4. 然后处理 finished，在 onFinished 中 readAll() 读取全部数据
     *   以上是正常情况。
     *
     * 问题场景（某些 Android 设备上）：
     *     1. 最后一段数据到达，readyRead 准备发射
     *     2. 但此时数据已经全部到达，finished 信号也被发射
     *     3. 事件循环处理顺序被打乱，finished 先被执行
     *     4. onFinished 中 readAll() 读取到了最后一段数据（这里是对的）
     *     5. 然后 readyRead 才执行，又 readAll() 一次——但这次 readAll() 返回空
     *   这时的 APK 文件缺少最后一段数据，损坏了！
     *
     * 修复方案：
     *   不在 readyRead 中写入，所有数据在 onFinished 中通过 readAll() 一次性读入。
     *   这样 finished 无论何时触发，数据都已经完整可读了。
     *   虽然这是一个"不那么优雅"的解决方案（一次性读取大文件可能占用较多内存），
     *   但正确性比性能更重要。
     */

    /**
     * 连接 QNetworkReply::finished 信号
     *
     * finished 信号在以下任一情况发射：
     *   1. 正常下载完成（收到完整的 HTTP 响应体）
     *   2. 网络错误（连接断开、超时、DNS 解析失败等）
     *   3. 调用 abort() 中断请求
     *   4. 重定向跟随完成
     *
     * 注意：finished 只能发射一次，
     *   因为它内部的 QNetworkReplyImpl 的状态机在发射后进入 Finished 状态。
     *   再次 connect 已经在同一个状态下没有意义。
     */
    connect(currentReply, &QNetworkReply::finished,
            this, &ApkDownloader::onFinished);

    // startDownload 方法执行完毕
    // 此时请求已在后台运行，后面 onDownloadProgress 和 onFinished 会在
    // 事件循环中异步触发
}

// =============================================================================
// 私有槽函数：下载进度更新
// =============================================================================

/**
 * @brief 下载进度更新槽函数
 * @param bytesReceived 已接收的字节数（qint64 是 64 位有符号整数）
 * @param bytesTotal 总字节数（qint64，-1 表示未知）
 *
 * 这是一个 Qt 槽函数（Slot），通过信号-槽机制自动被调用。
 *
 * 信号-槽机制的底层原理：
 *   1. MOC（Meta-Object Compiler）处理 ApkDownloader.h，生成 moc_ApkDownloader.cpp
 *   2. moc 文件中包含 QMetaObject 的定义，记录类的信号和槽的索引
 *   3. connect() 在内部建立信号索引到槽索引的映射关系
 *   4. 信号被 emit 时，QMetaObject::activate() 查找映射表，调用对应的槽函数
 *   5. 整个过程默认是同步的（DirectConnection）——槽函数在发射信号的线程中立即执行
 *   6. 对于跨线程连接，Qt 使用 QueuedConnection，将调用放入事件队列异步执行
 */
void ApkDownloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    /**
     * bytesTotal > 0 的判断：
     *   如果服务器在 HTTP 响应头中包含了 Content-Length，
     *   bytesTotal 就是实际的文件大小（正数）。
     *   但有些服务器（尤其是流式传输或动态生成的内容）
     *   不返回 Content-Length，此时 bytesTotal = -1。
     *
     * 所以这里分两种情况处理：
     *   1. bytesTotal > 0：知道文件总大小，可以计算百分比
     *   2. bytesTotal = -1：不知道文件总大小，只显示已下载的量
     */
    if (bytesTotal > 0) {
        /**
         * static_cast<int>() 是 C++ 风格的类型转换
         * 将 (bytesReceived * 100) / bytesTotal 从 qint64 转为 int
         * 因为 setValue() 参数是 int 类型
         *
         * 先乘 100 再除总大小，目的是在整数运算中保留精度
         * 不能用 ((bytesReceived / bytesTotal) * 100)
         * 因为整数除法会截断小数：5/10=0，0*100=0（错误！）
         * 正确的：5*100/10=50（正确！）
         *
         * 为什么要用 static_cast 而不是 C 风格 (int)？
         *   C 风格转换：(int)value 不做任何安全检查
         *   static_cast<int>(value)：编译器会检查是否兼容类型的转换
         *   在 C++ 中优先使用 static_cast 而不是 C 风格转换
         */
        int percent = static_cast<int>((bytesReceived * 100) / bytesTotal);

        // 更新进度条的当前值，触发界面重绘
        progressDialog->setProgress(percent);

        /**
         * QString("已下载 %1 / %2") 是一个模板字符串
         * %1 和 %2 是占位符，被 arg() 替换
         *
         * QString::arg() 的链式调用：
         *   QString("...%1...%2...").arg(value1).arg(value2)
         *   第一个 arg 替换 %1，第二个 arg 替换 %2
         *
         * 注意：arg() 返回的是一个新的 QString 对象（值语义），
         *       不是修改原字符串的引用。
         */
        QString text = QString("已下载 %1 / %2")
            .arg(formatFileSize(bytesReceived))  // 替换 %1：已下载大小
            .arg(formatFileSize(bytesTotal));     // 替换 %2：总大小

        // setProgressText 更新对话框中的文字
        progressDialog->setProgressText(text);
    } else {
        /**
         * bytesTotal = -1（服务器没有返回 Content-Length）的情况
         * 此时无法计算百分比，进度条无法正常显示进度
         * QProgressDialog 在这种情况下会自动进入"忙碌模式"（忙动动画）
         * 我们只显示已下载的数据量
         */
        progressDialog->setProgressText(
            QString("已下载 %1").arg(formatFileSize(bytesReceived)));
    }
}

// =============================================================================
// 私有槽函数：接收到数据（未使用 - 改用 onFinished 一次性读取）
// =============================================================================

void ApkDownloader::onReadyRead()
{
    /**
     * 函数体为空——这是一个故意留空的槽函数。
     *
     * 为什么需要这个空函数？
     *   头文件（ApkDownloader.h）中声明了 onReadyRead 作为私有槽：
     *     private slots:
     *         void onReadyRead();
     *   如果函数声明了但没有实现，链接器（Linker）会报错：
     *     "undefined reference to `ApkDownloader::onReadyRead()'"
     *   所以必须提供一个空实现。
     *
     * 为什么不把头文件中的声明也删掉？
     *   为以后可能的增量写入方案保留接口。
     *   如果有人想换回 readyRead 增量写入方式，只需要：
     *     1. 取消 connect readyRead 那一行的注释
     *     2. 在 onReadyRead() 中实现写入逻辑
     *   而不需要修改头文件中的声明。
     *
     * 设计模式：空对象模式（Null Object Pattern）
     *   提供什么都不做的默认实现，避免调用方需要不断判断"这个函数是否存在"。
     */
}

// =============================================================================
// 私有槽函数：下载完成
// =============================================================================

void ApkDownloader::onFinished()
{
    /**
     * hide() 隐藏进度对话框
     * 注意：不是 close()
     *   hide()  ：隐藏对话框，对话框对象仍然存在
     *   close() ：关闭对话框，如果设置了 WA_DeleteOnClose 则会自动 delete
     * 此时可能下载失败，也可能成功，我们先隐藏对话框，
     * 后面如果安装成功，ApkDownloader 会自动 deleteLater() 销毁自己，
     * 对话框作为子对象也会被 Qt 父子机制自动清理。
     */
    progressDialog->hide();

    // =========================================================================
    // 第一步：检查网络错误
    // =========================================================================

    /**
     * currentReply->error() 返回网络请求的错误码
     * QNetworkReply::NoError 是 0，表示没有错误
     *
     * 常见错误码（QNetworkReply::NetworkError 枚举）：
     *   NoError                  =  0：请求成功
     *   ConnectionRefusedError   =  1：连接被拒绝
     *   RemoteHostClosedError    =  2：远程主机关闭连接
     *   HostNotFoundError        =  3：找不到主机（DNS 解析失败）
     *   TimeoutError             =  4：超时
     *   OperationCanceledError   =  5：操作被取消（abort()）
     *   SslHandshakeFailedError  =  6：SSL 握手失败
     *   ContentNotFoundError     = 11：404 Not Found
     *   ContentAccessDenied      = 12：403 Forbidden
     *   ProtocolFailure          = 99：协议错误
     */
    if (currentReply->error() != QNetworkReply::NoError) {
        // errorString() 返回人类可读的错误描述，如 "Host not found"
        // 注意：errorString() 在不同系统上可能返回不同语言的文字
        QString errorMsg = currentReply->errorString();

        // 清理：关闭并删除文件对象
        if (apkFile) {
            apkFile->close();       // 关闭文件，确保所有数据被写入磁盘
            delete apkFile;          // 释放文件对象内存
            apkFile = nullptr;       // 置空防止悬空指针
        }

        // 清理：删除可能已经创建的不完整文件
        if (QFile::exists(apkFilePath))
            QFile::remove(apkFilePath);  // 不完整的 APK 文件留着也没用

        /**
         * 区分取消和真正错误的场景：
         *   OperationCanceledError = 5，表示请求被 abort() 中断
         *   这通常是因为用户点了"取消"按钮，不是真正的错误
         *   所以只打日志，不发射 downloadFailed 信号
         *   否则用户取消下载也会看到"下载失败"的提示，体验不好
         */
        if (currentReply->error() == QNetworkReply::OperationCanceledError)
            qDebug() << "用户取消下载";  // qDebug() 输出到调试控制台（Android 上是 logcat）
        else
            emit downloadFailed("下载失败：" + errorMsg);  // 通知调用方

        // 清理 network reply 对象
        currentReply->deleteLater();   // 安全删除，延迟到事件循环
        currentReply = nullptr;         // 置空

        /**
         * 关键：this->deleteLater()
         * 这个调用计划删除 ApkDownloader 对象自身
         * 为什么不是 delete this？
         *   delete this 是立即删除，如果在栈帧中还有对 this 的访问，
         *   就会导致"在已释放内存上继续执行"的崩溃。
         *   deleteLater() 将删除推迟到事件循环的下一次迭代，
         *   确保当前函数执行完毕后安全删除。
         *
         * 注意：调了 deleteLater() 后，这个对象就不能再被使用了，
         *       所有成员变量（包括 progressDialog）都会被一起删除。
         *       但 QProgressDialog 设置了 WA_DeleteOnClose，
         *       它在 close 时已被自动 delete，不能重复 delete。
         *       不过我们的 progressDialog 只是 hide() 不是 close()，
         *       所以它会被 deleteLater() 链式删除。
         */
        this->deleteLater();
        return;  // 结束处理，不继续执行后续代码
    }

    // =========================================================================
    // 第二步：一次性读取全部数据并写入文件（核心修复）
    // =========================================================================

    if (apkFile && apkFile->isOpen()) {
        /**
         * currentReply->readAll() 一次性读取响应体的全部数据
         *
         * QByteArray 是一个字节数组，可以存储任意二进制数据
         * 包括图片、APK 文件等非文本数据
         *
         * 内存占用考虑：
         *   如果 APK 文件是 100MB，QByteArray allData 就会分配 100MB 内存
         *   虽然一次性读取内存占用较高，但避免了 readyRead 的竞态条件
         *   在内存充足的现代手机上（通常 6GB+ RAM），100MB 不是问题
         *   但如果适配低端机（1GB RAM），可能需要考虑分批写入的方案
         */
        QByteArray allData = currentReply->readAll();
        qDebug() << "一次性读取到" << allData.size() << "字节的数据";

        /**
         * 检查数据是否为空
         * allData.isEmpty() = true 表示没有读取到任何数据
         * allData.size() == 0 与 isEmpty() 等价，两个条件写一个即可
         * 这里两个条件都写，是"防御性编程"——双重保险
         *
         * 为什么数据可能为空？
         *   服务器返回 204 No Content 或 304 Not Modified 时，
         *   响应体为空，readAll() 返回空 QByteArray
         */
        if (allData.isEmpty() || allData.size() == 0) {
            emit downloadFailed("下载的数据为空");
            // 清理资源：文件 + 文件 + reply + 自身
            apkFile->close();
            delete apkFile;
            apkFile = nullptr;
            QFile::remove(apkFilePath);     // 删除空文件
            currentReply->deleteLater();
            currentReply = nullptr;
            this->deleteLater();
            return;
        }

        /**
         * apkFile->write(allData) 将数据写入磁盘文件
         * 返回值 qint64 written 是实际写入的字节数
         *
         * 为什么返回值可能不等于 allData.size()？
         *   磁盘空间不足：只写了一部分数据就满了
         *   写入中断：操作系统层面的原因
         *
         * 场景：磁盘空间还剩 10MB，下载了 50MB 的 APK
         *   write() 只写入 10MB 后返回 10485760
         *   而 allData.size() = 52428800
         *   written != allData.size() → 判断为写入不完整
         */
        qint64 written = apkFile->write(allData);

        // 检查写入是否完整
        if (written != allData.size()) {
            emit downloadFailed("写入文件不完整");

            // 同样的清理流程：文件 + reply + 自身
            apkFile->close();
            delete apkFile;
            apkFile = nullptr;
            QFile::remove(apkFilePath);     // 不完整的文件也要删除
            currentReply->deleteLater();
            currentReply = nullptr;
            this->deleteLater();
            return;
        }

        qDebug() << "成功写入" << written << "字节到文件";

        /**
         * apkFile->flush() 将缓冲区数据强制写入磁盘
         *
         * QFile 内部使用缓冲区（buffer），
         * write() 是先写入缓冲区，缓冲区满了再真正写入磁盘。
         * flush() 强制将缓冲区中的剩余数据写入磁盘。
         *
         * 如果不 flush()，在极端情况下（如突然断电），
         * 缓冲区中的数据会丢失，导致 APK 文件损坏。
         * flush() 确保数据已经被操作系统接收（但不保证已经写入物理磁盘，
         * 因为操作系统也有自己的磁盘缓存）。
         */
        apkFile->flush();

        // 关闭文件并释放文件对象
        apkFile->close();
        delete apkFile;
        apkFile = nullptr;

        // 此时 APK 文件已完整写入磁盘
    }

    // =========================================================================
    // 第三步：验证文件大小
    // =========================================================================

    /**
     * QFileInfo 获取文件信息
     * QFileInfo 不直接操作文件，而是获取文件系统的元数据
     * 通过它我们可以获取文件的：大小、修改时间、权限、类型等信息
     */
    QFileInfo fileInfo(apkFilePath);

    /**
     * fileInfo.size() 返回文件大小（字节）
     * 为什么要用 QFileInfo 重新获取？
     *   刚才写入时我们用 write() 的返回值知道了写入的字节数，
     *   但用 QFileInfo::size() 再次验证是双重检查：
     *   确保操作系统正确地记录了文件大小。
     */
    qint64 fileSize = fileInfo.size();
    qDebug() << "APK 文件大小:" << formatFileSize(fileSize);

    /**
     * 最小大小校验：10KB
     * 真实的 Android APK 最小也在几十 KB 以上（最精简的 Hello World）
     * 10KB 是经验值，用于过滤明显的错误下载：
     *   - 服务器返回 HTML 错误页面（如 404 页面，通常 1-5KB）
     *   - 服务器返回 JSON 错误信息
     *   - 只下载了一小部分就被中断
     *
     * 10 * 1024 = 10240 字节 ≈ 10KB
     * 注意：这里用了 operator<，不包括等于 10240 的情况
     * 所以严格来说是"小于 10KB"视为无效
     */
    if (fileSize < 1024 * 10) {
        // 删除无效文件
        QFile::remove(apkFilePath);

        // 发射错误信号，并包含文件大小信息方便调试
        emit downloadFailed("下载的文件太小（" + formatFileSize(fileSize)
                           + "），可能不是有效的 APK");

        // 清理 reply 和自身
        currentReply->deleteLater();
        currentReply = nullptr;
        this->deleteLater();
        return;
    }

    // =========================================================================
    // 第四步：释放网络回复对象
    // =========================================================================

    /**
     * deleteLater() 安全删除 QNetworkReply 对象
     * 为什么不直接 delete？
     *   QNetworkReply 可能在事件循环中还有待处理的信号或事件，
     *   直接 delete 可能导致"在析构后访问已删除对象"的崩溃。
     *   deleteLater() 确保所有事件处理完毕后再删除。
     */
    currentReply->deleteLater();
    currentReply = nullptr;

    // =========================================================================
    // 第五步：等待操作系统完成磁盘写入
    // =========================================================================

    /**
     * QThread::msleep(300) 让当前线程休眠 300 毫秒
     *
     * 为什么需要这个延时？
     *   flush() 和 close() 已经将数据交给了操作系统，
     *   但操作系统可能还没有完成实际的磁盘写入（有缓存延迟）。
     *   300ms 是个经验值，足够操作系统完成磁盘 I/O。
     *
     * 为什么不直接调用 installApk()？
     *   Android 系统的 FileProvider 在访问文件时，
     *   如果文件内容还没有完全写入磁盘，可能读取到不完整的数据。
     *   300ms 的延时大大降低了这个概率。
     *
     * 注意：QThread::msleep() 会阻塞当前线程。
     *   但这里是在主线程（UI 线程）中调用，
     *   阻塞 300ms 会导致界面"卡住"300ms。
     *   好在 installApk() 之后会启动新的 Activity，
     *   用户感知不到这 300ms 的卡顿。
     *   更好的方案是用 QTimer::singleShot(300, this, &installApk)，
     *   但为了代码简洁，这里用了简单的 msleep。
     */
    QThread::msleep(300);

    // =========================================================================
    // 第六步：安装 APK
    // =========================================================================

    // installApk() 会通过 JNI 调用 Android 系统安装器
    installApk();

    /**
     * 安装完成后，自动销毁 ApkDownloader 对象
     * 因为整个下载-安装流程已经结束，这个对象不再需要了
     * 如果调用方需要再次下载，应该创建新的 ApkDownloader 实例
     */
    this->deleteLater();
}

// =============================================================================
// 私有方法：安装 APK
// =============================================================================

void ApkDownloader::installApk()
{
    // =========================================================================
    // 第一步：检查文件是否存在
    // =========================================================================

    /**
     * QFile::exists(apkFilePath) 可以在文件被外部程序删除时及时发现
     * 例如用户手动删除了文件，或者 Android 系统清理了应用数据
     */
    if (!QFile::exists(apkFilePath)) {
        emit downloadFailed("APK 文件不存在");
        return;
    }

    // =========================================================================
    // 第二步：魔数校验（Magic Number）
    // =========================================================================

    /**
     * 为什么需要验证文件头？
     *   虽然我们已经检查了文件大小（>10KB），
     *   但有些情况下下载的内容可能不是 APK 格式：
     *     - 服务器返回了 HTML 页面（重定向被破坏）
     *     - Gitee 返回了 JSON 格式的认证错误
     *   APK 文件本质上是 ZIP 格式，ZIP 文件的头部必须是 'P' 'K'
     *   这两个字节称为"魔数"（Magic Number）。
     *
     * 常见的文件魔数：
     *   PK  → ZIP/APK（PK = Phil Katz，ZIP 格式的发明者）
     *   PNG → PNG 图片
     *   GIF → GIF 图片
     *   %PDF → PDF 文件
     *   MZ  → Windows EXE 文件（MS-DOS 头）
     *   0xFFD8 → JPEG 图片
     */
    QFile verifyFile(apkFilePath);
    if (!verifyFile.open(QIODevice::ReadOnly)) {
        emit downloadFailed("无法打开下载的文件进行校验");
        return;
    }

    // 读取文件的前 4 个字节
    // QFile.read(maxSize) 读取最多 maxSize 字节的数据
    // 这里读取 4 字节，前 2 字节是魔数，后 2 字节是冗余
    QByteArray magicBytes = verifyFile.read(4);
    verifyFile.close();  // 校验完立即关闭，不占用文件句柄

    qDebug() << "APK 文件魔数(hex):" << magicBytes.toHex(' ');
    // toHex(' ') 将二进制转为十六进制字符串，中间用空格分隔
    // 例如：PK → 50 4B（ASCII 码 P=80=0x50, K=75=0x4B）

    qDebug() << "APK 文件魔数(text):" << QString::fromLatin1(magicBytes);
    // fromLatin1() 将 Latin-1（ISO-8859-1）编码的字节转为 QString
    // 对于 'P' 'K' 这种 ASCII 字符，Latin-1 和 UTF-8 编码相同

    /**
     * magicBytes.startsWith("PK") 检查文件头是否以 "PK" 开头
     * 注意：startsWith 的参数是 QByteArray("PK")，不是字符串字面量
     * 这里发生了隐式类型转换（Implicit Conversion），
     * C++ 编译器自动将 "PK"（const char*）转为 QByteArray
     *
     * 负负得正的逻辑：
     *   if (!magicBytes.startsWith("PK")) 表示：
     *   如果 魔数 不 以 "PK" 开头 → 文件可能不是 ZIP/APK 格式
     */
    if (!magicBytes.startsWith("PK")) {
        // 魔数校验失败，说明下载的内容可能被服务器替换了
        // 比如 Gitee 返回了登录页面的 HTML
        QFile::remove(apkFilePath);  // 删除无效文件
        emit downloadFailed("下载的文件不是有效的 APK（文件头错误）");
        return;
    }

    qDebug() << "APK 魔数校验通过，确认是有效的 ZIP/APK 格式";

    // =========================================================================
    // 第三步：通过 JNI 调用 Android 系统安装器
    // =========================================================================

    /**
     * JNI（Java Native Interface）：
     *   Java 和 C/C++ 之间的互操作接口。
     *   允许 C++ 代码调用 Java 方法。
     *
     * QJniObject 是 Qt 对 JNI 的封装类：
     *   让 C++ 开发者可以"假装在写 Java 代码"
     *   底层仍然是 JNI，但 API 更友好
     *
     * 这里要调用的 Java 代码等价于：
     *   // Java 伪代码：
     *   Activity activity = QtNative.activity();
     *   Context context = activity.getApplicationContext();
     *   String pkgName = context.getPackageName();
     *   String authority = pkgName + ".qtprovider";
     *
     *   File file = new File("/data/data/.../update.apk");
     *   Uri uri = FileProvider.getUriForFile(context, authority, file);
     *
     *   Intent intent = new Intent(Intent.ACTION_VIEW);
     *   intent.setDataAndType(uri, "application/vnd.android.package-archive");
     *   intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
     *   intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
     *   activity.startActivity(intent);
     */

    /**
     * QJniObject::callStaticMethod<jobject>() 调用 Java 静态方法
     *
     * 参数解释：
     *   1. "org/qtproject/qt/android/QtNative"：完整类名（Java 包名+类名）
     *   2. "activity"：方法名
     *   3. "()Landroid/app/Activity;"：方法签名（JNI 签名格式）
     *
     * JNI 方法签名格式：
     *   (参数类型)返回值类型
     *   L 开头表示对象类型
     *   ()  表示无参数
     *   Landroid/app/Activity; 表示返回值类型是 Activity 对象
     *
     * 函数签名详解：
     *   "()Landroid/app/Activity;" =
     *     ()                       ：参数列表为空
     *       Landroid/app/Activity; ：返回一个 Activity 对象
     *
     * 常见类型签名对照：
     *   Z    → boolean
     *   B    → byte
     *   C    → char
     *   S    → short
     *   I    → int
     *   J    → long
     *   F    → float
     *   D    → double
     *   V    → void
     *   Lxxx; → 对象类型（xxx 是完整类路径）
     *   [I   → int[]（一维数组）
     *   [Lxxx; → 对象数组
     */
    QJniObject activity = QJniObject::callStaticMethod<jobject>(
        "org/qtproject/qt/android/QtNative",  // Java 类名
        "activity",                             // 方法名
        "()Landroid/app/Activity;"              // JNI 方法签名
    );

    /**
     * activity.isValid() 检查 JNI 对象是否有效
     * 如果 Qt/QtNative.activity() 返回 null，
     * 或者 JNI 调用本身失败，isValid() 返回 false
     */
    if (!activity.isValid()) {
        emit downloadFailed("无法获取 Activity");
        return;
    }

    // 调用 activity.getApplicationContext() 获取应用上下文
    // Java 等效代码：Context context = activity.getApplicationContext();
    QJniObject context = activity.callMethod<jobject>(
        "getApplicationContext",                // 方法名
        "()Landroid/content/Context;"           // 返回 Context 对象
    );

    /**
     * 调用 context.getPackageName() 获取应用的包名
     * 包名在 AndroidManifest.xml 的 package 属性中定义
     * 例如：com.example.calculationtools
     *
     * callMethod<jstring>() 返回 QJniObject 封装了 Java 的 String，
     * 调用 .toString() 将其转为 C++ 的 QString
     *
     * 为什么用 QJniObject::toString() 而不是直接赋值？
     *   jstring 是 Java String 的 JNI 句柄类型，
     *   QJniObject::toString() 内部调用了 GetStringUTFChars() 获取 UTF-8 字符
     *   然后用等价的 C++ 字符串构造 QString
     */
    QString packageName = context.callMethod<jstring>(
        "getPackageName"                         // 方法名
    ).toString();

    /**
     * 拼接 FileProvider 的 authority
     * Java 等效代码：String authority = packageName + ".qtprovider"
     * authority 是 FileProvider 的唯一标识符，在 AndroidManifest.xml 中注册
     *
     * 示例：
     *   包名 = "com.example.calculationtools"
     *   authority = "com.example.calculationtools.qtprovider"
     */
    QString authority = packageName + ".qtprovider";

    // 打印调试信息，方便排查问题
    qDebug() << "FileProvider authority:" << authority;
    qDebug() << "APK file path:" << apkFilePath;

    // =========================================================================
    // 第四步：创建 Java File 对象
    // =========================================================================

    /**
     * QJniObject::fromString(apkFilePath) 将 QString 转为 QJniObject
     * 这个 QJniObject 内部封装了一个 Java String 对象
     *
     * 为什么要先转为 String，再传给 File 构造函数？
     *   因为 Java 的 File 构造函数的签名是：
     *     File(String pathname)
     *   我们需要一个 Java String 类型的参数。
     */
    QJniObject jFile = QJniObject::fromString(apkFilePath);

    /**
     * QJniObject("java/io/File", "(Ljava/lang/String;)V", jFile.object<jstring>())
     * 创建 Java File 对象
     *
     * 构造函数调用格式：
     *   QJniObject(className, constructorSignature, arg1, arg2, ...)
     *
     * 参数解析：
     *   1. "java/io/File"：Java 类名（注意用 / 而不是 .）
     *   2. "(Ljava/lang/String;)V"：构造函数的 JNI 签名
     *      (Ljava/lang/String;)  = 参数：一个 String 对象
     *      V                     = 返回值：void
     *   3. jFile.object<jstring>()：获取封装的 jstring 句柄
     *
     * 为什么用 object<jstring>() 而不是直接传 jFile？
     *   constructorSignature 中声明了参数类型是 Ljava/lang/String;，
     *   所以必须传一个 jstring 类型的值。
     *   jFile 是 QJniObject，它的底层可能持有不同类型的 JNI 句柄，
     *   object<jstring>() 确保我们取出的是 jstring 类型。
     */
    jFile = QJniObject("java/io/File", "(Ljava/lang/String;)V",
                       jFile.object<jstring>());

    // =========================================================================
    // 第五步：通过 FileProvider 获取 Content URI
    // =========================================================================

    /**
     * 调用 FileProvider.getUriForFile() 获取 content:// URI
     *
     * 为什么需要 FileProvider？
     *   Android 7.0（API 24）开始，禁止在 Intent 中传递 file:// URI，
     *   因为 file:// URI 暴露了文件系统的真实路径，有安全隐患。
     *   必须使用 FileProvider 生成 content:// URI，
     *   这样目标应用（系统安装器）可以临时获得访问权限。
     *
     * Java 等效代码：
     *   Uri uri = FileProvider.getUriForFile(
     *       context,
     *       packageName + ".qtprovider",
     *       new File(apkFilePath)
     *   );
     *
     * JNI 调用签名：
     *   FileProvider.getUriForFile(
     *     Context context,
     *     String authority,
     *     File file
     *   ) → Uri
     *
     * JNI 签名： (Landroid/content/Context;Ljava/lang/String;Ljava/io/File;)Landroid/net/Uri;
     *
     * 参数的 JNI 类型与实参的对应关系：
     *   context.object()           → jobject (Context)
     *   QJniObject::fromString(authority).object<jstring>() → jstring
     *   jFile.object()             → jobject (File)
     */
    QJniObject uri = QJniObject::callStaticMethod<jobject>(
        "androidx/core/content/FileProvider",                    // 类名
        "getUriForFile",                                          // 方法名
        "(Landroid/content/Context;Ljava/lang/String;Ljava/io/File;)Landroid/net/Uri;", // 签名
        context.object(),                                         // context 参数
        QJniObject::fromString(authority).object<jstring>(),     // authority 参数（jstring）
        jFile.object());                                          // file 参数（jobject）

    // 检查 URI 获取是否成功
    if (!uri.isValid()) {
        // qWarning() 输出警告日志，比 qDebug() 级别高
        qWarning() << "Failed to get content URI! authority:" << authority
                    << "file:" << apkFilePath;
        emit downloadFailed("无法获取文件 Uri");
        return;
    }

    qDebug() << "成功获取 content URI";

    // =========================================================================
    // 第六步：创建并配置 Intent
    // =========================================================================

    /**
     * 创建 Intent 对象，指定 Action 为 ACTION_VIEW
     *
     * Android Intent 机制：
     *   Intent 是 Android 四大组件间的通信载体，
     *   封装了"要做什么事"的信息。
     *
     *   ACTION_VIEW 是最通用的 Action，表示"查看"某个资源。
     *   系统会根据 MIME 类型找到能处理这个 Intent 的 Activity：
     *     - MIME "application/vnd.android.package-archive" → 系统安装器
     *     - MIME "image/png"                              → 图片查看器
     *     - MIME "text/html"                              → 浏览器
     *
     * "android.intent.action.VIEW" 就是 Intent.ACTION_VIEW 的字符串值
     * 这里直接写字符串，避免了 JNI 调用获取静态字段值。
     *
     * 构造函数签名：
     *   Intent(String action) → JNI: (Ljava/lang/String;)V
     */
    QJniObject intent("android/content/Intent", "(Ljava/lang/String;)V",
                      QJniObject::fromString("android.intent.action.VIEW").object<jstring>());

    /**
     * setDataAndType() 设置 Intent 的数据（URI）和 MIME 类型
     *
     * 这里没有先用 setData(uri) 再 setType(mime) 的原因：
     *   setData() 会清空之前设置的 Type，
     *   setType() 会清空之前设置的 Data，
     *   所以必须用 setDataAndType() 同时设置两者。
     *
     * MIME 类型 "application/vnd.android.package-archive"：
     *   这是 Android 对 APK 文件的官方 MIME 类型。
     *   系统通过这个 MIME 类型找到包安装器来处理这个 Intent。
     *
     * 方法签名：
     *   Intent setDataAndType(Uri data, String type) → JNI: (Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;
     */
    intent.callMethod<jobject>("setDataAndType",
        "(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;",
        uri.object(),
        QJniObject::fromString("application/vnd.android.package-archive").object<jstring>());

    /**
     * addFlags() 添加 Intent Flag（标记位）
     *
     * FLAG_GRANT_READ_URI_PERMISSION：
     *   授予目标应用（系统安装器）临时读取 content:// URI 内容的权限
     *   权限在接收 Intent 的 Activity 处于活动期间有效，
     *   Activity 结束后权限自动回收
     *
     * FLAG_ACTIVITY_NEW_TASK：
     *   在新的 Task（任务栈）中启动安装 Activity
     *   因为我们的计算器应用不是一个 Activity（只有 Qt Activity），
     *   不加这个 Flag 可能导致 startActivity() 失败
     *   这个 Flag 确保系统安装器在独立的栈中运行
     *
     * QJniObject::getStaticField<jint>() 获取 Java 静态字段的值
     *   "android/content/Intent"    ：类名
     *   "FLAG_GRANT_READ_URI_PERMISSION"：字段名
     *   "I"                         ：字段类型签名（I = int）
     *   这个方法相当于 Java 的 Intent.FLAG_GRANT_READ_URI_PERMISSION
     */
    intent.callMethod<jobject>("addFlags", "(I)Landroid/content/Intent;",
        QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_GRANT_READ_URI_PERMISSION"));
    intent.callMethod<jobject>("addFlags", "(I)Landroid/content/Intent;",
        QJniObject::getStaticField<jint>("android/content/Intent", "FLAG_ACTIVITY_NEW_TASK"));

    // =========================================================================
    // 第七步：启动安装 Activity
    // =========================================================================

    /**
     * activity.startActivity(intent) 启动系统安装器
     *
     * 最终效果：
     *   1. Android 系统收到 Intent
     *   2. 根据 MIME 类型找到包安装器
     *   3. FileProvider 提供 content:// URI 的文件访问
     *   4. 系统安装器读取 APK 文件并显示安装界面
     *   5. 用户确认安装，APK 被安装到设备上
     *
     * startActivity 方法签名：
     *   void startActivity(Intent intent) → JNI: (Landroid/content/Intent;)V
     */
    activity.callMethod<void>(
        "startActivity",
        "(Landroid/content/Intent;)V",
        intent.object()
    );

    qDebug() << "已启动安装 Activity";
}
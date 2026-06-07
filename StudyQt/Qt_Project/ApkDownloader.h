/******************************************************************************
 * ApkDownloader.h
 * APK 下载器 - 头文件
 *
 * 功能说明：
 *   在 Android 平台实现"应用内下载并安装 APK"功能。
 *   用户点击"立即更新"后，不需要跳转到浏览器，直接在本应用内完成：
 *   下载 → 校验 → 安装 的完整流程。
 *
 * 涉及的 Qt 核心类：
 *   - QNetworkAccessManager : 网络请求管理器，负责建立 HTTP 连接
 *   - QNetworkReply         : 网络响应对象，接收下载数据流
 *   - QFile                 : 文件读写类，将下载数据写入本地存储
 *   - QProgressDialog       : 模态进度对话框，显示下载进度百分比
 *   - QJniObject            : Qt Android 扩展，用于通过 JNI 调用 Java API
 *
 * Android 特有知识点：
 *   - FileProvider          : Android 7.0+ 的安全文件共享方案
 *   - content:// URI        : 替代 file:// 的安全 URI 方案
 *   - Intent.ACTION_VIEW    : Android 隐式意图，打开系统安装器
 *   - APK 魔数 "PK"         : ZIP 文件格式的前两个字节标识
 *
 * 使用方式：
 *   // 在检测到新版本后，创建下载器并开始下载
 *   ApkDownloader* downloader = new ApkDownloader(this);
 *   connect(downloader, &ApkDownloader::downloadFailed,
 *           this, [](const QString& err) { QMessageBox::warning(nullptr, "错误", err); });
 *   downloader->startDownload("https://example.com/app-1.1.0.apk");
 *
 * 注意：
 *   本模块仅在 Android 平台下有效（通过 #ifdef Q_OS_ANDROID 编译条件控制），
 *   在其他平台（Windows、iOS、Linux）下不会被编译。
 ******************************************************************************/

#ifndef APKDOWNLOADER_H       // 头文件保护宏：防止同一头文件被多次包含
#define APKDOWNLOADER_H

#include <QObject>                // Qt 对象基类：所有 Qt 类的根，支持信号槽机制
#include <QNetworkAccessManager>  // 网络访问管理器：用于发起 HTTP/HTTPS 下载请求
#include <QNetworkReply>          // 网络响应对象：持有服务器返回的数据和状态信息
#include <QFile>                  // 文件操作类：用于将下载的数据写入本地磁盘文件
#include <QProgressDialog>        // 进度对话框：模态弹窗，显示下载进度和取消按钮
#include <QWidget>                // 窗口基类：用于获取父窗口指针，处理对话框的父级关系

/**
 * @class ApkDownloader
 * @brief APK 下载器类（仅限 Android 平台使用）
 *
 * 设计思路：
 *   这是一个"一次性"对象——创建后调用 startDownload() 开始下载，
 *   下载完成后自动安装并销毁自身。如果需要多次下载，应该多次创建 ApkDownloader 实例。
 *
 * 下载流程：
 *   1. startDownload(url)
 *        ↓
 *   2. 创建 QNetworkRequest 并调用 networkManager->get()
 *        ↓  (异步下载，不阻塞 UI 线程)
 *   3. onDownloadProgress() — 实时更新进度对话框
 *   4. onReadyRead()        — 将数据逐块写入文件
 *   5. onFinished()         — 下载完成，校验文件大小和魔数
 *        ↓
 *   6. installApk()         — 通过 JNI 调用 Android 系统安装器
 *
 * 错误处理策略：
 *   下载过程中任何步骤失败（网络错误、文件写入错误、校验失败），
 *   都会发射 downloadFailed(error) 信号通知调用方，
 *   然后通过 deleteLater() 自动销毁自身。
 *   调用方只需要连接 downloadFailed 信号处理错误提示即可。
 */
class ApkDownloader : public QObject
{
    Q_OBJECT  // Qt 元对象宏：启用信号槽、属性系统等 Qt 元对象特性

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针（QWidget* 而非 QObject*）
     *
     * 为什么参数是 QWidget* 而不是 QObject*？
     *   因为 ApkDownloader 内部创建了 QProgressDialog 模态对话框，
     *   QProgressDialog 需要一个 QWidget 指针作为父窗口来居中显示。
     *   QWidget 继承自 QObject，所以不影响 Qt 的对象树内存管理机制。
     *
     * 初始化内容：
     *   1. 创建 QNetworkAccessManager 实例，用于发起网络请求
     *   2. 将所有指针成员初始化为 nullptr（空指针），
     *      表示尚未开始下载操作
     */
    explicit ApkDownloader(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     *
     * 清理动态分配的资源：
     *   - 如果当前有正在进行的下载（currentReply != nullptr），中止它
     *   - 关闭并清理已打开的文件（apkFile）
     *   - 删除进度对话框（progressDialog）
     *
     * Qt 对象树说明：
     *   由于 ApkDownloader 继承自 QObject，创建时如果指定了父对象，
     *   QObject 的析构函数会自动删除所有子对象。
     *   但这里我们仍然在析构函数中显式清理，确保即使父对象被删除，
     *   也可以优雅地中止未完成的下载。
     */
    ~ApkDownloader();

    /**
     * @brief 开始下载 APK 文件
     * @param url APK 文件的 HTTPS 下载链接
     *
     * 调用此方法后：
     *   1. 创建 QProgressDialog 进度对话框并显示
     *   2. 使用 QNetworkAccessManager 发起异步 HTTP GET 请求
     *   3. 返回后立即继续执行（不阻塞）
     *
     * 下载流程详解：
     *   - 阶段一（创建请求）：
     *     QUrl → QNetworkRequest → networkManager->get()
     *   - 阶段二（下载数据）：
     *     QNetworkReply 的 readyRead 信号 → onReadyRead() 写入文件
     *     QNetworkReply 的 downloadProgress 信号 → onDownloadProgress() 更新进度
     *   - 阶段三（完成处理）：
     *     QNetworkReply 的 finished 信号 → onFinished() 校验并安装
     *
     * 线程安全性：
     *   本方法及所有后续处理都在主线程（Qt 事件循环线程）执行，
     *   网络 I/O 由 Qt 内部的工作线程处理，数据就绪后通过信号发送到主线程。
     *   因此不需要额外的线程同步（互斥锁、信号量等）。
     */
    void startDownload(const QString& url);

signals:
    /**
     * @brief 下载失败信号
     * @param error 人类可读的错误描述字符串（中文）
     *
     * 发射场景：
     *   - 网络连接失败（无网络、DNS 解析失败、超时等）
     *   - 服务器返回非 200 状态码
     *   - 文件写入失败（磁盘空间不足、权限不足）
     *   - 文件校验失败（大小异常、魔数错误）
     *   - JNI 调用失败（Activity 不可用、FileProvider 未配置）
     *
     * 信号的安全性：
     *   信号是 Qt 的元对象系统提供的类型安全回调机制。
     *   如果调用方没有连接此信号，发射信号不会产生任何副作用。
     *
     * 推荐用法：
     *   connect(downloader, &ApkDownloader::downloadFailed, [](const QString& err) {
     *       QMessageBox::warning(nullptr, "更新失败", err);
     *   });
     */
    void downloadFailed(const QString& error);

    /**
     * @brief 安装完成信号
     *
     * 在 installApk() 成功调用 startActivity() 启动系统安装器后发射。
     *
     * 注意：此信号仅表示"已成功启动安装界面"，
     * 不代表"用户已确认安装"或"安装已完成"。
     * 因为安装过程发生在系统安装器中，属于另一个进程，
     * 应用本身无法获知最终的安装结果。
     *
     * 推荐用法：
     *   连接此信号以记录日志或进行统计分析。
     */
    void installFinished();

private slots:
    /**
     * @brief 下载进度更新槽函数
     * @param bytesReceived 已接收字节数（累加值）
     * @param bytesTotal 文件总字节数（-1 表示服务器未返回 Content-Length）
     *
     * 此槽函数连接到 QNetworkReply::downloadProgress 信号。
     * 信号槽连接在 startDownload() 方法中建立：
     *   connect(currentReply, &QNetworkReply::downloadProgress,
     *           this, &ApkDownloader::onDownloadProgress);
     *
     * 更新进度对话框的内容：
     *   1. 进度条的百分比值（0-100）
     *   2. 标签文字："已下载 3.2 GB / 4.8 GB (67%)"
     *
     * 特殊情况处理：
     *   - bytesTotal == -1：文件大小未知，显示"正在下载..."
     *   - 进行格式化时使用 KB/MB/GB 等单位，提高可读性
     *   - 如果用户点击"取消"按钮，取消下载并清理资源
     */
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    /**
     * @brief 接收到数据时写入文件槽函数
     *
     * 此槽函数连接到 QNetworkReply::readyRead 信号。
     *
     * QNetworkReply 内部维护一个接收缓冲区，
     * 当有新的数据到达时，readyRead 信号被触发。
     * 在此槽函数中调用 readAll() 读取所有可用数据，
     * 然后写入到本地文件。
     *
     * 为什么不用一次性读取全部数据再写入？
     *   对于大文件（APK 可能有 50MB+），如果等待所有数据接收完毕再写入，
     *   内存中需要缓存整个文件的数据，占用大量内存。
     *   readyRead + 即时写入的方式，每次只处理当前到达的数据块，
     *   内存占用仅为当前数据块的大小（通常几十 KB）。
     *   这种模式称为"流式处理"（Streaming）。
     */
    void onReadyRead();

    /**
     * @brief 下载完成后的处理槽函数
     *
     * 此槽函数连接到 QNetworkReply::finished 信号。
     *
     * 执行流程：
     *   1. 检查网络请求是否成功（error() == NoError）
     *   2. 关闭文件（flush() + close() 确保数据完全写入）
     *   3. 校验文件合法性（最小大小 + ZIP 魔数检查）
     *   4. 通过 JNI 调用 Android 安装器
     *   5. 自动销毁自身（deleteLater()）
     *
     * 如果下载取消：
     *   当用户关闭进度对话框时，需要中止下载并删除临时文件。
     *   这通过监听进度对话框的取消信号实现。
     */
    void onFinished();

private:
    /**
     * @brief 安装已下载的 APK 文件（私有方法）
     *
     * 这是整个类最复杂的部分，通过 JNI（Java Native Interface）
     * 调用 Android 系统的 Java API 启动安装器。
     *
     * JNI 调用链：
     *   1. QtNative.activity() → 获取当前 Activity 对象
     *   2. activity.getApplicationContext() → 获取应用上下文
     *   3. context.getPackageName() → 获取包名（用于 FileProvider authority）
     *   4. new File(apkFilePath) → 创建 Java File 对象
     *   5. FileProvider.getUriForFile() → 获取 content:// URI
     *   6. new Intent(ACTION_VIEW) → 创建安装意图 Intent
     *   7. intent.setDataAndType() → 设置 URI 和 MIME 类型
     *   8. intent.addFlags() → 添加权限标记（读取 URI + 新任务栈）
     *   9. activity.startActivity() → 启动系统安装界面
     *
     * 权限要求（AndroidManifest.xml）：
     *   - <uses-permission android:name="android.permission.REQUEST_INSTALL_PACKAGES" />
     *   - FileProvider 需要在 AndroidManifest.xml 中注册
     *
     * 安全保障：
     *   安装前会验证 APK 文件的魔数（Magic Number）是否为 "PK"，
     *   确保下载的内容确实是 ZIP/APK 格式，防止恶意文件。
     */
    void installApk();

    // =========================================================================
    // 成员变量
    // =========================================================================

    QNetworkAccessManager* networkManager;  ///< 网络请求管理器 (QObject 子类，自动加入 Qt 对象树)
    QNetworkReply* currentReply;            ///< 当前正在进行的下载请求 (nullptr 时表示没有活跃下载)
    QFile* apkFile;                         ///< 本地 APK 文件对象 (数据写入目标)
    QProgressDialog* progressDialog;        ///< 模态进度对话框 (显示下载进度条)
    QString apkFilePath;                    ///< APK 文件在设备上的完整路径 (如 "/data/data/com.example/cache/update.apk")
    QWidget* parentWidget;                  ///< 父窗口指针 (用于对话框居中、Activity 获取等)
};

#endif // APKDOWNLOADER_H
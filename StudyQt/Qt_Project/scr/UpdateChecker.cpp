/******************************************************************************
 * UpdateChecker.cpp
 * 检查更新模块 - 实现文件
 *
 * 本文件实现了从 Gitee 服务器获取版本信息并对比的完整流程。
 * 核心流程：创建请求 → 发送请求 → 接收响应 → 解析 JSON → 对比版本 → 通知结果
 ******************************************************************************/

#include "UpdateChecker.h"   // 更新检查器头文件：声明 UpdateChecker 类
#include <QUrl>              // URL 类，用于构造网络请求地址
#include <QDebug>            // 调试输出，用于打印日志

// =============================================================================
// 常量定义
// =============================================================================

/**
 * @brief Gitee 上 version.json 文件的 raw 链接
 *
 * 这个链接直接返回 JSON 文件的原始内容，不带 HTML 页面包装。
 * 格式：https://gitee.com/用户名/仓库名/raw/分支名/文件路径
 *
 * 注意：必须是 "原始数据" 链接，不能是文件浏览页面的链接。
 */
// Gitee raw 文件链接
// 格式：https://gitee.com/{owner}/{repo}/raw/{branch}/{path}
static const QString VERSION_URL =
    "https://gitee.com/MoonYY05/calculator/raw/master/version.json";

// =============================================================================
// 构造函数
// =============================================================================

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject(parent)                           // 调用父类构造函数
    , networkManager(new QNetworkAccessManager(this))  // 创建网络管理器，设置父对象
    , localVersionCode(0)                       // 初始化本地版本号为 0
{
    /**
     * 连接 QNetworkAccessManager 的 finished 信号到本类的 onReplyFinished 槽函数。
     *
     * 信号槽机制说明：
     *   - QNetworkAccessManager::finished(QNetworkReply*) 是 Qt 内置信号
     *   - 当任意一个网络请求完成时（成功或失败），都会发射此信号
     *   - 第二个参数是响应对象，包含服务器返回的数据
     *
     * 为什么用信号槽而不是回调函数：
     *   - 网络请求是异步的，不会阻塞主线程
     *   - 请求完成后自动触发槽函数，无需轮询等待
     *   - 多个请求可以共用同一个槽函数处理
     */
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &UpdateChecker::onReplyFinished);
}

// =============================================================================
// 公共方法：开始检查更新
// =============================================================================

void UpdateChecker::checkUpdate(const QString& currentVersion, int currentVersionCode)
{
    // 保存本地版本信息，用于后续与服务器版本对比
    localVersion = currentVersion;
    localVersionCode = currentVersionCode;

    /**
     * 第一步：构造请求 URL
     * QUrl 类会自动处理 URL 编码，确保特殊字符正确转义。
     */
    QUrl url(VERSION_URL);

    /**
     * 第二步：创建 QNetworkRequest 对象
     * QNetworkRequest 用于配置请求的各种参数，如 URL、请求头、超时等。
     */
    QNetworkRequest request(url);

    /**
     * 第三步：设置请求头（HTTP Header）
     *
     * Content-Type: application/json
     *   告诉服务器我们期望接收 JSON 格式的响应。
     *   虽然 Gitee raw 链接不检查这个，但规范的做法是加上。
     */
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /**
     * 第四步：设置 User-Agent
     *
     * User-Agent 是 HTTP 标准头，标识客户端身份。
     * 有些服务器会检查 User-Agent，拒绝无标识的请求（防止爬虫）。
     * 格式：应用名/版本号
     */
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

    /**
     * 第五步：设置 Referer
     *
     * Referer 告诉服务器请求是从哪个页面发起的。
     * Gitee 的 raw 文件链接有时会检查 Referer，防止被第三方直接引用。
     */
    request.setRawHeader("Referer", "https://gitee.com/MoonYY05/calculation-tools-update");

    /**
     * 第五步：发起 GET 请求
     *
     * QNetworkAccessManager::get() 是异步方法，立即返回，不会阻塞。
     * 请求在后台线程执行，完成后通过 finished 信号通知。
     *
     * 返回值是 QNetworkReply*，但这里不需要保存，
     * 因为 finished 信号的参数就是 reply 指针。
     */
    networkManager->get(request);

    // 请求已发出，等待 onReplyFinished() 被调用...
}

// =============================================================================
// 私有槽函数：网络请求完成后的处理
// =============================================================================

void UpdateChecker::onReplyFinished(QNetworkReply* reply)
{
    /**
     * QNetworkReply 使用完后必须释放内存。
     * deleteLater() 是 Qt 的安全删除方式：
     *   - 不会立即删除，而是等到事件循环的下一次迭代
     *   - 避免在槽函数中直接 delete 可能导致的崩溃
     *
     * 为什么在这里就调用：
     *   后面的代码还会使用 reply，但 deleteLater() 只是标记删除，
     *   实际删除发生在事件循环的后面，所以是安全的。
     */
    reply->deleteLater();

    // =========================================================================
    // 第一步：检查网络请求是否成功
    // =========================================================================

    /**
     * QNetworkReply::error() 返回请求的错误状态。
     * QNetworkReply::NoError 表示请求成功（HTTP 200）。
     *
     * 常见错误：
     *   - ConnectionRefusedError : 连接被拒绝（服务器没开）
     *   - RemoteHostClosedError  : 远程主机关闭连接
     *   - HostNotFoundError      : 找不到主机（DNS 解析失败）
     *   - TimeoutError           : 请求超时
     *   - NoError                : 请求成功
     */
    if (reply->error() != QNetworkReply::NoError) {
        // 请求失败，打印详细错误信息到调试输出，便于排查问题
        qDebug() << "网络请求错误码:" << reply->error();
        qDebug() << "错误描述:" << reply->errorString();
        qDebug() << "HTTP状态码:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "请求URL:" << reply->request().url().toString();

        // 发射 checkFailed 信号，传递错误描述
        emit checkFailed("网络请求失败：" + reply->errorString());
        return;  // 结束处理
    }

    // =========================================================================
    // 第二步：读取服务器返回的数据
    // =========================================================================

    /**
     * QNetworkReply 继承自 QIODevice，可以像文件一样读取数据。
     * readAll() 读取所有响应内容，返回 QByteArray（字节数组）。
     *
     * 为什么用 QByteArray 而不是 QString：
     *   - 网络传输的是原始字节，可能包含非文本数据
     *   - QByteArray 可以保存任意二进制数据
     *   - 需要时再转换为 QString（指定编码）
     */
    QByteArray data = reply->readAll();

    // =========================================================================
    // 第三步：解析 JSON 数据
    // =========================================================================

    /**
     * QJsonDocument::fromJson() 将 QByteArray 解析为 JSON 文档。
     * 如果解析失败，返回的文档 isNull() 为 true。
     *
     * JSON（JavaScript Object Notation）是一种轻量级数据交换格式，
     * 易于人阅读和编写，也易于机器解析和生成。
     */
    QJsonDocument doc = QJsonDocument::fromJson(data);

    // 检查解析是否成功
    if (doc.isNull() || !doc.isObject()) {
        // 解析失败：数据不是有效的 JSON，或者不是 JSON 对象
        emit checkFailed("版本信息解析失败");
        return;
    }

    // 获取 JSON 文档的根对象
    QJsonObject obj = doc.object();

    // =========================================================================
    // 第四步：提取 JSON 中的字段值
    // =========================================================================

    /**
     * QJsonObject::value("key") 根据键名获取值。
     * toString()、toInt()、toBool() 等方法将 JSON 值转换为 C++ 类型。
     *
     * 如果键不存在，返回默认值：
     *   - toString() 返回空字符串 ""
     *   - toInt()    返回 0
     *   - toBool()   返回 false
     */
    UpdateInfo info;
    info.version = obj.value("version").toString();           // 版本号字符串
    info.versionCode = obj.value("versionCode").toInt();       // 版本号数字
    info.downloadUrl = obj.value("downloadUrl").toString();    // 下载链接
    info.changelog = obj.value("changelog").toString();        // 更新日志
    info.forceUpdate = obj.value("forceUpdate").toBool();      // 是否强制更新

    // =========================================================================
    // 第五步：验证数据有效性
    // =========================================================================

    /**
     * 验证规则：
     *   - version 不能为空字符串
     *   - versionCode 必须大于 0
     *
     * 如果验证失败，说明服务器返回的数据格式不正确。
     */
    info.valid = !info.version.isEmpty() && info.versionCode > 0;

    if (!info.valid) {
        emit checkFailed("版本信息格式错误");
        return;
    }

    // =========================================================================
    // 第六步：版本号对比
    // =========================================================================

    /**
     * 双重对比策略：
     *   1. 字符串版本号对比（如 "1.1.0" vs "1.0.0"）
     *   2. 数字版本号对比（如 2 vs 1）
     *
     * 只要有一个条件满足，就认为有新版本。
     * 这样可以防止字符串解析出错时，数字版本号还能正常工作。
     */
    int cmp = compareVersion(info.version, localVersion);

    if (cmp > 0 || info.versionCode > localVersionCode) {
        // 服务器版本更新，发射 newVersionAvailable 信号
        emit newVersionAvailable(info);
    } else {
        // 本地版本已经是最新，发射 alreadyLatest 信号
        emit alreadyLatest();
    }
}

// =============================================================================
// 私有静态方法：比较两个版本号
// =============================================================================

int UpdateChecker::compareVersion(const QString& v1, const QString& v2)
{
    /**
     * 版本号比较算法：
     *
     * 输入："1.2.3" 和 "1.1.5"
     * 步骤：
     *   1. 按 '.' 分割 → ["1", "2", "3"] 和 ["1", "1", "5"]
     *   2. 逐位比较：
     *      - 第 0 位：1 == 1，继续
     *      - 第 1 位：2 > 1，返回正数（v1 更新）
     *
     * 输入："1.0" 和 "1.0.0"
     * 步骤：
     *   1. 按 '.' 分割 → ["1", "0"] 和 ["1", "0", "0"]
     *   2. 逐位比较：
     *      - 第 0 位：1 == 1，继续
     *      - 第 1 位：0 == 0，继续
     *      - 第 2 位：v1 没有第 2 位，补 0 → 0 == 0
     *   3. 全部相等，返回 0
     */

    // QString::split('.') 按 '.' 分割字符串为字符串列表
    QStringList parts1 = v1.split('.');
    QStringList parts2 = v2.split('.');

    // 取两个列表的最大长度，确保遍历到所有位
    int maxLen = qMax(parts1.size(), parts2.size());

    for (int i = 0; i < maxLen; ++i) {
        // 如果当前位不存在，补 0（处理 "1.0" vs "1.0.0" 的情况）
        int num1 = (i < parts1.size()) ? parts1[i].toInt() : 0;
        int num2 = (i < parts2.size()) ? parts2[i].toInt() : 0;

        // 发现不相等，立即返回差值
        if (num1 != num2) {
            return num1 - num2;  // 正数表示 v1 > v2，负数表示 v1 < v2
        }
    }

    // 所有位都相等，版本号相同
    return 0;
}
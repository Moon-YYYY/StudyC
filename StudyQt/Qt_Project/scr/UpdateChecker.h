/******************************************************************************
 * UpdateChecker.h
 * 检查更新模块 - 头文件
 *
 * 功能说明：
 *   从远程服务器（Gitee）获取版本信息文件（version.json），
 *   解析后与本地版本号对比，判断是否需要更新。
 *
 * 涉及的核心 Qt 网络类：
 *   - QNetworkAccessManager : 网络访问管理器，负责发起 HTTP 请求
 *   - QNetworkReply         : 网络响应对象，包含服务器返回的数据
 *   - QNetworkRequest       : 网络请求对象，配置请求参数（URL、Header 等）
 *   - QJsonDocument         : JSON 文档类，用于解析 JSON 字符串
 *   - QJsonObject           : JSON 对象类，用于读取 JSON 中的键值对
 ******************************************************************************/

#ifndef UPDATECHECKER_H       // 头文件保护宏：防止同一个头文件被多次包含
#define UPDATECHECKER_H       // 定义头文件保护宏（与 #ifndef 配对使用）

#include <QObject>                  // Qt 对象基类，支持信号槽机制
#include <QNetworkAccessManager>     // 网络访问管理器，发起 HTTP 请求
#include <QNetworkReply>             // 网络响应对象，接收服务器返回数据
#include <QJsonDocument>             // JSON 文档解析类
#include <QJsonObject>               // JSON 对象类，读取键值对

/**
 * @struct UpdateInfo
 * @brief 从服务器获取的版本信息结构体
 *
 * 这个结构体对应 Gitee 上 version.json 文件的内容格式。
 * 服务器返回的 JSON 示例：
 * {
 *     "version": "1.1.0",           // 版本号字符串
 *     "versionCode": 2,              // 版本号数字，用于比较大小
 *     "downloadUrl": "https://...",  // 下载链接
 *     "changelog": "修复了bug",       // 更新日志
 *     "forceUpdate": false           // 是否强制更新
 * }
 */
struct UpdateInfo {
    QString version;        // 最新版本号，如 "1.1.0"
    int versionCode;        // 版本号数字，用于比较（数字越大版本越新）
    QString downloadUrl;    // 新版本安装包的下载链接
    QString changelog;      // 更新日志，显示给用户看
    bool forceUpdate;       // 是否强制更新（true 则用户必须更新才能使用）
    bool valid;             // 数据是否解析成功（解析失败时为 false）
};

/**
 * @class UpdateChecker
 * @brief 检查更新类
 *
 * 工作流程：
 *   1. 调用 checkUpdate() 发起网络请求
 *   2. 等待服务器响应
 *   3. 解析 JSON 数据
 *   4. 对比本地版本和服务器版本
 *   5. 发射信号通知调用方结果
 *
 * 使用方式：
 *   UpdateChecker* checker = new UpdateChecker(this);
 *   connect(checker, &UpdateChecker::newVersionAvailable, this, &MyClass::onNewVersion);
 *   checker->checkUpdate("1.0.0", 1);
 */
class UpdateChecker : public QObject
{
    Q_OBJECT                          // 启用 Qt 的元对象系统（信号槽必需）

public:
    /**
     * @brief 构造函数
     * @param parent 父对象，用于内存管理（Qt 父子对象机制）
     *
     * 在构造函数中会创建 QNetworkAccessManager 实例，
     * 并连接其 finished 信号到槽函数 onReplyFinished()。
     */
    explicit UpdateChecker(QObject *parent = nullptr);

    /**
     * @brief 开始检查更新
     * @param currentVersion 当前应用版本号，如 "1.0.0"
     * @param currentVersionCode 当前应用版本号数字，如 1
     *
     * 此方法会异步发起 HTTP GET 请求，不会阻塞界面。
     * 请求完成后通过信号通知结果。
     */
    void checkUpdate(const QString& currentVersion, int currentVersionCode);

signals:
    /**
     * @brief 发现新版本的信号
     * @param info 包含新版本详细信息的结构体
     *
     * 当服务器版本号大于本地版本号时发射此信号。
     * 调用方应连接此信号，弹出更新提示对话框。
     */
    void newVersionAvailable(const UpdateInfo& info);

    /**
     * @brief 已是最新版本的信号
     *
     * 当服务器版本号小于等于本地版本号时发射此信号。
     * 调用方可以提示用户"当前已是最新版本"。
     */
    void alreadyLatest();

    /**
     * @brief 检查失败的信号
     * @param error 错误描述信息
     *
     * 当网络请求失败或 JSON 解析失败时发射此信号。
     * 常见原因：无网络连接、服务器返回 404、JSON 格式错误等。
     */
    void checkFailed(const QString& error);

private slots:
    /**
     * @brief 网络请求完成的槽函数
     * @param reply 网络响应对象，包含服务器返回的数据和状态信息
     *
     * 这是 QNetworkAccessManager::finished 信号的槽函数。
     * 当 HTTP 请求完成（无论成功或失败）时自动调用。
     * 在此函数中解析响应数据，判断更新状态，并发射相应信号。
     */
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* networkManager;  // 网络访问管理器实例，负责发起 HTTP 请求
    QString localVersion;                    // 缓存的本地版本号字符串
    int localVersionCode;                    // 缓存的本地版本号数字

    /**
     * @brief 比较两个版本号的大小
     * @param v1 第一个版本号，如 "1.2.3"
     * @param v2 第二个版本号，如 "1.1.5"
     * @return 正数表示 v1 > v2，负数表示 v1 < v2，0 表示相等
     *
     * 比较逻辑：
     *   1. 按 '.' 分割版本号为数字数组
     *   2. 从左到右逐位比较
     *   3. 位数不足时补 0
     *
     * 示例：
     *   compareVersion("1.2.0", "1.1.5") 返回 >0（1.2.0 更新）
     *   compareVersion("1.0.0", "1.0.0") 返回 0（版本相同）
     *   compareVersion("0.9.0", "1.0.0") 返回 <0（0.9.0 更旧）
     */
    static int compareVersion(const QString& v1, const QString& v2);
};

#endif // UPDATECHECKER_H
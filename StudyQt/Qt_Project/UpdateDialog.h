/******************************************************************************
 * UpdateDialog.h
 * 更新提示弹窗 - 头文件
 *
 * 本文件定义了更新提示对话框的界面布局和交互逻辑。
 *
 * 功能说明：
 *   当检测到新版本时，弹出此对话框提示用户。
 *   对话框包含更新日志、下载按钮和取消按钮。
 *
 * 界面特点：
 *   - 无边框圆角窗口（FramelessWindowHint）
 *   - 窗口可拖动（mousePressEvent/mouseMoveEvent）
 *   - 自定义半透明背景
 ******************************************************************************/

#ifndef UPDATEDIALOG_H        // 头文件保护宏：防止同一个头文件被多次包含
#define UPDATEDIALOG_H        // 定义头文件保护宏（与 #ifndef 配对使用）

#include <QDialog>            // Qt 对话框基类
#include "UpdateChecker.h"    // 更新检查器头文件：包含 UpdateInfo 结构体的完整定义
#include <QLabel>             // 文本标签，用于显示更新日志
#include <QPushButton>        // 按钮控件，用于"下载"和"取消"
#include <QVBoxLayout>        // 垂直布局管理器
#include <QHBoxLayout>        // 水平布局管理器
#include <QMouseEvent>        // 鼠标事件，实现窗口拖动


/**
 * @class UpdateDialog
 * @brief 更新提示对话框类
 *
 * 工作流程：
 *   1. 构造函数接收 UpdateInfo 结构体作为参数
 *   2. 根据版本信息设置对话框的显示内容
 *   3. 用户点击"下载"按钮后，发射 downloadRequested 信号
 *   4. 用户点击"取消"或"更新"按钮（强制更新时无取消按钮）
 */
class UpdateDialog : public QDialog
{
    Q_OBJECT                          // 启用 Qt 的元对象系统（信号槽必需）

public:
    /**
     * @brief 构造函数
     * @param info 版本信息结构体
     * @param parent 父窗口指针
     *
     * 根据传入的版本信息构建对话框界面。
     */
    explicit UpdateDialog(const UpdateInfo& info, QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~UpdateDialog();

signals:
    /**
     * @brief 下载请求信号
     * @param url 下载链接
     *
     * 当用户点击"下载"按钮时发射此信号。
     * 调用方（通常是 MainWindow）应连接此信号，
     * 创建 ApkDownloader 开始下载。
     */
    void downloadRequested(const QString& url);

protected:
    /**
     * @brief 自定义绘制事件处理器
     * @param event 绘制事件对象
     *
     * 重写 QWidget 的 paintEvent 方法，实现自定义绘制：
     *   - 绘制顶部渐变装饰条（增加视觉效果）
     *   - 绘制半透明背景
     *
     * 绘制流程：
     *   1. 调用父类 paintEvent 完成基本绘制
     *   2. 创建 QPainter 对象（绘制器）
     *   3. 设置抗锯齿渲染（Antialiasing）
     *   4. 绘制背景圆角矩形
     *   5. 绘制顶部渐变装饰条
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 鼠标按下事件处理器
     * @param event 鼠标事件对象
     *
     * 记录按下位置，用于窗口拖动。
     * 按住窗口任意位置即可拖动。
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标移动事件处理器
     * @param event 鼠标事件对象
     *
     * 根据鼠标移动距离更新窗口位置。
     * 在 mousePressEvent 中记录拖动起始点。
     */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    /**
     * @brief 初始化界面布局
     *
     * 设置对话框内的所有控件：
     *   - 标题标签（"发现新版本"）
     *   - 版本号标签（"v1.1.0"）
     *   - 更新日志文本区域
     *   - 按钮区域（下载/取消）
     *
     * 布局结构：
     *   ┌─────────────────────────┐
     *   │   ██████████████████    │  ← 绿色渐变装饰条（paintEvent 绘制）
     *   │   发现新版本！           │
     *   │   v1.1.0                │
     *   │   ───────────────       │
     *   │   更新内容：             │
     *   │   1. 修复了 XX Bug      │
     *   │   2. 新增了 XX 功能     │
     *   │   ───────────────       │
     *   │      [取消]  [下载]     │
     *   └─────────────────────────┘
     */
    void setupUI();

    UpdateInfo m_info;               // 缓存的版本信息
    QLabel *m_titleLabel;            // 标题标签："发现新版本！"
    QLabel *m_versionLabel;          // 版本号标签
    QLabel *m_changelogLabel;        // 更新日志内容标签
    QPushButton *m_downloadButton;   // 下载按钮
    QPushButton *m_cancelButton;     // 取消按钮（强制更新时隐藏）
    QPoint m_dragPosition;           // 窗口拖动起始位置
};

#endif // UPDATEDIALOG_H
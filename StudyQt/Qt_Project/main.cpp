/*
 * ============================================================================
 * 文件：main.cpp
 * 描述：应用程序入口点
 *
 * Qt 应用程序的标准入口文件。
 * main() 函数是整个程序的起点，负责创建 QApplication 对象和主窗口，
 * 然后进入 Qt 的事件循环（Event Loop），等待并分发用户操作和系统事件。
 *
 * Qt 的事件循环机制（QCoreApplication::exec()）：
 *   - 是一个无限循环，不断从系统消息队列中取出事件（鼠标点击、键盘输入、窗口重绘等）
 *   - 将事件分发到对应的 QObject 对象的 event() 函数
 *   - 直到调用 QCoreApplication::quit() 或窗口关闭时退出循环
 * ============================================================================
 */

// 包含主窗口类的头文件，MainWindow 继承自 QMainWindow，是应用的主界面容器
#include "mainwindow.h"

/*
 * QApplication 是 Qt GUI 应用的核心类。
 * 它管理全局的应用程序资源、字体、调色板、样式表等。
 * 必须在使用任何 QWidget 控件之前创建 QApplication 对象。
 * argc 和 argv 用于解析命令行参数（Qt 特有的参数如 -style, -stylesheet 等）。
 */
#include <QApplication>

/**
 * @brief  应用程序入口函数
 *
 * 程序执行的第一个函数，由操作系统在进程启动时调用。
 *
 * @param  argc  命令行参数的个数（argument count），从操作系统传入
 * @param  argv  命令行参数的指针数组（argument vector），每个元素是一个 C 风格字符串
 *               例如 argv[0] 总是程序本身的路径
 *
 * @return int  应用程序退出码：
 *               0  表示正常退出
 *              非0 表示异常退出（如崩溃、错误等）
 */
int main(int argc, char *argv[])
{
    /*
     * 创建 QApplication 对象。
     *
     * QApplication(argc, argv) 构造函数会：
     *   1. 初始化 Qt 的全局资源
     *   2. 解析标准 Qt 命令行参数（如 -style, -stylesheet, -qmljsdebugger 等）
     *   3. 从 argv 中移除已识别的 Qt 参数，修改 argc 和 argv
     *   4. 设置应用程序的 organizationName、applicationName 等（用于 QSettings）
     *
     * 在 Android 平台上，argc 通常为 0，argv 为 nullptr，
     * 由 Android 的 Activity 生命周期管理应用的启动。
     *
     * a 是一个栈上对象，它的生命周期覆盖整个程序运行期间。
     * 在 return 语句之前析构，确保 Qt 资源被正确清理。
     */
    QApplication a(argc, argv);

    /*
     * 创建主窗口对象。
     *
     * MainWindow 继承自 QMainWindow，是应用的主界面容器。
     * QMainWindow 提供了：
     *   - 菜单栏（Menu Bar）：通常在窗口顶部
     *   - 多个工具栏（Tool Bars）：可拖拽停靠
     *   - 中央控件区域（Central Widget）：使用 setCentralWidget() 设置
     *   - 状态栏（Status Bar）：窗口底部显示提示信息
     *   - 停靠窗口（Dock Widgets）：可浮动或停靠在主窗口边缘
     *
     * 这里没有指定父窗口（parent = nullptr），
     * 因为 MainWindow 本身就是顶层窗口（Top-Level Widget）。
     * 在 Qt 的父子对象树中，顶层窗口的父对象为 nullptr。
     *
     * w 是栈上对象，在 main() 函数结束时自动析构。
     * 但在 Qt 中，通常建议在堆上创建窗口（new MainWindow），
     * 或确保窗口关闭后不重复使用。
     */
    MainWindow w;

    /*
     * 以全屏模式显示主窗口。
     *
     * showFullScreen() 是 QWidget 提供的成员函数，其效果：
     *   1. 将窗口设置为全屏状态（覆盖整个屏幕，包括系统任务栏和状态栏）
     *   2. 在 Windows/Linux 上会隐藏任务栏
     *   3. 在 Android 上会覆盖状态栏（显示电池、时间等的区域）
     *   4. 调用后窗口会获得焦点（成为活动窗口）
     *
     * 在 Android 平台上，状态栏默认会覆盖在应用窗口之上，
     * 导致应用内容在状态栏区域显示不正常（出现黑色背景）。
     * showFullScreen() 可以解决这个问题：
     *   - 让窗口填满整个屏幕
     *   - 状态栏会浮在应用内容之上（非侵入模式）
     *   - 应用的内容可以绘制到状态栏下方区域
     *
     * 与 show() 的区别：
     *   show()        : 以正常窗口模式显示，Android 上状态栏区域为黑色
     *   showFullScreen(): 全屏模式，状态栏浮在应用上方
     *   showMaximized(): 最大化显示，但仍保留窗口标题栏和边框
     *   showMinimized(): 最小化到任务栏/后台
     *
     * 注意：在全屏模式下，用户需要从屏幕顶部下滑才能拉出状态栏。
     * showFullScreen() 不会有动画过渡效果，如果需要动画过渡，
     * 可以结合 QPropertyAnimation 设置窗口的 geometry 属性。
     */
    w.showFullScreen();

    /*
     * 进入 Qt 事件循环，阻塞等待事件发生。
     *
     * QCoreApplication::exec() 是静态函数，其工作原理：
     *   1. 进入主事件循环（Main Event Loop）
     *   2. 循环内部不断从事件队列中取出事件
     *   3. 通过 QObject::event() 将事件分发到对应的目标对象
     *   4. 事件处理完成后继续等待下一个事件
     *
     * 事件类型包括：
     *   - QEvent::MouseButtonPress  : 鼠标按下
     *   - QEvent::KeyPress           : 键盘按键
     *   - QEvent::Paint              : 窗口需要重绘
     *   - QEvent::Timer              : QTimer 定时器触发
     *   - QEvent::Resize             : 窗口大小改变
     *   - 用户自定义事件
     *
     * 此函数只有在以下情况才会返回：
     *   - 调用 QCoreApplication::quit() 或 QApplication::quit()
     *   - 所有顶层窗口都关闭（取决于 quitOnLastWindowClosed 属性，默认 true）
     *
     * 返回值是应用程序的退出码，当 exec() 返回后，
     * main() 函数也随即返回，程序结束。
     */
    return QCoreApplication::exec();
}
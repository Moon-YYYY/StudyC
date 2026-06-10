/*
 * ============================================================================
 * 文件：mainwindow.h
 * 描述：主窗口类的头文件
 *
 * MainWindow 是整个应用程序的主界面容器，继承自 QMainWindow。
 * QMainWindow 是 Qt 框架中用于构建应用程序主窗口的基类，它提供了：
 *   1. 菜单栏（Menu Bar）——使用 setMenuBar() 设置
 *   2. 工具栏（Tool Bars）——使用 addToolBar() 添加
 *   3. 中央控件（Central Widget）——使用 setCentralWidget() 设置
 *   4. 状态栏（Status Bar）——使用 statusBar() 获取
 *   5. 停靠窗口（Dock Widgets）——使用 addDockWidget() 添加
 *
 * 本应用没有使用 QMainWindow 的标准布局机制，而是通过绝对定位
 * 将各个子控件放置在窗口上，实现自定义的 UI 布局。
 *
 * 主控件的层级关系：
 *   MainWindow (QMainWindow)
 *     ├── KeyBoardUI (CircleButton) —— 键盘背景容器，占据全屏
 *     │     ├── 数字按钮 (QPushButton * 12)
 *     │     ├── 运算符按钮 (QPushButton * 5)
 *     │     └── 功能按钮 (QPushButton * 4)
 *     ├── SettingButton (CircleButton) —— 汉堡菜单按钮，位于右上角
 *     ├── calculatorwindow (CalculatorWindow) —— 计算结果显示区域
 *     ├── menu (menuButton) —— 侧边栏菜单面板
 *     ├── settingsWidget (SettingsWidget) —— 设置页面
 *     ├── updateChecker (UpdateChecker) —— 网络更新检查器
 *     └── stackedWidget (QStackedWidget) —— 页面切换容器
 *
 * 作者：CalculationTools Team
 * ============================================================================
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
 * 包含头文件说明：
 *
 * QMainWindow 是 Qt Widgets 模块中专门用于创建主窗口的类。
 * 它继承自 QWidget，并在此基础上增加了菜单栏、工具栏、状态栏、
 * 停靠窗口等主窗口特有的功能组件。
 *
 * QStackedWidget 继承自 QFrame，是一个页面切换容器控件。
 * 它管理一组子页面（QWidget），在同一时刻只显示其中一个页面。
 * 类似于 Android 中的 ViewPager 或 iOS 中的 UIPageViewController。
 * 常用 API：
 *   - addWidget(QWidget*)  ：添加一个页面，返回该页面的索引
 *   - currentIndex()       ：获取当前显示的页面索引
 *   - setCurrentIndex(int) ：切换到指定索引的页面
 */
#include <QMainWindow>
#include <QStackedWidget>

/*
 * 包含自定义控件的头文件：
 *   CircleButton    ：圆形按钮类，可呈现为圆形或圆角矩形，支持点击动画效果
 *   CalculatorWindow：计算器显示区域控件，显示用户输入的数字和运算结果
 *   menuButton      ：侧边栏菜单面板控件，从屏幕左侧滑入，包含菜单选项
 *   SettingsWidget  ：设置页面控件，提供背景颜色修改等功能
 */
#include "CircleButton.h"
#include "CalculatorWindow.h"
#include "menuButton.h"
#include "SettingsWidget.h"

/*
 * 前向声明（Forward Declaration）：
 *
 * 告诉编译器 UpdateChecker 是一个类，但不需要知道它的完整定义。
 * 在头文件中，如果只需要声明指针或引用，使用前向声明可以：
 *   1. 减少头文件之间的依赖关系
 *   2. 减少编译时间
 *   3. 避免循环包含的问题
 *
 * 实际的类定义在 UpdateChecker.h 中，在 mainwindow.cpp 中包含即可。
 */
class UpdateChecker;

/*
 * Qt 命名空间的前向声明。
 * QT_BEGIN_NAMESPACE 和 QT_END_NAMESPACE 是 Qt 提供的宏，
 * 用于将 Qt 内部的类放在适当的命名空间中（通常在 Qt 命名空间中）。
 *
 * 在 namespace Ui 中声明 MainWindow 类，这是 Qt Designer
 * 自动生成的 UI 类的命名空间约定。
 * ui_mainwindow.h 文件中会自动生成：
 *   namespace Ui {
 *       class MainWindow;
 *   }
 * 这个 UI 类负责管理 .ui 文件中定义的控件（如 QPushButton、QLabel 等）。
 */
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief 主窗口类
 *
 * 继承自 QMainWindow，是应用程序的核心控制器。
 * 负责初始化所有 UI 组件、管理页面切换、处理用户交互事件。
 *
 * 使用了 Q_OBJECT 宏，这是所有使用 Qt 信号槽机制的类必须包含的。
 * Q_OBJECT 宏会为类添加：
 *   1. metaObject() 函数 —— 返回类的元对象信息
 *   2. tr() 函数 —— 用于字符串国际化翻译
 *   3. 信号槽机制所需的元数据
 *   4. 动态属性系统支持
 */
class MainWindow : public QMainWindow
{
    /*
     * Q_OBJECT 是 Qt 的元对象编译器（MOC，Meta-Object Compiler）
     * 识别类的标记。
     *
     * 当 MOC 处理包含 Q_OBJECT 宏的头文件时，会生成对应的
     * moc_xxx.cpp 文件，其中包含：
     *   - 信号和槽的元数据表
     *   - 类型信息（类名、父类等）
     *   - 属性系统支持代码
     *
     * 如果没有 Q_OBJECT 宏，将无法使用信号槽机制。
     */
    Q_OBJECT

public:
    /*
     * 构造函数。
     *
     * explicit 关键字：防止编译器进行隐式类型转换。
     * 例如没有 explicit 时，以下代码可以编译通过：
     *   MainWindow w = 42;  // 将 int 隐式转换为 MainWindow*
     * 加上 explicit 后，这种隐式转换会被禁止。
     *
     * QWidget *parent 参数说明：
     *   - 设置父对象，实现 Qt 的对象树内存管理
     *   - 父对象销毁时，子对象也会被自动销毁
     *   - nullptr 表示这是一个顶层窗口，没有父对象
     *   - 默认值为 nullptr，即如果不传参就是顶层窗口
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /*
     * 析构函数。
     *
     * override 关键字（C++11）：
     *   告诉编译器这个函数重写（覆盖）了基类的虚函数。
     *   如果基类中没有对应的虚函数，编译器会报错。
     *   这可以避免因拼写错误导致的重写失败。
     *
     *   QMainWindow 的析构函数是虚函数，所以这里用 override
     *   确保正确重写。
     */
    ~MainWindow() override;

private:
    /*
     * ========================================================================
     * 成员变量说明
     * ========================================================================
     *
     * 注意：这里只是声明指针，不能在声明时进行 new 操作。
     * 因为此时 MainWindow 对象还在构造过程中，this 指针指向不完整，
     * 子控件的构造函数如果需要父对象，传入不完整的 this 指针
     * 会导致未定义行为（undefined behavior）。
     *
     * 所有子控件对象（new）都在构造函数体中进行创建。
     * ========================================================================
     */

    /*
     * 键盘背景容器（CircleButton 类型用作容器）。
     *
     * CircleButton 本意是"圆形按钮"，但这里利用它的容器能力：
     *   将 CircleButton 的大小设置为全屏，作为所有按键的父容器。
     *   它负责管理键盘按键的布局和绘制。
     *
     * CircleButton 内部会创建 21 个 QPushButton（0-9数字、+、-、×、÷、
     * =、.、正负号、%、C、CE、MC、MR、M+、M-），
     * 并在 keyboardUI() 函数中进行布局排列。
     */
    CircleButton* KeyBoardUI;

    /*
     * 计算器显示窗口——用于显示用户输入的数字和运算结果。
     * CalculatorWindow 继承自 QWidget，内部包含一个 QLineEdit
     * 作为显示文本框，并持有 calculateWay 计算逻辑对象。
     *
     * 它位于键盘区域上方（y = 120），宽度与屏幕宽度一致，
     * 高度为剩余空间。
     */
    CalculatorWindow* calculatorwindow;

    /*
     * 侧边栏菜单的入口按钮（汉堡图标按钮）。
     * 以"rectangle"（圆角矩形）样式呈现，位于屏幕左上角。
     * 点击后触发 menu 侧边栏的弹出动画。
     *
     * 使用 CircleButton 构造函数：
     *   CircleButton(0, 20, "rectangle", this)
     *   参数说明：x=0, y=20, 形状=rectangle, 父对象=this
     */
    CircleButton* SettingButton;

    /*
     * 侧边栏菜单面板——从屏幕左侧滑入。
     * menuButton 包含三个部分：
     *   1. 半透明黑色蒙层（覆盖在主界面之上，防止用户操作）
     *   2. 白色菜单面板（包含菜单选项按钮）
     *   3. 自定义动画引擎（实现平滑的滑入/滑出动画）
     *
     * 菜单按钮包括："关于"、"检查更新"、"设置"、"版本"、"更新内容"
     */
    menuButton* menu;

    /*
     * 更新检查器——负责检查是否有新版本可用。
     * UpdateChecker 使用 QNetworkAccessManager 发起 HTTP 请求，
     * 从 Gitee Raw 获取 version.json 文件，解析后与本地版本对比。
     *
     * 涉及的网络请求是异步的，通过信号槽机制返回结果：
     *   - newVersionAvailable(UpdateInfo) ：发现新版本
     *   - alreadyLatest()                  ：已是最新
     *   - checkFailed(QString)             ：检查失败
     */
    UpdateChecker* updateChecker;

    /*
     * 设置页面控件。
     * SettingsWidget 继承自 QWidget，是一个全屏的设置页面。
     * 包含：
     *   - 返回按钮：点击后回到主界面
     *   - "设置"标题
     *   - "修改背景颜色"配置行（带有颜色选择器）
     *
     * 首次创建后，通过滑入动画显示；关闭时通过滑出动画隐藏。
     * 懒加载模式：只有用户第一次点击"设置"时才创建。
     */
    SettingsWidget* settingsWidget;

    /*
     * 页面切换容器。
     * QStackedWidget 允许将多个子页面叠加管理，
     * 但同一时刻只显示一个页面。
     *
     * 在这个应用中，由于设置页面是通过动画滑入、单独的控件方式
     * 实现的，QStackedWidget 实际上是预留的，以备后续需要
     * 管理多个独立页面时使用。
     *
     * QStackedWidget 工作原理：
     *   - 将所有子控件按照添加顺序排列（索引从 0 开始）
     *   - 使用 setCurrentIndex() 或 setCurrentWidget() 切换页面
     *   - 切换时可以有动画过渡（需要额外实现）
     *   - 未显示的页面不接收用户输入事件
     */
    QStackedWidget* stackedWidget;

    /*
     * Qt Designer 自动生成的 UI 类指针。
     *
     * Ui::MainWindow 类定义在 ui_mainwindow.h 中，
     * 由 uic（UI Compiler）工具根据 .ui 文件自动生成。
     * .ui 文件是用 Qt Designer 可视化设计的界面布局文件（XML 格式）。
     *
     * ui->setupUi(this) 会：
     *   1. 根据 .ui 文件中的描述创建所有控件
     *   2. 设置控件的属性（位置、大小、文字、样式等）
     *   3. 将这些控件添加为当前窗口的子控件
     *   4. 设置窗口的大小和布局
     *
     * 注意：由于我们在代码中手动布局了所有控件，
     * .ui 文件中可能没有太多内容，但是 setupUi() 仍然需要调用。
     */
    Ui::MainWindow *ui;

    /*
     * 自动/手动检查更新的标记变量。
     *
     * true  = 启动时的自动检查：
     *         仅当发现新版本时弹窗提示用户
     *         已是最新版本或网络失败时静默处理，不打扰用户
     *
     * false = 菜单中的手动检查（用户点击"检查更新"按钮）：
     *         无论结果如何都弹窗提示用户
     *         新增版本弹更新对话框
     *         已是最新弹"已是最新"提示
     *         失败弹"检查失败"警告
     *
     * 使用场景：
     *   - 每次启动程序时，构造函数中设置为 true，调用 checkUpdate()
     *   - 用户点击菜单"检查更新"时，设置为 false，调用 checkUpdate()
     *   - 信号槽回调中根据此值决定是否弹窗
     */
    bool m_isAutoCheckup;
};

/*
 * 头文件结束保护符。
 * #ifndef/#define/#endif 是 C++ 中防止头文件被多次包含的标准做法。
 * 当预处理器第一次处理这个文件时，MAINWINDOW_H 还没有被定义，
 * 所以会继续处理 #define 和之后的代码。
 * 当第二次处理时，MAINWINDOW_H 已经定义，#ifndef 条件为假，
 * 跳过整个文件内容。
 * 这样可以避免重复定义导致的编译错误。
 */
#endif // MAINWINDOW_H
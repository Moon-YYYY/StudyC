/*
 * ============================================================================
 * 文件：mainwindow.cpp
 * 描述：主窗口类的实现文件
 *
 * 本文件实现了 MainWindow 类的构造函数、析构函数以及所有的交互逻辑。
 * 是应用程序的核心控制逻辑所在。
 *
 * 构造函数执行流程概览：
 *   1. 调用 ui->setupUi(this) —— 初始化 .ui 文件中定义的控件
 *   2. 获取屏幕尺寸 —— 用于自适应各种屏幕分辨率（手机、平板等）
 *   3. 加载保存的背景颜色 —— 从 QSettings 中读取
 *   4. 设置窗口背景颜色 —— 使用 QPalette
 *   5. 创建键盘容器（CircleButton）—— 绘制计算器键盘按钮
 *   6. 创建设置按钮（汉堡图标）—— 位于屏幕左上角
 *   7. 创建计算器显示窗口 —— 显示输入和结果
 *   8. 创建侧边栏菜单面板 —— 从左侧滑入
 *   9. 初始化更新检查器 —— 连接信号槽
 *   10. 启动自动检查更新
 *   11. 连接菜单按钮的点击处理
 * ============================================================================
 */

/*
 * 包含主窗口类的头文件。
 * 这里包含了 mainwindow.h，它会间接包含所有需要的自定义控件的头文件。
 */
#include "mainwindow.h"

/*
 * 包含 ui_mainwindow.h —— 由 uic（UI Compiler）编译器自动生成。
 * uic 读取 mainwindow.ui（XML 格式的界面文件），
 * 生成对应的 C++ 代码，定义 Ui::MainWindow 类。
 * 这个类包含了 .ui 文件中所有控件的创建和布局代码。
 */
#include "ui_mainwindow.h"

/*
 * 包含自定义控件的头文件（部分已在 mainwindow.h 中通过前向声明引用，
 * 这里包含完整的定义，因为需要调用这些类的成员函数）。
 */
#include "CircleButton.h"       // 圆形按钮/容器类
#include "CalculatorWindow.h"   // 计算器显示窗口
#include "UpdateChecker.h"      // 更新检查器（包含 UpdateInfo 结构体定义）
#include "UpdateDialog.h"       // 更新提示弹窗
#include "SettingsWidget.h"     // 设置页面

/*
 * Qt 核心模块的头文件。
 *
 * QString     ：Qt 的字符串类，使用 Unicode 编码，支持隐式共享（Copy-on-Write）
 * QScreen     ：屏幕信息类，用于获取屏幕尺寸、DPI、刷新率等信息
 * QRect       ：矩形区域类，表示一个由 (x, y, width, height) 定义的矩形
 * QApplication：应用管理类，提供了全局的设置和事件循环
 * QPushButton ：标准按钮控件
 * QMessageBox ：消息对话框，用于显示提示信息、警告、错误等
 * QStackedWidget：页面切换容器
 * QPalette    ：调色板类，管理控件的颜色方案（背景色、前景色、文本色等）
 * QPropertyAnimation：属性动画类，用于实现控件属性的平滑过渡动画
 * QEasingCurve：缓动曲线类，定义动画的变化速率（匀速、加速、弹性等）
 */
#include <QString>
#include <QScreen>
#include <QRect>
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget>
#include <QPalette>
#include <QPropertyAnimation>
#include <QEasingCurve>

/*
 * ========================================================================
 * 应用版本号定义
 * ========================================================================
 *
 * #define 是 C++ 预处理器指令，在编译之前进行文本替换。
 * 这里定义了两个宏：
 *
 * APP_VERSION      ：版本名称，以字符串形式表示，用于显示给用户
 *                    格式为 "主版本号.次版本号.修订号"
 *
 * APP_VERSION_CODE ：版本代码，以整数形式表示，用于版本对比
 *                    每次发布新版本时递增，确保新版本号 > 旧版本号
 *
 * 这两个宏被传递给 UpdateChecker::checkUpdate()，
 * 与服务端 version.json 中的 versionName 和 versionCode 进行对比。
 * 对比规则：
 *   - 如果 server.versionCode > APP_VERSION_CODE，说明有新版本
 *   - 如果 server.versionCode <= APP_VERSION_CODE，说明已是最新
 */
#define APP_VERSION "1.5.0"
#define APP_VERSION_CODE 17

////////test注释用来测试，可忽略/////
/*
 * 构造函数：MainWindow::MainWindow
 *
 * 创建主窗口对象并初始化所有子控件。
 * 这个函数在 main() 函数中通过 "MainWindow w;" 或 "new MainWindow()" 调用。
 *
 * 执行流程（按代码顺序）：
 *   1. 调用基类 QMainWindow 的构造函数初始化框架
 *   2. 初始化成员变量（初始化列表）
 *   3. 调用 setupUi 加载 .ui 文件
 *   4. 获取屏幕尺寸
 *   5. 加载保存的颜色设置
 *   6. 设置背景颜色
 *   7. 创建键盘区域
 *   8. 创建设置按钮
 *   9. 创建计算器显示窗口
 *   10. 创建侧边栏菜单
 *   11. 初始化更新检查器
 *   12. 连接信号槽
 *   13. 启动自动检查更新
 *
 * @param parent 父窗口指针，默认为 nullptr（顶层窗口）
 */
MainWindow::MainWindow(QWidget *parent)
    /*
     * 构造函数初始化列表（Constructor Initializer List）：
     *
     * 在进入构造函数体之前，先初始化基类和成员变量。
     * 语法：ClassName(params) : baseClass(params), member1(value1), member2(value2)
     *
     * 使用初始化列表的优势：
     *   1. 更高效：直接调用拷贝构造函数，而不是先默认构造再赋值
     *   2. 必须使用：初始化 const 成员、引用成员、没有默认构造函数的成员
     *   3. 顺序：按照成员变量在类中声明的顺序执行，而不是初始化列表中的顺序
     *
     * QMainWindow(parent) —— 调用基类的构造函数，将 parent 传递给它
     * ui(new Ui::MainWindow) —— 创建 UI 类实例
     * settingsWidget(nullptr) —— 初始化设置页面指针为 nullptr（懒加载）
     */
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingsWidget(nullptr)
{
    /*
     * ui->setupUi(this)：
     *
     * 这是 Qt Designer 自动生成的关键函数调用。
     * setupUi() 由 uic（User Interface Compiler）根据 .ui 文件自动生成，
     * 它执行以下操作：
     *   1. 设置对象的名称（objectName）
     *   2. 根据 .ui 中的描述创建所有子控件
     *   3. 设置控件属性（geometry, text, font, stylesheet 等）
     *   4. 建立信号槽连接（.ui 文件中定义的）
     *   5. 设置窗口的标题（windowTitle）
     *   6. 将子控件添加到 Qt 对象树中（建立父子关系）
     *
     * 注意：setupUi() 必须在所有其他 UI 初始化操作之前调用，
     * 因为它建立了窗口的基本框架。
     */
    ui->setupUi(this);

    /*
     * ========================================================================
     * 获取屏幕尺寸
     * ========================================================================
     *
     * QApplication::primaryScreen()：
     *   获取主屏幕（primary screen）的信息。
     *   在 Android 设备上，通常只有一个屏幕。
     *   在多显示器系统中，主屏幕是系统设置中指定的主要显示器。
     *
     * QScreen::geometry()：
     *   返回一个 QRect 对象，表示屏幕的尺寸和位置。
     *   QRect 包含四个属性：x, y（左上角坐标）, width, height（宽度和高度）。
     *   返回值以像素（pixels）为单位。
     *
     * 获取到的屏幕尺寸用于自适应布局：
     *   将所有控件的大小设置为相对于屏幕尺寸的比例。
     *   这样在不同分辨率的设备上都能正确显示。
     */
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    /*
     * 设置窗口大小为屏幕尺寸。
     *
     * QWidget::resize(int width, int height)：
     *   设置控件的宽度和高度。
     *   这是 QWidget 的常用函数之一，用于手动控制控件大小。
     *   如果使用了布局管理器（QLayout），resize() 的效果会被布局覆盖。
     *
     * 这里将 MainWindow 的大小设置为全屏尺寸。
     * 结合 main.cpp 中的 showFullScreen()，可以实现全屏显示。
     */
    this->resize(screenWidth, screenHeight);

    /*
     * ========================================================================
     * 加载保存的背景颜色设置
     * ========================================================================
     *
     * SettingsWidget::loadSavedColor() 是静态函数（static function），
     * 不需要创建 SettingsWidget 实例即可调用。
     *
     * 静态函数的特点：
     *   1. 属于类而不是对象
     *   2. 可以直接通过类名调用（ClassName::functionName()）
     *   3. 不能访问非静态成员变量（因为没有 this 指针）
     *   4. 常用于工具函数或单例模式
     *
     * loadSavedColor() 内部使用 QSettings 读取持久化存储：
     *   QSettings 是 Qt 的配置读写类，底层实现取决于平台：
     *     - Windows: 注册表（HKEY_CURRENT_USER\Software）
     *     - Linux: ~/.config/ 目录下的配置文件
     *     - Android: SharedPreferences
     *     - macOS: plist 文件
     *   存储的数据是键值对（Key-Value）形式。
     */
    QColor savedBgColor = SettingsWidget::loadSavedColor();

    /*
     * ========================================================================
     * 设置背景颜色
     * ========================================================================
     *
     * QPalette（调色板）是 Qt 中管理颜色方案的核心类。
     * 它定义了一组颜色角色（Color Role），每个角色对应一个颜色组。
     *
     * 颜色组（Color Group）有三种：
     *   - Active（活动组） ：当前活动窗口使用的颜色
     *   - Inactive（非活动组）：非活动窗口使用的颜色
     *   - Disabled（禁用组）：禁用状态控件使用的颜色
     *
     * 常见的颜色角色（Color Role）：
     *   QPalette::Window   ：背景色（适用于 QWidget、QMainWindow 等）
     *   QPalette::Text     ：前景文本色
     *   QPalette::Button   ：按钮背景色
     *   QPalette::Highlight：选中项的背景色
     *   QPalette::Base     ：输入框、列表、表格等的背景色
     *
     * setBrush(QPalette::Window, savedBgColor)：
     *   为 Window 角色设置背景颜色。
     *   注意：这里使用的是 setBrush() 而不是 setColor()，
     *   setBrush 可以接受 QColor、QPixmap、QImage 等，
     *   可以设置纯色填充、渐变填充或纹理填充。
     *
     * this->setPalette(palette)：
     *   将调色板应用到当前窗口及其子控件。
     *   在 Qt 的控件树中，子控件默认继承父控件的调色板。
     */
    QPalette palette;
    palette.setBrush(QPalette::Window, savedBgColor);
    this->setPalette(palette);

    /*
     * 同步全局背景颜色到 CircleButton 的静态变量。
     *
     * CircleButton::setGlobalBackgroundColor() 是静态函数，
     * 它设置 CircleButton 类的静态成员变量 s_bgColor。
     *
     * 静态成员变量的特点：
     *   1. 属于类，所有对象共享同一个副本
     *   2. 在程序启动时分配内存，程序结束时释放
     *   3. 必须在类外单独定义和初始化
     *   4. 可以通过类名或对象访问
     *
     * 这里的目的是让 CircleButton 中的所有按键按钮
     * 都能获取到当前的背景颜色，用于绘制按键时判断
     * 使用深色或浅色的边框（根据背景色的明暗程度）。
     */
    CircleButton::setGlobalBackgroundColor(savedBgColor);

    /*
     * ========================================================================
     * 创建键盘容器（CircleButton）
     * ========================================================================
     *
     * new CircleButton(this)：
     *   在堆上创建 CircleButton 对象，传入 this（MainWindow）作为父对象。
     *   这样 MainWindow 就成为 CircleButton 的父控件。
     *
     * Qt 对象树（Object Tree）机制：
     *   当创建 QObject 子类时指定了父对象（parent），
     *   父对象会将该子对象添加到自己的子对象列表中。
     *   当父对象被销毁时，会自动销毁所有子对象。
     *   这大大简化了内存管理，减少了内存泄漏的风险。
     *
     * CircleButton 虽然名字是"圆形按钮"，但在这里被当作容器使用。
     * 它的 keyboardUI() 函数会在内部创建 21 个 QPushButton
     * 作为它的子控件，并按照计算器布局排列这些按钮。
     *
     * resize(screenWidth, screenHeight)：
     *   将键盘容器的大小设置为全屏，使其覆盖整个窗口。
     *   这样键盘按钮可以占据屏幕的所有区域。
     */
    KeyBoardUI = new CircleButton(this);
    KeyBoardUI->resize(screenWidth, screenHeight);

    /*
     * 调用键盘渲染函数。
     *
     * keyboardUI() 是 CircleButton 的核心函数，它：
     *   1. 创建 21 个 QPushButton 对象
     *   2. 设置每个按钮的文本、字体、样式
     *   3. 计算按钮的布局位置（行列排列）
     *   4. 将按钮添加到键盘容器中
     *   5. 连接按钮的点击信号到计算器逻辑
     *
     * 按钮布局（以 4x6 网格为主）：
     *   Row 0: MC、MR、M+、M-（内存操作）
     *   Row 1: C、CE、%、÷
     *   Row 2: 7、8、9、×
     *   Row 3: 4、5、6、-
     *   Row 4: 1、2、3、+
     *   Row 5: 正负号、0、.、=
     */
    KeyBoardUI->keyboardUI();
    KeyBoardUI->show();   // 显示键盘容器（子控件的 show 依赖于父部件）

    /*
     * ========================================================================
     * 创建侧边栏菜单入口按钮（汉堡图标）
     * ========================================================================
     *
     * CircleButton(0, 20, "rectangle", this)：
     *   创建一个圆角矩形样式的按钮。
     *   参数说明：
     *     - 0    ：x 坐标（相对父控件左边缘的偏移）
     *     - 20   ：y 坐标（相对父控件上边缘的偏移）
     *     - "rectangle"：形状类型，这里表示圆角矩形
     *     - this ：父控件为 MainWindow
     *
     * 这个按钮是一个触发器，点击后弹出侧边栏菜单。
     * 它被 raise() 确保在所有控件之上显示。
     */
    SettingButton = new CircleButton(0, 20, "rectangle", this);
    SettingButton->show();
    SettingButton->raise();  // 确保设置按钮在所有控件之上

    /*
     * ========================================================================
     * 创建计算器显示窗口
     * ========================================================================
     *
     * new CalculatorWindow(KeyBoardUI, this)：
     *   创建 CalculatorWindow 对象。
     *   第一个参数 KeyBoardUI 是键盘容器指针，
     *   CalculatorWindow 通过它可以连接键盘按钮的点击事件。
     *
     * CalculatorWindow 内部包含：
     *   - QLineEdit：显示输入的数字和结果
     *   - calculateWay：计算逻辑对象，处理四则运算
     *
     * move(0, 120)：
     *   QWidget::move(int x, int y) 将控件移动到指定位置。
     *   这里将显示窗口定位在屏幕上方 y=120 的位置，
     *   为键盘区域和状态栏留出空间。
     *
     * updateDisplayBackground(savedBgColor)：
     *   将当前的背景颜色传递给 CalculatorWindow，
     *   让显示区域的背景与整体主题保持一致。
     */
    calculatorwindow = new CalculatorWindow(KeyBoardUI, this);
    calculatorwindow->move(0, 120);
    calculatorwindow->updateDisplayBackground(savedBgColor);
    calculatorwindow->show();
    calculatorwindow->raise();

    /*
     * ========================================================================
     * 创建侧边栏菜单面板
     * ========================================================================
     *
     * QString menuStr[buttonNumber]：
     *   创建一个字符串数组，包含菜单按钮的文本。
     *   buttonNumber 是在 menuButton.h 中定义的常量。
     *
     * new menuButton(menuStr, screenWidth, screenHeight, this)：
     *   创建菜单面板对象。
     *   参数：
     *     - menuStr      ：菜单按钮文本数组
     *     - screenWidth  ：屏幕宽度（用于计算面板大小）
     *     - screenHeight ：屏幕高度（面板与屏幕等高）
     *     - this         ：父控件
     *
     * 面板宽度为 screenWidth * 0.7（覆盖屏幕宽度的 70%）。
     * 初始状态下菜单是隐藏的（不显示）。
     *
     * connect(SettingButton, &QPushButton::clicked, menu, &menuButton::showMenu)：
     *   连接汉堡按钮的点击信号到菜单的显示函数。
     *   当用户点击汉堡按钮时，菜单从左侧滑入。
     */
    QString menuStr[buttonNumber] = {"关于", "检查更新", "设置", "版本", "更新内容"};
    menu = new menuButton(menuStr, screenWidth, screenHeight, this);
    menu->resize(screenWidth * 0.7, screenHeight);
    menu->move(0, 0);

    connect(SettingButton, &QPushButton::clicked, menu, &menuButton::showMenu);

    /*
     * =========================================================================
     * 初始化更新检查器（网络模块核心）
     * =========================================================================
     *
     * new UpdateChecker(this)：
     *   创建 UpdateChecker 对象，设置 MainWindow 为父对象。
     *
     * UpdateChecker 的工作原理：
     *   1. 构造 QNetworkAccessManager 对象（Qt 的网络请求管理器）
     *   2. 在 checkUpdate() 被调用时，发送 HTTP GET 请求到 Gitee
     *   3. 请求 URL：https://gitee.com/xxx/CalculationTools/raw/master/version.json
     *   4. 接收到响应后，解析 JSON 数据
     *   5. 对比服务端版本号和本地版本号
     *   6. 根据对比结果发射对应的信号
     *
     * 网络请求是异步的（Asynchronous）：
     *   - checkUpdate() 立即返回，不会阻塞主线程
     *   - 请求完成后通过信号（Signal）通知调用方
     *   - 这样在等待网络响应时，应用界面仍然可以响应用户操作
     */
    updateChecker = new UpdateChecker(this);

    /*
     * ========================================================================
     * 连接信号槽：发现新版本
     * ========================================================================
     *
     * connect() 是 Qt 信号槽机制的核心函数。
     *
     * 函数原型（Qt5 新语法）：
     *   connect(sender, &Sender::signal, receiver, &Receiver::slot)
     *   或使用 lambda 表达式（C++11）：
     *   connect(sender, &Sender::signal, receiver, [](params) { ... })
     *
     * 参数说明：
     *   - sender   ：信号发射者（UpdateChecker 对象）
     *   - signal   ：信号函数指针（&UpdateChecker::newVersionAvailable）
     *   - receiver ：信号接收者（this 指向 MainWindow）
     *   - slot     ：槽函数，这里使用 lambda 表达式
     *
     * Lambda 表达式 [this](const UpdateInfo& info) { ... }：
     *   C++11 引入的匿名函数语法。
     *   [this]   ：捕获列表，捕获 this 指针以便在 lambda 中访问 MainWindow 成员
     *   (params) ：参数列表，与信号函数的参数类型一致
     *   { body } ：函数体，执行具体的处理逻辑
     *
     * 信号发射条件：
     *   当 UpdateChecker 解析服务端响应后，发现
     *   server.versionCode > APP_VERSION_CODE 时发射此信号。
     *
     * UpdateInfo 结构体包含：
     *   - versionName      ：新版本的版本名称（如 "1.6.0"）
     *   - versionCode      ：新版本的版本代码（如 18）
     *   - downloadUrl      ：APK 下载链接
     *   - updateLog        ：更新日志内容
     *   - forceUpdate      ：是否强制更新（true = 必须更新才能继续使用）
     *
     * 处理流程：
     *   1. 创建 UpdateDialog 弹窗，传入版本信息
     *   2. 设置 WA_DeleteOnClose 属性，对话框关闭时自动释放内存
     *   3. exec() 模态显示对话框，阻塞等待用户操作
     */
    connect(updateChecker, &UpdateChecker::newVersionAvailable, this, [this](const UpdateInfo& info) {
        UpdateDialog* dialog = new UpdateDialog(info, this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->exec();
    });

    /*
     * ========================================================================
     * 连接信号槽：已是最新版本
     * ========================================================================
     *
     * 当服务端版本号 <= 本地版本号时发射此信号。
     *
     * 只有手动检查时弹窗提示，自动检查时不打扰用户。
     * m_isAutoCheckup 标记控制是否弹窗：
     *   - 自动检查时（m_isAutoCheckup == true）：不弹窗
     *   - 手动检查时（m_isAutoCheckup == false）：弹窗提示"当前已是最新版本"
     *
     * QMessageBox::information()：
     *    Qt 的标准信息对话框。
     *    参数：
     *       - parent ：父窗口（nullptr 表示无父窗口，居中显示在屏幕上）
     *       - title  ：对话框标题
     *       - text   ：显示的信息文本
     *    这是一个静态函数，会创建对话框、显示、等待用户点击"确定"、然后销毁。
     *    是模态对话框（Modal Dialog），阻塞当前线程直到用户关闭。
     */
    connect(updateChecker, &UpdateChecker::alreadyLatest, this, [this]() {
        if (!m_isAutoCheckup) {
            QMessageBox::information(nullptr, "检查更新", "当前已是最新版本！");
        }
    });

    /*
     * ========================================================================
     * 连接信号槽：检查失败
     * ========================================================================
     *
     * 当网络请求失败或 JSON 解析失败时发射此信号。
     *
     * 失败原因可能包括：
     *   - 网络不可用（飞行模式、无信号、Wi-Fi 未连接）
     *   - DNS 解析失败（域名无法解析）
     *   - 连接超时（服务器无响应）
     *   - HTTP 错误（404、500 等）
     *   - JSON 解析错误（服务端返回的数据格式不正确）
     *
     * 同样，只有手动检查时弹窗提示，自动检查时静默处理。
     *
     * QMessageBox::warning()：
     *    Qt 的标准警告对话框，带有警告图标。
     *    与 information 类似，也是模态对话框。
     *    用于提示用户需要注意的情况。
     */
    connect(updateChecker, &UpdateChecker::checkFailed, this, [this](const QString& error) {
        if (!m_isAutoCheckup) {
            QMessageBox::warning(nullptr, "检查更新", "检查失败：" + error);
        }
    });

    /*
     * ========================================================================
     * 启动时的自动检查更新
     * ========================================================================
     *
     * 设置 m_isAutoCheckup = true，然后调用 checkUpdate()。
     *
     * 这样做的目的：
     *   1. 每次用户打开应用时，自动检查是否有新版本
     *   2. 如果发现新版本，弹出更新对话框提示用户
     *   3. 如果已是最新或网络失败，不打扰用户
     *   4. 与手动检查（菜单按钮）的行为区分开
     *
     * checkUpdate() 的参数：
     *   - APP_VERSION      ：本地版本名称（如 "1.5.0"）
     *   - APP_VERSION_CODE ：本地版本代码（如 17）
     *   这些值被 UpdateChecker 用于与服务端数据做对比。
     */
    m_isAutoCheckup = true;
    updateChecker->checkUpdate(APP_VERSION, APP_VERSION_CODE);

    /*
     * ========================================================================
     * 连接菜单按钮的点击处理
     * ========================================================================
     *
     * menuButton::menuButtonClicked(int index) 信号：
     *   当用户点击菜单中的某个选项时发射。
     *   index 参数表示点击的是第几个按钮（从 0 开始）。
     *
     * Lambda 表达式 [=](int index)：
     *   [=] 表示按值捕获所有外部变量（screenWidth 等）。
     *   这意味着 lambda 内部将持有这些变量的副本。
     *   注意：按值捕获 this 指针时使用 [=] 会捕获 this，
     *   但在 lambda 内访问成员变量需要通过 this。
     *
     * 菜单按钮索引对应关系：
     *   索引 0："关于"
     *   索引 1："检查更新"
     *   索引 2："设置"
     *   索引 3："版本"
     *   索引 4："更新内容"
     */
    connect(menu, &menuButton::menuButtonClicked, this, [=](int index) {

        /*
         * --------------------------------------------------------------------
         * 索引 0：关于
         * --------------------------------------------------------------------
         *
         * QMessageBox::about()：
         *   Qt 的"关于"对话框，显示应用程序的基本信息。
         *   包含应用图标、名称、版本、版权信息等。
         *   参数：
         *     - parent     ：父窗口
         *     - title      ：对话框标题
         *     - text       ：显示的信息文本
         *
         * 宏 APP_VERSION 在编译时会被替换为字符串 "1.5.0"。
         * 字符串字面量相邻时会自动拼接：
         *   "版本：" APP_VERSION 等同于 "版本：1.5.0"
         */
        if (index == 0) {
            QMessageBox::about(this, "关于", "CalculationTools\n版本：" APP_VERSION "\n\n一个简单又不简单的的计算工具应用。");
        }

        /*
         * --------------------------------------------------------------------
         * 索引 1：检查更新（手动检查）
         * --------------------------------------------------------------------
         *
         * 设置 m_isAutoCheckup = false，表示这是手动触发的检查。
         * 这样无论结果如何都会弹窗提示用户。
         *
         * 然后调用 updateChecker->checkUpdate() 发起网络请求。
         * 网络请求完成后，通过之前连接的信号槽回调处理结果。
         */
        else if (index == 1) {
            m_isAutoCheckup = false;
            updateChecker->checkUpdate(APP_VERSION, APP_VERSION_CODE);
        }

        /*
         * --------------------------------------------------------------------
         * 索引 2：设置
         * --------------------------------------------------------------------
         *
         * 懒加载模式：只有第一次点击时才创建 SettingsWidget 对象。
         * 之后复用已创建的实例。
         *
         * 设置页面具有滑入/滑出的动画效果：
         *   - 进入动画：从屏幕右侧滑入（x 从 screenWidth 到 0）
         *   - 退出动画：从当前位置滑出到屏幕右侧（x 从 0 到 screenWidth）
         *
         * QPropertyAnimation（属性动画）：
         *   是 Qt 的动画框架核心类，可以对 QObject 的任何属性进行动画。
         *   原理：
         *     1. 指定目标对象和属性名称
         *     2. 设置起始值（startValue）和结束值（endValue）
         *     3. 设置持续时间（duration，单位毫秒）
         *     4. 设置缓动曲线（easingCurve，控制动画速率变化）
         *     5. 启动动画后，Qt 的动画框架会在每一帧更新属性值
         *     6. 直到属性值达到 endValue，动画完成
         *
         * QEasingCurve::OutCubic：
         *   缓动曲线的一种，实现"减速效果"。
         *   动画开始时速度较快，结束时逐渐减速停下。
         *   这种曲线让滑动动画看起来更加自然流畅。
         *
         * QAbstractAnimation::DeleteWhenStopped：
         *   动画停止时自动删除动画对象，防止内存泄漏。
         *   这是一个枚举值，作为启动动画的标志参数。
         */
        else if (index == 2) {
            if (!settingsWidget) {
                settingsWidget = new SettingsWidget(this);
                settingsWidget->setGeometry(0, 0, screenWidth, screenHeight);

                /*
                 * 连接 SettingsWidget 的 backToMain 信号：
                 * 当用户点击设置页面的返回按钮时触发。
                 * 执行退出动画：从当前位置向右滑出到屏幕之外。
                 */
                connect(settingsWidget, &SettingsWidget::backToMain, this, [this]() {
                    if (settingsWidget) {
                        // 创建属性动画对象，动画目标是 settingsWidget 的 "pos"（位置）属性
                        QPropertyAnimation* exitAnim = new QPropertyAnimation(settingsWidget, "pos");
                        int w = settingsWidget->width();  // 获取设置页面的宽度

                        // 动画持续时间：280 毫秒
                        exitAnim->setDuration(280);
                        // 起始位置：当前所在位置（即 (0, 0)）
                        exitAnim->setStartValue(settingsWidget->pos());
                        // 结束位置：屏幕右侧之外（x = 屏幕宽度）
                        exitAnim->setEndValue(QPoint(w, 0));
                        // 缓动曲线：减速曲线，让滑动逐渐停止
                        exitAnim->setEasingCurve(QEasingCurve::OutCubic);

                        // 动画结束时隐藏设置页面
                        connect(exitAnim, &QPropertyAnimation::finished, this, [this]() {
                            settingsWidget->hide();
                        });

                        // 启动动画，设置动画结束后自动删除
                        exitAnim->start(QAbstractAnimation::DeleteWhenStopped);
                    }
                });

                /*
                 * 连接 SettingsWidget 的 colorChanged 信号：
                 * 当用户在设置页面选择新颜色时触发。
                 *
                 * 需要同步更新三个地方的背景色：
                 *   1. 主窗口（MainWindow）的背景
                 *   2. 键盘区域（CircleButton 的静态颜色变量）
                 *   3. 计算器显示区域（CalculatorWindow）
                 */
                connect(settingsWidget, &SettingsWidget::colorChanged, this, [this](const QColor& color) {
                    // 1. 更新主窗口背景色
                    QPalette pal;
                    pal.setBrush(QPalette::Window, color);
                    this->setPalette(pal);

                    // 2. 更新键盘区域的全局背景色（静态变量）
                    CircleButton::setGlobalBackgroundColor(color);

                    // 3. 更新计算器显示区域背景色
                    if (calculatorwindow) {
                        calculatorwindow->updateDisplayBackground(color);
                    }
                });
            }

            // 隐藏菜单面板
            menu->hideMenu();

            /*
             * 进入动画：
             * 设置页面从屏幕右侧滑入。
             * 动画流程类似退出动画的反向过程。
             */
            {
                int w = settingsWidget->width();
                settingsWidget->move(w, 0);  // 从屏幕右侧开始
                settingsWidget->show();
                settingsWidget->raise();     // 提升到最上层显示
                settingsWidget->setFocus();  // 获得焦点

                // 创建滑入动画
                QPropertyAnimation* enterAnim = new QPropertyAnimation(settingsWidget, "pos");
                enterAnim->setDuration(280);
                enterAnim->setStartValue(QPoint(w, 0));       // 从右侧开始
                enterAnim->setEndValue(QPoint(0, 0));         // 滑入到屏幕左侧
                enterAnim->setEasingCurve(QEasingCurve::OutCubic);
                enterAnim->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }

        /*
         * --------------------------------------------------------------------
         * 索引 3：版本
         * --------------------------------------------------------------------
         *
         * 显示当前应用版本号。
         * QMessageBox::information 显示一个简单的信息对话框。
         * 宏 APP_VERSION 在编译时会被替换为实际的版本字符串。
         */
        else if (index == 3) {
            QMessageBox::information(this, "版本", "当前版本：" APP_VERSION);
        }

        /*
         * --------------------------------------------------------------------
         * 索引 4：更新内容
         * --------------------------------------------------------------------
         *
         * 显示当前版本的更新日志。
         * 每次发布新版本时，应该更新这里的文字内容。
         * 目前显示的是上一版本的更新内容。
         */
        else if(index == 4){
            QMessageBox::information(this, "更新内容", "新增背景颜色的选择设置");
        }
    });
}

/*
 * 析构函数：MainWindow::~MainWindow()
 *
 * 当 MainWindow 对象被销毁时自动调用。
 * 负责清理在堆上分配的资源。
 *
 * 注意：
 *   - 由于 Qt 的对象树机制，大多数子对象会在父对象销毁时自动销毁
 *   - 但某些对象（如 KeyBoardUI）没有设置父子关系，需要手动 Delete
 *   - 使用 delete 操作符释放 new 分配的内存
 *
 * 析构顺序：
 *   1. 执行析构函数体中的代码
 *   2. 按照成员变量声明的逆序调用成员变量的析构函数
 *   3. 调用基类的析构函数
 */
MainWindow::~MainWindow()
{
    /*
     * 手动删除 KeyBoardUI 对象。
     * 虽然 KeyBoardUI 是以 this 为父对象创建的，理论上会由 Qt 自动销毁，
     * 但显式 delete 可以确保正确的释放顺序。
     *
     * Qt 的对象树销毁顺序：
     *   当父对象销毁时，会先销毁所有子对象。
     *   子对象的析构顺序与创建顺序相反（后创建的先销毁）。
     *
     * 注意：delete 一个 QObject 时，它会从父对象的子对象列表中移除自身，
     * 并删除自己的所有子对象。
     */
    delete KeyBoardUI;
    delete settingsWidget;
    delete updateChecker;
    delete ui;
}
# =============================================================================
# CalculationTools.pro
# Qt 项目配置文件（Project File）
#
# 本文件是 qmake 构建系统的项目描述文件，告诉 qmake 如何构建这个项目。
# qmake 会读取此文件生成 Makefile，然后由 make/nmake 执行实际的编译和链接。
#
# Qt 项目文件基础：
#   .pro 文件是 Qt 项目的"入口"，类似于 CMakeLists.txt（CMake）或
#   package.json（Node.js）。qmake 读取这个文件后：
#     1. 解析变量定义（SOURCES, HEADERS, QT 等）
#     2. 根据平台和配置生成对应的 Makefile
#     3. 调用编译器完成编译
#
# 文件扩展名说明：
#   .pro  = project（项目文件），用于 qmake 构建系统
#   .pri  = project include（项目包含文件），可被多个 .pro 共享
#   .prf  = project feature（项目特性文件），定义构建特性
# =============================================================================

# =============================================================================
# 1. Qt 模块引入
# =============================================================================

# QT 变量声明项目依赖的 Qt 模块
# qmake 会自动添加这些模块的包含路径和链接库
#
# core     - QtCore 核心模块：QObject、QString、QFile、事件循环等基础类
# widgets  - QtWidgets 控件模块：QWidget、QPushButton、QLabel、QDialog 等 UI 类
# network  - QtNetwork 网络模块：QNetworkAccessManager、QNetworkReply 等网络类
#
# 为什么不需要显式引入其他模块？
#   Qt5 中 core 和 gui 是默认模块，会自动包含。
#   但本项目的 UpdateChecker/ApkDownloader 明确使用了 QNetworkAccessManager
#   （属于 QtNetwork 模块），所以必须显式声明 network。
QT       += core widgets network

# =============================================================================
# 2. 目标和模板设置
# =============================================================================

# TARGET：生成的可执行文件名称
# 在 Windows 上会生成 CalculationTools.exe
# 在 Android 上会生成 libCalculationTools.so（被 Java 层加载）
# 在 Linux/macOS 上会生成 CalculationTools（无后缀）
TARGET = CalculationTools

# TEMPLATE：构建模板类型，决定 qmake 生成哪种 Makefile
# app    - 应用程序（生成可执行文件，最常用）
# lib    - 库文件（生成 .so/.dll/.dylib）
# subdirs - 子目录项目（多个子项目）
# vcapp/vclib - Visual Studio 项目（很少用）
TEMPLATE = app

# =============================================================================
# 3. 源文件（SOURCES）
# =============================================================================

# SOURCES 变量列出所有需要编译的 C++ 源文件（.cpp）
# 每个 .cpp 文件会独立编译为 .o（目标文件），最后链接为可执行文件
#
# 反斜杠续行符说明：
#   每行末尾的 \ 表示下一行和当前行属于同一个变量定义
#   这是 Makefile 风格的续行写法，不是 C++ 语法
#
# 编译顺序说明：
#   qmake 会处理这些文件之间的依赖关系（通过 #include 头文件分析），
#   自动决定编译顺序。但链接时是按照这里列出的顺序？
#   不，链接顺序由编译器决定，不依赖于 SOURCES 的排列顺序。
SOURCES += \
    main.cpp \                 # 程序入口点，包含 main() 函数
    mainwindow.cpp \           # 主窗口：顶部标题栏 + 图标按钮
    CircleButton.cpp \         # 圆形按钮控件：自定义绘制的圆形 QPushButton
    CalculatorWindow.cpp \     # 计算器窗口：数字按钮 + 显示面板 + 计算引擎调用
    calculateWay.cpp \         # 计算引擎：表达式解析和四则运算核心算法
    menuButton.cpp \           # 菜单按钮：点击弹出设置/主题/关于等菜单
    SettingsWidget.cpp \       # 设置页面：主题切换 + 按键音 + 其他偏好设置
    UpdateChecker.cpp \        # 更新检查器：从 Gitee 获取版本信息
    UpdateDialog.cpp \         # 更新弹窗：显示版本号和更新日志的模态对话框
    StyledDialog.cpp \         # 统一风格弹窗：替代 QMessageBox 的通用模态对话框
    StyledProgressDialog.cpp   # 统一风格进度弹窗：下载更新进度条

# =============================================================================
# 4. 头文件（HEADERS）
# =============================================================================

# HEADERS 变量列出所有头文件（.h）
# 头文件不会被直接编译，但会被 #include 到源文件中
# qmake 会分析头文件依赖，确保相关源文件在头文件变更后重新编译
#
# MOC（Meta-Object Compiler）说明：
#   Qt 的元对象系统需要预处理带有 Q_OBJECT 宏的类头文件。
#   qmake 会自动检测哪些头文件包含 Q_OBJECT，并为其生成 moc_xxx.cpp。
#   这也是为什么 HEADERS 中要列出所有头文件——qmake 需要扫描它们。
HEADERS += \
    mainwindow.h \           # 主窗口类声明（QMainWindow 子类）
    CircleButton.h \         # 圆形按钮类声明（QPushButton 子类，含 Q_OBJECT）
    CalculatorWindow.h \     # 计算器窗口类声明（QWidget 子类，含 Q_OBJECT）
    calculateWay.h \         # 计算引擎类声明（普通 C++ 类，不含 Q_OBJECT）
    menuButton.h \           # 菜单按钮类声明（QPushButton 子类，含 Q_OBJECT）
    SettingsWidget.h \       # 设置页面类声明（QWidget 子类，含 Q_OBJECT）
    UpdateChecker.h \        # 更新检查器类声明（QObject 子类，含 Q_OBJECT + 信号槽）
    UpdateDialog.h \         # 更新弹窗类声明（QDialog 子类，含 Q_OBJECT + paintEvent）
    StyledDialog.h \         # 统一风格弹窗类声明（QDialog 子类，含 Q_OBJECT + paintEvent）
    StyledProgressDialog.h   # 统一风格进度弹窗类声明（QDialog 子类，含 Q_OBJECT + UI）

# =============================================================================
# 5. UI 表单文件（FORMS）
# =============================================================================

# FORMS 变量列出 Qt Designer 生成的 .ui 文件
# qmake 会调用 uic（UI Compiler）将 .ui 文件转换为 ui_xxx.h
# 生成的 ui_xxx.h 可以直接被 #include 使用
#
# 主窗口布局说明：
#   mainwindow.ui 包含了主窗口的静态布局部分（控件位置、大小等）。
#   而动态创建的控件（如计算器数字按钮）在 CalculatorWindow.cpp 中
#   通过代码创建，不在 .ui 文件中。
FORMS += \
    mainwindow.ui

# =============================================================================
# 6. 平台特定配置：Android
# =============================================================================

# android { ... } 是一个作用域（scope）块
# 仅在目标平台为 Android 时，作用域内的代码才会生效
#
# qmake 作用域语法：
#   condition { ... } 等价于 if (condition) { ... }
#   这里的 condition 是平台标识符（android、win32、unix、macx 等）
#
# 为什么需要平台特定配置？
#   1. ApkDownloader 功能（应用内安装 APK）完全依赖 Android 的 JNI 机制
#   2. 在 Windows 上编译 ApkDownloader.cpp 会导致链接错误（缺少 JNI 库）
#   3. 所以 ApkDownloader 的源文件仅在 Android 构建时加入
android {
    # ANDROID_PACKAGE_SOURCE_DIR：Android 打包资源目录
    # qmake 会将此目录下的文件打包到 APK 中
    # 包含 AndroidManifest.xml、res/ 资源、Java 源码等
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    # 仅在 Android 平台添加 APK 下载器的源文件
    # 这样在 Windows 上编译不会因为找不到 JNI 头文件而报错
    SOURCES += ApkDownloader.cpp

    # 引入 OpenSSL 库（Android 运行时必需）
    # Qt 的 QNetworkAccessManager 在 Android 上依赖 OpenSSL 3.x 动态库
    # libcrypto_3.so + libssl_3.so 必须打包进 APK，否则应用启动后无法运行
    include($$PWD/android_openssl/openssl.pri)
}

# =============================================================================
# 7. 其他说明
# =============================================================================
#
# 这个项目没有用到以下常用的 qmake 配置：
#
# CONFIG 变量：
#   不在这里设置。默认值（release/debug、qt、warn_on 等）已足够。
#   如果设置为 CONFIG += c++17 可以启用 C++17 标准。
#
# RESOURCES 变量：
#   没有 .qrc 资源文件。所有图标/图片通过代码直接加载路径或内联绘制。
#
# LIBS 变量：
#   不需要额外的外部库。所有功能都使用 Qt 内置模块（core/widgets/network）。
#
# DEFINES 变量：
#   没有定义额外的预处理器宏。所有条件编译通过 Q_OS_ANDROID 等 Qt 内置宏控制。
#
# INCLUDEPATH 变量：
#   不需要额外的包含路径。所有头文件都在项目根目录下。
#
# 关于 Q_OS_ANDROID 宏：
#   这是 Qt 预定义的平台检测宏，在 Android 编译时自动定义。
#   类似的还有 Q_OS_WIN（Windows）、Q_OS_LINUX（Linux）、Q_OS_IOS（iOS）等。
#   这些宏由 qmake 的 mkspecs 配置自动定义，开发者不需要手动添加。
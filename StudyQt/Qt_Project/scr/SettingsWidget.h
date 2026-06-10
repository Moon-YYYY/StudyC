/*
 * ============================================================================
 * 文件：SettingsWidget.h
 * 描述：设置页面的头文件
 *
 * 本文件定义了三个类：
 *   1. ColorDelegate    —— 颜色下拉列表项的自定义绘制代理（列表画笔）
 *   2. ColorPickerWidget —— 自定义颜色选择器控件（小巧的三角箭头 + 弹出面板）
 *   3. SettingsWidget   —— 设置页面主控件，提供背景颜色修改功能
 *
 * 类层级关系：
 *   SettingsWidget 包含 ColorPickerWidget
 *   ColorPickerWidget 内部使用 ColorDelegate
 *
 * 设置页面的用户交互流程：
 *   1. 用户点击主菜单中的"设置"按钮
 *   2. SettingsWidget 从右侧滑入屏幕
 *   3. 用户看到"修改背景颜色"选项
 *   4. 点击三角箭头，弹出颜色选择面板
 *   5. 选择一个颜色后，立即生效并保存到配置文件
 *   6. 点击返回按钮，设置页面滑出
 * ============================================================================
 */

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

/*
 * QWidget 是所有用户界面对象的基类（Base Class）。
 * 它继承自 QObject 和 QPaintDevice。
 * QWidget 提供了：
 *   - 窗口属性管理（大小、位置、可见性）
 *   - 鼠标和键盘事件处理
 *   - 绘制功能（paintEvent）
 *   - 字体和颜色管理
 *   - 拖放（Drag & Drop）支持
 *   等等
 */
#include <QWidget>

#include <QComboBox>       // 下拉列表控件（Qt 内置）
#include <QLabel>          // 标签控件（显示文本或图片）
#include <QPushButton>     // 按钮控件（可点击）
#include <QSettings>       // 持久化设置存储类（读写配置文件）
#include <QColor>          // 颜色类（RGB 或 HSV 表示）
#include <QStyledItemDelegate> // 自定义列表项绘制代理基类
#include <QPainter>        // 绘图类（用于自定义绘制）
#include <QButtonGroup>    // 按钮组（管理互斥按钮的选中状态）
#include <QGridLayout>     // 网格布局（将控件排列成行和列）
#include <QVBoxLayout>     // 垂直布局（按垂直方向排列控件）
#include <QHBoxLayout>     // 水平布局（按水平方向排列控件）

/**
 * @class ColorDelegate
 * @brief 自定义下拉列表项绘制代理
 *
 * ColorDelegate 继承自 QStyledItemDelegate（Qt 的样式化列表项代理类）。
 *
 * 什么是 Delegate（代理）？
 *   Qt 的 Model/View（模型/视图）架构中，分三个部分：
 *   - Model（模型）    ：管理数据（如 QComboBox 的内部列表）
 *   - View（视图）     ：显示数据（如 QComboBox 的下拉列表区域）
 *   - Delegate（代理） ：控制数据的绘制和编辑方式
 *
 * QStyledItemDelegate 的作用：
 *   当我们想改变列表项（如 QComboBox、QListView、QTableView）的
 *   显示效果时，可以继承 QStyledItemDelegate 并重写 paint() 方法，
 *   来自定义每一项的绘制方式。
 *
 * 本例中，ColorDelegate 让下拉列表的每一项显示为：
 *   ● 颜色圆形 + 颜色名称文字（如 "淡蓝"），替代默认的纯文字显示。
 *
 * 使用场景：
 *   当 QComboBox 的每个选项需要显示颜色预览时使用。
 *   在 ColorPickerWidget 的 createPopupPanel() 中没有直接使用它，
 *   而是使用自定义的弹窗面板 + QPushButton 实现了更美观的视觉效果。
 *   这个类目前保留供备选使用。
 */
class ColorDelegate : public QStyledItemDelegate
{
    /*
     * Q_OBJECT 宏是所有使用 Qt 信号槽机制的类必须包含的。
     * 对于继承自 QStyledItemDelegate 的类，虽然不一定会使用信号槽，
     * 但添加 Q_OBJECT 可以让类支持 Qt 的元对象系统。
     */
    Q_OBJECT

public:
    /*
     * explicit 防止隐式类型转换。
     * QObject *parent 设置父对象（用于 Qt 内存管理）。
     */
    explicit ColorDelegate(QObject* parent = nullptr);

    /*
     * paint() 是 QStyledItemDelegate 的虚函数，重写以实现自定义绘制。
     * QPainter* painter         ：绘图工具对象，就像一支画笔
     * const QStyleOptionViewItem& option ：当前绘制项的状态信息（选中、悬停等）
     * const QModelIndex& index   ：当前绘制的数据项索引
     *
     * override 关键字：显式声明重写基类的虚函数。
     */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /*
     * sizeHint() 返回每个列表项的建议尺寸。
     * 重写它可以控制列表项的高度（例如设为 36px）。
     *
     * override 关键字：显式声明重写基类的虚函数。
     */
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

/**
 * @class ColorPickerWidget
 * @brief 自定义颜色选择器控件
 *
 * 这是一个紧凑型的颜色选择器控件，整体只有 30x30 像素大小。
 * 未点击时只显示一个灰色的向下三角箭头 ▼；
 * 点击后弹出一个带圆角边框的浮动颜色选择面板。
 *
 * 设计思路：
 *   使用 QComboBox 可以达到类似效果，但 QComboBox 的样式定制比较困难。
 *   所以使用自定义控件实现更灵活的 UI。
 *
 * 关键机制：
 *   1. 点击 -> mousePressEvent 触发
 *   2. 创建浮动面板（QWidget 设置 Qt::Popup 窗口标志）
 *   3. 面板包含 5 个颜色按钮
 *   4. 点击按钮 -> 发射 colorSelected 信号
 *   5. 面板由 Qt::Popup 机制自动管理关闭（点击面板外部时自动关闭）
 *
 * QButtonGroup 按钮组：
 *   管理一组 QPushButton，提供以下功能：
 *   - 互斥选择（同一时刻只有一个按钮被选中）
 *   - 通过 ID 识别哪个按钮被点击
 *   - 发射 idClicked(int) 信号
 */
class ColorPickerWidget : public QWidget
{
    /*
     * Q_OBJECT 宏启用信号槽机制。
     * ColorPickerWidget 自定义了 colorSelected 信号，
     * 所以必须包含 Q_OBJECT 宏。
     */
    Q_OBJECT

signals:
    /*
     * signals: 关键字定义信号（Signal）。
     *
     * 信号是 Qt 的特殊成员函数，特点如下：
     *   1. 只需要声明，不需要实现（由 MOC 自动生成）
     *   2. 返回类型必须是 void
     *   3. 可以被 emit 发射（emit colorSelected(color)）
     *   4. 可以连接一个或多个槽函数
     *   5. 参数类型必须在 Qt 的类型系统中注册
     *
     * 这个信号在用户选择颜色时发射。
     * 参数 const QColor& color 表示新选择的颜色。
     *
     * 连接方（SettingsWidget）接收这个信号后：
     *   1. 保存颜色到 QSettings 文件
     *   2. 发射 colorChanged 信号通知 MainWindow 更新界面
     */
    void colorSelected(const QColor& color);

public:
    /*
     * 构造函数。
     * 初始化颜色选择器控件。
     * 默认选中淡蓝色 (220, 240, 255)。
     */
    explicit ColorPickerWidget(QWidget* parent = nullptr);

    /*
     * 设置当前选中的颜色。
     * 当外部需要同步颜色选择器的显示状态时调用。
     * 例如从 QSettings 加载保存的颜色后调用此函数。
     *
     * @param color 要设置的颜色
     */
    void setCurrentColor(const QColor& color);

    /*
     * 获取当前选中的颜色。
     * inline 内联函数，直接将返回值写在类定义中。
     * 内联（inline）意味着编译器会直接将函数体嵌入调用处，
     * 避免函数调用的开销。
     *
     * @return 当前选中的颜色
     */
    QColor currentColor() const { return m_selectedColor; }

protected:
    /*
     * QWidget 的绘图事件处理函数。
     * 当窗口需要重绘时由 Qt 自动调用（例如窗口显示、大小改变、调用 update()）。
     * 重写此函数来实现自定义绘制。
     *
     * 在 ColorPickerWidget 中，绘制一个灰色的向下三角箭头 ▼。
     *
     * @param event 绘图事件对象，包含需要重绘的区域等信息
     */
    void paintEvent(QPaintEvent* event) override;

    /*
     * QWidget 的鼠标按下事件处理函数。
     * 当用户在控件上按下鼠标按钮时由 Qt 自动调用。
     * 重写此函数来处理点击事件。
     *
     * 在 ColorPickerWidget 中，点击后弹出颜色选择面板。
     *
     * @param event 鼠标事件对象，包含点击位置、按钮类型等信息
     */
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    /*
     * private slots: 私有的槽函数。
     * 槽（Slot）是 Qt 信号槽机制中接收信号的函数。
     * 槽函数可以是任何普通的成员函数，但可以连接信号。
     *
     * 这个槽函数连接 QButtonGroup::idClicked 信号，
     * 当用户点击颜色选择面板中的某个按钮时触发。
     *
     * @param id 被点击按钮的标识符（0-4，对应 5 种颜色）
     */
    void onColorButtonClicked(int id);

private:
    /*
     * 创建弹出颜色选择面板。
     *
     * 私有函数，仅在用户点击控件时由 mousePressEvent 调用。
     * 每次调用都会重新创建面板（更新选中状态的显示）。
     *
     * 创建的面板使用 Qt::Popup 窗口类型：
     *   - 是一个独立的顶层窗口（无父窗口）
     *   - 没有窗口边框和标题栏（FramelessWindowHint）
     *   - 点击面板外部区域时自动关闭
     *   - 通常用于上下文菜单、下拉选择器等
     */
    void createPopupPanel();

    QColor m_selectedColor;      // 当前选中的颜色值
    QString m_selectedName;      // 当前选中颜色的名称（如 "淡蓝"）

    QWidget* m_popupPanel;       // 弹出颜色选择面板（Qt::Popup 类型窗口）
    QGridLayout* m_popupLayout;  // 面板内的网格布局管理器
    QButtonGroup* m_buttonGroup; // 按钮组，管理 5 个颜色按钮的互斥选择
};

/**
 * @class SettingsWidget
 * @brief 设置页面控件，提供背景颜色修改功能
 *
 * 该控件显示一个全屏的设置页面，用户可以修改计算器的背景颜色。
 * 设置会保存到配置文件（QSettings），下次启动时自动恢复。
 *
 * 页面布局结构：
 *   ┌──────────────────────────────────┐
 *   │  ← 返回     设置                 │  ← 顶部标题栏
 *   ├──────────────────────────────────┤
 *   │                                  │
 *   │  修改背景颜色          ▼         │  ← 颜色设置行
 *   │                                  │
 *   │       （其他设置项预留空间）      │
 *   │                                  │
 *   └──────────────────────────────────┘
 *
 * 整个页面被包裹在一个 QScrollArea 中，支持滚动（为将来更多设置项准备）。
 */
class SettingsWidget : public QWidget
{
    /*
     * Q_OBJECT 宏启用信号槽机制。
     * SettingsWidget 自定义了 backToMain 和 colorChanged 两个信号。
     */
    Q_OBJECT

signals:
    /*
     * 返回主界面时发射的信号。
     * MainWindow 连接此信号后执行退出动画（设置页面滑出）。
     */
    void backToMain();

    /*
     * 颜色被修改时发射的信号。
     * MainWindow 连接此信号后更新所有界面控件的背景色。
     *
     * @param color 新的背景颜色
     */
    void colorChanged(const QColor& color);

public:
    /*
     * 构造函数。
     * @param parent 父窗口指针，用于 Qt 对象树内存管理
     */
    explicit SettingsWidget(QWidget *parent = nullptr);

    /*
     * 析构函数。~SettingsWidget() 的默认实现使用 = default，
     * 表示让编译器生成默认的析构函数体。
     */
    ~SettingsWidget();

    /*
     * 静态函数：从持久化存储中加载保存的背景颜色。
     *
     * static 关键字：
     *   该函数属于类本身，而不是某个对象实例。
     *   调用方式：SettingsWidget::loadSavedColor()
     *   不需要创建 SettingsWidget 对象即可调用。
     *
     * 内部实现：
     *   使用 QSettings 读取 "CalculationTools/App" 配置组中
     *   的 "backgroundColor" 键。如果未找到，返回默认的淡蓝色。
     *
     * @return 之前保存的颜色，如果从未保存过则返回默认淡蓝色 (220,240,255)
     */
    static QColor loadSavedColor();

    /*
     * 静态函数：将颜色保存到持久化存储。
     *
     * @param color 要保存的颜色
     */
    static void saveColor(const QColor& color);

protected:
    /*
     * 重写绘图事件，为设置页面绘制白色背景。
     * 设置页面的整体背景是白色的，与主界面的主题色区分开。
     *
     * @param event 绘图事件对象
     */
    void paintEvent(QPaintEvent* event) override;

private:
    /*
     * 以下成员变量是指针，初始化为 nullptr。
     * 它们在 setupUI() 函数中创建具体的对象。
     */

    QLabel* titleLabel;             // "设置" 标题文本标签
    QLabel* colorSettingLabel;      // "修改背景颜色" 文本标签
    ColorPickerWidget* colorPicker; // 自定义颜色选择器控件（三角箭头）
    QPushButton* backButton;        // 返回按钮（"←" 文本）

    /*
     * 可选颜色列表的结构体定义。
     *
     * 结构体（struct）是 C++ 中定义简单数据聚合的方式。
     * 在 C++ 中 struct 和 class 的区别：
     *   - struct 的成员默认是 public
     *   - class 的成员默认是 private
     * 这里使用 struct 只是为了方便携带 name 和 color 两个数据。
     */
    struct ColorOption {
        QString name;   // 颜色的中文名称（如 "淡蓝"）
        QColor color;   // 颜色的 RGB 值
    };

    /*
     * 静态常量数组，定义了 5 种可选的背景颜色。
     *
     * static    ：属于类，所有实例共享一份
     * const     ：数组内容不可修改（只读）
     * [5]       ：数组大小为 5
     *
     * 这 5 种颜色硬编码在程序的设置页面中。
     * 在 cpp 文件中进行初始化。
     */
    static const ColorOption s_colorOptions[5];

    /*
     * 初始化 UI 组件。
     * 这是私有辅助函数，在构造函数中被调用。
     * 负责创建所有子控件、设置布局、连接信号槽。
     */
    void setupUI();
};

#endif // SETTINGSWIDGET_H
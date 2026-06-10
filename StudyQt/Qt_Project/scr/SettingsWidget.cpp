/*
 * ============================================================================
 * 文件：SettingsWidget.cpp
 * 描述：设置页面的实现文件
 *
 * 本文件实现了三个类的成员函数：
 *   1. ColorDelegate          —— 颜色选项的自定义绘制代理
 *   2. ColorPickerWidget      —— 自定义颜色选择器控件
 *   3. SettingsWidget         —— 设置页面主控件
 *
 * 核心功能：颜色选择 -> 持久化保存 -> 通知主窗口更新背景色
 * ============================================================================
 */

/*
 * 包含 SettingsWidget 的头文件。
 * 头文件中已经包含了 QWidget 和相关 Qt 类的声明。
 */
#include "SettingsWidget.h"

/*
 * Qt 布局管理类：
 *   QVBoxLayout  ：垂直盒子布局，自上而下排列子控件
 *   QHBoxLayout  ：水平盒子布局，自左向右排列子控件
 *
 * QVBoxLayout 和 QHBoxLayout 都继承自 QBoxLayout（盒子布局）。
 * 盒子布局的工作原理：
 *   1. 计算所有子控件的理想尺寸（sizeHint）
 *   2. 按照添加顺序排列子控件
 *   3. 剩余空间分配给设置了 stretch（拉伸因子）的控件
 *   4. 如果没有 stretch，剩余空间作为边距
 */
#include <QVBoxLayout>
#include <QHBoxLayout>

/*
 * QPainter —— Qt 的 2D 绘图引擎。
 * 可以绘制：线条、矩形、椭圆、多边形、文本、图片、渐变等。
 * 使用步骤：
 *   1. 创建 QPainter 对象，传入绘图设备（QPaintDevice）
 *   2. 设置画笔（pen）和画刷（brush）
 *   3. 调用 drawXXX() 方法绘制
 *   4. 销毁 QPainter 对象（或调用 end()）
 *
 * QPaintEvent —— 绘图事件，包含需要重绘的区域信息。
 * 当系统检测到窗口需要重绘时，会通过 QEvent::Paint 事件
 * 触发控件的 paintEvent() 方法。
 */
#include <QPainter>
#include <QPaintEvent>

/*
 * QSettings —— 持久化设置存储类。
 * 以键值对（Key-Value）方式存储应用程序的配置信息。
 *
 * 底层实现随平台变化：
 *   Windows ：注册表（HKEY_CURRENT_USER\Software）
 *   Android ：SharedPreferences（XML 文件）
 *   Linux   ：~/.config/ 目录下的 INI 文件
 *   macOS   ：plist 文件
 *
 * 使用方式：
 *   QSettings settings("组织名", "应用名");
 *   settings.value("key", defaultValue);  // 读取
 *   settings.setValue("key", value);      // 写入
 */
#include <QSettings>

/*
 * QApplication —— 应用程序级别的全局管理类。
 * 提供：
 *   - 全局样式表（setStyleSheet）
 *   - 全局字体（setFont）
 *   - 全局调色板（setPalette）
 *   - 桌面信息（primaryScreen）
 *   等静态函数
 */
#include <QApplication>

/*
 * QStyleOptionViewItem —— 视图项的风格选项。
 * 用于 QStyledItemDelegate::paint() 中携带当前绘制项的状态信息，
 * 如：选中状态、悬停状态、背景色、文本色等。
 */
#include <QStyleOptionViewItem>

/*
 * QScrollArea —— 可滚动的区域控件。
 * 当子控件内容超出显示区域时，自动显示滚动条。
 * 通过 setWidget() 设置要滚动的内容。
 */
#include <QScrollArea>

/*
 * QButtonGroup —— 按钮组，管理一组互斥按钮。
 * 关键功能：
 *   - setExclusive(true) 设置互斥（同一时刻只能选中一个）
 *   - addButton(QAbstractButton*, int id) 添加按钮并指定 ID
 *   - idClicked(int id) 信号——哪个 ID 的按钮被点击
 */
#include <QButtonGroup>

/*
 * QGridLayout —— 网格布局管理器。
 * 将子控件排列成行（row）和列（column）的网格。
 * 通过 addWidget(widget, row, column, rowSpan, colSpan) 添加控件。
 * rowSpan 和 colSpan 表示控件跨越的行数和列数。
 */
#include <QGridLayout>

/*
 * QScreen —— 屏幕信息类。
 * 提供：
 *   - geometry()      ：屏幕尺寸（像素）
 *   - physicalSize()  ：物理尺寸（毫米）
 *   - devicePixelRatio() ：设备像素比（高 DPI 缩放）
 *   - refreshRate()   ：刷新率
 *   等
 */
#include <QScreen>

/*
 * QRect —— 矩形区域类。
 * 由 (x, y, width, height) 四个整数定义。
 * 常用方法：
 *   - left(), right(), top(), bottom() ：获取四边坐标
 *   - center()                         ：获取中心点
 *   - contains(QPoint)                 ：判断点是否在矩形内
 *   - intersects(QRect)                ：判断是否与另一个矩形相交
 */
#include <QRect>

/*
 * QMouseEvent —— 鼠标事件类。
 * 包含鼠标事件的详细信息：
 *   - pos() / localPos()   ：鼠标在控件内的坐标
 *   - globalPos()          ：鼠标在屏幕上的全局坐标
 *   - button()             ：哪个按键被按下（左键、右键、中键）
 *   - buttons()            ：当前按下的所有按键组合
 *   - modifiers()          ：键盘修饰键状态（Ctrl、Shift、Alt）
 */
#include <QMouseEvent>

// ==================== ColorDelegate 实现 ====================

/*
 * ColorDelegate 构造函数。
 *
 * @param parent 父对象，用于 Qt 的对象树内存管理
 *
 * QStyledItemDelegate(parent) 调用基类构造函数，
 * 将父对象传递给基类。
 */
ColorDelegate::ColorDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    // 构造函数体为空：委托类不需要额外的初始化
}

/*
 * paint() —— 自定义绘制颜色列表项。
 *
 * 此函数被 Qt 的视图系统自动调用，用于绘制列表/下拉框中的每一项。
 *
 * 重写此函数是为了实现：
 *   每一个列表项的左侧绘制一个彩色圆形（颜色预览），
 *   右侧显示颜色名称文字（如"淡蓝"）。
 *
 * 这个函数是 const 的，表示不会修改成员变量。
 *
 * @param painter 绘图工具对象（Qt 的画笔），用于执行实际的绘制操作
 * @param option  当前选项的状态信息，包含：
 *                - rect     ：该项的矩形区域（位置和大小）
 *                - palette  ：当前的调色板（颜色方案）
 *                - state    ：状态标志（State_Selected、State_MouseOver 等）
 *                - font     ：字体信息
 * @param index   数据模型索引，通过它可以获取该项的数据：
 *                - index.data(Qt::DisplayRole) ：显示文本
 *                - index.data(Qt::UserRole)    ：自定义数据（这里存储 QColor）
 */
void ColorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    /*
     * painter->save()：
     * 保存当前绘图状态（画笔、画刷、变换矩阵等）到栈中。
     * 当调用 painter->restore() 时恢复。
     * 这是为了防止当前绘制影响到后续的绘制。
     */
    painter->save();

    /*
     * setRenderHint(QPainter::Antialiasing)：
     * 启用抗锯齿渲染。
     * 抗锯齿（Antialiasing）是一种图形技术：
     *   - 通过在边缘像素处使用半透明颜色
     *   - 使锯齿状的线条看起来更平滑
     *   - 对圆形和斜线效果特别明显
     */
    painter->setRenderHint(QPainter::Antialiasing);

    /*
     * QStyle::State_Selected 和 QStyle::State_MouseOver：
     * 是 QStyle::StateFlag 枚举值，表示控件的状态。
     *
     * option.state 是一个位掩码（bitmask），通过 & 运算检查特定状态。
     * 位掩码（bitmask）是指用二进制位的 0/1 表示多个状态：
     *   0001 = State_Selected
     *   0010 = State_MouseOver
     *   0100 = State_HasFocus
     *   1000 = State_Enabled
     *   可以用 | 组合，用 & 检查
     */
    if (option.state & QStyle::State_Selected) {
        /*
         * 如果该项被选中，绘制高亮背景。
         *
         * painter->fillRect(QRect, QColor)：
         * 用指定的颜色填充矩形区域。
         * 与 drawRect 不同，fillRect 只填充不描边。
         *
         * option.rect 是该项在视图中的矩形区域。
         *
         * option.palette.highlight()：
         * 返回调色板中"高亮"角色的颜色。
         * 通常是系统主题的选中色（蓝色系）。
         */
        painter->fillRect(option.rect, option.palette.highlight());
    } else if (option.state & QStyle::State_MouseOver) {
        /*
         * 如果鼠标悬停在该项上（但没有选中），
         * 绘制浅灰色背景以提示用户这是可点击的。
         *
         * QColor(230, 230, 230) 是一个浅灰色。
         */
        painter->fillRect(option.rect, QColor(230, 230, 230));
    }

    /*
     * 从数据模型中获取该项的颜色数据。
     *
     * index.data(Qt::UserRole)：
     * Qt 的 Model/View 框架中，每个数据项可以有多个"角色"（Role）：
     *   - Qt::DisplayRole     ：显示的文本（如 "淡蓝"）
     *   - Qt::DecorationRole  ：图标
     *   - Qt::ToolTipRole     ：提示文本
     *   - Qt::UserRole        ：用户自定义角色（从 0x0100 开始）
     *
     * value<QColor>()：
     * QVariant 的模板方法，将存储的值转换为指定的类型。
     * 如果类型不匹配，返回一个默认构造的值。
     */
    QColor itemColor = index.data(Qt::UserRole).value<QColor>();

    /*
     * 计算圆形颜色预览区域。
     *
     * int circleRadius = 8：圆的半径是 8 像素。
     *
     * int leftMargin = 12：左侧边距 12 像素。
     *
     * option.rect.center().y()：
     * 获取该项矩形区域的垂直中心 Y 坐标。
     *
     * QRect circleRect(leftMargin, centerY - circleRadius, circleRadius * 2, circleRadius * 2)：
     * 创建一个正方形区域，用于内切绘制圆形。
     * leftMargin           ：矩形左上角 X 坐标
     * centerY - circleRadius：矩形左上角 Y 坐标（居中）
     * circleRadius * 2     ：矩形宽度（直径）
     * circleRadius * 2     ：矩形高度（直径）
     */
    int circleRadius = 8;
    int leftMargin = 12;
    int centerY = option.rect.center().y();
    QRect circleRect(leftMargin, centerY - circleRadius, circleRadius * 2, circleRadius * 2);

    /*
     * 设置画刷为颜色圆的填充色。
     * QPainter::setBrush(const QBrush&) 设置填充样式。
     * QBrush 可以使用纯色、渐变、纹理等。
     */
    painter->setBrush(itemColor);

    /*
     * 根据颜色的亮度决定是否绘制边框。
     *
     * itemColor.lightness() > 240：
     * lightness() 返回 HSL 颜色模型中的亮度值（0-255）。
     * 亮度值 240 以上的颜色非常亮（如纯白亮度=255），
     * 在白色背景上难以分辨边界，所以绘制浅灰色边框。
     *
     * setPen(QPen(QColor(180, 180, 180), 1))：
     * 设置画笔为 1 像素宽的灰色线条。
     *
     * setPen(Qt::NoPen)：
     * 不绘制边框（颜色圆边缘直接与背景融合）。
     */
    if (itemColor == QColor(255, 255, 255) || itemColor.lightness() > 240) {
        painter->setPen(QPen(QColor(180, 180, 180), 1));
    } else {
        painter->setPen(Qt::NoPen);
    }

    /*
     * drawEllipse(const QRect&)：
     * 在指定的矩形内绘制椭圆。
     * 当矩形是正方形时，绘制的是正圆形。
     * 圆会内切于矩形，即与矩形的四条边相切。
     */
    painter->drawEllipse(circleRect);

    /*
     * 设置文字颜色。
     * setPen 同时影响线条和文字的绘制颜色。
     * option.palette.text().color() 返回标准文本色（通常是黑色）。
     */
    painter->setPen(option.palette.text().color());

    /*
     * 设置字体。
     * QFont option.font 包含字体的信息：
     *   - family（字体族，如 "微软雅黑"）
     *   - pointSize（字号）
     *   - bold（是否加粗）
     *   - italic（是否斜体）
     */
    QFont font = option.font;
    painter->setFont(font);

    /*
     * 计算文字绘制区域。
     * 文字区域在颜色圆形右侧。
     *
     * textRect.setLeft(leftMargin + circleRadius * 2 + 10)：
     *   文字从左边缘（颜色位置 + 圆形直径 + 10px间隔）开始。
     */
    QRect textRect = option.rect;
    textRect.setLeft(leftMargin + circleRadius * 2 + 10);

    /*
     * 获取要显示的文字。
     * index.data(Qt::DisplayRole).toString() 返回显示文本。
     */
    QString text = index.data(Qt::DisplayRole).toString();

    /*
     * drawText(QRect, int flags, QString)：
     * 在矩形区域中绘制文字。
     * flags 控制对齐方式：
     *   Qt::AlignVCenter ：垂直居中
     *   Qt::AlignLeft    ：左对齐
     * 多个对齐标志用 | 组合。
     */
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

    /*
     * painter->restore()：
     * 恢复之前保存的绘图状态。
     * 确保在函数返回后，painter 的状态与进入时一致。
     * 这是一种防御性编程习惯，避免影响调用者。
     */
    painter->restore();
}

/*
 * sizeHint() —— 返回列表项的推荐尺寸。
 *
 * 通过重写此函数可以控制列表项的高度。
 * 这里将默认高度改为 36 像素。
 *
 * @param option 选项的状态信息（包含字体等）
 * @param index  数据模型索引
 * @return 推荐的大小
 */
QSize ColorDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    /*
     * QStyledItemDelegate::sizeHint()：
     * 调用基类的 sizeHint() 获取默认的推荐尺寸。
     * 基类会根据字体和内容计算合适的尺寸。
     */
    QSize baseSize = QStyledItemDelegate::sizeHint(option, index);

    /*
     * QSize(int width, int height)：
     * 保持基类返回的宽度不变，将高度改为 36 像素。
     * 36 像素足够显示一个颜色圆形和文字，且整体较为紧凑。
     */
    return QSize(baseSize.width(), 36);
}

// ==================== ColorPickerWidget 实现 ====================

/*
 * ColorPickerWidget 构造函数。
 *
 * 初始化颜色选择器控件：
 *   1. 设置默认选中颜色为淡蓝
 *   2. 设置控件固定大小为 30x30 像素
 *   3. 设置鼠标样式为手型（表示可点击）
 *   4. 初始化弹出面板指针为 nullptr
 *   5. 创建按钮组
 *   6. 连接按钮组的点击信号到槽函数
 *
 * @param parent 父控件指针
 */
ColorPickerWidget::ColorPickerWidget(QWidget* parent)
    : QWidget(parent)
{
    /*
     * m_selectedColor 和 m_selectedName：
     * 私有成员变量，前缀 m_ 表示 member（成员变量）。
     * 这是 Qt 项目中常见的命名约定（匈牙利命名法变体）。
     *
     * QColor(220, 240, 255) 是淡蓝色：
     *   - R = 220（红色分量）
     *   - G = 240（绿色分量）
     *   - B = 255（蓝色分量）
     *   - 未指定 Alpha 通道（透明度），默认为 255（完全不透明）
     *
     * QString::fromUtf8()：
     * 将 UTF-8 编码的字符串转换为 QString 对象。
     * QString 内部使用 UTF-16 编码存储 Unicode 字符。
     * 对于中文字符，必须使用 fromUtf8() 或直接使用 u8"xxx" 前缀。
     */
    m_selectedColor = QColor(220, 240, 255);
    m_selectedName = QString::fromUtf8("淡蓝");

    /*
     * setFixedSize(int width, int height)：
     * 设置控件的固定大小。
     * 与 resize() 不同，setFixedSize() 设置的尺寸不能被用户或布局管理器改变。
     * 控件在宽度和高度方向上都不能伸缩。
     */
    setFixedSize(30, 30);

    /*
     * setCursor(Qt::PointingHandCursor)：
     * 设置鼠标悬停在控件上时的光标形状。
     * Qt::PointingHandCursor 是手型光标（手指指向），
     * 表示这个控件可以被点击。
     *
     * 其他常见的光标形状：
     *   Qt::ArrowCursor       ：标准箭头
     *   Qt::IBeamCursor       ：文本选择
     *   Qt::CrossCursor       ：十字准星
     *   Qt::ClosedHandCursor  ：抓取状态
     */
    setCursor(Qt::PointingHandCursor);

    /*
     * 初始化弹出面板相关指针为 nullptr。
     * 面板在用户首次点击时创建（懒加载）。
     */
    m_popupPanel = nullptr;
    m_popupLayout = nullptr;
    m_popupPanel = nullptr;

    /*
     * QButtonGroup 是 QObject 的子类，用于管理按钮组。
     * 它为组内的按钮提供互斥选择功能。
     *
     * new QButtonGroup(this)：
     *   创建按钮组，设置父对象为当前 ColorPickerWidget。
     *   父对象销毁时，按钮组也会自动销毁。
     *
     * setExclusive(true)：
     *   设置互斥模式（exclusive mode）。
     *   在互斥模式下，同一时刻只能有一个按钮被选中。
     *   选中一个按钮会自动取消其他按钮的选中状态。
     *
     * QOverload<int>::of(&QButtonGroup::idClicked)：
     *   Qt5 的连接语法，用于解决信号重载问题。
     *   QButtonGroup 有两个 clicked 信号：
     *     - clicked(int id)        —— 带按钮 ID
     *     - clicked(QAbstractButton*) —— 带按钮指针
     *   QOverload<int>::of() 明确指出使用 int 版本的重载。
     */
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);
    connect(m_buttonGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &ColorPickerWidget::onColorButtonClicked);

    /*
     * setMouseTracking(true)：
     * 启用鼠标跟踪。
     * 默认情况下，只有按下鼠标按钮时才会发送 mouseMoveEvent。
     * 启用后，即使不按鼠标按钮，也会持续发送 mouseMoveEvent。
     * 这里用于实现在三角箭头控件上的悬停效果。
     */
    setMouseTracking(true);
}

/*
 * createPopupPanel() —— 创建弹出颜色选择面板。
 *
 * 每次点击控件时都会重新创建面板，确保面板中的按钮
 * 能够反映当前选中状态（选中项用黑色环形高亮标记）。
 *
 * 面板使用 Qt::Popup 窗口类型：
 *   这是一种特殊的顶层窗口，通常用于：
 *     - 上下文菜单（Context Menu）
 *     - 下拉选择器（Dropdown Picker）
 *     - 工具提示（Tooltip）
 *   特点：
 *     - 没有窗口边框和标题栏
 *     - 点击面板外部区域时自动关闭
 *     - 不显示在任务栏中
 *     - 不会获得焦点（WA_ShowWithoutActivating）
 */
void ColorPickerWidget::createPopupPanel()
{
    /*
     * 如果已经有面板存在，先销毁旧的。
     * deleteLater() 将对象放入待删除队列，在事件循环
     * 的下一轮处理中删除。相比 delete 操作符更安全，
     * 避免当前正在处理的事件中对象被意外销毁。
     */
    if (m_popupPanel) {
        m_popupPanel->deleteLater();
        m_popupPanel = nullptr;
    }

    /*
     * 清空按钮组：移除所有已注册的按钮。
     * 因为旧面板中的按钮通过 deleteLater 延迟销毁，
     * 但按钮组仍然持有它们的指针，需要先清除。
     * QButtonGroup 没有直接的 clear() 方法，
     * 所以通过遍历移除所有按钮来实现。
     * 注意：从按钮组移除按钮不会删除按钮对象本身。
     */
    QList<QAbstractButton*> allButtons = m_buttonGroup->buttons();
    for (QAbstractButton* btn : allButtons) {
        m_buttonGroup->removeButton(btn);
    }

    /*
     * new QWidget(nullptr)：
     *   创建一个无父对象的 QWidget。
     *   因为 Qt::Popup 窗口必须是顶层窗口（无父窗口）。
     *   如果有父窗口，Popup 的行为可能不正常。
     *
     * setWindowFlags(Qt::Popup | Qt::FramelessWindowHint)：
     *   设置窗口标志。
     *   Qt::Popup ：
     *     - 这是一个弹出式窗口
     *     - 点击外部区域自动关闭
     *     - 不显示在任务栏
     *   Qt::FramelessWindowHint：
     *     - 无窗口边框和标题栏
     *     - 通常与 Popup 一起使用
     *
     * setAttribute(Qt::WA_ShowWithoutActivating)：
     *   显示窗口时不激活它（不获取焦点）。
     *   这样用户在点击面板时，主窗口仍然保持活动状态。
     *
     * setStyleSheet(...)：
     *   设置面板的样式表（QSS，Qt Style Sheets）。
     *   QSS 类似于 CSS，用于自定义 Qt 控件的外观。
     *   语法：selector { property: value; }
     *   这里设置了：
     *     - 白色背景
     *     - 1 像素灰色边框
     *     - 8 像素圆角
     */
    m_popupPanel = new QWidget(nullptr);
    m_popupPanel->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    m_popupPanel->setAttribute(Qt::WA_ShowWithoutActivating);
    m_popupPanel->setStyleSheet(
        "QWidget { background: white; border: 1px solid #ccc; border-radius: 8px; }"
    );

    /*
     * 创建网格布局管理器。
     * QGridLayout 把面板空间划分为行和列。
     *
     * setContentsMargins(8, 8, 8, 8)：
     *   设置内容边距（内边距）。
     *   参数顺序：左、上、右、下。
     *   这里设置 8 像素的内边距，让内容不紧贴边框。
     *
     * setSpacing(6)：
     *   设置子控件之间的间距为 6 像素。
     */
    m_popupLayout = new QGridLayout(m_popupPanel);
    m_popupLayout->setContentsMargins(8, 8, 8, 8);
    m_popupLayout->setSpacing(6);

    /*
     * 定义 5 种可选颜色。
     *
     * 在函数内部定义结构体数组（匿名结构体）：
     *   C++ 允许在函数内部定义临时的结构体类型。
     *   但这样的类型不能在函数外部使用。
     *
     * QString::fromUtf8() 将 UTF-8 中文字符串转换为 QString。
     */
    struct { QString name; QColor color; } colors[5] = {
        {QString::fromUtf8("淡蓝"), QColor(220, 240, 255)},
        {QString::fromUtf8("粉色"), QColor(255, 200, 210)},
        {QString::fromUtf8("黄色"), QColor(255, 255, 200)},
        {QString::fromUtf8("白色"), QColor(255, 255, 255)},
        {QString::fromUtf8("黑色"), QColor(50, 50, 50)},
    };

    /*
     * 循环创建 5 个颜色按钮，添加到网格布局的一行中。
     *
     * for (int i = 0; i < 5; ++i)：
     *   ++i 是前缀自增，与 i++ 的区别：
     *   ++i 先递增再返回，i++ 先返回再递增。
     *   对于 int 类型没有区别，但对于迭代器等重载类型，
     *   ++i 更高效（避免创建临时对象）。
     */
    for (int i = 0; i < 5; ++i) {
        /*
         * new QPushButton(m_popupPanel)：
         *   创建按钮，设置面板为父对象。
         */
        QPushButton* btn = new QPushButton(m_popupPanel);
        btn->setFixedSize(28, 28);                                // 按钮大小 28x28 像素
        btn->setCursor(Qt::PointingHandCursor);                   // 手型光标
        btn->setToolTip(colors[i].name);                          // 悬停提示文字

        /*
         * 判断当前颜色是否被选中。
         * QColor 重载了 == 运算符，可以直接比较。
         * 注意：QColor 的 == 是逐个分量比较（R, G, B, A）。
         */
        bool isSelected = (colors[i].color == m_selectedColor);

        /*
         * 创建 28x28 像素的 QPixmap（像素图/画布）。
         * QPixmap 是一个离屏（off-screen）的像素缓冲区，
         * 用于在内存中进行绘图操作，然后可以设置为按钮的图标。
         *
         * pix.fill(Qt::transparent)：
         * 用透明色填充画布，使按钮背景透明。
         */
        QPixmap pix(28, 28);
        pix.fill(Qt::transparent);

        /*
         * 在像素图上绘制圆形颜色图标。
         *
         * QPainter p(&pix)：
         * 创建 QPainter，关联到 pix 像素图。
         * 所有绘制操作都会绘制到 pix 上。
         *
         * p.setRenderHint(QPainter::Antialiasing)：
         * 启用抗锯齿，使圆形边缘平滑。
         */
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);

        if (isSelected) {
            /*
             * 选中状态下绘制黑色环形外圈（高亮边框）：
             *
             * 绘制原理：先画一个较大的黑色实心圆作为"环底"，
             * 再在其上覆盖更小的颜色实心圆，露出黑色边缘形成环形。
             *
             * 这种"上色覆盖"的方式比直接绘制环形路径更可靠，
             * 可以保证环形间隙在各个方向上完全均匀（均为 2 像素）。
             *
             * 步骤一：绘制黑色实心圆（环形底色）
             *   drawEllipse(2, 2, 24, 24)：
             *     在 28x28 画布上，距各边 2px 的位置画直径 24px 的黑色实心圆。
             *     这个圆将作为选中高亮的黑色环形背景。
             *
             * 步骤二：在黑色圆之上覆盖颜色实心圆
             *   drawEllipse(4, 4, 20, 20)：
             *     在黑色圆的内部画一个更小的颜色圆，
             *     距各边 4px，直径 20px。
             *     这样黑色圆从边缘向内露出 2px 宽度的黑色环形，
             *     形成均匀的选中高亮效果。
             */
            // 黑色实心底层（环形基础）
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(0, 0, 0));
            p.drawEllipse(2, 2, 24, 24);

            // 内部颜色圆（覆盖在黑色环之上）
            p.setBrush(colors[i].color);
            if (colors[i].color == QColor(255, 255, 255) || colors[i].color.lightness() > 240) {
                p.setPen(QPen(QColor(180, 180, 180), 1));
            } else {
                p.setPen(Qt::NoPen);
            }
            p.drawEllipse(4, 4, 20, 20);
        } else {
            /*
             * 未选中状态：简单绘制色块圆形。
             *
             * setBrush(colors[i].color)：
             *   使用颜色填充。
             *
             * 白色或亮色绘制浅灰色边框以便区分。
             *
             * drawEllipse(2, 2, 24, 24)：
             *   绘制直径 24 像素的圆形色块。
             */
            p.setBrush(colors[i].color);
            if (colors[i].color == QColor(255, 255, 255) || colors[i].color.lightness() > 240) {
                p.setPen(QPen(QColor(180, 180, 180), 1));
            } else {
                p.setPen(Qt::NoPen);
            }
            p.drawEllipse(2, 2, 24, 24);
        }

        /*
         * p.end()：
         * 结束绘制，释放 QPainter 占用的资源。
         * QPainter 的析构函数会自动调用 end()，
         * 但显式调用是一种好习惯。
         */
        p.end();

        /*
         * btn->setIcon(QIcon(pix))：
         * 将绘制好的像素图设置为按钮的图标。
         * QIcon 可以包含多个不同状态的图片（正常、禁用、选中等）。
         *
         * setIconSize(QSize(24, 24))：
         * 设置图标的显示尺寸为 24x24 像素。
         * 图标会自动缩放以适应此尺寸（保持宽高比）。
         */
        btn->setIcon(QIcon(pix));
        btn->setIconSize(QSize(24, 24));

        /*
         * 设置按钮的样式表。
         * "QPushButton { background: transparent; border: 2px solid transparent; border-radius: 14px; }"
         *   - 透明背景
         *   - 透明边框（预留边框空间，防止悬停时布局变化）
         *   - 14px 圆角（28x28 按钮的一半，形成圆形）
         *
         * "QPushButton:hover { border: 2px solid #888; }"
         *   鼠标悬停时显示灰色边框（淡入淡出效果需要 Qt 处理）。
         */
        /*
         * 设置按钮的样式表：
         * "background: transparent"             —— 透明背景
         * "border: none"                        —— 无边框（去掉默认的按钮边框和悬停灰色环形）
         * "border-radius: 14px"                 —— 14px 圆角（28x28 按钮的一半，形成圆形）
         *
         * 注意：移除了 hover 灰色边框（border: 2px solid #888），
         * 因为点击后按钮的 hover 状态残留会在图标外围显示灰色环形，
         * 与面板刚打开时的样式不一致。选中状态已由图标内部的黑色环形清晰指示。
         */
        btn->setStyleSheet(
            "QPushButton { background: transparent; border: none; border-radius: 14px; }"
        );

        /*
         * m_buttonGroup->addButton(btn, i)：
         * 将按钮添加到按钮组，并分配 ID 为 i（0-4）。
         * 按钮组会管理按钮的互斥选中状态。
         */
        m_buttonGroup->addButton(btn, i);

        /*
         * m_popupLayout->addWidget(btn, 0, i, Qt::AlignCenter)：
         * 将按钮添加到网格布局的第 0 行、第 i 列。
         * Qt::AlignCenter 使按钮在单元格中居中。
         */
        m_popupLayout->addWidget(btn, 0, i, Qt::AlignCenter);
    }

    /*
     * adjustSize()：
     * 根据子控件和布局自动调整面板的大小。
     * 它会遍历所有子控件，计算所需的总尺寸。
     * 最终面板大小由颜色按钮 + 边距 + 间距决定。
     */
    m_popupPanel->adjustSize();

    /*
     * 计算面板的显示位置。
     *
     * mapToGlobal(QPoint(0, height() + 2))：
     * 将当前控件内部的坐标 (0, height+2) 转换为屏幕全局坐标。
     *   - x = 0：与当前控件左对齐
     *   - y = 当前控件底部 + 2px 间距
     *
     * 这样面板会出现在三角箭头的正下方。
     */
    QPoint globalPos = mapToGlobal(QPoint(0, height() + 2));

    /*
     * 防止面板超出屏幕右侧边界。
     *
     * QApplication::primaryScreen()：
     * 获取主屏幕对象。在 Android 上通常是唯一的屏幕。
     *
     * screen->geometry()：
     * 获取屏幕的几何信息（位置和尺寸）。
     *
     * 如果面板右侧超出屏幕右边界，向左偏移。
     */
    QScreen* screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int panelRight = globalPos.x() + m_popupPanel->width();
        if (panelRight > screenGeometry.right()) {
            globalPos.setX(screenGeometry.right() - m_popupPanel->width() - 8);
        }
        // 防止超出屏幕左侧边界
        if (globalPos.x() < screenGeometry.left()) {
            globalPos.setX(screenGeometry.left() + 8);
        }
    }

    /*
     * move(globalPos)：
     * 将面板移动到计算出的位置。
     */
    m_popupPanel->move(globalPos);
}

/*
 * onColorButtonClicked(int id) —— 颜色按钮点击的槽函数。
 *
 * 用户点击面板中的某个颜色按钮时触发。
 *
 * 槽函数的访问权限是 private slots，意味着：
 *   - 只能在类内部使用
 *   - 可以被信号连接（这是 slots 关键字的本质作用）
 *
 * @param id 被点击按钮的 ID（0 = 淡蓝, 1 = 粉色, 2 = 黄色, 3 = 白色, 4 = 黑色）
 */
void ColorPickerWidget::onColorButtonClicked(int id)
{
    /*
     * 在函数内部定义颜色数组。
     * 这是 C++ 允许的局部结构体定义。
     * 虽然每调用一次就定义一次，但编译器会优化掉。
     */
    struct { QString name; QColor color; } colors[5] = {
        {QString::fromUtf8("淡蓝"), QColor(220, 240, 255)},
        {QString::fromUtf8("粉色"), QColor(255, 200, 210)},
        {QString::fromUtf8("黄色"), QColor(255, 255, 200)},
        {QString::fromUtf8("白色"), QColor(255, 255, 255)},
        {QString::fromUtf8("黑色"), QColor(50, 50, 50)},
    };

    /*
     * 检查 ID 是否在有效范围内（0-4）。
     * 防御性编程：即使按钮组传入了错误的 ID，也不会访问越界。
     */
    if (id >= 0 && id < 5) {
        /*
         * 更新当前选中颜色和名称。
         * m_selectedColor 和 m_selectedName 是成员变量，
         * colorSelected 信号会携带 m_selectedColor 发送。
         */
        m_selectedColor = colors[id].color;
        m_selectedName = colors[id].name;

        /*
         * 发射颜色变更信号。
         * emit 是 Qt 的关键字（宏），用于发射信号。
         * 信号发射后，所有连接到这个信号的槽函数会被调用。
         * 连接方式是直接连接（DirectConnection）：槽函数在信号
         * 发射的线程中立即执行。
         */
        emit colorSelected(m_selectedColor);

        /*
         * 刷新弹出面板中所有按钮的图标。
         *
         * 由于 m_selectedColor 已经更新，需要让每个按钮重新绘制图标，
         * 使原先选中项的黑色环形消失，新选中项显示黑色环形。
         *
         * 实现方式：遍历按钮组中的所有按钮，根据每个按钮的 ID
         * 判断是否被选中，重新生成对应的 QPixmap 图标并设置到按钮上。
         * 这样不需要销毁和重建整个面板，避免了面板闪烁。
         */
        if (m_popupPanel) {
            QList<QAbstractButton*> btns = m_buttonGroup->buttons();
            for (int i = 0; i < btns.size(); ++i) {
                QPushButton* btn = qobject_cast<QPushButton*>(btns[i]);
                if (!btn) continue;
                int btnId = m_buttonGroup->id(btn);  // 获取按钮 ID（0-4）

                /*
                 * 判断此按钮对应的颜色是否为当前选中颜色。
                 * 如果匹配，绘制黑色环形选中态；
                 * 否则绘制普通色块。
                 */
                const QColor& btnColor = colors[btnId].color;
                bool isSelected = (btnColor == m_selectedColor);

                /*
                 * 重新生成 28x28 的按钮图标。
                 * 绘制逻辑与 createPopupPanel() 中完全一致。
                 */
                QPixmap pix(28, 28);
                pix.fill(Qt::transparent);
                QPainter p(&pix);
                p.setRenderHint(QPainter::Antialiasing);

                if (isSelected) {
                    // 选中态：黑色实心底层 + 内部颜色圆
                    p.setPen(Qt::NoPen);
                    p.setBrush(QColor(0, 0, 0));
                    p.drawEllipse(2, 2, 24, 24);

                    p.setBrush(btnColor);
                    if (btnColor == QColor(255, 255, 255) || btnColor.lightness() > 240) {
                        p.setPen(QPen(QColor(180, 180, 180), 1));
                    } else {
                        p.setPen(Qt::NoPen);
                    }
                    p.drawEllipse(4, 4, 20, 20);
                } else {
                    // 未选中态：纯色块
                    p.setBrush(btnColor);
                    if (btnColor == QColor(255, 255, 255) || btnColor.lightness() > 240) {
                        p.setPen(QPen(QColor(180, 180, 180), 1));
                    } else {
                        p.setPen(Qt::NoPen);
                    }
                    p.drawEllipse(2, 2, 24, 24);
                }
                p.end();

                btn->setIcon(QIcon(pix));
                btn->setIconSize(QSize(24, 24));
            }
        }

        /*
         * update()：
         * 请求 Qt 重新绘制当前控件（三角箭头区域）。
         */
        update();
    }
}

/*
 * setCurrentColor(const QColor& color) —— 从外部设置当前颜色。
 *
 * 用于在初始化时加载保存的颜色设置。
 * 如果颜色与当前不同，更新选择并刷新绘制。
 *
 * @param color 要设置的颜色
 */
void ColorPickerWidget::setCurrentColor(const QColor& color)
{
    /*
     * 仅当颜色不同时更新。
     * QColor 的 operator!= 逐个分量比较（R, G, B, A）。
     * 这样可以避免不必要的重绘。
     */
    if (color != m_selectedColor) {
        /*
         * 在颜色数组中查找匹配的颜色。
         * 如果找到，更新 m_selectedColor 和 m_selectedName。
         * 如果没找到（不属于预设的 5 种颜色），不更新。
         *
         * 注意：由于只支持预设的 5 种颜色，如果外部传入的颜色
         * 不在列表中，设置将被忽略。
         */
        struct { QString name; QColor color; } colors[5] = {
            {QString::fromUtf8("淡蓝"), QColor(220, 240, 255)},
            {QString::fromUtf8("粉色"), QColor(255, 200, 210)},
            {QString::fromUtf8("黄色"), QColor(255, 255, 200)},
            {QString::fromUtf8("白色"), QColor(255, 255, 255)},
            {QString::fromUtf8("黑色"), QColor(50, 50, 50)},
        };
        for (int i = 0; i < 5; ++i) {
            if (colors[i].color == color) {
                m_selectedColor = colors[i].color;
                m_selectedName = colors[i].name;
                break;  // 找到后跳出循环
            }
        }
        /*
         * 请求重绘。
         * 虽然三角箭头的颜色不随选中颜色改变，
         * 但调用 update() 保持规范，为将来可能的视觉变化做好准备。
         */
        update();
    }
}

/*
 * paintEvent(QPaintEvent* event) —— 绘制三角箭头。
 *
 * QWidget 的虚函数，当控件需要重绘时被 Qt 自动调用。
 * 调用时机：
 *   1. 控件首次显示时
 *   2. 调用 update() 或 repaint() 时
 *   3. 控件被遮挡后重新暴露时
 *   4. 控件大小改变时
 *
 * 这里绘制一个向下的灰色三角箭头 ▼，表示"点击展开"。
 *
 * @param event 绘图事件，包含需要重绘的区域
 */
void ColorPickerWidget::paintEvent(QPaintEvent* event)
{
    /*
     * Q_UNUSED(event)：
     * Qt 提供的宏，用于在函数中标记未使用的参数。
     * 它防止编译器发出"未使用参数"的警告。
     * 这里 event 参数虽然传入但不需要使用（重绘整个控件）。
     */
    Q_UNUSED(event)

    /*
     * 创建 QPainter 对象，开始绘制。
     * QPainter(this) 表示在当前控件上绘制。
     * 注意：QPainter 必须在 paintEvent() 中创建，不能在外部。
     * 这是因为绘制操作必须在系统的绘制事件上下文中进行。
     */
    QPainter painter(this);

    /*
     * 启用抗锯齿。
     * 对三角箭头也启用，让倾斜的边缘更平滑。
     */
    painter.setRenderHint(QPainter::Antialiasing);

    /*
     * 计算三角箭头的顶点坐标。
     *
     * 三角箭头由一个等腰三角形表示：
     *   ▲ （但这里绘制的是向下的版本）
     *        ____
     *    ->  \  /   <- 尖端朝下
     *         \/
     *
     * arrowSize = 5：三角形边长的一半（总宽度为 10 像素，高度为 5 像素）。
     * centerY = height() / 2：控件的垂直中心。
     * centerX = width() / 2：控件的水平中心。
     * arrowTop = centerY - arrowSize + 2：三角形上边缘的位置（微调使视觉居中）。
     *
     * QPolygon 是一个由 QPoint 组成的多边形。
     * 这里使用三个点定义三角形：
     *   - 左上顶点： (centerX - arrowSize, arrowTop)
     *   - 右上顶点： (centerX + arrowSize, arrowTop)
     *   - 底部顶点： (centerX, arrowTop + arrowSize)
     *
     * << 运算符是 QPolygon 的添加点操作符重载。
     */
    int arrowSize = 5;
    int centerY = height() / 2;
    int centerX = width() / 2;
    int arrowTop = centerY - arrowSize + 2;
    QPolygon arrow;
    arrow << QPoint(centerX - arrowSize, arrowTop)
          << QPoint(centerX, arrowTop + arrowSize)
          << QPoint(centerX + arrowSize, arrowTop);

    /*
     * 设置填充色和描边：
     *   setBrush(QColor(160, 160, 160))：灰色填充
     *   setPen(Qt::NoPen)：不描边
     */
    painter.setBrush(QColor(160, 160, 160));
    painter.setPen(Qt::NoPen);

    /*
     * drawPolygon(const QPolygon&)：
     * 绘制多边形。会自动连接首尾点形成封闭图形。
     * 内部使用当前设置的 brush 填充，pen 描边。
     */
    painter.drawPolygon(arrow);
}

/*
 * mousePressEvent(QMouseEvent* event) —— 鼠标点击事件处理。
 *
 * 当用户在控件上按下鼠标按钮时被 Qt 调用。
 * 这里忽略 event 参数（不关心点击的位置和按钮），
 * 直接弹出颜色选择面板。
 */
void ColorPickerWidget::mousePressEvent(QMouseEvent* event)
{
    /*
     * Q_UNUSED(event)：
     * 标记 event 参数未使用，避免编译器警告。
     */
    Q_UNUSED(event)

    /*
     * 创建弹出面板。
     * 面板是在每次点击时重新创建的，以保证按钮的选中状态更新。
     * 虽然效率略低，但保证了界面的实时性。
     */
    createPopupPanel();

    /*
     * 如果面板创建成功，显示它。
     *
     * show() 会：
     *   1. 设置窗口为可见状态
     *   2. 触发窗口的 showEvent
     *   3. 触发子控件的递归显示
     *   4. 请求重绘（插入 QPaintEvent）
     */
    if (m_popupPanel) {
        m_popupPanel->show();
    }
}

// ==================== SettingsWidget 实现 ====================

/*
 * 静态常量颜色选项数组的定义和初始化。
 *
 * 由于 s_colorOptions 在头文件中被声明为 static const，
 * 必须在 cpp 文件中提供实际的初始值。
 *
 * 这里定义的 5 种颜色与 ColorPickerWidget 中的颜色列表一致。
 */
const SettingsWidget::ColorOption SettingsWidget::s_colorOptions[5] = {
    {QString::fromUtf8("淡蓝"), QColor(220, 240, 255)},
    {QString::fromUtf8("粉色"), QColor(255, 200, 210)},
    {QString::fromUtf8("黄色"), QColor(255, 255, 200)},
    {QString::fromUtf8("白色"), QColor(255, 255, 255)},
    {QString::fromUtf8("黑色"), QColor(50, 50, 50)},
};

/*
 * SettingsWidget 构造函数。
 *
 * 初始化设置页面：
 *   1. 初始化成员变量指针为 nullptr
 *   2. 调用 setupUI() 创建所有子控件
 *
 * @param parent 父窗口指针
 */
SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)             // 调用基类 QWidget 的构造函数
    , titleLabel(nullptr)         // 初始化标题标签指针为 nullptr
    , colorSettingLabel(nullptr)  // 初始化颜色设置标签指针为 nullptr
    , colorPicker(nullptr)        // 初始化颜色选择器指针为 nullptr
    , backButton(nullptr)         // 初始化返回按钮指针为 nullptr
{
    /*
     * 调用 setupUI() 创建所有子控件和布局。
     * 这是将复杂的 UI 初始化逻辑从构造函数中分离出来的常见做法。
     */
    setupUI();
}

/*
 * 析构函数。
 *
 * = default 表示让编译器生成默认的析构函数。
 * 编译器生成的析构函数会：
 *   1. 执行空的函数体
 *   2. 按照成员变量声明的逆序调用成员变量的析构函数
 *   3. 调用基类的析构函数
 *
 * 由于 Qt 的对象树机制，子控件会自动被父对象销毁，
 * 所以不需要手动 delete。
 */
SettingsWidget::~SettingsWidget() = default;

/*
 * loadSavedColor() —— 静态函数：从持久化存储加载颜色。
 *
 * 静态函数可以在没有对象实例的情况下调用：
 *   QColor color = SettingsWidget::loadSavedColor();
 *
 * 使用 QSettings 读取之前保存的 backgroundColor。
 * 如果从未保存过，返回默认的淡蓝色。
 *
 * @return 保存的颜色，或默认淡蓝
 */
QColor SettingsWidget::loadSavedColor()
{
    /*
     * QSettings 构造函数：
     * QSettings("组织名", "应用名")。
     * 组织名和应用名决定了配置文件的存储路径。
     *
     * 在 Android 上，QSettings 使用系统自带的 SharedPreferences，
     * 存储路径为：/data/data/包名/shared_prefs/ 下。
     *
     * 这里使用中文作为组织名和应用名。
     */
    QSettings settings(QString::fromUtf8("CalculationTools"), QString::fromUtf8("App"));

    /*
     * settings.value("key", defaultValue)：
     * 从配置中读取指定键的值。
     * 如果键不存在，返回指定的默认值。
     *
     * value() 返回 QVariant 类型（Qt 的通用变体类型）。
     * QVariant 可以存储各种 Qt 类型：int、QString、QColor、QRect 等。
     *
     * canConvert<QColor>() 检查 QVariant 是否能够转换为 QColor。
     * 这是类型安全的检查，防止读取了错误类型的数据。
     */
    QVariant colorVar = settings.value(QString::fromUtf8("backgroundColor"), QColor(220, 240, 255));
    if (colorVar.canConvert<QColor>()) {
        /*
         * value<QColor>()：
         * QVariant 的模板方法，将存储的值转换为 QColor。
         * 如果之前保存时正确存储了 QColor 类型，可以成功转换。
         */
        return colorVar.value<QColor>();
    }
    /*
     * 如果无法转换（例如配置文件损坏），返回默认颜色。
     */
    return QColor(220, 240, 255);
}

/*
 * saveColor() —— 静态函数：保存颜色到持久化存储。
 *
 * @param color 要保存的颜色
 */
void SettingsWidget::saveColor(const QColor& color)
{
    /*
     * 创建 QSettings 对象，组织名和应用名必须与读取时一致。
     */
    QSettings settings(QString::fromUtf8("CalculationTools"), QString::fromUtf8("App"));

    /*
     * setValue("key", value)：
     * 将键值对写入配置文件。
     * QColor 类型会被 Qt 自动序列化为可存储的格式（字符串）。
     *
     * 写入后，下次启动应用时可以通过 loadSavedColor() 读取。
     */
    settings.setValue(QString::fromUtf8("backgroundColor"), color);
}

/*
 * paintEvent(QPaintEvent* event) —— 绘制设置页面背景。
 *
 * 设置页面的背景是纯白色，与主界面的颜色区分开。
 * 白色背景给人"设置面板"的视觉印象。
 */
void SettingsWidget::paintEvent(QPaintEvent* event)
{
    /*
     * Q_UNUSED(event)：
     * 标记未使用的参数。
     */
    Q_UNUSED(event)

    /*
     * 创建 QPainter 对象。
     */
    QPainter painter(this);

    /*
     * fillRect(QRect, QColor)：
     * 用纯白色 (255, 255, 255) 填充整个窗口区域。
     * rect() 返回当前控件的矩形区域（从 (0,0) 到 (width, height)）。
     */
    painter.fillRect(rect(), QColor(255, 255, 255));
}

/*
 * setupUI() —— 初始化设置页面的 UI 组件。
 *
 * 这是设置页面的核心布局函数，负责：
 *   1. 创建 QScrollArea（滚动区域）
 *   2. 创建顶部标题栏（返回按钮 + "设置"标题）
 *   3. 创建颜色设置行（标签 + 颜色选择器）
 *   4. 连接信号槽
 *   5. 加载已保存的颜色
 *
 * 布局层次：
 *   SettingsWidget (QWidget)
 *     └── QVBoxLayout (外部布局)
 *           └── QScrollArea (滚动区域)
 *                 └── QWidget (内容容器)
 *                       ├── QVBoxLayout (主布局)
 *                       │     ├── QHBoxLayout (顶部栏)
 *                       │     │     ├── QPushButton "←" (返回)
 *                       │     │     └── QLabel "设置" (标题)
 *                       │     ├── Spacer 20px
 *                       │     └── QHBoxLayout (颜色设置行)
 *                       │           ├── QLabel "修改背景颜色"
 *                       │           ├── Stretch (弹性空间)
 *                       │           └── ColorPickerWidget (三角箭头)
 *                       └── Stretch (底部弹性空间)
 */
void SettingsWidget::setupUI()
{
    /*
     * setAutoFillBackground(true)：
     * 启用自动填充背景。
     * 当启用时，Qt 会在调用 paintEvent() 之前用调色板的
     * Window 角色颜色自动填充控件的背景。
     * 这样可以避免手动在 paintEvent 中绘制背景。
     */
    setAutoFillBackground(true);

    // ===== 外层布局：使用 QScrollArea 支持上下滚动 =====

    /*
     * 创建外部垂直布局，设置无边距。
     * 这个布局只包含一个子控件：QScrollArea。
     */
    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    /*
     * 创建滚动区域。
     *
     * QScrollArea 的工作原理：
     *   1. 创建一个视口（viewport），作为滚动内容的窗口
     *   2. 将子控件放入视口中
     *   3. 当子控件大小超过视口时，显示滚动条
     *   4. 滚动条可以拖拽或使用鼠标滚轮
     *
     * setWidgetResizable(true)：
     *   设置为 true 时，滚动区域会动态调整子控件的大小。
     *   子控件会被设置为视口的大小（当小于视口时）
     *   或保持自身大小（当大于视口时）。
     */
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    /*
     * 设置滚动区域的样式表。
     *
     * "QScrollArea { border: none; background: white; }"
     *   - 无边框
     *   - 白色背景
     *
     * 滚动条样式（垂直滚动条）：
     * "QScrollBar:vertical { width: 8px; background: #f0f0f0; border-radius: 4px; }"
     *   - 宽度 8 像素
     *   - 浅灰色背景
     *   - 4px 圆角
     *
     * "QScrollBar::handle:vertical { background: #ccc; border-radius: 4px; min-height: 30px; }"
     *   - 滚动条滑块（拖拽控件）
     *   - 灰色
     *   - 4px 圆角
     *   - 最小高度 30px
     *
     * "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
     *   - 隐藏上下箭头按钮（在移动端不实用）
     */
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; background: white; }"
        "QScrollBar:vertical { width: 14px; background: #f0f0f0; border-radius: 7px; }"
        "QScrollBar::handle:vertical { background: #ccc; border-radius: 7px; min-height: 30px; margin: 2px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
    );

    // ===== 内容容器 =====

    /*
     * 创建内容容器控件。
     * 这个控件将作为 QScrollArea 的内容（通过 setWidget() 设置）。
     * 所有实际的 UI 控件都放在这个容器中。
     */
    QWidget* contentWidget = new QWidget();
    contentWidget->setStyleSheet("background: white;");

    /*
     * 创建主垂直布局。
     * 垂直排列所有设置项。
     *
     * setContentsMargins(0, 30, 0, 40)：
     *   上边距 30px，下边距 40px，左右无边距。
     *
     * setSpacing(10)：
     *   控件之间的垂直间距为 10px。
     */
    QVBoxLayout* mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(0, 30, 0, 40);
    mainLayout->setSpacing(10);

    // ===== 顶部栏：返回箭头 + "设置"标题 =====

    /*
     * 创建水平布局，放置返回按钮和标题。
     * 左右排列，返回按钮在左，标题在右。
     */
    QHBoxLayout* topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 0, 0, 0);

    /*
     * 创建返回按钮（显示 "←" 符号）。
     *
     * QPushButton 是 Qt 的标准按钮控件。
     * 它可以显示文本、图标或两者兼有。
     *
     * 设置字体大小为 18 点，不加粗。
     */
    backButton = new QPushButton(QString::fromUtf8("←"), contentWidget);
    QFont backFont = backButton->font();
    backFont.setPointSize(18);
    backFont.setBold(false);
    backButton->setFont(backFont);
    backButton->setFixedSize(44, 44);                        // 固定大小 44x44px
    backButton->setCursor(Qt::PointingHandCursor);           // 手型光标

    /*
     * 设置按钮样式表：
     * 正常状态：透明背景，深灰色文字，无边框
     * 悬停状态：半透明黑色背景（5% 透明度）
     */
    backButton->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent;"
        "  color: #333;"
        "  border: none;"
        "  font-size: 20px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(0, 0, 0, 0.05);"
        "  border-radius: 6px;"
        "}"
    );

    /*
     * 创建 "设置" 标题标签。
     *
     * QLabel 是 Qt 的标签控件，用于显示：
     *   - 纯文本
     *   - 富文本（HTML）
     *   - 图片（QPixmap）
     *
     * setAlignment(Qt::AlignLeft | Qt::AlignVCenter)：
     *   左对齐、垂直居中。
     */
    titleLabel = new QLabel(QString::fromUtf8("设置"), contentWidget);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /*
     * 将按钮和标题添加到水平布局。
     * addWidget(backButton) ：添加返回按钮
     * addWidget(titleLabel, 1)：添加标题，拉伸因子为 1
     *   - 拉伸因子（stretch factor）控制控件在布局中占用的空间比例
     *   - 1 表示标题会占据剩余的所有空间
     * addStretch() ：添加弹性空间，占位用
     */
    topBarLayout->addWidget(backButton);
    topBarLayout->addWidget(titleLabel, 1);
    topBarLayout->addStretch();
    mainLayout->addLayout(topBarLayout);

    /*
     * 添加 20px 的垂直间距。
     * addSpacing(int) 添加固定大小的空白空间。
     */
    mainLayout->addSpacing(20);

    // ===== "修改背景颜色" 配置行 =====

    /*
     * 创建水平布局，放置 "修改背景颜色" 标签和颜色选择器。
     */
    QHBoxLayout* colorLayout = new QHBoxLayout();
    colorLayout->setContentsMargins(20, 0, 20, 0);

    /*
     * 创建 "修改背景颜色" 标签。
     * 字体大小 13 点。
     * 显式设置文本颜色为黑色（#333），防止在 Android 上因父级样式
     * 继承导致文字颜色异常（显示为白色而不可见）。
     */
    colorSettingLabel = new QLabel(QString::fromUtf8("修改背景颜色"), contentWidget);
    QFont labelFont = colorSettingLabel->font();
    labelFont.setPointSize(13);
    colorSettingLabel->setFont(labelFont);
    colorSettingLabel->setStyleSheet("color: #333; background: transparent;");
    colorLayout->addWidget(colorSettingLabel);

    /*
     * 添加弹性空间，将标签和选择器推到两端。
     * 标签靠左，选择器靠右。
     */
    colorLayout->addStretch();

    /*
     * 创建自定义颜色选择器控件（三角箭头）。
     */
    colorPicker = new ColorPickerWidget(contentWidget);
    colorLayout->addWidget(colorPicker);

    mainLayout->addLayout(colorLayout);

    // ===== 浅色分割线 =====

    /*
     * 在颜色设置行下方添加一条浅灰色分割线。
     *
     * QFrame 是 QWidget 的子类，用于显示各种框架效果。
     * 设置 QFrame::HLine 使其显示为一条水平线。
     *
     * setFrameShape(QFrame::HLine)：
     *   设置为水平线形状。QFrame 支持多种形状：
     *   - QFrame::HLine  ：水平线
     *   - QFrame::VLine  ：垂直线
     *   - QFrame::Box    ：矩形框
     *   - QFrame::Panel  ：面板
     *   - QFrame::StyledPanel：带样式的面板
     *
     * setFrameShadow(QFrame::Sunken)：
     *   设置为凹陷阴影效果（与 QFrame::HLine 组合时，
     *   在大多数平台上显示为一条线）。
     *
     * 样式表设置颜色和边距：
     *   "color: #e0e0e0;"    —— 浅灰色线条
     *   "margin-left: 20px;" —— 左侧边距 20px，与颜色标签对齐
     *   "margin-right: 20px;"—— 右侧边距 20px，保持对称
     *   "max-height: 1px;"   —— 最大高度 1px，防止分割线过粗
     *
     * 这样设置后，当未来添加更多设置项时，每个设置项之间
     * 都可以放置一条分割线，使页面布局更清晰工整。
     */
    QFrame* separatorLine = new QFrame(contentWidget);
    separatorLine->setFrameShape(QFrame::HLine);
    separatorLine->setFrameShadow(QFrame::Sunken);
    separatorLine->setStyleSheet(
        "color: #e0e0e0;"
        "margin-left: 20px;"
        "margin-right: 20px;"
        "max-height: 1px;"
    );
    mainLayout->addWidget(separatorLine);

    /*
     * 添加弹性空间，将所有设置项推到顶部。
     * 这样当页面内容不足一屏时，设置项聚集在顶部，
     * 而不是垂直居中。
     */
    mainLayout->addStretch();

    // ===== 设置滚动区域的内容 =====

    /*
     * setWidget(contentWidget)：
     * 将内容容器设置为滚动区域的子控件。
     * 由于 setWidgetResizable 为 true，
     * 内容容器会自动调整大小以适应视口。
     */
    scrollArea->setWidget(contentWidget);

    /*
     * 将滚动区域添加到外部布局。
     * 外部布局会确保滚动区域填满整个 SettingsWidget。
     */
    outerLayout->addWidget(scrollArea);

    // ===== 连接信号槽 =====

    /*
     * 连接颜色选择器的选中信号。
     *
     * 当用户在颜色选择器中点击某个颜色时：
     *   1. ColorPickerWidget 发射 colorSelected(color) 信号
     *   2. 这个 lambda 被调用，参数是选中的颜色
     *   3. 保存颜色到 QSettings
     *   4. 发射 colorChanged 信号通知 MainWindow
     *
     * 注意：这里使用 this 作为接收者（context 对象），
     * 当 this（SettingsWidget）被销毁时，连接自动断开。
     * 这是 Qt 的安全连接机制（安全断开）。
     */
    connect(colorPicker, &ColorPickerWidget::colorSelected,
            this, [this](const QColor& color) {
        /*
         * 保存颜色到持久化存储。
         * saveColor() 是静态函数，可以直接调用。
         */
        saveColor(color);

        /*
         * 发射 colorChanged 信号。
         * MainWindow 连接此信号后，会更新所有界面的背景色。
         */
        emit colorChanged(color);
    });

    /*
     * 连接返回按钮的点击信号。
     * 当用户点击返回按钮时，发射 backToMain 信号。
     * MainWindow 连接此信号后执行滑出动画。
     *
     * clicked 是 QPushButton 继承自 QAbstractButton 的信号，
     * 当按钮被点击时发射。
     */
    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit backToMain();
    });

    /*
     * 加载已保存的颜色并设置为颜色选择器的当前颜色。
     *
     * loadSavedColor() 是静态函数，返回之前保存的颜色。
     * 如果从未保存过，返回默认淡蓝色。
     *
     * setCurrentColor() 会让颜色选择器更新其内部状态，
     * 使得下一次弹出面板时，正确的颜色按钮显示选中状态。
     */
    QColor savedColor = loadSavedColor();
    colorPicker->setCurrentColor(savedColor);
}
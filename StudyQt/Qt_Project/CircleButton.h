/*
 * ============================================================================
 * 文件：CircleButton.h
 * 描述：圆形按钮/键盘容器类的头文件
 *
 * CircleButton 是本应用的核心 UI 控件，具有双重角色：
 *   1. 容器角色（isContainer = true）：
 *      作为计算器键盘按钮的父容器，管理 21 个按键的布局和绘制
 *   2. 按钮角色（isContainer = false）：
 *      作为一个圆形或圆角矩形按钮，用于"汉堡菜单图标"等场景
 *
 * 关键技术特性：
 *   - 使用 QPropertyAnimation 实现点击颜色过渡动画（点击按压 -> 灰色闪烁 -> 恢复白色）
 *   - 通过 QPainter 自绘实现圆形/圆角矩形效果
 *   - 阴影效果（QGraphicsDropShadowEffect）增加层次感
 *   - 全局背景色静态变量（s_bgColor）用于在 UI 更新时同步颜色变化
 *   - 单击连接计算器逻辑，长按实现按键连续触发
 * ============================================================================
 */

#ifndef CIRCLEBUTTON_H
#define CIRCLEBUTTON_H

/*
 * QPushButton 是 Qt 的标准按钮控件。
 * 继承自 QAbstractButton，提供了：
 *   - clicked() / pressed() / released() 信号
 *   - 文本（text）、图标（icon）设置
 *   - checked 状态（toggle 按钮）
 *   - 快捷键（shortcut）支持
 *
 * CircleButton 继承 QPushButton：
 *   1. 保留 QPushButton 的点击事件处理能力
 *   2. 重写 paintEvent 实现自定义圆形/圆角矩形绘制
 *   3. 添加 QPropertyAnimation 实现颜色变化动画
 */
#include <QPushButton>

/*
 * QPainter —— Qt 的 2D 绘图引擎，在 paintEvent 中使用
 * QPropertyAnimation —— Qt 的属性动画类，实现属性值的平滑过渡
 * QGraphicsDropShadowEffect —— 图形阴影效果，为按钮添加阴影
 * QWidget —— 所有 UI 控件的基类（QPushButton 间接继承自 QWidget）
 * QColor —— 颜色类，表示 RGB/HSL 颜色
 */
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QColor>

/**
 * @class CircleButton
 * @brief 圆形按钮/键盘容器类
 *
 * 这是本应用中最核心的 UI 组件，负责：
 *   1. 计算器键盘的创建和布局（通过 keyboardUI() 函数）
 *   2. 键盘按键的点击事件处理
 *   3. 按钮的自定义绘制（圆形或圆角矩形风格）
 *   4. 点击动画效果
 *
 * 绘制时机（paintEvent 被调用的场景）：
 *   - show() / setVisible(true) 时
 *   - update() / repaint() 被调用时
 *   - 窗口被遮挡后恢复显示时
 *   - 控件大小改变时
 *   - 属性动画更新时（通过 Q_PROPERTY 注册的属性变化时）
 *
 * 颜色动画流程（当按钮被点击时）：
 *   用户按下 -> backgroundColor 从白色(255,255,255) -> 灰色(200,200,200) [100ms]
 *           -> 松开后 backgroundColor 从灰色(200,200,200) -> 白色(255,255,255) [100ms]
 */
class CircleButton : public QPushButton
{
    /*
     * Q_OBJECT 宏必须包含，因为：
     *   1. 使用了信号槽（虽然这个类没有自定义信号，但通过 connect 连接了按钮点击）
     *   2. 使用了 Q_PROPERTY 宏（属性系统需要 MOC 支持）
     *   3. 需要元对象信息（class name、父类信息等）
     */
    Q_OBJECT

    /*
     * Q_PROPERTY 宏 —— 在 Qt 元对象系统中注册一个属性。
     *
     * 属性注册后，QPropertyAnimation 可以识别并动画该属性。
     *
     * 语法：Q_PROPERTY(type name READ getter WRITE setter NOTIFY signal)
     *
     * 参数说明：
     *   QColor backgroundColor     ：属性类型和名称
     *   READ backgroundColor        ：getter 函数名
     *   WRITE setBackgroundColor    ：setter 函数名
     *
     * 动画原理：
     *   1. 启动动画时，指定目标属性和起止值
     *   2. Qt 的动画框架在每一帧计算插值（线性插值）
     *   3. 通过 setter 函数将新值写入属性
     *   4. 写入后触发 update() 请求重绘
     *   5. paintEvent 中使用 QColor 的值绘制按钮背景
     *
     * 例如从白色(255,255,255) 到 灰色(200,200,200)：
     *   第 0% 时间：R=255, G=255, B=255
     *   第 50% 时间：R=228, G=228, B=228（中间值）
     *   第 100% 时间：R=200, G=200, B=200
     */
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)

public:
    /*
     * ========================================================================
     * 构造函数（三个重载版本）
     * ========================================================================
     */

    /*
     * 构造函数版本 1：仅指定文本和父对象。
     * 用于创建计算器键盘中的各个按键。
     *
     * @param str    按钮上显示的文本（如 "7"、"+"、"=" 等）
     * @param parent 父控件指针（通常是 KeyBoardUI 容器）
     */
    explicit CircleButton(const QString& str, QWidget* parent);

    /*
     * 构造函数版本 2：仅指定父对象（容器模式）。
     * 用于创建承载键盘整体布局的容器。
     *
     * 此构造函数将 isContainer 设为 true，
     * 在 paintEvent 中只绘制背景不绘制按钮样式。
     *
     * @param parent 父控件指针
     */
    explicit CircleButton(QWidget* parent);

    /*
     * 构造函数版本 3：指定位置、形状类型和父对象。
     * 用于创建"设置按钮"等特殊形状的按钮。
     *
     * @param x       左上角 x 坐标
     * @param y       左上角 y 坐标
     * @param shape   形状类型字符串（"rectangle" 表示圆角矩形，其他值表示圆形）
     * @param parent  父控件指针
     */
    CircleButton(int x, int y, const QString& shape, QWidget* parent);

    /*
     * ========================================================================
     * 析构函数
     * ========================================================================
     */
    ~CircleButton();

    /*
     * ========================================================================
     * 核心公共接口
     * ========================================================================
     */

    /*
     * 创建计算器键盘布局。
     *
     * 这个函数是 CircleButton 的核心功能之一。
     * 它在内部创建 21 个 QPushButton 作为子控件，
     * 并按照标准计算器的布局排列：
     *
     *   ┌────┬────┬────┬────┐
     *   │ MC │ MR │ M+ │ M- │  ← 内存操作行
     *   ├────┼────┼────┼────┤
     *   │ C  │ CE │ %  │ ÷  │  ← 清除/运算符行
     *   ├────┼────┼────┼────┤
     *   │ 7  │ 8  │ 9  │ ×  │  ← 数字行 1
     *   ├────┼────┼────┼────┤
     *   │ 4  │ 5  │ 6  │ -  │  ← 数字行 2
     *   ├────┼────┼────┼────┤
     *   │ 1  │ 2  │ 3  │ +  │  ← 数字行 3
     *   ├────┼────┼────┼────┤
     *   │ ±  │ 0  │ .  │ =  │  ← 功能行
     *   └────┴────┴────┴────┘
     *
     * 每个按钮的大小和间距由屏幕尺寸计算得出。
     */
    void keyboardUI();

    /*
     * 获取数字键盘按钮。
     * 根据行和列的索引从 numberkeyboard 数组中获取对应的按钮指针。
     *
     * @param row 行索引（0-4）
     * @param col 列索引（0-3）
     * @return 对应位置的 CircleButton 指针，如果索引越界则返回 nullptr
     */
    CircleButton* getNumberButton(int row, int col);

    /*
     * 设置按钮形状。
     * 根据传入的字符串修改 isRectangle 标志，决定绘制圆形还是圆角矩形。
     *
     * @param shape 形状类型（"circle" = 圆形, "rectangle" = 圆角矩形）
     */
    void setShape(const QString& shape);

    /*
     * 设置字体颜色。
     * 通过 QPalette 修改 ButtonText 角色的颜色来实现。
     *
     * @param color 新的字体颜色
     */
    void setFontColor(const QColor& color);

    /*
     * 设置字体加粗。
     * 通过 QFont::setBold() 实现。
     *
     * @param bold true 表示加粗，false 表示正常
     */
    void setFontBold(bool bold);

    /*
     * ========================================================================
     * 全局背景颜色管理（静态方法）
     * ========================================================================
     */

    /*
     * 设置全局背景颜色（静态函数）。
     *
     * 静态函数的特点：
     *   属于类本身，不依赖某个实例。
     *   修改 s_bgColor 静态变量，影响所有 CircleButton 实例。
     *
     * 调用时机：
     *   用户在设置页面选择新颜色时。
     *
     * @param color 新的背景颜色
     */
    static void setGlobalBackgroundColor(const QColor& color);

    /*
     * 获取当前全局背景颜色（静态函数）。
     *
     * @return 当前设定的全局背景颜色
     */
    static QColor globalBackgroundColor();

    /*
     * ========================================================================
     * Q_PROPERTY 属性的 getter/setter
     * ========================================================================
     */

    /*
     * 获取当前背景颜色。
     * 这个函数作为 Q_PROPERTY 的 READ 函数被注册，
     * 供 QPropertyAnimation 读取属性的当前值。
     *
     * @return 当前背景颜色
     */
    QColor getBackgroundColor() const;

    /*
     * 设置背景颜色。
     * 这个函数作为 Q_PROPERTY 的 WRITE 函数被注册，
     * 每当属性动画更新时，Qt 会调用此函数将新值写入。
     *
     * 函数内部会：
     *   1. 更新成员变量 backgroundColor
     *   2. 调用 update() 请求重绘
     *
     * @param color 新的背景颜色
     */
    void setBackgroundColor(const QColor& color);

protected:
    /*
     * ========================================================================
     * 事件处理函数（重写 QWidget 的虚函数）
     * ========================================================================
     */

    /*
     * 绘图事件处理器。
     *
     * 重写此函数实现自定义绘制：
     *   1. 如果是 isMenuToggle，绘制汉堡菜单 ☰ 图标
     *   2. 如果是 isRectangle，使用 QPushButton 的默认绘制
     *   3. 如果是 isContainer，只绘制背景色
     *   4. 默认情况下，绘制带阴影的圆形按钮和居中文本
     *
     * @param event 绘图事件，包含重绘区域信息
     */
    void paintEvent(QPaintEvent* event) override;

    /*
     * 鼠标按下事件处理器。
     *
     * 当用户按下按钮时被调用。
     * 在此函数中启动颜色动画的第一阶段（当前颜色 -> 灰色）。
     *
     * @param event 鼠标事件，包含点击位置、按钮类型等信息
     */
    void mousePressEvent(QMouseEvent* event) override;

    /*
     * 鼠标释放事件处理器。
     *
     * 当用户松开按钮时被调用。
     * 在此函数中标记动画完成（实际动画在 finish 信号处理中完成）。
     *
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    /*
     * ========================================================================
     * 成员变量
     * ========================================================================
     */

    /*
     * 容器模式标志。
     *
     * true  = 作为容器（键盘背景），不响应点击事件
     * false = 作为普通按钮，响应点击和动画
     *
     * 主要区别：
     *   - 容器模式下，paintEvent 只绘制背景，不绘制边框和文字
     *   - 容器模式下，不响应鼠标事件
     */
    bool isContainer;

    /*
     * 当前背景颜色（用于动画）。
     *
     * 默认为白色 (255, 255, 255) 或淡蓝色 (220, 240, 255)。
     * 通过 Q_PROPERTY 注册，供 QPropertyAnimation 驱动。
     * 动画过程：当前颜色 -> 灰(200,200,200) -> 当前颜色
     */
    QColor backgroundColor;

    /*
     * 颜色动画对象。
     *
     * QPropertyAnimation 类型实例。
     * 负责驱动 backgroundColor 属性的变化。
     *
     * 动画生命周期：
     *   点击时 start() -> 100ms 后 finished -> 再次点击 start()
     *   每次使用前需要重新设置 startValue 和 endValue。
     */
    QPropertyAnimation* colorAnimation;

    /*
     * 动画执行中标志。
     *
     * 防止在动画还未结束时再次触发新的动画。
     */
    bool isAnimating;

    /*
     * 鼠标按下状态标志。
     *
     * true  = 用户正在按住按钮
     * false = 按钮处于正常状态
     *
     * 用于在动画结束回调中判断是执行第二段动画（灰->白），
     * 还是用户还在按住（保持灰色）。
     */
    bool isPressed;

    /*
     * 矩形模式标志。
     *
     * true  = 绘制为圆角矩形（设置按钮使用）
     * false = 绘制为正圆形（计算器按键使用）
     */
    bool isRectangle;

    /*
     * 菜单按钮标志。
     *
     * 专门为"汉堡菜单"按钮设计。
     * 当为 true 时，绘制三条横线的汉堡图标 ☰。
     */
    bool isMenuToggle;

    /*
     * 阴影效果对象。
     *
     * QGraphicsDropShadowEffect 为控件添加投影：
     *   - 阴影颜色：半透明黑色
     *   - 阴影偏移：通常是向右和向下偏移
     *   - 模糊半径：控制阴影的柔和度
     *
     * 阴影通过 setGraphicsEffect() 添加到控件上。
     */
    QGraphicsDropShadowEffect* shadowEffect;

    /*
     * 数字键盘按钮数组（5行 x 4列）。
     *
     * 存储计算器所有按键的指针，共 20 个槽位。
     * 布局如下：
     *   [0,0]="AC" [0,1]="<-"  [0,2]="+/-" [0,3]="÷"
     *   [1,0]="7"  [1,1]="8"   [1,2]="9"   [1,3]="×"
     *   [2,0]="4"  [2,1]="5"   [2,2]="6"   [2,3]="-"
     *   [3,0]="1"  [3,1]="2"   [3,2]="3"   [3,3]="+"
     *   [4,0]="%"  [4,1]="0"   [4,2]="."   [4,3]="="
     *
     * 通过 getNumberButton(row, col) 访问。
     */
    CircleButton* numberkeyboard[5][4];

    /*
     * 设置按钮指针。
     *
     * 指向键盘容器内的"设置"图标按钮。
     * 在计算器界面中用于打开设置页面。
     */
    CircleButton* SettingButton;

    /*
     * ========================================================================
     * 静态成员变量
     * ========================================================================
     */

    /*
     * 全局背景颜色（静态）。
     *
     * 所有 CircleButton 实例共享此变量。
     * 用于在用户修改背景色后，统一更新所有按钮的边框颜色。
     *
     * 静态成员变量必须在类外单独定义和初始化（在 cpp 文件中）。
     */
    static QColor s_bgColor;
};

#endif // CIRCLEBUTTON_H

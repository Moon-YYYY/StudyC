#ifndef CIRCLEBUTTON_H
#define CIRCLEBUTTON_H
#include<QPushButton>
#include<QString>
#include<QPainter>
#include<QPropertyAnimation>
#include<QColor>

/**
 * @class CircleButton
 * @brief 圆形按钮类，支持阴影效果和颜色过渡动画
 * 
 * 该类继承自QPushButton，实现了圆形按钮的绘制、阴影效果以及点击时的颜色过渡动画
 */
class CircleButton : public QPushButton{
    Q_OBJECT
    // 声明backgroundColor属性，用于颜色动画
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
private:
    QString str;              // 按钮显示的文本
    bool isContainer;         // 是否为容器按钮（用于键盘布局）
    CircleButton* numberkeyboard[5][4]; // 数字键盘按钮数组
    CircleButton* SettingButton; // 设置按钮
    QColor backgroundColor;   // 按钮背景颜色
    bool isRectangle;         // 是否为矩形按钮入口
    QPropertyAnimation* colorAnimation; // 颜色过渡动画
    QColor targetColor;       // 目标颜色（未使用，预留）
    bool isAnimating;         // 动画状态标志
    bool isPressed;           // 按钮按下状态标志
public:
    /**
     * @brief 构造函数，创建带文本的圆形按钮
     * @param str 按钮显示的文本
     * @param parent 父窗口
     */
    CircleButton(const QString& str, QWidget* parent = nullptr);
    
    /**
     * @brief 构造函数，创建容器按钮
     * @param parent 父窗口
     */
    CircleButton(QWidget* parent = nullptr);
    /**
     * @brief 构造函数，创建菜单按钮入口
     * @param x 按钮位置的x坐标
     * @param y 按钮位置的y坐标
     * @param shape 按钮形状
     * @param parent 父窗口
     */
    CircleButton(int x, int y, const QString& shape = "" , QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~CircleButton();

    /**
     * @brief 设置键盘的排列布局
     */
    void keyboardUI();

    /**
     * @brief 重写paintEvent，实现圆形按钮的绘制
     * @param Event 绘图事件
     */
    void paintEvent(QPaintEvent* Event) override;
    
    /**
     * @brief 获取当前背景颜色
     * @return 当前背景颜色
     */
    QColor getBackgroundColor() const;
    
    /**
     * @brief 设置背景颜色
     * @param color 新的背景颜色
     */
    void setBackgroundColor(const QColor& color);
    
    /**
     * @brief 获取数字键盘按钮
     * @param row 行索引
     * @param col 列索引
     * @return 对应的圆形按钮
     */
    CircleButton* getNumberButton(int row, int col);
    
    /**
     * @brief 设置按钮形状
     * @param shape 按钮形状
     */
    void setShape(const QString& shape);
    
    /**
     * @brief 设置字体颜色
     * @param color 字体颜色
     */
    void setFontColor(const QColor& color);
    
    /**
     * @brief 设置字体加粗
     * @param bold 是否加粗
     */
    void setFontBold(bool bold);

protected:
    /**
     * @brief 重写mousePressEvent，处理鼠标按下事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent* event) override;
    
    /**
     * @brief 重写mouseReleaseEvent，处理鼠标释放事件
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // CIRCLEBUTTON_H

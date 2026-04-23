#include "CircleButton.h"
#include <QMouseEvent>
#include <QSequentialAnimationGroup>

/**
 * @brief 构造函数，创建带文本的圆形按钮
 * @param str 按钮显示的文本
 * @param parent 父窗口
 */
CircleButton::CircleButton(const QString& str, QWidget* parent)
    :QPushButton(str, parent), isContainer(false), backgroundColor(QColor(255, 255, 255)),
     isAnimating(false), isPressed(false), isRectangle(false){
    // 设置按钮样式为透明背景，无边框
    setStyleSheet("background: transparent; border: none;");
    
    // 初始化颜色动画
    colorAnimation = new QPropertyAnimation(this, "backgroundColor", this);
    colorAnimation->setDuration(100); // 动画持续时间：0.1秒
    colorAnimation->setEasingCurve(QEasingCurve::InOutCubic); // 使用三次方缓动曲线，使动画更加自然
    
    // 连接动画结束信号
    connect(colorAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (isPressed) {
            // 第一阶段动画结束，开始第二阶段动画（从灰色变回白色）
            colorAnimation->setStartValue(QColor(200, 200, 200));
            colorAnimation->setEndValue(QColor(255, 255, 255));
            colorAnimation->start();
            isPressed = false;
        } else {
            // 第二阶段动画结束，动画完成
            isAnimating = false;
        }
    });
}

CircleButton::CircleButton(QWidget* parent)
    :QPushButton(parent), isContainer(true), backgroundColor(QColor(255, 255, 255)),
     isAnimating(false), isPressed(false), isRectangle(false){
    // 设置按钮样式为透明背景，无边框
    setStyleSheet("background: transparent; border: none;");
    
    // 初始化数字键盘数组
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 4; ++j){
            numberkeyboard[i][j] = nullptr;
        }
    }
    
    // 初始化设置按钮
    SettingButton = nullptr;
    
    // 初始化颜色动画
    colorAnimation = new QPropertyAnimation(this, "backgroundColor", this);
    colorAnimation->setDuration(100); // 动画持续时间：0.1秒
    colorAnimation->setEasingCurve(QEasingCurve::InOutCubic); // 使用三次方缓动曲线，使动画更加自然
    
    // 连接动画结束信号
    connect(colorAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (isPressed) {
            // 第一阶段动画结束，开始第二阶段动画（从灰色变回白色）
            colorAnimation->setStartValue(QColor(200, 200, 200));
            colorAnimation->setEndValue(QColor(255, 255, 255));
            colorAnimation->start();
            isPressed = false;
        } else {
            // 第二阶段动画结束，动画完成
            isAnimating = false;
        }
    });
}

CircleButton::CircleButton(int x, int y, const QString& shape, QWidget* parent):
    QPushButton(parent), isContainer(false), backgroundColor(QColor(220, 240, 255)),
     isAnimating(false), isPressed(false), isRectangle(true){
       // 设置按钮位置
    move(x, y);
    // 设置按钮大小
    setFixedSize(50, 50);
    // 设置按钮形状
    setShape(shape);
    // 设置按钮文本
    setText("...");
    // 设置按钮字体大小
    setFont(QFont("Arial", 16, QFont::Bold));
    // 设置按钮样式：文本居中，背景淡蓝色用rgb写，无边框
    setStyleSheet("QPushButton {"
              "text-align: center;"
              "border: none;"
              "background-color: rgb(220, 240, 255);"
              "}"
              "QPushButton:pressed {"
              "background-color: rgba(187, 195, 200, 1);"
              "}"
              "QPushButton:focus {"
              "outline: none;"
              "}");

    // 设置按钮背景颜色为淡蓝色
    setBackgroundColor(backgroundColor);
    
    // // 初始化颜色动画
     //colorAnimation = new QPropertyAnimation(this, "backgroundColor", this);
    // colorAnimation->setDuration(100); // 动画持续时间：0.1秒
    // colorAnimation->setEasingCurve(QEasingCurve::InOutCubic); // 使用三次方缓动曲线，使动画更加自然
    
    // // 连接动画结束信号
    // connect(colorAnimation, &QPropertyAnimation::finished, this, [this]() {
    //     if (isPressed) {
    //         // 第一阶段动画结束，开始第二阶段动画（从灰色变回白色）
    //         colorAnimation->setStartValue(QColor(200, 200, 200));
    //         colorAnimation->setEndValue(QColor(255, 255, 255));
    //         colorAnimation->start();
    //         isPressed = false;
    //     } else {
    //         // 第二阶段动画结束，动画完成
    //         isAnimating = false;
    //     }
    // });
}

/**
 * @brief 设置按钮形状
 * @param shape 按钮形状
 */
void CircleButton::setShape(const QString& shape){
    if(shape == "circle"){
        isRectangle = false;
    } else if(shape == "rectangle"){
        isRectangle = true;
    }
    update(); // 触发重绘
}

/**
 * @brief 设置字体颜色
 * @param color 字体颜色
 */
void CircleButton::setFontColor(const QColor& color){
    QPalette palette = this->palette();
    palette.setColor(QPalette::ButtonText, color);
    this->setPalette(palette);
}

/**
 * @brief 设置字体加粗
 * @param bold 是否加粗
 */
void CircleButton::setFontBold(bool bold){
    QFont font = this->font();
    font.setBold(bold);
    this->setFont(font);
}

/**
 * @brief 析构函数，释放资源
 */
CircleButton::~CircleButton(){
    // 释放数字键盘按钮内存
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 4; ++j){
            delete numberkeyboard[i][j];
        }
    }
    //释放设置按钮的内存
    delete SettingButton;
    // 释放颜色动画内存
    delete colorAnimation;
}

/**
 * @brief 设置键盘的排列布局
 */
void CircleButton::keyboardUI(){
    // 设置按键的数字和符号
    QString str[5][4] = {{"AC", "<-", "+/-", "÷"},
                         {"7", "8", "9", "×"},
                         {"4", "5", "6", "-"},
                         {"1", "2", "3", "+"},
                         {"%", "0", ".", "="}};
    
    // 计算按钮的宽
    int buttonSizeX = this->width() / 4 - 10;

    // 计算按钮的高
    int buttonSizeY = (this->height() * 0.6 - 50) / 5;

    // 取宽高最小值，防止出现重叠现象
    int buttonSize = std::min(buttonSizeX, buttonSizeY);
    
    // 确保buttonSize为正数，防止按钮大小为负数
    if (buttonSize < 20) {
        buttonSize = 40; // 设置最小按钮大小
    }
    
    // 计算按钮的横向每个按钮间距，屏幕宽度减去左边距再减去四个按钮的尺寸除间距数量
    int buttonSpacingX = (this->width() - 20 - 4 * buttonSize) / 3;

    // 计算按钮的纵向每个按钮间距，0.6代表按钮占用0.6倍屏幕高度的空间，减掉40的底边距再减去按钮的尺寸*数量再除间距数量4
    int buttonSpacingY = (this->height() * 0.6 - 40 - 5 * buttonSize) / 4;

    // 计算横向排列坐标倍数
    int ButtonPosX = (buttonSize + buttonSpacingX);

    // 计算纵向排列坐标倍数
    int ButtonPosY = buttonSize + buttonSpacingY;

    // 使用循环申请空间，设置属性
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 4; ++j){
            numberkeyboard[i][j] = new CircleButton(str[i][j], this);
            numberkeyboard[i][j]->resize(buttonSize, buttonSize);
            numberkeyboard[i][j]->move(10 + j * ButtonPosX, this->height() - 20 - (5 - i) * ButtonPosY);
            numberkeyboard[i][j]->show();
            numberkeyboard[i][j]->raise(); // 确保数字按钮在容器按钮之上
        }
    }
}

/**
 * @brief 获取当前背景颜色
 * @return 当前背景颜色
 */
QColor CircleButton::getBackgroundColor() const {
    return backgroundColor;
}

/**
 * @brief 设置背景颜色
 * @param color 新的背景颜色
 */
void CircleButton::setBackgroundColor(const QColor& color) {
    backgroundColor = color;
    update(); // 触发重绘，使颜色变化生效
}

/**
 * @brief 获取数字键盘按钮
 * @param row 行索引
 * @param col 列索引
 * @return 对应的圆形按钮
 */
CircleButton* CircleButton::getNumberButton(int row, int col) {
    if (row >= 0 && row < 5 && col >= 0 && col < 4) {
        return numberkeyboard[row][col];
    }
    return nullptr;
}

/**
 * @brief 重写mousePressEvent，处理鼠标按下事件
 * @param event 鼠标事件
 */
void CircleButton::mousePressEvent(QMouseEvent* event) {
    // 调用父类的mousePressEvent方法
    QPushButton::mousePressEvent(event);
    //如果是方形直接返回
    if(isRectangle){
        return;
    }
    // 如果动画正在进行中，不处理新的按下事件
    if (isAnimating) {
        return;
    }
    
    // 开始第一阶段动画：从当前颜色到灰色
    isAnimating = true;
    isPressed = true;
    if(colorAnimation != nullptr){
    colorAnimation->stop(); // 停止当前动画，防止动画叠加
    colorAnimation->setStartValue(backgroundColor); // 设置动画起始值为当前颜色
    colorAnimation->setEndValue(QColor(200, 200, 200)); // 设置动画结束值为灰色
    colorAnimation->start(); // 开始动画
    }
}

/**
 * @brief 重写mouseReleaseEvent，处理鼠标释放事件
 * @param event 鼠标事件
 */
void CircleButton::mouseReleaseEvent(QMouseEvent* event) {
    // 调用父类的mouseReleaseEvent方法
 
    QPushButton::mouseReleaseEvent(event);
    // 鼠标释放时不需要立即开始第二阶段动画
    // 动画会在第一阶段结束后自动开始第二阶段
    //如果是方形直接返回
    if(isRectangle){
        return;
    }
    
    isAnimating = false;
}

/**
 * @brief 重写paintEvent，实现圆形按钮的绘制
 * @param Event 绘图事件
 */
void CircleButton::paintEvent(QPaintEvent* Event){
    //如果是方形直接返回
       if(isRectangle){
        QPushButton::paintEvent(Event);
        return;
    }
    QPainter painter(this);

    // 如果是容器则使用与主窗口协调的背景颜色
    if(isContainer){
        // 绘制背景
        painter.setBrush(QColor(220, 240, 255));  // 与主窗口一致的淡蓝色背景
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect());  // 绘制整个矩形区域
        return;
    }
    // 开启抗锯齿，使绘制更加平滑
    painter.setRenderHint(QPainter::Antialiasing);

    // 检查按钮是否被按下或动画是否正在进行
    bool isButtonPressed = isDown() || isAnimating;

    
    // 只有当按钮未被按下且动画未在进行时才绘制阴影
    if(!isButtonPressed) {
        painter.setBrush(QColor(0, 0, 0, 40)); // 黑色半透明阴影
        painter.setPen(Qt::NoPen);
            painter.drawEllipse(1, 1, rect().width() - 1, rect().height() - 1); // 绘制圆形阴影
    }

    // 绘制按钮背景
    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, rect().width() - 2, rect().height() - 2); // 绘制圆形按钮
    
    
    // 绘制按钮文本
    painter.setPen(Qt::black); // 设置文本颜色为黑色
    painter.setFont(font()); // 使用按钮的字体
    painter.drawText(rect(), Qt::AlignCenter, text()); // 居中绘制文本

}


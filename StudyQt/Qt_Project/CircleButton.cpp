#include "CircleButton.h"
CircleButton::CircleButton(const QString& str, QWidget* parent)
    :QPushButton(str, parent), isContainer(false){
    setStyleSheet("background: transparent; border: none;");
    //resize(buttonSize, buttonSize);
}

CircleButton::CircleButton(QWidget* parent)
    :QPushButton(parent), isContainer(true){
    ///////////////背景：透明的： 边框：没有
    setStyleSheet("background: transparent; border: none;");
}

CircleButton::~CircleButton(){
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 4; ++j){
            delete numberkeyboard[i][j];
        }
    }
}

void CircleButton::keyboardUI(){
    //设置按键的数字和符号
    QString str[5][4] = {{"AC", "<-", "+/-", "÷"},
                         {"7", "8", "9", "×"},
                         {"4", "5", "6", "-"},
                         {"1", "2", "3", "+"},
                         {"%", "0", ".", "="}};

    //计算按钮的宽
    int buttonSizeX = this->width() / 4 - 10;

    //计算按钮的高
    int buttonSizeY = (this->height() * 0.6 - 50) / 5;

    //取宽高最小值，防止出现重叠现象
    int buttonSize = std::min(buttonSizeX, buttonSizeY);

    //计算按钮的横向每个按钮间距，屏幕宽度减去左边距再减去四个按钮的尺寸除间距数量
    int buttonSpacingX = (this->width() - 20 - 4 * buttonSize) / 3;

    //计算按钮的纵向每个按钮间距，0.6代表按钮占用0.6倍屏幕高度的空间，减掉40的底边距再减去按钮的尺寸*数量再除间距数量4
    int buttonSpacingY = (this->height() * 0.6 - 40 - 5 * buttonSize) / 4;

    //计算横向排列坐标倍数
    int ButtonPosX = (buttonSize + buttonSpacingX);

    //计算纵向排列坐标倍数
    int ButtonPosY = buttonSize + buttonSpacingY;

    //使用循环申请空间，设置属性
    for(int i = 0; i < 5; ++i){
        for(int j = 0; j < 4; ++j){
            numberkeyboard[i][j] = new CircleButton(str[i][j], this);
            numberkeyboard[i][j]->resize(buttonSize, buttonSize);
            numberkeyboard[i][j]->move(10 + j * ButtonPosX, this->height() - 20 - (5 - i) * ButtonPosY);
            numberkeyboard[i][j]->show();
        }
    }
}
void CircleButton::paintEvent(QPaintEvent* Event){
    QPainter painter(this);

    //如果是容器则让背景颜色变成奶白色
    if(isContainer){  QPainter painter(this);
        painter.setBrush(QColor("#e0ffff"));  // 设置背景颜色
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect());  // 绘制整个矩形区域
        return;
    }


    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    //是否按下
    bool isPressed = isDown();


    QColor backgroundColor = QColor(QColorConstants::Svg::lightpink);

    if(isPressed){
        backgroundColor = QColor(QColorConstants::Svg::linen);
    }

    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);

    painter.drawEllipse(rect());
    painter.setPen(Qt::black);
    painter.setFont(font());
    painter.drawText(rect(), Qt::AlignCenter, text());

}
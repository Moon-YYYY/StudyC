#include "menuButton.h"
#include <QPainter>
#include <QtCore>
#define buttonNumber 4
/**
 * @brief 构造函数
 * @param parent 父窗口
 */
menuButton::menuButton(QWidget *parent)
    : QWidget(parent)
{
    // 设置按钮样式
    setStyleSheet("background: transparent; border: none;");

}

/**
 * @brief 构造函数，创建带文本的菜单按钮
 * @param text 按钮显示的文本
 * @param parent 父窗口
 */
menuButton::menuButton(QString* str, int screenWidth, int screenHeight, QWidget *parent)
    : QWidget(parent), isVisible(false), screenwidth(screenWidth), screenheight(screenHeight)
{
    // 设置面板样式，添加背景色和边框
    setStyleSheet("background-color: rgb(255, 255, 255); border-right: 1px solid #ddd;");

    move(-screenWidth*0.7, 0);
    // 创建垂直布局管理器
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 20, 0, 0); // 顶部留出20像素空间，避免覆盖状态栏
    layout->setAlignment(Qt::AlignTop); // 确保按钮从顶部开始排列


    for(int i = 0; i < buttonNumber; i++){
        QPushButton* button = new QPushButton(str[i]);
        button->setStyleSheet("QPushButton {"
              "text-align: center;"
              "border: none;"
              "border-bottom: 1px solid #ddd;"
              "background-color: rgb(255, 255, 255);"
              "padding: 15px;"
              "margin: 0px;"
              "}"
              "QPushButton:pressed {"
              "background-color: rgba(187, 195, 200, 1);"
              "}"
              "QPushButton:focus {"
              "outline: none;"
              "}");
        button->setFixedHeight(50); // 设置固定高度
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // 设置尺寸策略
        layout->addWidget(button);
        buttons.append(button);

        // 连接信号槽
        connect(button, &QPushButton::clicked, this, &menuButton::hideMenu);

    }
    animation = new QPropertyAnimation(this, "geometry", this);
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutCubic);
}

/**
 * @brief 析构函数
 */
menuButton::~menuButton()
{
    animation->stop();
    delete animation;
    animation = nullptr;
    delete layout;
    layout = nullptr;
    buttons.clear();
    for(int i = 0; i < buttons.size(); ++i){
        buttons[i] = nullptr;
    }
    isVisible = false;
    setVisible(false);
    update();
}

/**
 * @brief 重写paintEvent，实现菜单按钮的绘制
 * @param event 绘图事件
 */
void menuButton::paintEvent(QPaintEvent* event)
{
    // 绘制面板背景
    QPainter painter(this);

    painter.setBrush(QColor(255, 255, 255));//设置填充颜色

    painter.setPen(QPen(QColor(221, 221, 221), 1));//绘制形状边框

    painter.drawRect(rect());
}
/**
 * @brief 显示菜单面板
 */
void menuButton::showMenu()
{
    if(isVisible){
        return;
    }
    
    QRect startRect(-width(), 0, width(), height());
    QRect endRect(0, 0, width(), height());
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->start();

    isVisible = true;
    show();
    raise();
    
}

/**
 * @brief 隐藏菜单面板
 */
 void menuButton::hideMenu()
{
    if(!isVisible){
        return;
    }

    QRect startRect(0, 0, width(), height());
    QRect endRect(-width(), 0, width(), height());
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->start();

    isVisible = false;
}
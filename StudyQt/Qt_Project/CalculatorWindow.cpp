#include "CalculatorWindow.h"
#include <QPainter>
#include <QScreen>
#include <QRect>
#include <QApplication>

/**
 * @brief 构造函数
 * @param parent 父窗口
 */
CalculatorWindow::CalculatorWindow(QWidget *parent)
    : QMainWindow(parent), displayText("0"), currentValue(0)
{
    // 获取屏幕尺寸
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // 设置窗口大小为屏幕尺寸
    this->resize(screenWidth, screenHeight);
    
    // 设置背景颜色为淡蓝色
    QPalette palette;
    palette.setBrush(QPalette::Window, QColor(220, 240, 255)); // 淡蓝色背景
    this->setPalette(palette);
    
    // 创建容器按钮
    containerButton = new CircleButton(this);
    containerButton->resize(screenWidth, screenHeight);
    containerButton->show();
    
    // 初始化键盘布局
    containerButton->keyboardUI();
    
    // 连接数字按钮的点击事件
    connectNumberButtons();
    
    // 确保容器按钮在显示区域下方
    containerButton->lower();
}

/**
 * @brief 连接数字按钮的点击事件
 */
void CalculatorWindow::connectNumberButtons() {
    // 数字按钮位置
    int numberButtonPositions[10][2] = {
        {4, 1}, // 0
        {3, 0}, // 1
        {3, 1}, // 2
        {3, 2}, // 3
        {2, 0}, // 4
        {2, 1}, // 5
        {2, 2}, // 6
        {1, 0}, // 7
        {1, 1}, // 8
        {1, 2}  // 9
    };
    
    // 连接数字按钮
    for (int i = 0; i < 10; ++i) {
        int row = numberButtonPositions[i][0];
        int col = numberButtonPositions[i][1];
        CircleButton* button = containerButton->getNumberButton(row, col);
        if (button) {
            connect(button, &QPushButton::clicked, this, [this, i]() {
                this->addNumber(QString::number(i));
            });
        }
    }
    
    // 连接小数点按钮
    CircleButton* dotButton = containerButton->getNumberButton(4, 2);
    if (dotButton) {
        connect(dotButton, &QPushButton::clicked, this, [this]() {
            this->addNumber(".");
        });
    }
}

/**
 * @brief 析构函数
 */
CalculatorWindow::~CalculatorWindow()
{
    delete containerButton;
}

/**
 * @brief 添加数字到显示区域
 * @param num 要添加的数字
 */
void CalculatorWindow::addNumber(const QString& num)
{
    // 如果当前显示为0且不是添加小数点，则替换为新数字
    if (displayText == "0" && num != ".") {
        displayText = num;
    } else {
        // 否则追加数字
        displayText += num;
    }
    // 更新显示
    update();
}

/**
 * @brief 重写paintEvent，实现计算器界面的绘制
 * @param event 绘图事件
 */
void CalculatorWindow::paintEvent(QPaintEvent* event)
{
    QMainWindow::paintEvent(event);
    
    QPainter painter(this);
    
    // 绘制显示区域
    painter.setBrush(QColor(255, 255, 255)); // 白色背景
    painter.setPen(Qt::NoPen);
    int displayHeight = height() * 0.3;
    painter.drawRect(10, 10, width() - 20, displayHeight);
    
    // 绘制显示文本
    painter.setPen(Qt::black); // 黑色文本
    painter.setFont(QFont("Arial", 30, QFont::Bold)); // 黑体
    painter.drawText(10, 10, width() - 20, displayHeight, Qt::AlignRight | Qt::AlignVCenter, displayText);
}

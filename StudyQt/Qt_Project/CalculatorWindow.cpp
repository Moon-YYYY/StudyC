#include "CalculatorWindow.h"
#include <QScreen>
#include <QRect>
#include <QApplication>
#include <QLineEdit>
#include "CircleButton.h"

/**
 * @brief 构造函数
 * @param parent 父窗口
 */
CalculatorWindow::CalculatorWindow(QWidget *parent)
    : QMainWindow(parent), currentValue(0)
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
    
    // 创建容器按钮（覆盖整个屏幕）
    containerButton = new CircleButton(this);
    // 容器按钮覆盖整个屏幕，这样就能获取到完整的屏幕大小
    containerButton->setGeometry(0, 0, screenWidth, screenHeight);
    containerButton->show();
    
    // 初始化键盘布局
    containerButton->keyboardUI();
    
    // 创建显示区域
    displayEdit = new QLineEdit("0");
    displayEdit->setParent(this);
    displayEdit->setReadOnly(true);
    displayEdit->setAlignment(Qt::AlignRight);
    displayEdit->setFont(QFont("Arial", 30, QFont::Bold));
    ///////////////////////////////////////////////////////////rgb在此处设置输入框的颜色////////////////////////////////////////////////////////
    displayEdit->setStyleSheet("QLineEdit { background-color: rgb(220, 240, 255); border: none; padding: 10px; }"); // 淡蓝色背景，与按钮背景一致
    // 输入框显示在屏幕上方，占屏幕高度的30%
    displayEdit->setGeometry(0, 0, screenWidth, screenHeight * 0.3);
    displayEdit->show();
    
    // 确保输入框在容器按钮之上
    displayEdit->raise();
    
    // 连接数字按钮的点击事件
    connectNumberButtons();
}

/**
 * @brief 析构函数
 */
CalculatorWindow::~CalculatorWindow()
{
    delete containerButton;
    delete displayEdit;
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
 * @brief 添加数字到显示区域
 * @param num 要添加的数字
 */
void CalculatorWindow::addNumber(const QString& num)
{
    // 获取当前显示的文本
    QString currentText = displayEdit->text();
    
    // 如果当前显示为0且不是添加小数点，则替换为新数字
    if (currentText == "0" && num != ".") {
        displayEdit->setText(num);
    } else {
        // 否则追加数字
        displayEdit->setText(currentText + num);
    }
}



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
CalculatorWindow::CalculatorWindow(CircleButton* keyBoardUI, QWidget *parent)
    : QWidget(parent), KeyBoardUI(keyBoardUI), currentValue(0)
{
    // 获取屏幕尺寸
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // 设置窗口大小为屏幕尺寸*0.3
    this->resize(screenWidth, screenHeight * 0.2);
    
    // 设置背景颜色为淡蓝色
    QPalette palette;
    palette.setBrush(QPalette::Window, QColor(220, 240, 255)); // 淡蓝色背景
    this->setPalette(palette);
    
    // 初始化计算逻辑对象
    calculator = new CalculateWay();

    // 创建显示区域
    displayEdit = new QLineEdit("0");
    displayEdit->setParent(this);
    //设置只读状态（true）
    displayEdit->setReadOnly(true);
    //设置文本对齐方式（右对齐）
    displayEdit->setAlignment(Qt::AlignRight);
    //设置字体
    displayEdit->setFont(QFont("Arial", 30, QFont::Bold));
    ///////////////////////////////////////////////////////////rgb在此处设置输入框的颜色////////////////////////////////////////////////////////
    displayEdit->setStyleSheet("QLineEdit { background-color: rgb(220, 240, 255); border: none; padding: 10px; }"); // 淡蓝色背景，与按钮背景一致
    // 输入框显示在屏幕上方，占屏幕高度的30%
    displayEdit->setGeometry(0, 0, screenWidth, screenHeight * 0.3);
    displayEdit->show();
        
    // 连接数字按钮的点击事件
    connectNumberButtons();
}

/**
 * @brief 析构函数
 */
CalculatorWindow::~CalculatorWindow()
{
    delete displayEdit;
    delete calculator;
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
        CircleButton* button = KeyBoardUI->getNumberButton(row, col);
        if (button) {
            connect(button, &QPushButton::clicked, this, [this, i]() {
                this->addNumber(QString::number(i));
            });
        }
    }
    
    // 连接小数点按钮
    CircleButton* dotButton = KeyBoardUI->getNumberButton(4, 2);
    if (dotButton) {
        connect(dotButton, &QPushButton::clicked, this, [this]() {
            this->addNumber(".");
        });
    }
    
    // 连接AC按钮（归零）
    CircleButton* acButton = KeyBoardUI->getNumberButton(0, 0);
    if (acButton) {
        connect(acButton, &QPushButton::clicked, this, [this]() {
            displayEdit->setText("0");
            currentValue = 0;
        });
    }
    
    // 连接删除按钮
    CircleButton* deleteButton = KeyBoardUI->getNumberButton(0, 1);
    if (deleteButton) {
        connect(deleteButton, &QPushButton::clicked, this, [this]() {
            QString currentText = displayEdit->text();
            if (currentText.length() > 1) {
                currentText.chop(1);
                displayEdit->setText(currentText);
            } else {
                displayEdit->setText("0");
            }
        });
    }
    
    // 连接正负交换按钮
    CircleButton* plusMinusButton = KeyBoardUI->getNumberButton(0, 2);
    if (plusMinusButton) {
        connect(plusMinusButton, &QPushButton::clicked, this, [this]() {
            QString currentText = displayEdit->text();
            if (currentText == "0") return;
            
            double value = currentText.toDouble();
            double result = calculator->handlePlusMinus(value);
            displayEdit->setText(QString::number(result));
            currentValue = result;
        });
    }
    
    // 连接乘号按钮
    CircleButton* multiplyButton = KeyBoardUI->getNumberButton(1, 3);
    if (multiplyButton) {
        connect(multiplyButton, &QPushButton::clicked, this, [this]() {
            // 这里可以添加乘法运算逻辑
            this->addNumber("×");
        });
    }
    
    // 连接减号按钮
    CircleButton* subtractButton = KeyBoardUI->getNumberButton(2, 3);
    if (subtractButton) {
        connect(subtractButton, &QPushButton::clicked, this, [this]() {
            // 这里可以添加减法运算逻辑
            this->addNumber("-");
        });
    }
    
    // 连接加号按钮
    CircleButton* addButton = KeyBoardUI->getNumberButton(3, 3);
    if (addButton) {
        connect(addButton, &QPushButton::clicked, this, [this]() {
            // 这里可以添加加法运算逻辑
            this->addNumber("+");
        });
    }
    
    // 连接除号按钮
    CircleButton* divideButton = KeyBoardUI->getNumberButton(0, 3);
    if (divideButton) {
        connect(divideButton, &QPushButton::clicked, this, [this]() {
            // 这里可以添加除法运算逻辑
            this->addNumber("÷");
        });
    }
    
    // 连接等于号按钮
    CircleButton* equalsButton = KeyBoardUI->getNumberButton(4, 3);
    if (equalsButton) {
        connect(equalsButton, &QPushButton::clicked, this, [this]() {
            // 计算表达式结果
            QString currentText = displayEdit->text();
            double result = calculator->calculate(currentText);
            displayEdit->setText(QString::number(result));
            currentValue = result;
        });
    }
    
    // 连接百分号按钮
    CircleButton* percentButton = KeyBoardUI->getNumberButton(4, 0);
    if (percentButton) {
        connect(percentButton, &QPushButton::clicked, this, [this]() {
            // 处理百分比
            QString currentText = displayEdit->text();
            double value = currentText.toDouble();
            double result = calculator->handlePercent(value);
            displayEdit->setText(QString::number(result));
            currentValue = result;
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



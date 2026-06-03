#ifndef CALCULATORWINDOW_H
#define CALCULATORWINDOW_H

#include <QLineEdit>
#include "CircleButton.h"
#include "calculateWay.h"

/**
 * @class CalculatorWindow
 * @brief 计算器主窗口类
 * 
 * 该类继承自QMainWindow，实现了计算器的主界面和数字显示功能
 */
class CalculatorWindow : public QWidget
{
    Q_OBJECT

private:
    CircleButton* KeyBoardUI;      // 键盘UI，从MainWindow传递
    QLineEdit* displayEdit;        // 显示的文本框
    double currentValue;           // 当前值
    CalculateWay* calculator;      // 计算逻辑对象

public:
    /**
     * @brief 构造函数
     * @param keyBoardUI 键盘UI
     * @param parent 父窗口
     */
    CalculatorWindow(CircleButton* keyBoardUI, QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~CalculatorWindow();
    
    /**
     * @brief 添加数字到显示区域
     * @param num 要添加的数字
     */
    void addNumber(const QString& num);

private:
    /**
     * @brief 连接数字按钮的点击事件
     */
    void connectNumberButtons();

};

#endif // CALCULATORWINDOW_H
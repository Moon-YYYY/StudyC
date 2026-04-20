#ifndef CALCULATORWINDOW_H
#define CALCULATORWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "CircleButton.h"

/**
 * @class CalculatorWindow
 * @brief 计算器主窗口类
 * 
 * 该类继承自QMainWindow，实现了计算器的主界面和数字显示功能
 */
class CalculatorWindow : public QMainWindow
{
    Q_OBJECT

private:
    CircleButton* containerButton; // 容器按钮，用于放置数字键盘
    QLineEdit* displayEdit;        // 显示的文本框
    double currentValue;           // 当前值

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口
     */
    CalculatorWindow(QWidget *parent = nullptr);
    
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
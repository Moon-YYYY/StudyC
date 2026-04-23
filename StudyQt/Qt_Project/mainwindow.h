#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CircleButton.h"
#include "CalculatorWindow.h"
#include "menuButton.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    CircleButton* KeyBoardUI;//不能在这里进行new申请空间，会导致this指针指向不完整
    CalculatorWindow* calculatorwindow;
    CircleButton* SettingButton;//菜单入口按钮
    menuButton* menu;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

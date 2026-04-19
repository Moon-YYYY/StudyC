#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CircleButton.h"
#include <QString>
#include <QScreen>
#include <QRect>
#include <QApplication>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    
    KeyBoardUI = new CircleButton(this);
    KeyBoardUI->resize(screenWidth, screenHeight);//需要子部件也调用resize函数
    //调用键盘的渲染
    KeyBoardUI->keyboardUI();
    KeyBoardUI->show();//子部件的show依赖于父部件，所以父子都需要show
}

MainWindow::~MainWindow()
{
    delete KeyBoardUI;
    delete ui;
}

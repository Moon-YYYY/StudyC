#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CircleButton.h"
#include "CalculatorWindow.h"
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
    // 获取屏幕尺寸，primaryScreen返回一个Qscreen指针类型
    QScreen *screen = QApplication::primaryScreen();
    //geometry返回一个QRect类型,此属性保存着屏幕的几何尺寸（以像素为单位）
    QRect screenGeometry = screen->geometry();
    //定义一个int类型来接受Rect屏幕的宽和高
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // 设置窗口大小为屏幕尺寸
    this->resize(screenWidth, screenHeight);
    
    // 设置背景颜色为淡蓝色，Qpalette包含了每一个控件下的颜色组
    QPalette palette;
    /* 第一个参数 QPalette::Window
        这是 QPalette 类中定义的一个枚举值，表示调色板中的一个角色（Role）
        QPalette::Window 角色专门用于指定窗口部件的背景区域
        其他常见的角色还有 QPalette::Text （文本颜色）、 QPalette::Button （按钮背景）等
    */
    palette.setBrush(QPalette::Window, QColor(220, 240, 255)); // 淡蓝色背景
    //将这个调色应用到当前窗口，当前窗口是mainwindow
    this->setPalette(palette);
    //为按钮的ui申请新的内存空间

    KeyBoardUI = new CircleButton(this);
    KeyBoardUI->resize(screenWidth, screenHeight);//需要子部件也调用resize函数
    //调用键盘的渲染
    KeyBoardUI->keyboardUI();
    KeyBoardUI->show();//子部件的show依赖于父部件，所以父子都需要show

    // 侧边栏菜单触发按钮（汉堡图标按钮）
    SettingButton = new CircleButton(0, 20, "rectangle", this);
    SettingButton->show();
    SettingButton->raise(); // 确保设置按钮在所有控件之上

    calculatorwindow = new CalculatorWindow(KeyBoardUI, this);
    calculatorwindow->move(0, 120);
    calculatorwindow->show();
    calculatorwindow->raise();

    // 侧边栏菜单面板
    QString menuStr[4] = {"关于", "检查更新", "设置", "版本"};
    menu = new menuButton(menuStr, screenWidth, screenHeight, this);
    menu->resize(screenWidth * 0.7, screenHeight);

    // 初始状态：侧边栏隐藏，不显示
    // 点击汉堡按钮时弹出侧边栏
    connect(SettingButton, &QPushButton::clicked, menu, &menuButton::showMenu);

    // connect(menu, &menuButton::buttonsClicked, this, [=] (int index){
    //     if(index == 0){
    //         // 关于
    //     } else if(index == 1){
    //         // 检查更新
    //     } else if(index == 2){
    //         // 设置
    //     } else if(index == 3){
    //         // 版本
    //     }
    //     else{
    //         menu->hideMenu();
    //     }
    // });
}

MainWindow::~MainWindow()
{
    delete KeyBoardUI;
    delete ui;
}


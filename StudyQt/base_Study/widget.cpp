#include "widget.h"
#include "ui_widget.h"
#include<QPushButton>
#include"mypushbutton.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QPushButton* btn = new QPushButton;
    btn->setParent(this);
    btn->setText("first button");
    QPushButton* btn2 = new QPushButton("second button",this);//按照控件的大小创建窗口
    //移动btn2
    btn2->move(100, 100);
    resize(900, 600);//重置窗口大小
    //设置窗口标题
    setWindowTitle("第一个窗口");
    //设置固定窗口大小
    setFixedSize(900, 600);
    //创建mypushbutton对象
    MyPushButton* mybtn = new MyPushButton;
    mybtn->setText("我自己的按钮");
    mybtn->move(200, 200);
    mybtn->setParent(this);
}

Widget::~Widget()
{
    delete ui;
}

#include "mypushbutton.h"
#include<QDebug>
MyPushButton::MyPushButton(QWidget *parent) : QPushButton{parent}{
    qDebug() << "我的按钮类构造函数";//输出，类似cout
}
MyPushButton::~MyPushButton(){
    qDebug() << "我的按钮类析构函数";
}

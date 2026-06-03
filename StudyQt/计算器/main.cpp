#include "mainwindow.h"
#include <QApplication>

/**
 * @brief  应用程序入口，初始化Qt主窗口并进入事件循环
 * @param  argc 命令行参数个数
 * @param  argv 命令行参数数组
 * @return 应用程序退出码
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}

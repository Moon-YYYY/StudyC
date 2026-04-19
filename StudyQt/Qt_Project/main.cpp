#include "CalculatorWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalculatorWindow w;
    w.show();
    return QCoreApplication::exec();
}

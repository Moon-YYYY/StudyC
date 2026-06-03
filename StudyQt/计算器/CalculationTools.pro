QT       += core widgets

TARGET = CalculationTools
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    CircleButton.cpp \
    CalculatorWindow.cpp \
    calculateWay.cpp \
    menuButton.cpp

HEADERS += \
    mainwindow.h \
    CircleButton.h \
    CalculatorWindow.h \
    calculateWay.h \
    menuButton.h

FORMS += \
    mainwindow.ui

# 为Android构建添加必要的配置
android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

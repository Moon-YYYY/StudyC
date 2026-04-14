#ifndef CIRCLEBUTTON_H
#define CIRCLEBUTTON_H
#include<QPushButton>
#include<QString>
#include<QPainter>

class CircleButton : public QPushButton{
    Q_OBJECT
private:
    QString str;
    bool isContainer;
    CircleButton* numberkeyboard[5][4];
public:
    CircleButton(const QString& str, QWidget* parent = nullptr);
    CircleButton(QWidget* parent = nullptr);
    ~CircleButton();

    //设置键盘的排列布局
    void keyboardUI();

    void paintEvent(QPaintEvent* Event);
};

#endif // CIRCLEBUTTON_H

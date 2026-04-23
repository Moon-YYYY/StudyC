#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QList>

/**
 * @class menuButton
 * @brief 菜单按钮类
 * 
 * 该类继承自QWidget，用于创建菜单按钮
 */
class menuButton : public QWidget
{
    Q_OBJECT

private:
    bool isVisible;//菜单面板是否可见
    QVBoxLayout* layout;//菜单面板布局器
    QPropertyAnimation* animation;//菜单面板动画
    QList<QPushButton*> buttons;//菜单按钮列表
    int screenwidth;
    int screenheight;


public:
    /**
     * @brief 构造函数
     * @param parent 父窗口
     */
    menuButton(QWidget *parent = nullptr);
    
    /**
     * @brief 构造函数，创建带文本的菜单按钮
     * @param text 按钮显示的文本
     * @param parent 父窗口
     */
    menuButton(QString* str, int screenWidth, int screenHeight, QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~menuButton();

    /**
     * @brief 显示菜单面板
     */
    void showMenu();
    
    /**
     * @brief 隐藏菜单面板
     */
    void hideMenu();
    
protected:
    /**
     * @brief 重写paintEvent，实现菜单按钮的绘制
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;
};

#endif // MENUBUTTON_H

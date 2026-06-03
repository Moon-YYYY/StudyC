#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QList>
#include <QLabel>
#include <QGraphicsOpacityEffect>

#define buttonNumber 4

/**
 * @class menuButton
 * @brief 侧边菜单面板类
 *
 * 实现从左侧滑入的菜单面板，包含蒙层遮罩、渐变标题栏、图标按钮和滑入/淡入动画
 */
class menuButton : public QWidget
{
    Q_OBJECT

signals:
    /**
     * @brief 菜单项被点击时发射的信号
     * @param index 被点击按钮的索引（0-关于, 1-检查更新, 2-设置, 3-版本）
     */
    void menuButtonClicked(int index);

private:
    bool panelVisible;                 // 菜单面板是否可见
    QVBoxLayout* layout;              // 菜单面板布局器
    QPropertyAnimation* slideAnim;     // 面板滑入滑出动画
    QWidget* overlay;                  // 半透明蒙层（遮罩）
    QGraphicsOpacityEffect* overlayEffect; // 蒙层透明度效果
    QPropertyAnimation* overlayFadeAnim;   // 蒙层淡入淡出动画
    QWidget* headerWidget;             // 标题栏区域
    QLabel* headerLabel;               // 标题文字
    QPushButton* closeButton;          // 关闭按钮(X)
    QList<QPushButton*> buttons;       // 菜单按钮列表
    int screenwidth;
    int screenheight;

public:
    menuButton(QWidget *parent = nullptr);
    menuButton(QString* str, int screenWidth, int screenHeight, QWidget *parent = nullptr);
    ~menuButton();

    void showMenu();
    void hideMenu();

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // MENUBUTTON_H
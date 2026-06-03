#include "menuButton.h"
#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QEvent>
#include <QTimer>
#include <QtCore>

/**
 * @brief 构造函数（默认）
 * @param parent 父窗口
 */
menuButton::menuButton(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background: transparent; border: none;");
}

/**
 * @brief 构造函数，创建带菜单项的侧边栏面板
 * @param str 菜单项文本数组
 * @param screenWidth 屏幕宽度
 * @param screenHeight 屏幕高度
 * @param parent 父窗口（MainWindow）
 */
menuButton::menuButton(QString* str, int screenWidth, int screenHeight, QWidget *parent)
    : QWidget(parent), panelVisible(false), screenwidth(screenWidth), screenheight(screenHeight)
{
    int panelWidth = screenWidth * 0.7;

    // ========== 1. 创建蒙层遮罩 ==========
    overlay = new QWidget(parent);
    overlay->setGeometry(0, 0, screenWidth, screenHeight);
    overlay->setStyleSheet("background-color: rgb(0, 0, 0);");
    overlayEffect = new QGraphicsOpacityEffect(overlay);
    overlayEffect->setOpacity(0.0);
    overlay->setGraphicsEffect(overlayEffect);
    overlay->hide();
    // 点击蒙层关闭菜单
    overlay->installEventFilter(this);
    overlay->setAttribute(Qt::WA_Hover, true);

    // 蒙层淡入淡出动画
    overlayFadeAnim = new QPropertyAnimation(overlayEffect, "opacity", this);
    overlayFadeAnim->setDuration(300);
    overlayFadeAnim->setEasingCurve(QEasingCurve::InOutCubic);

    // ========== 2. 面板自身样式 ==========
    setStyleSheet("background-color: rgb(255, 255, 255);");
    move(-panelWidth, 0);
    resize(panelWidth, screenHeight);
    hide();

    // ========== 3. 面板滑入滑出动画 ==========
    slideAnim = new QPropertyAnimation(this, "pos", this);
    slideAnim->setDuration(350);
    slideAnim->setEasingCurve(QEasingCurve::OutCubic);

    // ========== 4. 创建主布局 ==========
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setAlignment(Qt::AlignTop);

    // ========== 5. 创建标题栏区域 ==========
    headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(60);
    headerWidget->setStyleSheet(
        "QWidget {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 rgb(66, 133, 244), stop:1 rgb(52, 168, 83));"
        "  border-bottom: 2px solid rgb(200, 200, 200);"
        "}"
    );

    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(16, 0, 12, 0);

    // 标题文字
    headerLabel = new QLabel("  菜单", headerWidget);
    headerLabel->setStyleSheet(
        "QLabel {"
        "  color: white;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  background: transparent;"
        "  border: none;"
        "}"
    );
    headerLayout->addWidget(headerLabel);
    headerLayout->addStretch();

    // 关闭按钮 (X)
    closeButton = new QPushButton("X", headerWidget);
    closeButton->setFixedSize(32, 32);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(
        "QPushButton {"
        "  color: white;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  background: transparent;"
        "  border: none;"
        "  border-radius: 16px;"
        "}"
        "QPushButton:hover {"
        "  background: rgba(255, 255, 255, 40);"
        "}"
        "QPushButton:pressed {"
        "  background: rgba(255, 255, 255, 80);"
        "}"
    );
    connect(closeButton, &QPushButton::clicked, this, &menuButton::hideMenu);
    headerLayout->addWidget(closeButton);

    layout->addWidget(headerWidget);

    // ========== 6. 创建菜单按钮 ==========
    QString icons[4] = {"i", "U", "S", "V"};  // 纯 ASCII 图标前缀

    for (int i = 0; i < buttonNumber; i++) {
        QPushButton* button = new QPushButton(icons[i] + "  " + str[i], this);
        button->setCursor(Qt::PointingHandCursor);
        button->setFixedHeight(52);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        button->setStyleSheet(
            "QPushButton {"
            "  text-align: left;"
            "  padding-left: 20px;"
            "  border: none;"
            "  border-bottom: 1px solid rgb(238, 238, 238);"
            "  background-color: rgb(255, 255, 255);"
            "  color: rgb(60, 60, 60);"
            "  font-size: 16px;"
            "}"
            "QPushButton:hover {"
            "  background-color: rgb(245, 245, 245);"
            "}"
            "QPushButton:pressed {"
            "  background-color: rgb(230, 230, 230);"
            "}"
        );
        layout->addWidget(button);
        buttons.append(button);

        // 点击菜单项后关闭面板
        int index = i;
        connect(button, &QPushButton::clicked, this, [this, index]() {
            emit menuButtonClicked(index);
            hideMenu();
        });
    }

    // 底部留白弹性空间
    layout->addStretch();
}

/**
 * @brief 析构函数
 */
menuButton::~menuButton()
{
    if (slideAnim) {
        slideAnim->stop();
        delete slideAnim;
        slideAnim = nullptr;
    }

    if (overlayFadeAnim) {
        overlayFadeAnim->stop();
        delete overlayFadeAnim;
        overlayFadeAnim = nullptr;
    }

    if (overlayEffect) {
        overlay->setGraphicsEffect(nullptr);
        delete overlayEffect;
        overlayEffect = nullptr;
    }

    delete overlay;
    overlay = nullptr;

    delete layout;
    layout = nullptr;

    buttons.clear();
    panelVisible = false;
}

/**
 * @brief 过滤事件，处理蒙层点击关闭
 */
bool menuButton::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == overlay && event->type() == QEvent::MouseButtonPress) {
        hideMenu();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

/**
 * @brief 重写 paintEvent，绘制面板阴影效果
 * @param event 绘图事件
 */
void menuButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制白色背景（圆角）
    painter.setBrush(QColor(255, 255, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, 0), 0, 0);

    // 右侧阴影条
    QLinearGradient gradient(rect().topRight(), QPointF(rect().topRight().x() + 10, rect().topRight().y()));
    gradient.setColorAt(0.0, QColor(0, 0, 0, 30));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 0));
    painter.setBrush(gradient);
    painter.drawRect(QRectF(rect().right() - 10, 0, 10, rect().height()));
}

/**
 * @brief 显示菜单面板（弹出侧边栏）
 */
void menuButton::showMenu()
{
    if (panelVisible) {
        return;
    }

    int panelWidth = width();

    // 1. 先显示蒙层并淡入
    overlay->setGeometry(0, 0, screenwidth, screenheight);
    overlay->show();
    overlay->raise();
    overlayFadeAnim->setStartValue(0.0);
    overlayFadeAnim->setEndValue(0.35);
    overlayFadeAnim->start();

    // 2. 面板移到初始位置并显示
    move(-panelWidth, 0);
    show();
    raise();

    // 3. 面板滑入动画
    slideAnim->setStartValue(QPoint(-panelWidth, 0));
    slideAnim->setEndValue(QPoint(0, 0));
    slideAnim->start();

    panelVisible = true;
}

/**
 * @brief 隐藏菜单面板（收起侧边栏）
 */
void menuButton::hideMenu()
{
    if (!panelVisible) {
        return;
    }

    int panelWidth = width();

    // 1. 蒙层淡出
    overlayFadeAnim->setStartValue(0.35);
    overlayFadeAnim->setEndValue(0.0);
    overlayFadeAnim->start();

    // 2. 面板滑出动画
    slideAnim->setStartValue(QPoint(0, 0));
    slideAnim->setEndValue(QPoint(-panelWidth, 0));
    slideAnim->start();

    // 3. 动画结束后隐藏
    // 使用定时器在动画完成后隐藏
    QTimer::singleShot(350, this, [this]() {
        hide();
        overlay->hide();
    });

    panelVisible = false;
}
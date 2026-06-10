#include "menuButton.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QEvent>
#include <QTimer>
#include <QtCore>
#include <QtMath>
#include <QStyle>

/**
 * @brief 自定义图标按钮，用 QPainter 绘制矢量图标
 */
class IconButton : public QPushButton {
    int iconType; // 0=关于(i) 1=更新(循环箭头) 2=设置(齿轮) 3=版本(V)
public:
    IconButton(const QString& text, int type, QWidget* parent = nullptr)
        : QPushButton(text, parent), iconType(type) {
        setCursor(Qt::PointingHandCursor);
        setFixedHeight(52);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setStyleSheet(
            "QPushButton {"
            "  text-align: left;"
            "  padding-left: 50px;"
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
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPushButton::paintEvent(event); // 先绘制按钮背景、文本

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int iconSize = 22;
        int x = 14;
        int y = (height() - iconSize) / 2;
        QRect iconRect(x, y, iconSize, iconSize);

        QColor iconColor(100, 100, 100);
        painter.setPen(QPen(iconColor, 2));
        painter.setBrush(Qt::NoBrush);

        switch (iconType) {
        case 0: { // 关于：字母 i 带圆圈
            painter.drawEllipse(iconRect.adjusted(2, 2, -2, -2));
            QFont font = painter.font();
            font.setPixelSize(12);
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(iconColor);
            painter.drawText(iconRect, Qt::AlignCenter, "i");
            break;
        }
        case 1: { // 更新：循环箭头
            int cx = iconRect.center().x();
            int cy = iconRect.center().y();
            int r = 8;
            // 画圆弧
            painter.drawArc(cx - r, cy - r, r * 2, r * 2, 30 * 16, 300 * 16);
            // 画箭头
            painter.setBrush(iconColor);
            QPoint pts[3] = {
                QPoint(cx + r - 2, cy - 4),
                QPoint(cx + r + 3, cy + 1),
                QPoint(cx + r - 2, cy + 6)
            };
            painter.drawPolygon(pts, 3);
            break;
        }
        case 2: { // 设置：齿轮
            int cx = iconRect.center().x();
            int cy = iconRect.center().y();
            int r = 6;
            painter.drawEllipse(cx - r, cy - r, r * 2, r * 2);
            for (int i = 0; i < 8; ++i) {
                double angle = i * 45.0 * 3.14159 / 180.0;
                int x1 = cx + int((r - 2) * cos(angle));
                int y1 = cy + int((r - 2) * sin(angle));
                int x2 = cx + int((r + 3) * cos(angle));
                int y2 = cy + int((r + 3) * sin(angle));
                painter.drawLine(x1, y1, x2, y2);
            }
            break;
        }
        case 3: { // 版本：字母 V
            QFont font = painter.font();
            font.setPixelSize(14);
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(iconColor);
            painter.drawText(iconRect, Qt::AlignCenter, "V");
            break;
        }
        case 4: { // 更新内容：字母 ≈
            QFont font = painter.font();
            font.setPixelSize(14);
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(iconColor);
            painter.drawText(iconRect, Qt::AlignCenter, "≈");
            break;
        }
        }
    }
};

/**
 * @brief 自定义关闭按钮，绘制 X 图标
 */
class CloseIconButton : public QPushButton {
public:
    CloseIconButton(QWidget* parent = nullptr) : QPushButton(parent) {
        setFixedSize(32, 32);
        setCursor(Qt::PointingHandCursor);
        setStyleSheet(
            "QPushButton {"
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
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPushButton::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QColor penColor(255, 255, 255);
        painter.setPen(QPen(penColor, 2.5));

        int margin = 10;
        QRect r = rect().adjusted(margin, margin, -margin, -margin);
        painter.drawLine(r.topLeft(), r.bottomRight());
        painter.drawLine(r.topRight(), r.bottomLeft());
    }
};

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
    : QWidget(parent), panelVisible(false), screenwidth(screenWidth), screenheight(screenHeight),
      animTimer(nullptr), isAnimating(false)
{
    int panelWidth = screenWidth * 0.7;

    // ========== 1. 创建自绘蒙层遮罩（无需 QGraphicsOpacityEffect）==========
    overlay = new OverlayWidget(parent);
    overlay->setGeometry(0, 0, screenWidth, screenHeight);
    overlay->hide();
    // 点击蒙层关闭菜单
    overlay->installEventFilter(this);

    // ========== 2. 面板自身样式 ==========
    setStyleSheet("background-color: rgb(255, 255, 255);");
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(255, 255, 255));
    setPalette(pal);
    setAutoFillBackground(true);
    move(-panelWidth, 0);
    resize(panelWidth, screenHeight);
    hide();

    // ========== 3. 创建高帧率动画定时器 ==========
    animTimer = new QTimer(this);
    animTimer->setTimerType(Qt::PreciseTimer);  // 使用高精度定时器
    animTimer->setInterval(0);                   // 尽可能快的帧率
    connect(animTimer, &QTimer::timeout, this, [this]() {
        // 计算已过时间占总时长的比例
        qreal elapsedMs = static_cast<qreal>(elapsed.elapsed());
        animProgress = qBound(0.0, elapsedMs / animDuration, 1.0);

        // 应用缓动函数
        qreal eased = easeOutCubic(animProgress);

        // 更新面板位置（线性插值）
        int currentX = startPanelX + static_cast<int>((endPanelX - startPanelX) * eased);
        move(currentX, 0);

        // 更新蒙层透明度（线性变化，蒙层本身不需要缓动）
        qreal currentOpacity = startOverlayOpacity + (endOverlayOpacity - startOverlayOpacity) * animProgress;
        static_cast<OverlayWidget*>(overlay)->setOverlayOpacity(currentOpacity);

        // 请求重绘
        update();

        // 动画结束
        if (animProgress >= 1.0) {
            animTimer->stop();
            isAnimating = false;

            if (!isShowing) {
                // 滑出动画结束 -> 隐藏面板和蒙层
                hide();
                overlay->hide();
            }
        }
    });

    // ========== 4. 创建主布局 ==========
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignTop);

    // ========== 5. 创建标题栏区域 ==========
    headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(80);
    headerWidget->setStyleSheet(
        "QWidget {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 rgb(66, 133, 244), stop:1 rgb(52, 168, 83));"
        "  border-bottom: 2px solid rgb(200, 200, 200);"
        "}"
    );

    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(16, 20, 12, 0);

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

    // 关闭按钮 (X) - 使用自定义绘制
    closeButton = new CloseIconButton(headerWidget);
    connect(closeButton, &QPushButton::clicked, this, &menuButton::hideMenu);
    headerLayout->addWidget(closeButton);

    layout->addWidget(headerWidget);

    // ========== 6. 创建菜单按钮（带矢量图标）==========
    for (int i = 0; i < buttonNumber; i++) {
        IconButton* button = new IconButton(str[i], i, this);
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
    layout->addStretch(1);
}

/**
 * @brief 启动高帧率动画
 */
void menuButton::startHighFpsAnimation(int fromX, int toX,
                                       qreal fromOverlay, qreal toOverlay,
                                       int duration, bool showing)
{
    // 停止任何正在进行的动画
    if (isAnimating) {
        animTimer->stop();
    }

    startPanelX = fromX;
    endPanelX = toX;
    startOverlayOpacity = fromOverlay;
    endOverlayOpacity = toOverlay;
    animDuration = duration;
    isShowing = showing;
    isAnimating = true;

    // 重置计时器并启动
    elapsed.start();
    animProgress = 0.0;
    animTimer->start();
}

/**
 * @brief 析构函数
 */
menuButton::~menuButton()
{
    if (animTimer) {
        animTimer->stop();
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

void menuButton::paintEvent(QPaintEvent* event)
{
    // 显式填充白色背景，确保整个面板区域（包括底部留白）不透明
    QPainter painter(this);
    painter.fillRect(rect(), QColor(255, 255, 255));
    QWidget::paintEvent(event);
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

    // 1. 先显示蒙层（初始透明度为0）
    overlay->setGeometry(0, 0, screenwidth, screenheight);
    overlay->show();
    overlay->raise();

    // 2. 面板移到初始位置并显示
    move(-panelWidth, 0);
    show();
    raise();

    // 3. 启动高帧率滑入动画
    startHighFpsAnimation(
        -panelWidth,        // 起始X（屏幕左侧外）
        0,                  // 结束X（屏幕左侧边缘）
        0.0,                // 蒙层起始透明度
        0.35,               // 蒙层结束透明度
        350,                // 动画时长 350ms
        true                // 滑入
    );

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

    // ===== 修复按钮状态卡住问题 =====
    // 当菜单项被点击时，会先通过信号触发 StyledDialog::exec()。
    // exec() 会进入一个嵌套的事件循环并阻塞返回，导致按钮的
    // mouse release 事件无法被正常处理。
    //
    // 当 exec() 返回后，按钮的 "pressed" 状态仍然被 Qt 的
    // 样式系统缓存，即使调用 setDown(false) 也无法完全清除。
    //
    // 修复方案：
    //   1. setDown(false)：重置 Qt 内部的下压状态
    //   2. style()->unpolish(btn): 卸载当前样式，清除样式缓存
    //   3. style()->polish(btn): 重新加载样式，从初始状态开始
    //   4. btn->update(): 强制立即重绘
    //
    // 这种 unpolish/polish 循环是 Qt 中"完全重置控件状态"
    // 的标准做法，常用于解决伪类状态卡住的 BUG。
    for (auto* btn : buttons) {
        btn->setDown(false);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
        btn->update();
    }

    int panelWidth = width();

    // 启动高帧率滑出动画
    startHighFpsAnimation(
        0,                  // 起始X（屏幕左侧边缘）
        -panelWidth,        // 结束X（屏幕左侧外）
        0.35,               // 蒙层起始透明度
        0.0,                // 蒙层结束透明度
        300,                // 动画时长 300ms
        false               // 滑出
    );

    panelVisible = false;
}
#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QList>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>

#define buttonNumber 5

/**
 * @brief 自绘蒙层控件，直接通过 paintEvent 绘制半透明黑色背景
 *        避免了 QGraphicsOpacityEffect 的离屏渲染开销
 */
class OverlayWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal overlayOpacity READ overlayOpacity WRITE setOverlayOpacity)
public:
    explicit OverlayWidget(QWidget* parent = nullptr)
        : QWidget(parent), m_opacity(0.0) {
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_NoSystemBackground);
    }

    qreal overlayOpacity() const { return m_opacity; }
    void setOverlayOpacity(qreal opacity) {
        m_opacity = opacity;
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0, 0, 0, static_cast<int>(m_opacity * 255)));
    }

private:
    qreal m_opacity;
};

/**
 * @class menuButton
 * @brief 侧边菜单面板类
 *
 * 使用高帧率自定义动画引擎，自动适配设备刷新率（60Hz/90Hz/120Hz+）
 */
class menuButton : public QWidget
{
    Q_OBJECT

signals:
    /**
     * @brief 菜单项被点击时发射的信号
     * @param index 被点击按钮的索引
     */
    void menuButtonClicked(int index);

private:
    bool panelVisible;                 // 菜单面板是否可见
    QVBoxLayout* layout;              // 菜单面板布局器
    QWidget* overlay;                  // 半透明蒙层（遮罩）
    QWidget* headerWidget;             // 标题栏区域
    QLabel* headerLabel;               // 标题文字
    QPushButton* closeButton;          // 关闭按钮(X)
    QList<QPushButton*> buttons;       // 菜单按钮列表
    int screenwidth;
    int screenheight;

    // ===== 高帧率动画引擎 =====
    QTimer* animTimer;                 // 动画定时器
    QElapsedTimer elapsed;             // 精确计时器
    int animDuration;                  // 动画总时长(ms)
    qreal animProgress;                // 动画进度 0.0~1.0
    bool isAnimating;                  // 是否正在播放动画
    int startPanelX;                   // 面板起始X
    int endPanelX;                     // 面板结束X
    qreal startOverlayOpacity;        // 蒙层起始透明度
    qreal endOverlayOpacity;          // 蒙层结束透明度
    bool isShowing;                    // true=滑入 false=滑出

    /**
     * @brief 缓动函数：OutCubic，效果平滑自然
     */
    inline qreal easeOutCubic(qreal t) const {
        return 1.0 - qPow(1.0 - t, 3.0);
    }

    /**
     * @brief 启动高帧率动画
     * @param fromX 面板起始X
     * @param toX 面板结束X
     * @param fromOverlay 蒙层起始透明度
     * @param toOverlay 蒙层结束透明度
     * @param duration 动画时长(ms)
     * @param showing true=滑入 false=滑出
     */
    void startHighFpsAnimation(int fromX, int toX,
                               qreal fromOverlay, qreal toOverlay,
                               int duration, bool showing);

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
/******************************************************************************
 * StyledDialog.cpp
 * 统一风格弹窗 - 实现文件
 *
 * 本文件实现了与 UpdateDialog 风格一致的通用弹窗。
 * 包含白色圆角背景、顶部渐变装饰条、可滚动内容区域、固定居中位置。
 ******************************************************************************/

#include "StyledDialog.h"
#include <QPainter>
#include <QFont>
#include <QScrollArea>
#include <QScreen>
#include <QApplication>
#include <QMouseEvent>
#include <QGuiApplication>

// =============================================================================
// 构造函数
// =============================================================================

StyledDialog::StyledDialog(const QString& title, const QString& message, QWidget *parent)
    : QDialog(parent)
    , m_title(title)
    , m_message(message)
{
    // 无边框 + 半透明背景（与 UpdateDialog 一致）
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 根据内容长度自适应大小
    int textWidth = qMax(title.length(), message.length()) * 12;
    int dialogWidth = qBound(300, textWidth + 80, 400);
    setFixedSize(dialogWidth, 280);

    setupUI();

    // 窗口居中显示在父窗口或屏幕中央（此位置固定，不会跟随手指移动）
    if (parent) {
        QWidget* p = qobject_cast<QWidget*>(parent);
        if (p) {
            QPoint center = p->geometry().center();
            move(center.x() - width() / 2, center.y() - height() / 2);
        }
    } else {
        QScreen* screen = QApplication::primaryScreen();
        if (screen) {
            QRect screenRect = screen->geometry();
            move((screenRect.width() - width()) / 2, (screenRect.height() - height()) / 2);
        }
    }
}

StyledDialog::~StyledDialog()
{
}

// =============================================================================
// 私有方法：创建界面控件
// =============================================================================

void StyledDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // ---- 标题标签 ----
    m_titleLabel = new QLabel(m_title, this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("color: #333;");
    mainLayout->addWidget(m_titleLabel);

    // ---- 内容标签（放入滚动区域，支持滑动浏览） ----
    m_messageLabel = new QLabel(m_message, this);
    m_messageLabel->setWordWrap(true);
    m_messageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_messageLabel->setStyleSheet(
        "color: #555; font-size: 13px;"
        "background: #f5f5f5; border-radius: 8px; padding: 12px;"
    );
    m_messageLabel->setMinimumHeight(40);
    m_messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 将内容标签放入滚动区域
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_messageLabel);
    scrollArea->setMinimumHeight(60);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "  background: #f5f5f5; border: none; border-radius: 8px;"
        "}"
        "QScrollBar:vertical {"
        "  width: 10px; background: transparent;"
        "  border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #ccc; border-radius: 5px; min-height: 20px;"
        "  margin: 2px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height: 0;"
        "}"
    );
    mainLayout->addWidget(scrollArea);

    // ---- 确定按钮（居中） ----
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    m_okButton = new QPushButton("确定", this);
    m_okButton->setFixedHeight(44);
    m_okButton->setMinimumWidth(120);
    m_okButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 #4A90D9, stop:1 #5BAE7C);"
        "  color: white; border: none; border-radius: 22px;"
        "  font-size: 15px; font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 #3A80C9, stop:1 #4B9E6C);"
        "}"
    );

    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

// =============================================================================
// 重写绘制事件：自定义窗口外观（与 UpdateDialog 一致）
// =============================================================================

void StyledDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 第一步：绘制圆角白色背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255));
    painter.drawRoundedRect(rect(), 16, 16);

    // 第二步：绘制顶部渐变装饰条
    QLinearGradient gradient(0, 0, width(), 0);
    gradient.setColorAt(0.0, QColor(74, 144, 217));   // 蓝色
    gradient.setColorAt(1.0, QColor(91, 174, 124));   // 绿色

    painter.setBrush(gradient);
    painter.drawRoundedRect(0, 0, width(), 6, 6, 6);
}

// =============================================================================
// 鼠标事件：禁用拖动功能，窗口位置固定
// =============================================================================

void StyledDialog::mousePressEvent(QMouseEvent *event)
{
    // 记录按下的位置，但阻止默认行为（不移动窗口）
    m_dragPosition = event->globalPos();
    m_isDragging = true;
    event->ignore();  // 忽略事件，让滚动区域等子控件正常接收
}

void StyledDialog::mouseMoveEvent(QMouseEvent *event)
{
    // 阻止窗口拖动：不执行 move() 操作
    // 保留 m_dragPosition 记录但仅用于检测，不改变窗口位置
    Q_UNUSED(event);
    if (m_isDragging) {
        // 不移动窗口，保持固定位置
        // 让子控件（滚动区域）可以正常处理滑动
        event->ignore();
    }
}

void StyledDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_isDragging = false;
    event->ignore();
}

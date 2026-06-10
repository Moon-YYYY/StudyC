/******************************************************************************
 * StyledProgressDialog.cpp
 * 统一风格的进度弹窗 - 实现文件
 *
 * 替换原 ApkDownloader 中的 QProgressDialog，
 * 使用与 UpdateDialog / StyledDialog 一致的视觉风格。
 ******************************************************************************/

#include "StyledProgressDialog.h"
#include <QPainter>
#include <QHBoxLayout>

// =============================================================================
// 构造函数
// =============================================================================

StyledProgressDialog::StyledProgressDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(300, 200);
    setWindowModality(Qt::WindowModal);

    setupUI();
}

// =============================================================================
// 析构函数
// =============================================================================

StyledProgressDialog::~StyledProgressDialog()
{
}

// =============================================================================
// 初始化界面
// =============================================================================

void StyledProgressDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // ---- 标题 ----
    m_titleLabel = new QLabel("正在下载", this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("color: #333;");
    mainLayout->addWidget(m_titleLabel);

    // ---- 进度描述文字 ----
    m_progressTextLabel = new QLabel("正在下载新版本...", this);
    m_progressTextLabel->setAlignment(Qt::AlignCenter);
    m_progressTextLabel->setStyleSheet("color: #666; font-size: 13px;");
    m_progressTextLabel->setWordWrap(true);
    mainLayout->addWidget(m_progressTextLabel);

    // ---- 进度条 ----
    m_progressBar = new QProgressBar(this);
    m_progressBar->setFixedHeight(8);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);
    m_progressBar->setStyleSheet(
        "QProgressBar {"
        "  background: #e0e0e0;"
        "  border: none;"
        "  border-radius: 4px;"
        "}"
        "QProgressBar::chunk {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 #4A90D9, stop:1 #5BAE7C);"
        "  border-radius: 4px;"
        "}"
    );
    mainLayout->addWidget(m_progressBar);

    // ---- 弹性空间 ----
    mainLayout->addStretch();

    // ---- 取消按钮 ----
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    m_cancelButton = new QPushButton("取消", this);
    m_cancelButton->setFixedHeight(40);
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "  background: #e0e0e0; color: #666; border: none;"
        "  border-radius: 20px; font-size: 14px; font-weight: bold;"
        "}"
        "QPushButton:hover { background: #d0d0d0; }"
        "QPushButton:pressed { background: #c0c0c0; }"
    );

    connect(m_cancelButton, &QPushButton::clicked, this, [this]() {
        emit canceled();
        reject();
    });

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

// =============================================================================
// 设置进度
// =============================================================================

void StyledProgressDialog::setProgress(int value)
{
    m_progressBar->setValue(value);
}

void StyledProgressDialog::setProgressText(const QString &text)
{
    m_progressTextLabel->setText(text);
}

// =============================================================================
// 阻止窗口拖动（固定位置）
// =============================================================================

void StyledProgressDialog::mousePressEvent(QMouseEvent *event)
{
    // 不做任何移动操作，只调用基类处理
    QDialog::mousePressEvent(event);
}

void StyledProgressDialog::mouseMoveEvent(QMouseEvent *event)
{
    // 不做任何移动操作
    QDialog::mouseMoveEvent(event);
}

void StyledProgressDialog::mouseReleaseEvent(QMouseEvent *event)
{
    QDialog::mouseReleaseEvent(event);
}

// =============================================================================
// 绘制事件：圆角白色背景 + 顶部渐变装饰条
// =============================================================================

void StyledProgressDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // ---- 圆角白色背景 ----
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255));
    painter.drawRoundedRect(rect(), 16, 16);

    // ---- 顶部渐变装饰条 ----
    QLinearGradient gradient(0, 0, width(), 0);
    gradient.setColorAt(0.0, QColor(74, 144, 217));
    gradient.setColorAt(1.0, QColor(91, 174, 124));
    painter.setBrush(gradient);
    painter.drawRoundedRect(0, 0, width(), 6, 6, 6);
}
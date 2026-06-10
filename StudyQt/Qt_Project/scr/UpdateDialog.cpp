/******************************************************************************
 * UpdateDialog.cpp
 * 更新提示弹窗 - 实现文件
 *
 * 本文件实现了更新提示对话框的界面布局和自定义绘制。
 * 界面特点：无边框圆角窗口、顶部渐变装饰条、半透明背景。
 ******************************************************************************/

#include "UpdateDialog.h"    // 更新对话框头文件：声明 UpdateDialog 类
#include "UpdateChecker.h"   // 更新检查器头文件：包含 UpdateInfo 结构体的完整定义
#include <QPainter>          // 绘制器，用于自定义绘制窗口背景
#include <QScrollArea>       // 滚动区域，用于显示超长更新日志
#include <QMessageBox>       // 消息框，用于显示下载错误

// =============================================================================
// 构造函数
// =============================================================================

UpdateDialog::UpdateDialog(const UpdateInfo& info, QWidget *parent)
    : QDialog(parent)            // 调用父类构造函数，设置父窗口
    , m_info(info)               // 保存版本信息到成员变量 m_info
{
    /**
     * 设置窗口标志（Window Flags）：
     *
     * Qt::Dialog        - 对话框类型，有模态/非模态特性
     * Qt::FramelessWindowHint - 无边框，去掉系统默认的标题栏和边框
     *
     * 为什么要无边框：
     *   - 系统默认对话框样式老旧，不美观
     *   - 我们要自己绘制圆角背景和装饰条
     *   - 实现类似现代 App 的弹窗效果
     */
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    /**
     * 设置窗口属性：
     *
     * Qt::WA_TranslucentBackground - 半透明背景
     *   允许窗口背景透明，这样圆角外的区域不会显示黑色方块。
     *   配合 paintEvent() 中的绘制，实现真正的圆角效果。
     */
    setAttribute(Qt::WA_TranslucentBackground);

    // 设置固定大小，不允许用户调整
    setFixedSize(320, 280);

    // 创建界面控件
    setupUI();
}

// =============================================================================
// 析构函数
// =============================================================================

UpdateDialog::~UpdateDialog()
{
    // 所有子控件和布局通过 Qt 的父子对象树自动释放
    // 无需手动 delete
}

// =============================================================================
// 私有方法：创建界面控件
// =============================================================================

void UpdateDialog::setupUI()
{
    // =========================================================================
    // 创建主布局：垂直布局，控件从上到下排列
    // =========================================================================

    /**
     * QVBoxLayout 是垂直布局管理器，自动管理子控件的位置和大小。
     * 参数 this 表示这个布局应用到当前窗口（UpdateDialog）。
     */
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    /**
     * 设置布局边距：
     * setContentsMargins(左, 上, 右, 下)
     * 控件不会紧贴窗口边缘，留出 20 像素的空白。
     */
    mainLayout->setContentsMargins(20, 20, 20, 20);

    /**
     * 设置控件间距：
     * 相邻控件之间的垂直距离为 12 像素。
     */
    mainLayout->setSpacing(12);

    // =========================================================================
    // 标题标签："发现新版本！"
    // =========================================================================

    m_titleLabel = new QLabel("发现新版本", this);

    /**
     * 设置字体：
     * 先获取当前字体，修改后重新设置。
     * 这样不会丢失字体族名等其他属性。
     */
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(18);    // 字号 18pt，较大较醒目
    titleFont.setBold(true);       // 粗体
    m_titleLabel->setFont(titleFont);

    // 文字居中对齐
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // 设置文字颜色为深灰色
    m_titleLabel->setStyleSheet("color: #333;");

    // 将标签添加到布局
    mainLayout->addWidget(m_titleLabel);

    // =========================================================================
    // 版本号标签："最新版本：v1.1.0"
    // =========================================================================

    /**
     * QString::arg() 是字符串格式化方法，类似 printf 的 %s。
     * "%1" 是占位符，会被 m_info.version 替换。
     *
     * 示例：m_info.version = "1.1.0"
     * 结果："最新版本：1.1.0"
     */
    m_versionLabel = new QLabel(QString("最新版本：%1").arg(m_info.version), this);
    m_versionLabel->setAlignment(Qt::AlignCenter);
    m_versionLabel->setStyleSheet("color: #666; font-size: 14px;");
    mainLayout->addWidget(m_versionLabel);

    // =========================================================================
    // 更新日志区域：放入 QScrollArea 支持上下滑动
    // =========================================================================

    /**
     * 如果服务器没有提供更新日志，显示默认文本。
     * QString::isEmpty() 判断字符串是否为空（长度为 0）。
     */
    QString changelog = m_info.changelog.isEmpty()
                        ? "暂无更新说明"
                        : m_info.changelog;

    // ---- 先创建 QLabel 用于显示日志文字 ----
    m_changelogLabel = new QLabel(changelog, this);

    /**
     * setWordWrap(true) 启用自动换行：
     * 当文字长度超过标签宽度时，自动换到下一行。
     * 避免文字被截断或撑宽窗口。
     */
    m_changelogLabel->setWordWrap(true);

    // 左对齐，顶部对齐
    m_changelogLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_changelogLabel->setMinimumHeight(60);

    /**
     * 设置样式表（QSS）：
     * 类似 CSS，用于设置控件外观。
     */
    m_changelogLabel->setStyleSheet(
        "color: #555; font-size: 13px;"
        "background: transparent; padding: 8px;"
    );

    // ---- 将 QLabel 放入 QScrollArea ----
    QScrollArea* changelogScroll = new QScrollArea(this);
    changelogScroll->setWidgetResizable(true);
    changelogScroll->setWidget(m_changelogLabel);
    changelogScroll->setMinimumHeight(80);
    changelogScroll->setStyleSheet(
        "QScrollArea {"
        "  background: #f5f5f5; border: none; border-radius: 8px;"
        "}"
        /* 手机端友好：滚动条加粗到 14px */
        "QScrollBar:vertical {"
        "  width: 14px; background: transparent;"
        "  border-radius: 7px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #ccc; border-radius: 7px; min-height: 30px;"
        "  margin: 2px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height: 0;"
        "}"
    );

    mainLayout->addWidget(changelogScroll);

    // =========================================================================
    // 添加弹性空间：将下面的按钮推到底部
    // =========================================================================

    /**
     * addStretch() 添加一个弹性空间：
     * 会占据所有剩余空间，将下面的按钮推到底部。
     * 这样即使窗口变高，按钮也保持在底部。
     */
    mainLayout->addStretch();

    // =========================================================================
    // 按钮区域：水平布局
    // =========================================================================

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);  // 两个按钮之间的间距

    // -------------------------------------------------------------------------
    // "以后再说"（取消）按钮
    // -------------------------------------------------------------------------

    m_cancelButton = new QPushButton("以后再说", this);
    m_cancelButton->setFixedHeight(44);  // 固定高度

    /**
     * QSS 样式说明：
     * background: #e0e0e0  - 浅灰背景
     * color: #666          - 深灰文字
     * border: none         - 去掉边框
     * border-radius: 22px  - 圆角（高度 44 的一半，形成胶囊形状）
     */
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "  background: #e0e0e0; color: #666; border: none;"
        "  border-radius: 22px; font-size: 15px; font-weight: bold;"
        "}"
        "QPushButton:pressed { background: #d0d0d0; }"
    );

    /**
     * 连接按钮点击信号到对话框的 reject() 槽：
     * reject() 表示用户取消了操作，对话框关闭，exec() 返回 QDialog::Rejected。
     */
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // -------------------------------------------------------------------------
    // "立即更新"（下载）按钮
    // -------------------------------------------------------------------------

    m_downloadButton = new QPushButton("立即更新", this);
    m_downloadButton->setFixedHeight(44);

    /**
     * 渐变背景：
     * qlineargradient 是 Qt 支持的 QSS 渐变语法。
     *
     * x1:0, y1:0, x2:1, y2:0 表示从左到右的水平渐变。
     * stop:0 #4A90D9  - 起点颜色（蓝色）
     * stop:1 #5BAE7C  - 终点颜色（绿色）
     */
    m_downloadButton->setStyleSheet(
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

    /**
     * 连接按钮点击信号到 Lambda 表达式：
     *
     * [this]() { ... } 是 C++11 Lambda 捕获列表：
     *   - this 表示捕获当前对象的指针，可以访问成员变量（如 m_info）
     *
     * 点击"立即更新"后执行的操作：
     *   1. 发射 downloadRequested 信号，传递下载 URL
     *   2. 调用 accept() 关闭对话框（返回 QDialog::Accepted）
     */
    connect(m_downloadButton, &QPushButton::clicked, [this]() {
        // 发射下载请求信号，通知外部开始下载
        emit downloadRequested(m_info.downloadUrl);
        // 关闭对话框
        accept();
    });

    // 将两个按钮添加到水平布局
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_downloadButton);

    // 将按钮布局添加到主布局
    mainLayout->addLayout(buttonLayout);
}

// =============================================================================
// 重写绘制事件：自定义窗口外观
// =============================================================================

void UpdateDialog::paintEvent(QPaintEvent* event)
{
    // Q_UNUSED 宏用于标记未使用的参数，避免编译器警告
    Q_UNUSED(event);

    /**
     * QPainter 是 Qt 的 2D 绘制类，提供绘制基本形状、文字、图片等功能。
     * 参数 this 表示在当前窗口上绘制。
     */
    QPainter painter(this);

    /**
     * 设置抗锯齿（Antialiasing）：
     * 让圆角边缘更平滑，避免锯齿状。
     * 会轻微影响绘制性能，但对于静态窗口来说可以忽略不计。
     */
    painter.setRenderHint(QPainter::Antialiasing);

    // -------------------------------------------------------------------------
    // 第一步：绘制圆角白色背景
    // -------------------------------------------------------------------------

    // QPainter::NoPen 表示不绘制边框（只填充内部）
    painter.setPen(Qt::NoPen);

    // 设置填充颜色为纯白色（背景色）
    painter.setBrush(QColor(255, 255, 255));

    /**
     * drawRoundedRect() 绘制圆角矩形：
     *   参数1（rect()）：绘制区域，rect() 返回 Widget 的整个内部区域
     *   参数2（16）：水平方向圆角半径（像素）
     *   参数3（16）：垂直方向圆角半径（像素）
     *
     * 圆角效果说明：
     *   如果参数2==参数3，则四个角是 1/4 圆弧。
     *   16 像素的圆角会产生较明显的圆润效果。
     */
    painter.drawRoundedRect(rect(), 16, 16);

    // -------------------------------------------------------------------------
    // 第二步：绘制顶部渐变装饰条
    // -------------------------------------------------------------------------

    /**
     * QLinearGradient 线性渐变类：
     *   构造函数参数：(x1, y1, x2, y2)
     *   这里从 (0, 0) 到 (width(), 0) 表示水平渐变。
     *
     * setColorAt(位置, 颜色)：
     *   位置 0.0 表示起点（左侧），1.0 表示终点（右侧）。
     *   中间值按比例插值过渡。
     */
    QLinearGradient gradient(0, 0, width(), 0);
    gradient.setColorAt(0.0, QColor(74, 144, 217));   // 起点的蓝色
    gradient.setColorAt(1.0, QColor(91, 174, 124));   // 终点的绿色

    // 设置 Painter 的填充为渐变
    painter.setBrush(gradient);

    /**
     * 绘制顶部装饰条：
     *   位置：x=0, y=0 到 width, height=6
     *   高度 6 像素，作为窗口顶部的彩色渐变条。
     *   圆角半径也设为 6，使条的两端也是圆角。
     *
     * 最终效果类似 Material Design 风格对话框顶部的彩色条。
     */
    painter.drawRoundedRect(0, 0, width(), 6, 6, 6);
}

// =============================================================================
// 鼠标事件处理器：实现窗口拖动
// =============================================================================

void UpdateDialog::mousePressEvent(QMouseEvent *event)
{
    /**
     * 当左键按下时，记录当前鼠标的全局位置和窗口的当前位置。
     * 使用 dragDelta 方式（记录偏移差）代替 frameGeometry() 计算，
     * 避免在 Android / 无边框窗口上 frameGeometry() 返回不正确导致的拖拽乱跳。
     */
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint();  // 仅存鼠标全局位置
        event->accept();
    }
}

void UpdateDialog::mouseMoveEvent(QMouseEvent *event)
{
    /**
     * 鼠标移动时，通过计算两次坐标的差值（delta）来移动窗口。
     *
     * 计算方式：
     *   1. 计算 delta = 当前鼠标全局位置 - 按下时记录的鼠标全局位置
     *   2. 新位置 = 窗口当前位置 + delta
     *   3. 更新记录的鼠标位置为当前鼠标位置（以便下次计算正确的 delta）
     *
     * 这种方式不依赖 frameGeometry()，对 Android 触摸拖拽更稳定。
     */
    if (event->buttons() & Qt::LeftButton) {
        QPoint currentGlobal = event->globalPosition().toPoint();
        QPoint delta = currentGlobal - m_dragPosition;
        move(pos() + delta);
        m_dragPosition = currentGlobal;  // 更新记录位置
        event->accept();
    }
}

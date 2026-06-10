/******************************************************************************
 * StyledProgressDialog.h
 * 统一风格的进度弹窗 - 头文件
 *
 * 功能说明：
 *   替换原 ApkDownloader 中的 QProgressDialog，
 *   使用与 UpdateDialog / StyledDialog 一致的视觉风格。
 *
 * 界面特点：
 *   - 无边框圆角窗口（FramelessWindowHint）
 *   - 圆角白色背景 + 顶部渐变装饰条
 *   - QProgressBar 进度条 + 文字标签
 *   - 取消按钮
 *   - 窗口不可拖动（固定位置）
 ******************************************************************************/

#ifndef STYLEDPROGRESSDIALOG_H
#define STYLEDPROGRESSDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QCloseEvent>

class StyledProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StyledProgressDialog(QWidget *parent = nullptr);
    ~StyledProgressDialog();

    /**
     * @brief 设置进度值 (0-100)
     */
    void setProgress(int value);

    /**
     * @brief 设置进度描述文字
     * @param text 如 "已下载 3.2 MB / 4.8 MB (67%)"
     */
    void setProgressText(const QString &text);

signals:
    /**
     * @brief 用户点击取消按钮时发射此信号
     */
    void canceled();

protected:
    void paintEvent(QPaintEvent *event) override;

    /**
     * 阻止鼠标拖动——重写鼠标事件但不做任何移动操作
     */
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupUI();

    QLabel *m_titleLabel;         // 标题："正在下载"
    QLabel *m_progressTextLabel;  // 进度描述文字
    QProgressBar *m_progressBar;  // 进度条
    QPushButton *m_cancelButton;  // 取消按钮
};

#endif // STYLEDPROGRESSDIALOG_H
/******************************************************************************
 * StyledDialog.h
 * 统一风格弹窗 - 头文件
 *
 * 本文件定义了一个通用弹窗类，具有与 UpdateDialog 一致的外观风格：
 *   - 无边框圆角窗口
 *   - 白色半透明背景 + 顶部渐变装饰条
 *   - 窗口固定居中（不可拖动）
 *   - 支持滚动浏览内容
 *   - 统一的标题、内容和按钮样式
 ******************************************************************************/

#ifndef STYLEDDIALOG_H
#define STYLEDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class StyledDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param title   弹窗标题（如"关于"、"版本"）
     * @param message 弹窗内容文本
     * @param parent  父窗口指针
     */
    explicit StyledDialog(const QString& title, const QString& message, QWidget *parent = nullptr);
    ~StyledDialog();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 记录拖拽起始点（此处保留但禁用，仅用于阻止默认行为）
    QPoint m_dragPosition;
    bool m_isDragging = false;
    void setupUI();

    QString m_title;
    QString m_message;
    QLabel *m_titleLabel;
    QLabel *m_messageLabel;
    QPushButton *m_okButton;
};

#endif // STYLEDDIALOG_H
/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.07.27
 * Description: 无标题栏的窗口，支持拖曳，用作贴图使用
 ******************************************************************/
#ifndef XFRAMELESSWIDGET_H
#define XFRAMELESSWIDGET_H

#include <QWidget>

class QPoint;
class XFrameLessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XFrameLessWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;

private:
    QPoint m_p;   // 左上角的
    QPoint m_p1;
    QPoint m_p2;
};

#endif // XFRAMELESSWIDGET_H

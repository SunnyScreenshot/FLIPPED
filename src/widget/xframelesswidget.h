/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.07.27
 * Description: 无标题栏的窗口，支持拖曳，用作 Pin 贴图使用
 ******************************************************************/
#ifndef XFRAMELESSWIDGET_H
#define XFRAMELESSWIDGET_H

#include <QWidget>

class QLabel;
class QPoint;
class QPixmap;
class QMouseEvent;
class QWheelEvent;
class QGraphicsDropShadowEffect;

class XFrameLessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XFrameLessWidget(const QPixmap& pixmap, const QRect& geometry, QWidget* parent = nullptr);

private:
    void setScaledPixmapToLabel(const QSize& newSize, const qreal scale, const bool expanding);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QPoint m_p;   // 窗口的左上角
    QPoint m_p1;
    QPoint m_p2;

    QPixmap m_pixmap;
    QLabel* m_label;
    QGraphicsDropShadowEffect* m_shadowEffect;
};

#endif // XFRAMELESSWIDGET_H

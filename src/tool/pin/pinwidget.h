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
#include <QPointer>

class QMenu;
class QLabel;
class QPoint;
class QPixmap;
class QMouseEvent;
class QWheelEvent;
class QGraphicsDropShadowEffect;

class PinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PinWidget(const QPixmap& pixmap, const QRect& geometry, QWidget* parent = nullptr);

private:
    void initUI();
    void setScaledPixmapToLabel(const QSize& newSize, const qreal scale, const bool expanding);
    void createShadowEffect();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    void onChangeWinShadow(bool enable);
    void onChangeOpacity(int opacity);
    void onChangeMaxSize(double val);

private:
    QPoint m_p;   // 窗口的左上角
    QPoint m_p1;
    QPoint m_p2;

    QPixmap m_pixmap;
    QLabel* m_label;
    QPointer<QGraphicsDropShadowEffect> m_shadowEffect;

    QMenu* m_menu;
};

#endif // XFRAMELESSWIDGET_H

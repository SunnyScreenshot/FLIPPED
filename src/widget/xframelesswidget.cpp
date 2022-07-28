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
#include "xframelesswidget.h"
#include <QGuiApplication>
#include <QColor>
#include <QPoint>
#include <QLabel>
#include <QPixmap>
#include <QShortcut>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsDropShadowEffect>

XFrameLessWidget::XFrameLessWidget(const QPixmap &pixmap, const QRect &geometry, QWidget *parent)
    : QWidget(parent)
    , m_pixmap(pixmap)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);           // Otherwise it is a black background
    setAttribute(Qt::WA_DeleteOnClose);

    auto vLayout = new QVBoxLayout(this);
    const int margin = 7;
    vLayout->setContentsMargins(margin, margin, margin, margin);

    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setColor(QColor(255, 0, 0));
    m_shadowEffect->setBlurRadius(2 * margin);
    m_shadowEffect->setOffset(0, 0);
    setGraphicsEffect(m_shadowEffect);

    m_label = new QLabel();
    m_label->setPixmap(m_pixmap);
    vLayout->addWidget(m_label);

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close())); // TODO 2022.07.29: 替换为 Qt5 形式
    new QShortcut(Qt::Key_Escape, this, SLOT(close()));

    qreal devicePixelRatio = 1;
#if defined(Q_OS_MACOS)
    QScreen* currentScreen = QGuiAppCurrentScreen().currentScreen();
    if (currentScreen)
        devicePixelRatio = currentScreen->devicePixelRatio();
#endif
    const int m = margin * devicePixelRatio;
    QRect adjusted_pos = geometry + QMargins(m, m, m, m);
    setGeometry(adjusted_pos);

    adjustSize();
}

void XFrameLessWidget::setScaledPixmapToLabel(const QSize &newSize, const qreal scale, const bool expanding)
{
    QPixmap scaledPixmap;
    const auto aspectRatio = expanding ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio;

    scaledPixmap = m_pixmap.scaled(newSize * scale, aspectRatio, Qt::SmoothTransformation); // Qt::FastTransformation
    scaledPixmap.setDevicePixelRatio(scale);
    m_label->setPixmap(scaledPixmap);
}

void XFrameLessWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton) {
        m_p1 = e->globalPos();
        m_p2 = m_p1;
        m_p = pos();
    }
}

void XFrameLessWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton) {
        m_p2 = e->globalPos();
        move(m_p + m_p2 - m_p1);
    }
}

void XFrameLessWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton) {
        m_p2 = e->globalPos();
        m_p1 = m_p2;
    }
}

void XFrameLessWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton)
        close();
}

void XFrameLessWidget::wheelEvent(QWheelEvent *event)
{
    const QPoint degrees = event->angleDelta() / 8;
    const int direction = degrees.y() > 0 ? 1 : -1;       // zooming in or out

    const int step = degrees.manhattanLength() * direction;
    const int newWidth = qBound(50, m_label->width() + step, maximumWidth());
    const int newHeight = qBound(50, m_label->height() + step, maximumHeight());

    const QSize newSize(newWidth, newHeight);
    const qreal scale = qApp->devicePixelRatio();
    const bool isExpanding = direction > 0;
    setScaledPixmapToLabel(newSize, scale, isExpanding);

    adjustSize();                                         // Reflect scaling to the label
    event->accept();
}





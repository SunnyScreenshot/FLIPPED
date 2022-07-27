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
#include "xframelesswidget.h"
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPoint>

XFrameLessWidget::XFrameLessWidget(QWidget *parent)
    : QWidget(parent)
{
//    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

//    QGraphicsDropShadowEffect *shadow  = new QGraphicsDropShadowEffect(this); // Bug
//    shadow->setColor(QColor(43, 43, 43));
//    shadow->setOffset(20, 20);
//    shadow->setBlurRadius(8);

//    setGraphicsEffect(shadow);
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



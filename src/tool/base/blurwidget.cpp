/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.16
 * Description: 模拟 亚克力/透明磨砂 效果（此处取巧）
 ******************************************************************/
#include "blurwidget.h"
#include "../../xglobal.h"
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QGraphicsBlurEffect>

BlurWidget::BlurWidget(QWidget *parent)
    : QWidget(parent)
    , m_pix()
{
    m_pBlurEffect = new QGraphicsBlurEffect();
    m_pBlurEffect->setBlurRadius(5);
    m_pBlurEffect->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
    setGraphicsEffect(m_pBlurEffect);  // 透明磨砂效果
    setWindowFlags(Qt::FramelessWindowHint);
}

void BlurWidget::setPixmap(const QPixmap &pix, double blurRadius)
{
    m_pix = pix;

    if (m_pBlurEffect)
        m_pBlurEffect->setBlurRadius(blurRadius);
}

void BlurWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

     QPainter pa(this);
     pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
     if (!m_pix.isNull())
         pa.drawPixmap(rect(), m_pix);

     QColor colBrush("#131313");
     colBrush.setAlphaF(0.6);
     pa.setBrush(colBrush);
     QColor colPen("#FFFFFF");
     colPen.setAlphaF(0.1);
     QPen pen(colPen, 1);
     pa.setPen(pen);

     const int margin1 = 1;
     pa.drawRoundedRect(contentsRect().adjusted(margin1, margin1, -margin1, -margin1), B_RADIRS, B_RADIRS);

     colPen.setNamedColor("#000000");
     colPen.setAlphaF(0.1);
     pa.setPen(colPen);
     pa.setBrush(Qt::NoBrush);
     const int margin2 = 0;
     pa.drawRoundedRect(contentsRect().adjusted(margin2, margin2, -margin2, -margin2), B_RADIRS, B_RADIRS);
}

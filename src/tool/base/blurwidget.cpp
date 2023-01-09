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
#include <QPainterPath>
#include <QPen>
#include <QPixmap>

BlurWidget::BlurWidget(QWidget *parent)
    : QWidget(parent)
    , m_pix()
    , m_pBlurEffect(std::make_unique<QGraphicsBlurEffect>())
{
    m_pBlurEffect->setBlurRadius(5);
    m_pBlurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    setGraphicsEffect(m_pBlurEffect.get());
    setWindowFlags(Qt::FramelessWindowHint);
}

void BlurWidget::setPixmap(const QPixmap &pix, double blurRadius)
{
    if (!pix.isNull())
        m_pix = std::move(pix);

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

    QColor colBrush(19, 19, 19, 0.6 * 255);
    QColor colPen(255, 255, 255, 0.1 * 255);
    pa.setBrush(colBrush);
    QPen pen(colPen, 1);
    pa.setPen(pen);
    const int margin = 1;
    pa.drawRoundedRect(contentsRect().adjusted(margin, margin, -margin, -margin), B_RADIRS, B_RADIRS);

    colPen = QColor(0, 0, 0, 0.1 * 255);
    pa.setPen(colPen);
    pa.setBrush(Qt::NoBrush);
    pa.drawRoundedRect(contentsRect(), B_RADIRS, B_RADIRS);

    QPainterPath path;
    path.addRect(contentsRect());
    path.addRoundedRect(contentsRect(), B_RADIRS, B_RADIRS);
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(0, 0, 0, 0.5 * 255)); // 同遮罩
    pa.drawPath(path);
}

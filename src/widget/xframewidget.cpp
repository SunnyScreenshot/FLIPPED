/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.22
 * Description: 基础底部 widget
 ******************************************************************/
#include "xframewidget.h"
#include "../xglobal.h"
#include <QColor>
#include <QPainter>

XFrameWidget::XFrameWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);      // 去掉标题栏
    setAttribute(Qt::WA_TranslucentBackground);   // 设置透明，自绘画为圆角矩形
}

void XFrameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QColor colBrush("#131313");
    colBrush.setAlphaF(0.6);
    pa.setBrush(colBrush);
    QColor colPen("#FFFFFF");
    colPen.setAlphaF(0.1);
    pa.setPen(colPen);
    pa.drawRoundedRect(contentsRect().adjusted(1, 1, -1, -1), CW_RADIRS, CW_RADIRS);

    colPen.setNamedColor("#000000");
    colPen.setAlphaF(0.1);
    pa.setPen(colPen);
    pa.setBrush(Qt::NoBrush);

    pa.drawRoundedRect(contentsRect(), CW_RADIRS, CW_RADIRS);
}

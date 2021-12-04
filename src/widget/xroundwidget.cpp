//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/06.
//
#include "xroundwidget.h"
#include <QColor>
#include <QHBoxLayout>
#include <QPainter>

#define ROUNDED 8  // 圆角矩形的圆角大小

XRoundWidget::XRoundWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

XRoundWidget::~XRoundWidget()
{
}

void XRoundWidget::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint);      // 去掉标题栏
    setAttribute(Qt::WA_TranslucentBackground);   // 设置透明，自绘画为圆角矩形
    setContentsMargins(0, 0, 0, 0);
}

void XRoundWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    pa.setPen(Qt::red);
    pa.setBrush(QColor(255, 255, 255, 0.8 * 255));
    pa.drawRoundedRect(contentsRect(), ROUNDED, ROUNDED);
}

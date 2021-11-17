//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/09.
//
#include "xdraw.h"

#include <QPainter>

/*!
 * \brief The XDrawStep struct 绘画步骤
 */

/*!
 * \brief The XDraw class 绘画编辑截图类
 */

XDraw::XDraw(QObject *parent)
    : QObject(parent)
{
    // 变量还没有初始化
}

XDraw::~XDraw()
{

}

void XDraw::deawRect(QPainter &pa, QRect &rt, QPen pen, int width, QBrush brush)
{
    pen.setWidth(width);
    pa.setPen(pen);
    pa.setBrush(brush);
    pa.drawRect(rt);
}

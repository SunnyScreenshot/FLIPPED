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

void XDraw::drawRect(QPainter &pa, QRect rt, QPen pen, int width, QBrush brush)
{
    pen.setWidth(width);
    pen.setColor(Qt::red);
    pa.setPen(pen);
    pa.setBrush(brush);
//    pa.drawRect(rt);
}

void XDrawStep::clear()
{
    startPos = QPoint();
    endPos = QPoint();
    rt = QRect();
//    shape = XDrawShape::NoDraw; // 若为鼠标松开执行，则会无法继续绘画抽象图形

    rX = 8;
    rY = 8;
//    text = "==Test Text==";

//    pen = QPen(Qt::NoPen);
//    penWidth = 1;
//    brush = QBrush(Qt::NoBrush);
//    font = QFont();
//    fontSize = 16;
}


//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/09.
//
#include "drawhelper.h"

#include <QPainter>

/*!
 * \brief The XDrawStep struct 绘画步骤
 */

/*!
 * \brief The XDraw class 绘画编辑截图类
 */

DrawHelper::DrawHelper(QObject *parent)
    : QObject(parent)
{
    // 变量还没有初始化
}

DrawHelper::~DrawHelper()
{
}

void DrawHelper::drawRect(QPainter &pa, QRect rt, QPen pen, int width, QBrush brush)
{
    pen.setWidth(width);
    pen.setColor(Qt::red);
    pa.setPen(pen);
    pa.setBrush(brush);
}

void XDrawStep::clear()
{
	startPos = QPoint();
	endPos = QPoint();
	rt = QRect();
	//shape = DrawShape::NoDraw; // 若为鼠标松开执行，则会无法继续绘画抽象图形
	//pen = QPen(Qt::red);
	//penWidth = 2;
	//brush = QBrush(Qt::NoBrush);
	//brushWidth = 1;
	//transparency = 1;
	//rX = 8;
	//rY = 8;
	//lineEnd = LineEnds::EmptyToEmpty;
	//lineDashe = LineDashes::SolidLine;
	custPath.clear();
	//text = "==Test Text==";
	//font = QFont();
	//fontSize = 16;
	//mscPx = 3;
}


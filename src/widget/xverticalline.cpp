//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/04.
//
#include "xverticalline.h"

//frameRect : QRect//控件矩形
//frameWidth : const int//框架的宽度
//frameShadow : Shadow//有无阴影
//lineWidth : int//线宽度
//frameShape : Shape//框架形状
//midLineWidth : int//中间线的宽度

XVerticalLine::XVerticalLine(int height, QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
{
    setFrameShape(VLine);
    setFrameShadow(Sunken);
    setLineWidth(0);
    setMidLineWidth(2);
	setFixedHeight(height);
}

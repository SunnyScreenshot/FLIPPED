/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.05.14
 * Description: 定义几个基础通用的变量
 ******************************************************************/
#ifndef XTYPE_H
#define XTYPE_H

template<class T>
struct _X_POINT
{
    _X_POINT() : x(0), y(0) {}
    _X_POINT(T _x, T _y) : x(_x), y(_y) {}

    T  x;
    T  y;
};

typedef _X_POINT<int> X_POINT;
//typedef _X_POINT<float> FX_POINT;

template<class T>
struct _X_RECT
{
    _X_RECT() : left(), top(), right(), bottom() {}
    _X_RECT(T left, T top, T right, T bottom) :
        left(left), top(top), right(right), bottom(bottom) {}

    template<class Point>
    _X_RECT(Point p, T width, T height) :
      left(p.x), top(p.y), right(p.x + width), bottom(p.y + height) {}

    T left;
    T top;
    T right;
    T bottom;
};

typedef _X_RECT<int> X_RECT;
//typedef _X_RECT<float> FX_RECT;

#endif // XTYPE_H

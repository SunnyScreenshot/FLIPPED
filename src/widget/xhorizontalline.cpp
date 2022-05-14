/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2021.12.04
 * Description: 水平分割线
 ******************************************************************/

#include "xhorizontalline.h"

XHorizontalLine::XHorizontalLine(int width, QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
{
    setFrameShape(HLine);
    setFrameShadow(Sunken);
    setLineWidth(0);
    setMidLineWidth(2);
    setFixedWidth(width);
}

// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

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

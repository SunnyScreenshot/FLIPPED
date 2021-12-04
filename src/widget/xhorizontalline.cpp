//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/04.
//

#include "xhorizontalline.h"

XHorizontalLine::XHorizontalLine(QWidget *parent, Qt::WindowFlags f)
    : QFrame(parent, f)
{
    setFrameShape(HLine);
    setFrameShadow(Sunken);
    setLineWidth(0);
    setMidLineWidth(1);
}

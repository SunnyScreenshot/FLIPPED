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

#ifndef XHORIZONTALLINE_H
#define XHORIZONTALLINE_H

#include <QFrame>

// 水平分割线
class XHorizontalLine : public QFrame
{
    Q_OBJECT
public:
    XHorizontalLine(int width, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // XHORIZONTALLINE_H

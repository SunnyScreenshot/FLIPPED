/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2021.12.04
 * Description: 竖直分割线
 ******************************************************************/
#ifndef XVERTICALLINE_H
#define XVERTICALLINE_H

#include <QFrame>

class XVerticalLine : public QFrame
{
    Q_OBJECT
public:
    XVerticalLine(int height, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // XVERTICALLINE_H

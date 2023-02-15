// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

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

// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

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

//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/04.
//

#ifndef XVERTICALLINE_H
#define XVERTICALLINE_H

#include <QFrame>

// 竖直分割线
class XVerticalLine : public QFrame
{
    Q_OBJECT
public:
    XVerticalLine(int height, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // XVERTICALLINE_H

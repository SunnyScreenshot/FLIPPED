//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/04.
//

#ifndef XHORIZONTALLINE_H
#define XHORIZONTALLINE_H

#include <QFrame>

// 水平分割线
class XHorizontalLine : public QFrame
{
    Q_OBJECT
public:
    XHorizontalLine(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // XHORIZONTALLINE_H

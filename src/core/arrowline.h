// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

#ifndef ARROWLINE_H
#define ARROWLINE_H

#include <QtMath>
#include <QColor>
#include <QPainter>
#include <QPainterPath>

class ArrowLine
{
public:
    ArrowLine(double lineWidth, const QPointF& p1, const QPointF& p2);
    ~ArrowLine() = default;

    const QPainterPath arrowLineOpen() const;
    const QPainterPath arrowLine() const;
    const QPainterPath circleLine() const;
    const QPainterPath line() const;

private:
    void calcVertexes(QPointF& p1, QPointF& p2) const;

private:
    double m_lineWidth;
    const QPointF m_p1;
    const QPointF m_p2;
};

#endif // ARROWLINE_H

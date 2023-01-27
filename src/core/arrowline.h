#ifndef ARROWLINE_H
#define ARROWLINE_H

#include <QtMath>
#include <QColor>
#include <QPainter>
#include <QPainterPath>

enum class ArrowStyle
{
    AS_OPEN,
    AS_SOLID,
    AS_SOLID_FILLED,
    AS_DIAMOND,
    AS_DIAMOND_FILLED,
    AS_CIRCLE,
    AS_CIRCLE_FILLED
};

class ArrowLine
{
public:
    ArrowLine(double lineWidth, const QPointF& p1, const QPointF& p2);
    ~ArrowLine() = default;



    // Arrow Line draw
    const QLine shorterLine(const int thickness = 10) const;
    const QPainterPath arrowHead(const int thickness = 10) const;

    const QPainterPath arrowLine() const;

private:
    void calcVertexes(QPointF& p1, QPointF& p2) const;

private:
    double m_lineWidth;
    const QPointF m_p1;
    const QPointF m_p2;

    // shorterLine + QPainterPath using
    const int ArrowWidth = 10;
    const int ArrowHeight = 18;
};

#endif // ARROWLINE_H

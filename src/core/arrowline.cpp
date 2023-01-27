#include "arrowline.h"

ArrowLine::ArrowLine(double lineWidth, const QPointF &p1, const QPointF &p2)
    : m_lineWidth(lineWidth)
    , m_p1(p1)
    , m_p2(p2)
{

}

const QPainterPath ArrowLine::arrowLine() const
{
    QPointF p1;
    QPointF p2;
    calcVertexes(p1, p2);

    QPolygonF polygon;
    polygon << m_p2 << p1 << p2;

    QPainterPath path;
    path.addPolygon(polygon);
    path.closeSubpath();

    const QLineF l1(p1, p2);
    const QLineF l2(l1.center(), m_p2);
//    const double vertLine( + pow(l2.center().x() - m_p2.x(), 2)));
//    const QPointF tEnd(m_p2 - qAbs<double>(l2.center().y() - m_p2.y()) / 3);  //三角形垂线的 1/3 处
    path.moveTo(m_p1);
    path.lineTo(l2.center());
    return path;
}


const QLine ArrowLine::shorterLine(const int thickness) const
{
    const QPointF& p1 = m_p1;
    const QPointF& p2 = m_p2;
    QLineF l(p1, p2);
    int val = ArrowHeight + thickness * 4;
    if (l.length() < val)
        val = static_cast<int>(l.length() + thickness * 2);

    l.setLength(l.length() + thickness * 2 - val);
    return l.toLine();
}

const QPainterPath ArrowLine::arrowHead(const int thickness) const
{
    const QPointF& p1 = m_p1;
    const QPointF& p2 = m_p2;

    QLineF base(p1, p2);
    // Create the vector for the position of the base  of the arrowhead
    QLineF temp(QPoint(0, 0), p2 - p1);
    int val = ArrowHeight + thickness * 4;
    if (base.length() < val) {
        val = static_cast<int>(base.length() + thickness * 2);
    }
    temp.setLength(base.length() + thickness * 2 - val);
    // Move across the line up to the head
    QPointF bottomTranslation(temp.p2());

    // Rotate base of the arrowhead
    base.setLength(ArrowWidth + thickness * 2);
    base.setAngle(base.angle() + 90);
    // Move to the correct point
    QPointF temp2 = p1 - base.p2();
    // Center it
    QPointF centerTranslation((temp2.x() / 2), (temp2.y() / 2));

    base.translate(bottomTranslation);
    base.translate(centerTranslation);

    QPainterPath path;
    path.moveTo(p2);
    path.lineTo(base.p1());
    path.lineTo(base.p2());
    path.lineTo(p2);
    return path;
}

void ArrowLine::calcVertexes(QPointF &p1, QPointF &p2) const
{
    double lenght = 14;       // 箭头边线长度
    double degrees = 0.5;     // 箭头夹角
//    const double baseLine = sqrt(pow(m_p2.y() - m_p1.y(), 2) + pow(m_p2.x() - m_p1.x(), 2));
    lenght = qBound<double>(40, lenght + m_lineWidth * 4, 200);

    double angle = atan2(m_p2.y() - m_p1.y(), m_p2.x() - m_p1.x()) + M_PI;

    p1.setX(m_p2.x() + lenght * cos(angle - degrees));
    p1.setY(m_p2.y() + lenght * sin(angle - degrees));
    p2.setX(m_p2.x() + lenght * cos(angle + degrees));
    p2.setY(m_p2.y() + lenght * sin(angle + degrees));
}

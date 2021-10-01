//
// Created by xmuli on 2021/9/29.
//

#include "rectcalcu.h"
#include <QRect>
#include <QPoint>

// 通过任意两点获取一个选中矩形
QRect RectCalcu::getRect(QPoint pos1, QPoint pos2)
{
	int xMin = qMin<int>(pos1.x(), pos2.x());
	int xMax = qMax<int>(pos1.x(), pos2.x());
	int yMin = qMin<int>(pos1.y(), pos2.y());
	int yMax = qMax<int>(pos1.y(), pos2.y());

	if (xMin == xMax || yMin == yMax)
		return QRect();
	else
		return  QRect(QPoint(xMin, yMin), QPoint(xMax, yMax));
}

//QRect RectCalcu::getSelRect()
//{
//	return m_selRect;
//}

QRect RectCalcu::getSelRect()
{
	return getRect(m_startPos, m_EndPos);
}

QRect RectCalcu::getOuterSelRect(int interval)
{
	QPoint topLeft = m_selRect.topLeft();
	QPoint bottomRight = m_selRect.bottomRight() + QPoint(1, 1);
	return QRect(QPoint(topLeft.x() - interval, topLeft.y() - interval), QPoint(bottomRight.x() + interval, bottomRight.y() + interval));
}

QRect RectCalcu::getInnerSelRect(int interval)
{
	// TODO 2021-10-01: 考虑 rect 的width 和 height 本身 <= 2 * HAIF_INTERVAL？
	QPoint topLeft = m_selRect.topLeft();
	QPoint bottomRight = m_selRect.bottomRight() + QPoint(1, 1);
	return QRect(QPoint(topLeft.x() + interval, topLeft.y() + interval), QPoint(bottomRight.x() - interval, bottomRight.y() - interval));
}
void RectCalcu::clear()
{
	m_width = 0;
	m_height = 0;
	m_startPos = QPoint();
	m_movePos = QPoint();
	m_EndPos = QPoint();
	m_selRect = QRect();
	m_bClear = true;
}
void RectCalcu::setClear(bool clear)
{
	m_bClear = clear;
}
bool RectCalcu::isClear()
{
	return m_bClear;
}
//
//
//QPoint RectCalcu::getSelRectTopLeft()
//{
//	return m_selRect.topLeft();
//}
//
//void RectCalcu::setSelRectTopLeft(QPoint topLeft)
//{
//	m_selRect.setTopLeft(topLeft);
//}
//
//QPoint RectCalcu::getBottomRight()
//{
//	return m_selRect.bottomRight();
//}
//
//void RectCalcu::setBottomRight(QPoint bottomRight)
//{
//	m_selRect.setBottomRight(bottomRight);
//}

//
// Created by xmuli on 2021/9/29.
//

#include "rectcalcu.h"
#include <QRect>
#include <QPoint>

// 通过任意两点获取一个选中矩形
QRect& RectCalcu::getRect(QPoint pos1, QPoint pos2)
{
	int xMin = qMin<int>(pos1.x(), pos2.x());
	int xMax = qMax<int>(pos1.x(), pos2.x());
	int yMin = qMin<int>(pos1.y(), pos2.y());
	int yMax = qMax<int>(pos1.y(), pos2.y());

	if (xMin == xMax || yMin == yMax)
		m_rtSel = QRect();
	else
		m_rtSel = QRect(QPoint(xMin, yMin), QPoint(xMax, yMax));

	return  m_rtSel;
}


// 判断当前鼠标所在区域; false 为大概区域（显示光标）; true 为详细区域（修改矩形大小做准备）
const CursorArea RectCalcu::getCursorArea(QPoint pos, bool details)
{
	if (m_rtSel.isEmpty())
		return CursorArea::CursorOutSize;

	QRect rtOuter = getOuterSelRect(m_rtSel);
	QRect rtInner = getInnerSelRect(m_rtSel);
	int interval = (rtOuter.height() - rtInner.height()) / 2;

	QRect rtLeft(rtOuter.left(), rtInner.top(), interval, rtInner.height());
	QRect rtTop(rtInner.left(), rtOuter.top(), rtInner.width(), interval);
	QRect rtRight(rtInner.right(), rtInner.top(), interval, rtInner.height());
	QRect rtBottom(rtInner.left(), rtInner.bottom(), rtInner.width(), interval);
	QRect rtTopLeft(rtOuter.left(), rtOuter.top(), interval, interval);
	QRect rtTopRight(rtInner.right(), rtOuter.top(), interval, interval);
	QRect rtBottomLeft(rtOuter.left(), rtInner.bottom(), interval, interval);
	QRect rtBottomRight(rtInner.right(), rtInner.bottom(), interval, interval);

	if (!details) {
		if (rtLeft.contains(pos, true) | rtRight.contains(pos, true))
			return CursorArea::CursorCrossHorizontal;
		else if (rtTop.contains(pos, true) | rtBottom.contains(pos, true))
			return CursorArea::CursorCrossVertical;
		else if (rtTopLeft.contains(pos, true) | rtBottomRight.contains(pos, true))
			return CursorArea::CursorCrossTL2BR;
		else if (rtTopRight.contains(pos, true) | rtBottomLeft.contains(pos, true))
			return CursorArea::CursorCrossTR2BL;
		else if (rtInner.contains(pos, true))
			return CursorArea::CursorInner;
		else if (!rtOuter.contains(pos, false))
			return CursorArea::CursorOutSize;
		else
			return CursorArea::UnknowCursorArea;
	} else {
		if (rtLeft.contains(pos, true))
			return CursorArea::CursorCrossLeft;
		else if (rtRight.contains(pos, true))
			return CursorArea::CursorCrossRight;
		else if (rtTop.contains(pos, true))
			return CursorArea::CursorCrossTop;
		else if (rtBottom.contains(pos, true))
			return CursorArea::CursorCrossBottom;
		else if (rtTopLeft.contains(pos, true))
			return CursorArea::CursorCrossTopLeft;
		else if (rtBottomRight.contains(pos, true))
			return CursorArea::CursorCrossBottomRight;
		else if (rtTopRight.contains(pos, true))
			return CursorArea::CursorCrossTopRight;
		else if (rtBottomLeft.contains(pos, true))
			return CursorArea::CursorCrossBottomLeft;
		else if (rtInner.contains(pos, true))
			return CursorArea::CursorInner;
		else if (!rtOuter.contains(pos, false))
			return CursorArea::CursorOutSize;
		else
			return CursorArea::UnknowCursorArea;
	}
}

RectCalcu::RectCalcu()
{
	clear();
}

RectCalcu::~RectCalcu()
{
}

QRect& RectCalcu::getSelRect()
{
	if (m_cursorType == CursorType::Select)
		m_rtSel = getRect(m_startPos, m_EndPos);

	return m_rtSel;
//
//	switch (m_cursorType) {
//	case CursorType::Select:
//		return getRect(m_startPos, m_EndPos);
////	case CursorType::MovePosition:
////		break;
////	case CursorType::ModifWidth:
////		break;
////	case CursorType::ModifHeight:
////		break;
////	case CursorType::ModifSize:
////		break;
//	case CursorType::Move: {
//		return m_selRect;
//	}
////	case CursorType::Waiting:
////		break;
////	case CursorType::UnknowCursorType:
////		break;
//	default: {
//		m_selRect = QRect();
//		return m_selRect;
//	}
//	}
}

QRect RectCalcu::getOuterSelRect(QRect& rect, int interval)
{
	QPoint topLeft = rect.topLeft();
	QPoint bottomRight = rect.bottomRight() + QPoint(1, 1);
	return QRect(QPoint(topLeft.x() - interval, topLeft.y() - interval), QPoint(bottomRight.x() + interval, bottomRight.y() + interval));
}

QRect RectCalcu::getInnerSelRect(QRect& rect, int interval)
{
	// TODO 2021-10-01: 考虑 rect 的width 和 height 本身 <= 2 * HAIF_INTERVAL？
	QPoint topLeft = rect.topLeft();
	QPoint bottomRight = rect.bottomRight() + QPoint(1, 1);
	return QRect(QPoint(topLeft.x() + interval, topLeft.y() + interval), QPoint(bottomRight.x() - interval, bottomRight.y() - interval));
}

int RectCalcu::getMoveWidth()
{
	return m_moveEndPos.x() - m_moveStartPos.x();
}

int RectCalcu::getMoveHeight()
{
	return m_moveEndPos.y() - m_moveStartPos.y();
}

int RectCalcu::getModifyWidth()
{

	return m_modifyEndPos.x() - m_modifyStartPos.x();
}

int RectCalcu::getModifyHeight()
{
	return m_modifyEndPos.y() - m_modifyStartPos.y();
}

void RectCalcu::clear()
{
	m_startPos = QPoint();
	m_EndPos = QPoint();
	m_moveStartPos = QPoint();
	m_moveEndPos = QPoint();
	m_modifyStartPos = QPoint();
	m_modifyEndPos = QPoint();
	m_rtSel = QRect();
	m_cursorType = CursorType::Waiting;
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
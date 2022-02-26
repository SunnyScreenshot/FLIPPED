//
// Created by xmuli on 2021/9/29.
//

#include "rectcalcu.h"
#include <QRect>
#include <QPoint>

//// 通过任意两点获取一个选中矩形
//const QRect& RectCalcu::setSelRect()
//{
//	if (scrnType == ScrnType::Move) {
//		rtSel.translate(pos2 - pos1);
//	} else if (scrnType == ScrnType::Stretch) {
//		if (!rtSel.isValid())
//			rtSel = QRect();
//		else
//			rtSel = getStretchRect();
//	}
//
//	return  rtSel;
//}

// 返回副本
const QRect RectCalcu::getStretchRect()
{
	QRect rt;
	if (scrnType != ScrnType::Stretch)
		return rt;

	rt = rtSel;
	const int width = pos2.x() - pos1.x();
	const int height = pos2.y() - pos1.y();
	const QPoint offset(pos2 - pos1);
	const CursorArea cursArea = getCursorArea(pos1, true);

	if (cursArea == CursorArea::Left) {
		rt.setLeft(rt.left() + width);
	} else if (cursArea == CursorArea::Top) {
		rt.setTop(rt.top() + height);
	} else if (cursArea == CursorArea::Right) {
		rt.setRight(rt.right() + width);
	} else if (cursArea == CursorArea::Bottom) {
		rt.setBottom(rt.bottom() + height);
	} else if (cursArea == CursorArea::TopLeft) {
		rt.setTopLeft(rt.topLeft() + offset);
	} else if (cursArea == CursorArea::TopRight) {
		rt.setTopRight(rt.topRight() + offset);
	} else if (cursArea == CursorArea::BottomLeft) {
		rt.setBottomLeft(rt.bottomLeft() + offset);
	} else if (cursArea == CursorArea::BottomRight) {
		rt.setBottomRight(rt.bottomRight() + offset);
	} else {
	}

    // 修复拉伸矩形为负数但却可以绘画的场景
    if (!rt.isValid()) {
        if (rt.width() <= 0 || rt.height() <= 0) {
            QPoint p1(rt.topLeft());
            QPoint p2(rt.topLeft() + QPoint(rt.width(), rt.height()));
            rt = getRect(p1, p2);
        }
    }
	return rt;
}

// 计算后得出结果数据，其余归零
void RectCalcu::calcurRsultOnce()
{
    auto rt = getSelRect();
    rtSel = rt;

    if (bSmartMonitor)
        return;

    pos1 = QPoint();
    pos2 = QPoint();

}

// 仅被智能窗口选中时候使用，不要随意修改此数值;
void RectCalcu::setRtSel(const QRect rt)
{
    rtSel = rt;
}

// 限制选中矩形不超过虚拟屏幕的边界， rect 为当前选中矩形
QRect& RectCalcu::limitBound(QRect& rt, QRect rtDesktop)
{
	if (!rt.isValid())
		return rt;

	if (rt.left() <= rtDesktop.left())
		rt.setLeft(rtDesktop.left());
	if (rt.top() <= rtDesktop.top())
		rt.setTop(rtDesktop.top());
	if (rt.right() >= rtDesktop.right())
		rt.setRight(rtDesktop.right());
	if (rt.bottom() >= rtDesktop.bottom())
		rt.setBottom(rtDesktop.bottom());

	return rt;
}


// 判断当前鼠标所在区域; false 为大概区域（用来粗略计算显示光标区域即可）; true 为详细区域（精确计算所在区域，需要修改矩形大小做准备，显示不同光标）
const CursorArea RectCalcu::getCursorArea(const QPoint pos, bool details)
{
    QRect rt(rtSel);
	if (!rt.isValid() || !rt.isValid())
		return CursorArea::External;

	QRect rtOuter = getExteRect(rt);
	QRect rtInner = getInteRect(rt);
	int interval = (rtOuter.height() - rtInner.height()) / 2;

	QRect rtLeft(rtOuter.left(), rtInner.top(), interval, rtInner.height());
	QRect rtTop(rtInner.left(), rtOuter.top(), rtInner.width(), interval);
	QRect rtRight(rtInner.right(), rtInner.top(), interval, rtInner.height());
	QRect rtBottom(rtInner.left(), rtInner.bottom(), rtInner.width(), interval);
	QRect rtTopLeft(rtOuter.left(), rtOuter.top(), interval, interval);
	QRect rtTopRight(rtInner.right(), rtOuter.top(), interval, interval);
	QRect rtBottomLeft(rtOuter.left(), rtInner.bottom(), interval, interval);
	QRect rtBottomRight(rtInner.right(), rtInner.bottom(), interval, interval);

	// 内部、外部、边框
	if (rtInner.contains(pos, true)) {
		return CursorArea::Internal;
	} else if (!rtOuter.contains(pos, false)) {
		return CursorArea::External;
	} else if (!rtInner.contains(pos, true) && rtOuter.contains(pos, false)) {
		if (!details) {// 模糊区域
			return CursorArea::Border;
		} else {       // 需要更详细区域
			if (rtLeft.contains(pos, true))
				return CursorArea::Left;
			else if (rtRight.contains(pos, true))
				return CursorArea::Right;
			else if (rtTop.contains(pos, true))
				return CursorArea::Top;
			else if (rtBottom.contains(pos, true))
				return CursorArea::Bottom;
			else if (rtTopLeft.contains(pos, true))
				return CursorArea::TopLeft;
			else if (rtBottomRight.contains(pos, true))
				return CursorArea::BottomRight;
			else if (rtTopRight.contains(pos, true))
				return CursorArea::TopRight;
			else if (rtBottomLeft.contains(pos, true))
				return CursorArea::BottomLeft;
            else
                return CursorArea::Unknow;

		}
	} else {
		return CursorArea::Unknow;
	}
}

QRect RectCalcu::getRect(QPoint pos1, QPoint pos2)
{
	int xMin = qMin<int>(pos1.x(), pos2.x());
	int xMax = qMax<int>(pos1.x(), pos2.x());
	int yMin = qMin<int>(pos1.y(), pos2.y());
	int yMax = qMax<int>(pos1.y(), pos2.y());

    if (xMin == xMax && yMin == yMax) // 若是重复点，则会返回 QRect(0, 0, 1, 1);
        return QRect(0, 0, -1, -1);
    else
        return QRect(QPoint(xMin, yMin), QPoint(xMax, yMax));
	
}

RectCalcu::RectCalcu()
	: pos1(0, 0)
	, pos2(0, 0)
	, rtSel(0, 0, -1, -1)
	, m_bClear(false)
    , bSmartMonitor(true)
	, scrnType(ScrnType::Wait)
{
}

RectCalcu::~RectCalcu()
{
}

// 此时并不修改其 resel 数值
const QRect RectCalcu::getSelRect()
{
	if (scrnType == ScrnType::Select)
		return getRect(pos1, pos2);
	else if (scrnType == ScrnType::Move)
		return rtSel.translated(pos2 - pos1);
	else if (scrnType == ScrnType::Stretch)
		return getStretchRect(); // 返回副本
	else
		return rtSel;
}

QRect RectCalcu::getExteRect(QRect& rect, int interval)
{
	QPoint topLeft = rect.topLeft();
	QPoint bottomRight = rect.bottomRight() + QPoint(1, 1);
	return QRect(QPoint(topLeft.x() - interval, topLeft.y() - interval), QPoint(bottomRight.x() + interval, bottomRight.y() + interval));
}

QRect RectCalcu::getInteRect(QRect& rect, int interval)
{
	// TODO 2021-10-01: 考虑 rect 的width 和 height 本身 <= 2 * HAIF_INTERVAL？
	QPoint topLeft = rect.topLeft();
	QPoint bottomRight = rect.bottomRight() + QPoint(1, 1);
	return QRect(QPoint(topLeft.x() + interval, topLeft.y() + interval), QPoint(bottomRight.x() - interval, bottomRight.y() - interval));
}


void RectCalcu::clear()
{
	pos1 = QPoint();
	pos2 = QPoint();
	rtSel = QRect();
	scrnType = ScrnType::Wait;
	m_bClear = true;
    bSmartMonitor = true;
}

void RectCalcu::setClear(bool clear)
{
	m_bClear = clear;
}

bool RectCalcu::isClear()
{
	return m_bClear;
}

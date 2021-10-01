//
// Created by xmuli on 2021/9/29.
//

#ifndef PICSHOT_RECTCALCU_H
#define PICSHOT_RECTCALCU_H

#include <QWidget>

#define HAIF_INTERVAL 4    // 一半间隔，为 getOuterRect - getRect  == getRect - getInnerRect == HAIF_INTERVAL

QT_BEGIN_NAMESPACE
class QRect;
class QPoint;
QT_END_NAMESPACE

enum CursorType {
	Select = 0x00000000,
	MovePosition = 0x00000001,
	ModifWidth = 0x00000002,
	ModifHeight = 0x00000004,
	ModifSize = ModifWidth | ModifHeight,
	Move = 0x00000008,
	Waiting = 0x00000010,

	UnknowCursorType
};
Q_DECLARE_FLAGS(CursorTypes, CursorType)     // 枚举 CursorType 生成宏 CursorTypes
Q_DECLARE_OPERATORS_FOR_FLAGS(CursorTypes)  // 重载宏 CursorType 的 |() 函数

enum CursorArea {
	CursorCrossLeft = 0x00000000,
	CursorCrossTop = 0x00000001,
	CursorCrossRight = 0x00000002,
	CursorCrossBottom = 0x00000004,
	CursorCrossBorder = CursorCrossLeft | CursorCrossTop | CursorCrossRight | CursorCrossBottom,
	CursorCrossTopLeft = 0x00000010,
	CursorCrossTopRight = 0x00000020,
	CursorCrossBottomLeft = 0x00000040,
	CursorCrossBottomRight = 0x00000080,
	CursorCrossCorner = CursorCrossTopLeft | CursorCrossTopRight | CursorCrossBottomLeft | CursorCrossBottomRight,
	CursorCross = CursorCrossBorder | CursorCrossCorner,

	CursorOutSize = 0x00000100,
	CursorInner = 0x00000200,

	UnknowCursorArea
};
Q_DECLARE_FLAGS(CursorAreas, CursorArea)
Q_DECLARE_OPERATORS_FOR_FLAGS(CursorAreas)

// 矩形计算
class RectCalcu
{
public:

	QRect getSelRect();
	QRect getOuterSelRect(int interval = HAIF_INTERVAL);
	QRect getInnerSelRect(int interval = HAIF_INTERVAL);
	void clear();
	void setClear(bool clear);
	bool isClear();

	//QPoint getSelRectTopLeft();
	//void setSelRectTopLeft(QPoint topLeft);
	//QPoint getBottomRight();
	//void setBottomRight(QPoint bottomRight);
private:
	QRect getRect(QPoint pos1, QPoint pos2);

public:
    int m_width;
    int m_height;
	QPoint m_startPos;
	QPoint m_movePos;
	QPoint m_EndPos;
	QRect m_selRect;

private:
	bool m_bClear = true;  // 当前清理状态
	//QRect m_moveRect;
};


#endif //PICSHOT_RECTCALCU_H

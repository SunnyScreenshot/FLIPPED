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
	ModifBorderSize = ModifWidth | ModifHeight,
	ModifyTLAndBR = 0x00000010,
	ModifyTRAndBL = 0x00000020,
	ModifyCorner = ModifyTLAndBR | ModifyTRAndBL,
	ModifySize = ModifBorderSize | ModifyCorner,

	Move = 0x00000100,
	Waiting = 0x00000200,

	UnknowCursorType
};
Q_DECLARE_FLAGS(CursorTypes, CursorType)     // 枚举 CursorType 生成宏 CursorTypes
Q_DECLARE_OPERATORS_FOR_FLAGS(CursorTypes)  // 重载宏 CursorType 的 |() 函数

enum CursorArea {
	CursorCrossLeft = 0x00000000,
	CursorCrossTop = 0x00000001,
	CursorCrossRight = 0x00000002,
	CursorCrossBottom = 0x00000004,
	CursorCrossHorizontal = CursorCrossLeft | CursorCrossRight,
	CursorCrossVertical = CursorCrossTop | CursorCrossBottom,
	CursorCrossBorder = CursorCrossHorizontal | CursorCrossVertical,

	CursorCrossTopLeft = 0x00000010,
	CursorCrossTopRight = 0x00000020,
	CursorCrossBottomLeft = 0x00000040,
	CursorCrossBottomRight = 0x00000080,
	CursorCrossTL2BR = CursorCrossTopLeft | CursorCrossBottomRight,
	CursorCrossTR2BL = CursorCrossTopRight | CursorCrossBottomLeft,
	CursorCrossCorner = CursorCrossTL2BR | CursorCrossTR2BL,
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
	RectCalcu();
	~RectCalcu();


	QRect& getSelRect();
	QRect getOuterSelRect(QRect& rect, int interval = HAIF_INTERVAL);
	QRect getInnerSelRect(QRect& rect, int interval = HAIF_INTERVAL);
	int getMoveWidth();
	int getMoveHeight();
	int getModifyWidth();
	int getModifyHeight();
	void clear();
	void setClear(bool clear);
	bool isClear();
	const CursorArea getCursorArea(QPoint pos, bool details = false);

private:
	QRect& getRect(QPoint pos1, QPoint pos2);

public:
	QPoint m_startPos;
	QPoint m_EndPos;
	QPoint m_moveStartPos;
	QPoint m_moveEndPos;
	QPoint m_modifyStartPos;
	QPoint m_modifyEndPos;
	CursorType m_cursorType = CursorType::UnknowCursorType;     // 光标类型（对应此时鼠标的操作类型）

private:
	QRect m_rtSel;
	bool m_bClear;  // 当前清理状态
};


#endif //PICSHOT_RECTCALCU_H

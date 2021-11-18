//
// Created by xmuli on 2021/9/29.
//

#ifndef PICSHOT_RECTCALCU_H
#define PICSHOT_RECTCALCU_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>

// 1. HAIF_INTERVAL 为一半间隔，边框宽度一半高; 2 * HAIF_INTERVAL 为边框的宽度，为 getOuterRect - getRect  == getRect - getInnerRect == HAIF_INTERVAL
// 2. HAIF_INTERVAL 为边框一般的灵敏度，光标移动到上面便会变化形态
#define HAIF_INTERVAL 2

QT_BEGIN_NAMESPACE
class QRect;
class QPoint;
QT_END_NAMESPACE

// TODO 2021.11.09 后优化为 emun class、或写进 class 里面定，而非此全局形式
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
    Drawing = 0x00000400,

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

    static QRect getRect(QPoint pos1, QPoint pos2);
	QRect getRect(QRect rect, int px, CursorArea area);
	QRect& limitBound(QRect& rt, QRect rtDesktop = QApplication::desktop()->rect());

private:
	QRect& setSelRect(QPoint pos1, QPoint pos2);
	

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

//
// Created by xmuli on 2021/9/29.
//

#ifndef PICSHOT_RECTCALCU_H
#define PICSHOT_RECTCALCU_H

#include "../xglobal.h"
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>

QT_BEGIN_NAMESPACE
class QRect;
class QPoint;
QT_END_NAMESPACE

namespace XC {

	enum class ScrnType {
		// 基础的几种状态
		Wait,                                // 基础的等待状态（未有，和已有矩形局域）
		Select,                              // 基础的选中状态
		Move,                                // 基础的移动状态
		Draw,                                // 基础的绘画状态    
		Stretch,                             // 基础的拉伸状态
	};

	enum class CursorArea {
	   Left = 0x00000001,
	   Top = 0x00000002,
	   Right = 0x00000004,
	   Bottom = 0x00000008,
	   Horizontal = Left | Right,
	   Vertical = Top | Bottom,
	   Edga = Horizontal | Vertical,
	
	   TopLeft = 0x00000010,
	   TopRight = 0x00000020,
	   BottomLeft = 0x00000040,
	   BottomRight = 0x00000080,
	   TLAndBR = TopLeft | BottomRight,
	   TRAndBL = TopRight | BottomLeft,
	   Corner = TLAndBR | TRAndBL,
	
	   Border = Edga | Corner,               // 边框
	   External = 0x00000100,                // 外部
	   Internal = 0x00000200,                // 内部
	   Unknow
	};
	//Q_DECLARE_FLAGS(CursorAreas, CursorArea)
	//Q_DECLARE_OPERATORS_FOR_FLAGS(CursorAreas)
}

// ** 方便使用枚举 **
using namespace XC;

// 矩形计算
class RectCalcu
{
public:
	RectCalcu();
	~RectCalcu();

    static QRect getRect(QPoint pos1, QPoint pos2);
	const QRect getSelRect();

	void clear();
	void setClear(bool clear);
	bool isClear();
    const CursorArea getCursorArea(const QPoint pos, bool details = false);

	QRect& limitBound(QRect& rt, QRect rtDesktop);

	const QRect calcurRsultOnce();
    void setRtSel(const QRect rt);

private:
	//const QRect& setSelRect();
	const QRect getStretchRect(); // 仅拉伸时使用

	QRect getExteRect(QRect& rect, int interval = HAIF_INTERVAL); // 获取外部矩形
	QRect getInteRect(QRect& rect, int interval = HAIF_INTERVAL); // 获取内部矩形

	
public:
	QPoint pos1;
	QPoint pos2;
	ScrnType scrnType;          // 程序状态（对应此时鼠标的操作类型）
    bool bSmartMonitor;          // 模式为：是否智能获取窗口

private:
	QRect  rtSel;               // 由 pos1, pos2 所得
	bool m_bClear;              // 当前清理状态
	CursorArea cursArea;        // 光标对应区域
};

#endif //PICSHOT_RECTCALCU_H

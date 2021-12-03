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
#define HAIF_INTERVAL 4

QT_BEGIN_NAMESPACE
class QRect;
class QPoint;
QT_END_NAMESPACE

// Xps 为 XPicShot 的缩写
namespace Xps {

// TODO 2021.11.09 后优化为 emun class、或写进 class 里面定，而非此全局形式(那就需要重载 qDebug() 的 << 函数了，输出类)
enum CursorType {                                  // ------------（矩形区域）------------
    Select,                                        // 选中
    Move,                                          // 移动
    Waiting,                                       // 等待（未有，和已有矩形局域）
    ModifWidth,                                    // 拉伸左、右
    ModifHeight,                                   // 拉伸上、下
    ModifBorderSize = ModifWidth | ModifHeight,    // 拉伸任意一边（左、右、上、下）

    ModifyTLAndBR,                                 // 拉伸斜对角（左上、右下）
    ModifyTRAndBL,                                 // 拉伸斜对角（右上、左下）
    ModifyCorner = ModifyTLAndBR | ModifyTRAndBL,  // 拉伸斜任意一斜角（左上、右下、右上、左下）
    ModifySize = ModifBorderSize | ModifyCorner,   // 拉伸矩形任意一边或一斜角（左、右、上、下；左上、右下、右上、左下）

                                                   // ------------（绘画栏状态）------------
    Drawing,                                       // 绘画中（绘画按钮有处于点击状态）
    DrawSelect,                                    // 绘画时选中的绘画元素
    DrawMove,                                      // 绘画时移动的绘画元素
    DrawWaiting,                                   // 绘画时等待

                                                   // ------------（未知）------------
    UnknowCursorType                               // 未知
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
//Q_DECLARE_OPERATORS_FOR_FLAGS(CursorAreas)

}

// ** 方便使用枚举 **
using namespace Xps;

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
    CursorType m_cursorType = CursorType::UnknowCursorType;     // 程序状态（对应此时鼠标的操作类型）

private:
	QRect m_rtSel;
	bool m_bClear;  // 当前清理状态
};

#endif //PICSHOT_RECTCALCU_H

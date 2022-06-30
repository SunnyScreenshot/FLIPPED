/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.11.09
 * Description:
 ******************************************************************/
#ifndef XDRAW_H
#define XDRAW_H

#include "../xglobal.h"
#include <QObject>
#include <QPen>
#include <QRect>
#include <QFont>
#include <QPainterPath>
#include <QApplication>
#include <QScreen>
#include <QColor>

class QPainterPath;
class QLine;
class QPoint;


class QDomElement;
namespace XC {
	//enum ScrnStatus {
	//	// 基础的几种状态
	//	SS_WaitBase,                                // 基础的等待状态（未有，和已有矩形局域）
	//	SS_SelectBase,                              // 基础的选中状态
	//	SS_MoveBase,                                // 基础的移动状态
	//	SS_DrawBase,                                // 基础的绘画状态    
	//	SS_StretchBase,                             // 基础的拉伸状态
	//};

	//enum ScreenStatus {                                  // ------------（矩形区域）------------
	//	Auto,     // 自动检测
	//	Custom,   // 手动拖曳

	//	Left,
	//	Right,
	//	Top,
	//	Bottom,
	//	TopLeft,
	//	TopRight,
	//	BottomLeft,
	//	BottomRight,

	//	Width = Left | Right,                    // 左、右
	//	Height = Top | Bottom,                   // 上、下
	//	Edge = Width | Height,                   // 任意一边（左、右、上、下）
	//	TLAndBR = TopLeft | BottomRight,         // 斜对角（左上、右下）
	//	TRAndBL = TopRight | BottomLeft,         // 斜对角（右上、左下）
	//	Corner = TLAndBR | TRAndBL,              // 斜任意一斜角（左上、右下、右上、左下）
	//	Frame = Edge | Corner,                   // 矩形任意一边或一斜角（左、右、上、下；左上、右下、右上、左下）         

	//	UnknowCursorType                         // 未知
	//};
	//Q_DECLARE_FLAGS(CursorTypes, ScreenStatus)     // 枚举 ScrnType 生成宏 CursorTypes
	//	Q_DECLARE_OPERATORS_FOR_FLAGS(CursorTypes)   // 重载宏 ScrnType 的 |() 函数



	// C++11 新增带作用域的枚举，用 enum class  或enum struct（两者等价）声明。
	// https://blog.csdn.net/luckysym/article/details/1666114
	enum class DrawState {
		Draw,
		Move,
		Delete,
		Wateing,
		Unknow
	};

	enum class DrawShape {
		NoDraw,
		Rectangles,
		Ellipses,
        Line,
        Arrows,
        Pen,
        Mosaics,
		Text,
        SerialNumber
	};

	enum class LineEnds {
		EmptyToEmpty,
		EmptyToRecesedArrow,
		DotToRecesedArrow,
		EmptyToArrow,
		DotToArrow,
		EmptyToDot,
		DotToEmpty,
		DotToDot,
		RecesedArrowToRecesedArrow,
		ArrowToArrow
	};
}

using namespace XC;


//Rectangles,
//Ellipses,
//Arrows,
//Lines,
//Texts,
//Mosaics,
//NoDraw

struct  XDrawStep
{   // 公共元素 ------------------
	QPoint pos1;                             // 起点
	QPoint pos2;                             // 终点
	QRect rt;                                // 初始绘画位置
    DrawShape shape = DrawShape::NoDraw;     // 绘画形状
	bool bFill = false;                      // 绘画类型
	int index = -1;                          // 序号，亦 z 轴，越大越顶层
	static int g_index;

	QPen pen = QPen(Qt::red);
	int penWidth = 2;                         // 画笔宽度
	QBrush brush = QBrush(Qt::NoBrush);       // 画刷
	int brushWidth = 1;                       // 画刷宽度
	int transparency = 1;                     // 透明度

	// Rectangles ----------------

	// Ellipses ------------------
    int rX = 58;                              // 短长轴
    int rY = 58;                              // 竖长轴
	
	// Arrows || Brush(Customized path) ---
	LineEnds lineEnds = LineEnds::EmptyToEmpty;
	Qt::PenStyle lineDashes = Qt::SolidLine;
	QVector<QPoint> custPath;                 // 手绘路径

	// Texts ---------------------
	bool bDisplay = false;                       // true 准备显示，显示；false 显示结束，隐藏起来
	bool bTextComplete = true;                // 文字编辑完成
	QPoint editPos;                           // 显示控件
    QString text = "==Test Text==";
	QFont font = QFont();
	int fontSize = 16;

	// Mosaic ---------------------
	int mscPx = 3; // 马赛克默认模糊块

    void clear() {
        pos1 = QPoint();
        pos2 = QPoint();
        rt = QRect();
        //shape = DrawShape::NoDraw; // 若为鼠标松开执行，则会无法继续绘画抽象图形
        //pen = QPen(Qt::red);
        //penWidth = 2;
        //brush = QBrush(Qt::NoBrush);
        //brushWidth = 1;
        //transparency = 1;
        //rX = 8;
        //rY = 8;
        //lineEnd = LineEnds::EmptyToEmpty;
        //lineDashe = LineDashes::SolidLine;
        custPath.clear();
        //text = "==Test Text==";
        //font = QFont();
        //fontSize = 16;
        //mscPx = 3;

        index = -1;
        //g_index = -1;  永不重置

    }
};


class ScrnHelper : public QObject
{
    Q_OBJECT
public:
    explicit ScrnHelper(QObject* parent = nullptr);
    virtual ~ScrnHelper();
};

// ------------------------

namespace XHelp {
double getScale(QScreen *screen = QApplication::primaryScreen());
QColor highlightColor(QColor highlight = QColor("#1F7AFF"));

QIcon changeSVGColor(QString path, QString shape, QColor color, QSize size);
void setAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval);



// Mosaics draw
const QPixmap* SetMosaicSmooth(QPixmap* pixmap, int px);      // 毛玻璃马赛克
const QImage SetMosaicPixlelated(QPixmap* pixmap, int px = 20);  // 像素级马赛克

// Arrow Line draw
QPainterPath GetArrowHead(QPoint p1, QPoint p2, const int thickness = 10);
QLine GetShorterLine(QPoint p1, QPoint p2, const int thickness = 10);
}


#endif // XDRAW_H

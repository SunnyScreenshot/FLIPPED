//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/09.
//
#ifndef XDRAW_H
#define XDRAW_H

#include <QObject>
#include <QPen>
#include <QRect>
#include <QFont>

// C++11 新增带作用域的枚举，用 enum class  或enum struct（两者等价）声明。
// https://blog.csdn.net/luckysym/article/details/1666114
enum class XDrawState {
    Draw,
    Move,
    Delete,
    Wateing,
    Unknow
};

enum class XDrawShape {
    Rectangles,
    Ellipses,
    Brush,
    Arrows,
    Texts,
    Mosaics,
    NoDraw
};

//Rectangles,
//Ellipses,
//Arrows,
//Lines,
//Texts,
//Mosaics,
//NoDraw
struct  XDrawStep
{
	// 公共元素 ------------------
    QPoint startPos = QPoint();             // 起点
    QPoint endPos = QPoint();               // 终点
    QRect rt = QRect();                     // 初始绘画位置
    XDrawShape shape = XDrawShape::NoDraw;  // 绘画形状
	bool bFill = false;                     // 绘画类型

	QPen pen = QPen(Qt::red);
	int penWidth = 2;
	QBrush brush = QBrush(Qt::NoBrush);
	int brushWidth = 1;

	// Rectangles ----------------


	// Ellipses ------------------
    int rX = 58;                             // 短长轴
    int rY = 58;                             // 竖长轴

	// Arrows --------------------

	// Brush(Customized path) ----
	// CusPath;


	// Texts ---------------------
    QString text = "==Test Text==";
	QFont font = QFont();
	int fontSize = 16;

	// Mosaic ---------------------
	int mscPx = 3; // 马赛克默认模糊块

    void clear();
};

class DrawHelper : public QObject
{
    Q_OBJECT
public:
    explicit DrawHelper(QObject* parent = nullptr);
    virtual ~DrawHelper();

    void drawRect(QPainter& pa, QRect rt, QPen pen = QPen(Qt::red), int width = 2, QBrush brush = QBrush(Qt::NoBrush));
    bool drawStep();
    bool saveDrawStep(QVector<XDrawStep *>& steps);
    bool revoke();
    bool revoke(XDrawStep* step);

public:
    XDrawStep m_step;
    QVector<XDrawStep> m_vDrawStep;
};

//QDebug operator<<(QDebug myDebug, const XDrawStep& step)
//{
//    myDebug << step.rt << step.shape;
//    return myDebug;
//}

#endif // XDRAW_H

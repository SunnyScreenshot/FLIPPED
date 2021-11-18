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

enum class XDrawType {
    Rectangles,
    Ellipses,
    Lines,
    Arrows,
    Texts,
    Mosaics,
    NoDraw
};

struct  XDrawStep
{
    QPoint startPos = QPoint();
    QPoint endPos = QPoint();
    QRect rt = QRect();        // 初始绘画位置
    QPen pen = QPen(Qt::NoPen);
    QBrush brush = QBrush(Qt::NoBrush);
    QFont font = QFont();
    XDrawType drawType = XDrawType::NoDraw;  // 绘画类型
};

class XDraw : public QObject
{
    Q_OBJECT
public:
    explicit XDraw(QObject* parent = nullptr);
    virtual ~XDraw();

    void drawRect(QPainter& pa, QRect rt, QPen pen = QPen(Qt::red), int width = 2, QBrush brush = QBrush(Qt::NoBrush));
    bool drawStep();
    bool saveDrawStep(QVector<XDrawStep *>& steps);
    bool revoke();
    bool revoke(XDrawStep* step);



public:
    XDrawStep m_step;
    QVector<XDrawStep> m_vDrawStep;
};

#endif // XDRAW_H

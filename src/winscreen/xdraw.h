//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/09.
//
#ifndef XDRAW_H
#define XDRAW_H

#include <QObject>
#include <QPen>
#include <QRect>

// C++11 新增带作用域的枚举，用 enum class  或enum struct（两者等价）声明。
// https://blog.csdn.net/luckysym/article/details/1666114
enum class XDrawType {
    Draw,
    Move,
    Delete,
    Wateing,
    Unknow
};

struct  XDrawStep
{
    uint nId = 0;                  // 绘画序号
    QRect rtSour = QRect();        // 源位置
    QRect rtDest = QRect();        // 目标位置
    QPen m_pen = QPen(Qt::NoPen);
    QBrush m_brush = QBrush(Qt::NoBrush);
    XDrawType drawType = XDrawType::Unknow;  // 绘画类型
};

class XDraw : public QObject
{
    Q_OBJECT
public:
    explicit XDraw(QObject* parent = nullptr);
    virtual ~XDraw();

private:
    bool drawStep();
    bool saveDrawStep(QVector<XDrawStep *>& steps);
    bool revoke();
    bool revoke(XDrawStep* step);

private:
    XDrawStep m_step;
    QVector<XDrawStep *> m_vDrawStep;
};

#endif // XDRAW_H

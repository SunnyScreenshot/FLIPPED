#include "xlabel.h"
#include "../xglobal.h"
#include <QColor>
#include <QPainter>
#include <QPainter>
#include <QPalette>


XLabel::XLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    init();
}

XLabel::XLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
    init();
}

XLabel::~XLabel()
{
}

void XLabel::setInEllipseColor(QColor color, double alpha)
{
    m_inEllipse = color;
    m_inAlpha = alpha;

    update();
}

void XLabel::setOutEllipseColor(QColor color, double alpha)
{
    m_outEllipse = color;
    m_outAlpha = alpha;
}

void XLabel::init()
{
//    setWindowFlags(Qt::FramelessWindowHint);      // 去掉标题栏
//    setAttribute(Qt::WA_TranslucentBackground);   // 设置透明，自绘画为圆角矩形
//    setAutoFillBackground(false);

//    QColor palColor("#00FF00");
//    palColor.setAlphaF(1);
//    QPalette palette;
//    palette.setColor(QPalette::Background, palColor);
//    this->setPalette(palette);

    QColor col = this->palette().background().color();
    m_inEllipse = col;
    m_inAlpha = 1;
    m_outEllipse = col;
    m_outAlpha = 1;
}

void XLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);



//    pa.setPen(Qt::NoPen);
//    QColor tColBrush("#131313");
//    tColBrush.setAlphaF(0.6);
//    pa.setBrush(tColBrush);
//    pa.setBrush(Qt::red);
    QColor tColBrush("#131313");
    tColBrush.setAlphaF(0.6);
    pa.setBrush(tColBrush);
    QColor tColPen("#FFFFFF");
    tColPen.setAlphaF(0.1);
    pa.setPen(tColPen);
    pa.drawRect(contentsRect());
//    QColor tColPen("#FFFFFF");
//    tColPen.setAlphaF(0.1);
//    pa.setPen(tColPen);


    QColor colBrush(m_inEllipse);
    colBrush.setAlphaF(m_inAlpha);
    pa.setPen(Qt::NoPen);
    pa.setBrush(colBrush);
    pa.drawEllipse(contentsRect().adjusted(1, 1, -1, -1));

    QColor colPen("#000000");
    colPen.setAlphaF(0.08);
    pa.setBrush(Qt::NoBrush);
    pa.drawEllipse(contentsRect());
}

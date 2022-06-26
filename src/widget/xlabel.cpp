#include "xlabel.h"
#include "../xglobal.h"
#include <QPainter>
#include <QPainter>
#include <QPalette>
#include <QtGlobal>
#include <QMouseEvent>
#include "../core/xlog.h"

XLabel::XLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
    , m_inEllipse("#FFFFFF")
    , m_inAlpha(1)
    , m_outEllipse("#000000")
    , m_outAlpha(0.08)
    , m_bGradient(false)
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

void XLabel::setMargin(double margin)
{
    m_nMargin = margin;
}

void XLabel::setInEllipseR(double r)
{
    m_nInEllipseR = r;
}

void XLabel::setInEllipseColor(QColor color, double alpha)
{
    m_inEllipse = color;
    m_inAlpha = alpha;
}

void XLabel::setOutEllipseColor(QColor color, double alpha)
{
    m_outEllipse = color;
    m_outAlpha = alpha;
}

void XLabel::setConicalGradientColor(QPainter& pa)
{
    // 构造一个锥向渐变(center, angle) https://blog.csdn.net/weixin_37818081/article/details/118879134
    QConicalGradient conicalGradient(QPointF(contentsRect().center()), 300);

    conicalGradient.setColorAt(0, QColor("#EB004A"));
    conicalGradient.setColorAt(1 / 6.0, QColor("#FF0000"));
    conicalGradient.setColorAt(2 / 6.0, QColor("#F800FC"));
    conicalGradient.setColorAt(3 / 6.0, QColor("#4D00F3"));
    conicalGradient.setColorAt(4 / 6.0, QColor("#1BB8F2"));
    conicalGradient.setColorAt(5 / 6.0, QColor("#46EA48"));
    conicalGradient.setColorAt(1, QColor("#FFDB34"));

    // 使用锥向渐变创建一个画刷，用来填充
    pa.save();
    pa.setPen(Qt::NoPen);
    pa.setBrush(conicalGradient);
    const QRect& rt = contentsRect().adjusted(1, 1, -1, -1);
    pa.drawEllipse(rt.center(), rt.width() / 2, rt.height() / 2);
    pa.restore();
}

void XLabel::setEnablemGradient(bool enablem)
{
    m_bGradient = enablem;
}

void XLabel::init()
{
    // 此两行也可以注释掉，亲测不影响，可能是由于后面有一个 ColorParaBar 作为父类
    //setWindowFlags(Qt::FramelessWindowHint);      // 去掉标题栏
    setAttribute(Qt::WA_TranslucentBackground);   // 设置透明，自绘画为圆角矩形
//    setAutoFillBackground(false);

    QColor col = this->palette().background().color();
    m_inEllipse = col;
    m_inAlpha = 1;
    m_outEllipse = col;
    m_outAlpha = 1;
    m_bGradient = false;

    resize(24, 24);
    auto a = contentsRect();
    int width = this->width();
    int height = this->height();
    m_nInEllipseR = qMin(width, height) / 2.0;
    m_nMargin = 1;
}

void XLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    // 矩形和内切圆此部分不用绘画 避免出现 拖曳时刻出现残影和交叉部分显示黑色

    if (m_bGradient) {
        setConicalGradientColor(pa);
        // TODO 2022.06.21 故意缺少最外层一单层圈、 #000000、 0.08； 但是加上就很显示外圈线条很黑
    } else {
        QColor colBrush(m_inEllipse);
        colBrush.setAlphaF(m_inAlpha);
        pa.setPen(Qt::NoPen);
        pa.setBrush(colBrush);
//        pa.drawEllipse(contentsRect().adjusted(1, 1, -1, -1));
        const int r = m_nInEllipseR - 1;
        pa.drawEllipse(contentsRect().center(), r, r);

        // 此参数设计图写死
        QColor colPen("#000000");
        colPen.setAlphaF(0.08);
        pa.setPen(colPen);
        pa.setBrush(Qt::NoBrush);
        pa.drawEllipse(contentsRect().center(), r, r);
    }
}

//void XLabel::mousePressEvent(QMouseEvent* ev)
//{
//    ev->ignore();  //验证事件事件被忽略后会被传递给父对象
//}
//
//void XLabel::mouseReleaseEvent(QMouseEvent* ev)
//{
//    ev->ignore();
//}

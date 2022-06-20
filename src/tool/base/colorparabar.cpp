#include "colorparabar.h"
#include "../../xglobal.h"
#include "../../widget/xlabel.h"
#include "../../screen/drawhelper.h"
#include <QLabel>
#include <QBoxLayout>
#include <QGridLayout>
#include <QList>
#include <QVector>
#include <QMap>
#include <QPainter>
#include <QPen>
#include <QColor>

ColorParaBar::ColorParaBar(Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(new QGridLayout())
{
    QMap<QString, QString> labMap { {"lab0_Red", "#DB000F"}
                                  , {"lab1_Yellow", "#FFCF53"}
                                  , {"lab3_Green", "#12F63B"}
                                  , {"lab4_Blue", "#0E70FF"}
                                  , {"lab5_Pink", "#FB4288"}
                                  , {"lab6_Black", "#323232"}
                                  , {"lab7_White", "#FBFBFB"}
                                  , {"lab8_Pick", "#FFDB34"}};  // 最后一个彩色

    int rowMax = 0;
    int colMax = 0;
    if(orien == Qt::Horizontal) {
        rowMax = 2;
        colMax = labMap.size() / rowMax;
    } else {
        colMax = 2;
        rowMax = labMap.size() / colMax;
    }

    auto it = labMap.begin();
    for (int i = 0; i < rowMax; ++i) {
        for (int j = 0; j < colMax; ++j) {

//            QLabel* lab = new QLabel(this);
//            QPalette palette;
//            palette.setColor(QPalette::Background, it.value());
//            lab->setPalette(palette);


            XLabel* lab = new XLabel(this);
            lab->setObjectName(it.key());
            const int width(COLOR_LABEL_WIDTH * m_scal);
            lab->setFixedSize(width, width);
//            lab->setAutoFillBackground(true);

            lab->setInEllipseColor(it.value(), 1);
//            lab->setOutEllipseColor("#000000", 0.08);

            m_layout->addWidget(lab, i, j);
            it++;
        }
    }

    m_layout->setMargin(COLOR_PARA_MARGIN);
    m_layout->setHorizontalSpacing(COLOR_PARA_HOR_SPACING * m_scal);
    m_layout->setVerticalSpacing(COLOR_PARA_VER_SPACING * m_scal);  // 检查比例一下

    setLayout(m_layout);

}

ColorParaBar::~ColorParaBar()
{

}

void ColorParaBar::init()
{
    m_layout->setMargin(COLOR_PARA_MARGIN);
    m_layout->setHorizontalSpacing(COLOR_PARA_HOR_SPACING * m_scal);
    m_layout->setVerticalSpacing(COLOR_PARA_VER_SPACING * m_scal);  // 检查比例一下
}

void ColorParaBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing, true);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPen pen(Qt::NoPen);
    pa.setPen(pen);
    QColor colBrush("#131313");
    colBrush.setAlphaF(0.6);
    pa.setBrush(colBrush);
    QColor colPen("#FFFFFF");
    colPen.setAlphaF(0.1);
    pa.setPen(colPen);
    pa.drawRoundedRect(contentsRect().adjusted(1, 1, -1, -1), CW_RADIRS, CW_RADIRS);

    colPen.setNamedColor("#000000");
    colPen.setAlphaF(0.1);
    pa.setPen(colPen);
    pa.setBrush(Qt::NoBrush);

    pa.drawRoundedRect(contentsRect(), CW_RADIRS, CW_RADIRS);

}

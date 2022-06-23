/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.21
 * Description: 取色器面板
 ******************************************************************/
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
                                  , {"lab2_Green", "#12F63B"}
                                  , {"lab3_Blue", "#0E70FF"}
                                  , {"lab4_Pink", "#FB4288"}
                                  , {"lab5_Black", "#323232"}
                                  , {"lab6_White", "#FBFBFB"}
                                  , {"lab7_Pick", ""}};  // 最后一个彩色

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

            XLabel* lab = new XLabel(this);
            lab->setObjectName(it.key());
            int width(COLOR_LABEL_WIDTH * m_scal);
            lab->setFixedSize(width, width);
            lab->setInEllipseR(width / 2.0);

            if ((it + 1) == labMap.end()) {  // 最后一个渐变色
                lab->setEnablemGradient(true);
            } else {
                lab->setInEllipseColor(it.value(), 1);
            }

            m_layout->addWidget(lab, i, j);
            it++;
        }
    }

    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(COLOR_PARA_HOR_SPACING * m_scal);
    m_layout->setVerticalSpacing(COLOR_PARA_VER_SPACING * m_scal);  // 检查比例一下
    setLayout(m_layout);
}

ColorParaBar::~ColorParaBar()
{

}

void ColorParaBar::init()
{
    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(COLOR_PARA_HOR_SPACING * m_scal);
    m_layout->setVerticalSpacing(COLOR_PARA_VER_SPACING * m_scal);  // 检查比例一下
}

void ColorParaBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
//    Q_UNUSED(event)

//    QPainter pa(this);
//    pa.setRenderHints(QPainter::Antialiasing, true);
//    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//    QPen pen(Qt::NoPen);
//    pa.setPen(pen);
//    QColor colBrush("#131313");
//    colBrush.setAlphaF(0.6);
//    pa.setBrush(colBrush);
//    QColor colPen("#FFFFFF");
//    colPen.setAlphaF(0.1);
//    pa.setPen(colPen);
//    pa.drawRoundedRect(contentsRect().adjusted(1, 1, -1, -1), CW_RADIRS, CW_RADIRS);

//    colPen.setNamedColor("#000000");
//    colPen.setAlphaF(0.1);
//    pa.setPen(colPen);
//    pa.setBrush(Qt::NoBrush);

//    pa.drawRoundedRect(contentsRect(), CW_RADIRS, CW_RADIRS);

}

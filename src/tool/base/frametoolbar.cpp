/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.18
 * Description: 工具栏框架类，负责绘画上面各种矩形的那个 toolbar
 ******************************************************************/
#include "frametoolbar.h"
#include "../../xglobal.h"
#include "../../widget/xhorizontalline.h"
#include "../../widget/xverticalline.h"
#include "../../screen/drawhelper.h"
#include <QColor>
#include <QPainter>
#include <QBoxLayout>

FrameToolBar::FrameToolBar(Qt::Orientations orien, QWidget *parent)
    : XFrameWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(nullptr)
{
    initUI();
}

FrameToolBar::~FrameToolBar()
{

}

void FrameToolBar::addWidget(QWidget *w)
{
    if (w)
        m_layout->addWidget(w);
}

void FrameToolBar::addSpacer()
{
    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine((ICON_WIDTH - ICON_WIDTH_IN_MARGIN) * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine((ICON_WIDTH - ICON_WIDTH_IN_MARGIN) * m_scal, this));
}

inline void FrameToolBar::setMargin(int margin)
{
    if (m_layout)
        m_layout->setMargin(margin);
}

inline void FrameToolBar::setSpacing(int space)
{
    if (m_layout)
        m_layout->setSpacing(space);
}

void FrameToolBar::initUI()
{
    if (m_orien == Qt::Horizontal) {
        m_layout = new QHBoxLayout(this);
//        resize(395 * m_scal, 40 * m_scal);
    } else {
        m_layout = new QVBoxLayout(this);
//        resize(40 * m_scal, 395 * m_scal);
    }

    setLayout(m_layout);
    m_layout->setMargin(CW_MARGIN);
    m_layout->setSpacing(CW_ITEM_SPACE);

    // TEST BEGIN
//    m_layout->addWidget(new QPushButton("btn1"));
//    m_layout->addWidget(new QPushButton("btn2"));
//    addSpacer();
//    m_layout->addWidget(new QPushButton("btn3"));
    // TEST END
}

//void FrameToolBar::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event)

//    QPainter pa(this);
//    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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
//}

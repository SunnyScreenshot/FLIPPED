/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.22
 * Description: Line Widt
 ******************************************************************/
#include "widthparabar.h"

#include "../../xglobal.h"
#include "../../widget/xhorizontalline.h"
#include "../../widget/xverticalline.h"
#include "../../screen/drawhelper.h"
#include "../../widget/xlabel.h"
#include <QColor>
#include <QPainter>
#include <QBoxLayout>

WidthParaBar::WidthParaBar(Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(nullptr)
{
    initUI();
}

WidthParaBar::~WidthParaBar()
{

}

void WidthParaBar::addWidget(QWidget *w)
{
    if (w)
        m_layout->addWidget(w);
}

void WidthParaBar::addSpacer()
{
    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine((ICON_WIDTH - ICON_WIDTH_IN_MARGIN) * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine((ICON_WIDTH - ICON_WIDTH_IN_MARGIN) * m_scal, this));
}


void WidthParaBar::initUI()
{
    if (m_orien == Qt::Horizontal) {
        m_layout = new QHBoxLayout(this);
    } else {
        m_layout = new QVBoxLayout(this);
    }

    setLayout(m_layout);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    // objectName - LineWidth
    QMap<QString, int> labMap{ {"lab0", 6}
                              , {"lab1", 8}
                              , {"lab2", 12} };

    for (auto it = labMap.begin(); it != labMap.end(); it++) {
        XLabel* lab = new XLabel(this);
        lab->setObjectName(it.key());
        int width(WIDTH_LABEL_WIDTH * m_scal);
        lab->setFixedSize(width, width);
        lab->setInEllipseR(it.value() * 2 * m_scal / 2.0);  // 第一个 *2 是 xglobal.h 中的统一乘以 2
        addWidget(lab);
    }
}

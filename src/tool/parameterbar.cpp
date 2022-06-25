/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.22
 * Description: 绘画工具栏 下面那个具体参数 ToolBar
 ******************************************************************/
#include "parameterbar.h"
#include "../xglobal.h"
#include "../widget/xhorizontalline.h"
#include "../widget/xverticalline.h"
#include "../widget/xcombobox.h"
#include "../screen/drawhelper.h"
#include <QColor>
#include <QPainter>
#include <QBoxLayout>
#include <QWidget>
#include <QComboBox>

ParameterBar::ParameterBar(Qt::Orientations orien, QWidget *parent)
    : XFrameWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(nullptr)
    , m_widthBar(new WidthParaBar(orien))
    , m_colorBar(new ColorParaBar(orien))
    , m_serialBar(new XComboBox(this))

{
    initUI();
}

ParameterBar::~ParameterBar()
{
}


void ParameterBar::addWidget(QWidget *w)
{
    if (m_layout && w)
        m_layout->addWidget(w);
}

void ParameterBar::addSpacer()
{
    if (!m_layout)
        return;

    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine(SPACER_LINE_HEIGHT * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine(SPACER_LINE_HEIGHT * m_scal, this));
}

void ParameterBar::initUI()
{
    if (m_orien == Qt::Horizontal)
        m_layout = new QHBoxLayout(this);
    else
        m_layout = new QVBoxLayout(this);

    if (m_colorBar)
        m_colorBar->setVisible(true);

    const int bbMarginHor = BAR_MARGIN_HOR;
    int bbMarginVer = BAR_MARGIN_VER;
    if (m_colorBar->isVisible()) {
        bbMarginVer = BAR_MARGIN_VER_HAS_COLOR_PARA;
        m_layout->setContentsMargins(bbMarginHor, 0, 0, 0);
    } else {
        m_layout->setContentsMargins(bbMarginHor, bbMarginVer, bbMarginHor, bbMarginVer);
    }
    
    setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(BAR_MARGIN_HOR);  // TODO 最后一个大概是两个这个间隔（间隔 + 取色盘自带的）
    
    m_serialBar->setFixedSize(COMBOBOX_WIDTH * m_scal, COMBOBOX_HEIGHT * m_scal);
    m_layout->addWidget(m_serialBar);

    addSpacer();
    m_layout->addWidget(m_widthBar);
    addSpacer();
    m_layout->addWidget(m_colorBar);

//    adjustSize();  // 布局后重新计算一下大小尺寸
//    resize(size().width() + bbMarginHor * 2, size().height() + bbMarginVer * 2);
}

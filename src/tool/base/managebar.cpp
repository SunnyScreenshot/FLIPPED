/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.26
 * Description: 参数工具栏的一部分，作为管理类：矩形、椭圆、箭头、马赛克等
 ******************************************************************/
#include "managebar.h"
#include "../../xglobal.h"
#include "../../widget/xhorizontalline.h"
#include "../../widget/xverticalline.h"
#include "../../screen/drawhelper.h"
#include <QBoxLayout>

ManageBar::ManageBar(Qt::Orientations orien /*= Qt::Horizontal*/, QWidget* parent /*= nullptr*/)
    : QWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(nullptr)
{
    initUI();
}

void ManageBar::initUI()
{
    if (!m_layout) {
        if (m_orien == Qt::Horizontal)
            m_layout = new QHBoxLayout(this);
        else
            m_layout = new QVBoxLayout(this);
    }

    setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
    m_layout->setMargin(0);
    m_layout->setSpacing(CW_ITEM_SPACE);
}

void ManageBar::addWidget(QWidget* w)
{
    if (w)
        m_layout->addWidget(w);
}

void ManageBar::addSpacer()
{
    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine(SPACER_LINE_HEIGHT * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine(SPACER_LINE_HEIGHT * m_scal, this));
}

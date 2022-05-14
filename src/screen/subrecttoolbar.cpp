/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.12.04
 * Description:
 ******************************************************************/
#include "subrecttoolbar.h"
#include "screenshot.h"
#include "../xglobal.h"
#include "../widget/xverticalline.h"
#include <QBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSpinBox>

SubRectToolBar::SubRectToolBar(QWidget *parent)
	: SubAbsToolBar(parent)
    , m_spBorder(new QSpinBox(this))
{
    initUI();
}

void SubRectToolBar::initUI()
{
	initAbsTb(":/resources/icons/normal/rectangle.svg", tr("Rectangle"));
	initFilledAbsTb(":/resources/icons/normal/fill-rect.svg", tr("Filled Rectangle"));

	QLabel* labBorder = new QLabel(tr("Border"), this);
	const int iconEdge = ICON_HEIGHT * ScreenShot::getScale();
	labBorder->setMaximumHeight(iconEdge);
	addWidget(labBorder);

	m_spBorder->setValue(2);
	m_spBorder->setMinimum(0);
	addWidget(m_spBorder);
	addStretch(0);
}

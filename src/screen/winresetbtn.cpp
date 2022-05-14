/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.09.29
 * Description:
 ******************************************************************/
#include "winresetbtn.h"
#include <QPushButton>
#include <QBoxLayout>

WinResetBtn::WinResetBtn(QWidget * parent)
	: QWidget(parent)
	, m_btnReset(nullptr)
{
	init();
}

WinResetBtn::~WinResetBtn()
{
}

QPushButton* WinResetBtn::getBtnReset()
{
	return m_btnReset;
}

void WinResetBtn::init()
{
	QHBoxLayout* hLayout = new QHBoxLayout(this);
	hLayout->setSpacing(0);
	hLayout->setMargin(0);
	m_btnReset = new QPushButton(tr("Reset"));
	hLayout->addWidget(m_btnReset, 1);
	hLayout->addStretch(7);
}

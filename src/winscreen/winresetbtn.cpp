//
// Created by XMuli <xmulitech@gmail.com> on 2021/9/29.
//

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

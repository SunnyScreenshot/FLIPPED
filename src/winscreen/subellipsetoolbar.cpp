//
// Created by xmuli on 2021/12/14.
//
#include "subellipsetoolbar.h"
#include "screenshot.h"
#include "../xglobal.h"
#include <QBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSpinBox>

SubEllipseToolBar::SubEllipseToolBar(QWidget *parent)
	: SubAbsToolBar(parent)
    , m_spBorder(new QSpinBox(this))
{
    initUI();
}

void SubEllipseToolBar::initUI()
{
	initAbsTb(":/resources/icons/normal/ellipse.svg", tr("Ellipse"));
	initFilledAbsTb(":/resources/icons/normal/fill-ellipse.svg", tr("Filled Ellipse"));

	QLabel* labBorder = new QLabel(tr("Border"), this);
	addWidget(labBorder);

	m_spBorder->setValue(2);
	m_spBorder->setMinimum(0);
	addWidget(m_spBorder);
	addStretch(0);
}
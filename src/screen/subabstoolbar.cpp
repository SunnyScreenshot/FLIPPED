//
// Created by xmuli on 2021/12/27.
//
#include "subabstoolbar.h"
#include "screenshot.h"
#include "../xglobal.h"
#include "../widget/xverticalline.h"
#include <QLabel>
#include <QToolButton>

SubAbsToolBar::SubAbsToolBar(QWidget *parent)
    : QWidget(parent)
    , m_tbAbs(new QToolButton(this))
    , m_tbFilledAbs(new QToolButton(this))
{
    const int iconWidth = ICON_HEIGHT * ScreenShot::getScale();
    m_tbAbs->setIconSize(QSize(iconWidth, iconWidth));
    m_tbAbs->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbAbs->setAutoRaise(true);
	m_tbAbs->setCheckable(true);
    m_tbAbs->setChecked(true);

    m_tbFilledAbs->setIconSize(QSize(iconWidth, iconWidth));
    m_tbFilledAbs->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbFilledAbs->setAutoRaise(true);
	m_tbFilledAbs->setCheckable(true);
    m_tbFilledAbs->setChecked(false);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setObjectName("hLayout");
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    hLayout->addWidget(m_tbAbs);
    hLayout->addWidget(m_tbFilledAbs);

    const int spacing = 0; // 分割线周围的间距
    hLayout->addSpacing(spacing);
    hLayout->addWidget(new XVerticalLine(iconWidth, this));
    hLayout->addSpacing(spacing);

    setLayout(hLayout);

    connect(m_tbAbs, &QToolButton::clicked, this, &SubAbsToolBar::onClicked);
    connect(m_tbFilledAbs, &QToolButton::clicked, this, &SubAbsToolBar::onClicked);
	connect(this, &SubAbsToolBar::sigIsFill, this, &SubAbsToolBar::onToggleOnlyOneBtn);
}

void SubAbsToolBar::initAbsTb(QString tbPath, QString tbToolTip)
{
    if (tbPath.isEmpty() && !m_tbAbs)
        return;

    m_tbAbs->setIcon(QIcon(tbPath));
    m_tbAbs->setToolTip(tbToolTip);
}

void SubAbsToolBar::initFilledAbsTb(QString tbPath, QString tbToolTip)
{
    if (tbPath.isEmpty() && !m_tbFilledAbs)
        return;

    m_tbFilledAbs->setIcon(QIcon(tbPath));
    m_tbFilledAbs->setToolTip(tbToolTip);
}

// 确保只有一个被处于被点击状态
void SubAbsToolBar::onToggleOnlyOneBtn(bool bFill)
{
	if (bFill) {
		m_tbAbs->setChecked(false);
		m_tbFilledAbs->setChecked(true);
	} else {
		m_tbAbs->setChecked(true);
		m_tbFilledAbs->setChecked(false);
	}
}

void SubAbsToolBar::onClicked(bool checked)
{
    Q_UNUSED(checked);

    if (sender() == m_tbAbs)
        emit sigIsFill(false);
    else if (sender() == m_tbFilledAbs)
        emit sigIsFill(true);
}

void SubAbsToolBar::addWidget(QWidget *widget)
{
    QHBoxLayout* hLayout = findChild<QHBoxLayout *>("hLayout");
    if (!hLayout)
        return;

    hLayout->addWidget(widget);
}

void SubAbsToolBar::addStretch(int stretch)
{
    QHBoxLayout* hLayout = findChild<QHBoxLayout *>("hLayout");
    if (!hLayout)
        return;

    hLayout->addStretch(stretch);
}

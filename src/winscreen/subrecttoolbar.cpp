//
// Created by xmuli on 2021/12/04.
//
#include "subrecttoolbar.h"
#include "screenshot.h"
#include "../xglobal.h"
#include "../widget/xverticalline.h"
#include <QBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSpinBox>

SubRectToolBar::SubRectToolBar(QWidget *parent) : QWidget(parent)
	, m_tbRect(new QToolButton(this))
	, m_tbFilledRect(new QToolButton(this))
    , m_spBorder(new QSpinBox(this))
{
    initUI();

    connect(m_tbRect, &QToolButton::clicked, this, &SubRectToolBar::onClicked);
    connect(m_tbFilledRect, &QToolButton::clicked, this, &SubRectToolBar::onClicked);
}

void SubRectToolBar::initUI()
{
    m_tbRect->setIcon(QIcon(":/resources/icons/normal/rectangle.svg"));
    m_tbRect->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * ScreenShot::getScale());
    m_tbRect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbRect->setAutoRaise(true);
    m_tbRect->setToolTip(tr("Rectangle"));
    m_tbRect->setChecked(true);

    m_tbFilledRect->setIcon(QIcon(":/resources/icons/normal/fill-rect.svg"));
    m_tbFilledRect->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * ScreenShot::getScale());
    m_tbFilledRect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbFilledRect->setAutoRaise(true);
    m_tbFilledRect->setToolTip(tr("Filled Rectangle"));
    m_tbFilledRect->setChecked(true);

    setContentsMargins(0, 0, 0, 0);
    const int margin = 0;
    QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setObjectName("hLayout");
    hLayout->setContentsMargins(margin, margin, margin, margin);
    hLayout->setSpacing(0);
    hLayout->addWidget(m_tbRect);
    hLayout->addWidget(m_tbFilledRect);

    const int spacing = 4; // 分割线周围的间距
    hLayout->addSpacing(spacing);
    hLayout->addWidget(new XVerticalLine(m_tbRect->iconSize().height(), this));
    hLayout->addSpacing(spacing);

    QLabel* labBorder = new QLabel(tr("Border"), this);
    labBorder->setFixedHeight(m_tbRect->iconSize().height());
    hLayout->addWidget(labBorder);
    m_spBorder->setValue(2);
    m_spBorder->setMinimum(0);
	hLayout->addWidget(m_spBorder);

    hLayout->addStretch(0);
    setLayout(hLayout);
}

void SubRectToolBar::onClicked(bool checked)
{
    Q_UNUSED(checked);

    if (sender() == m_tbRect)
        emit sigIsFill(false);
    else if (sender() == m_tbFilledRect)
        emit sigIsFill(true);
}

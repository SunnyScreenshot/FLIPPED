//
// Created by xmuli on 2021/12/14.
//
#include "subellipsetoolbar.h"
#include "screenshot.h"
#include "../xglobal.h"
#include "../widget/xverticalline.h"
#include <QBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSpinBox>

SubEllipseToolBar::SubEllipseToolBar(QWidget *parent)
    : QWidget(parent)
    , m_tbEllipse(new QToolButton(this))
    , m_tbFilledEllipse(new QToolButton(this))
    , m_spBorder(new QSpinBox(this))
{
    initUI();

    connect(m_tbEllipse, &QToolButton::clicked, this, &SubEllipseToolBar::onClicked);
    connect(m_tbFilledEllipse, &QToolButton::clicked, this, &SubEllipseToolBar::onClicked);
}

void SubEllipseToolBar::initUI()
{
    m_tbEllipse->setIcon(QIcon(":/resources/icons/normal/ellipse.svg"));
    m_tbEllipse->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * ScreenShot::getScale());
    m_tbEllipse->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbEllipse->setAutoRaise(true);
    m_tbEllipse->setToolTip(tr("Ellipse"));
    m_tbEllipse->setChecked(false);

    m_tbFilledEllipse->setIcon(QIcon(":/resources/icons/normal/fill-ellipse.svg"));
    m_tbFilledEllipse->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * ScreenShot::getScale());
    m_tbFilledEllipse->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbFilledEllipse->setAutoRaise(true);
    m_tbFilledEllipse->setToolTip(tr("Filled Ellipse"));
    m_tbFilledEllipse->setChecked(false);

    setContentsMargins(0, 0, 0, 0);
    const int margin = 0;
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setObjectName("hLayout");
    hLayout->setContentsMargins(margin, margin, margin, margin);
    hLayout->setSpacing(0);
    hLayout->addWidget(m_tbEllipse);
    hLayout->addWidget(m_tbFilledEllipse);

    const int spacing = 4; // 分割线周围的间距
    hLayout->addSpacing(spacing);
    hLayout->addWidget(new XVerticalLine(m_tbEllipse->iconSize().height(), this));
    hLayout->addSpacing(spacing);

    QLabel* labBorder = new QLabel(tr("Border"), this);
    labBorder->setFixedHeight(m_tbEllipse->iconSize().height());
    hLayout->addWidget(labBorder);
    m_spBorder->setValue(2);
    m_spBorder->setMinimum(0);
    hLayout->addWidget(m_spBorder);
    hLayout->addStretch(0);

    setLayout(hLayout);
}

void SubEllipseToolBar::onClicked(bool checked)
{
    Q_UNUSED(checked);

    if (sender() == m_tbEllipse)
        emit sigIsFill(false);
    else if (sender() == m_tbFilledEllipse)
        emit sigIsFill(true);
}

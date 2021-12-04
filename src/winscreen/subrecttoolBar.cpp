//
// Created by xmuli on 2021/12/04.
//
#include "subrecttoolbar.h"
#include "winfullscreen.h"

#include <QBoxLayout>
#include "../widget/xverticalline.h"

SubRectToolBar::SubRectToolBar(QWidget *parent) : QWidget(parent)
  , m_tbRect(new QToolButton(this))
  , m_tbFilledRect(new QToolButton(this))
{
    init();
}

void SubRectToolBar::init()
{
    m_tbRect->setIcon(QIcon(":/resources/icons/normal/rectangle.svg"));
    m_tbRect->setIconSize(QSize(16, 16) * WinFullScreen::getScale());
    m_tbRect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbRect->setAutoRaise(true);
    m_tbRect->setToolTip(tr("Rectangle"));
    m_tbRect->setChecked(false);

    m_tbFilledRect->setIcon(QIcon(":/resources/icons/normal/fill-rect.svg"));
    m_tbFilledRect->setIconSize(QSize(16, 16) * WinFullScreen::getScale());
    m_tbFilledRect->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_tbFilledRect->setAutoRaise(true);
    m_tbFilledRect->setToolTip(tr("Filled Rectangle"));
    m_tbFilledRect->setChecked(false);

    setContentsMargins(0, 0, 0, 0);
    const int margin = 0;
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(margin, margin, margin, margin);
    hLayout->setSpacing(0);
    hLayout->addWidget(m_tbRect);
    hLayout->addWidget(m_tbFilledRect);

    XVerticalLine* vLine = new XVerticalLine(this);
    vLine->setFixedHeight(m_tbRect->height() - 10);
    const int spacing = 4; // 分割线周围的间距
    hLayout->addSpacing(spacing);
    hLayout->addWidget(vLine);
    hLayout->addSpacing(spacing);
    hLayout->addStretch(0);

    setLayout(hLayout);
}

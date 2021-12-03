//
// Created by xmuli on 2021/12/04.
//
#include "windrawrect.h"
#include "winfullscreen.h"

#include <QBoxLayout>

WinDrawRect::WinDrawRect(QWidget *parent) : QWidget(parent)
  , m_tbRect(new QToolButton(this))
  , m_tbFilledRect(new QToolButton(this))
{
    init();
}

void WinDrawRect::init()
{
    m_tbRect->setIcon(QIcon(":/resources/icons/normal/rectangle.svg"));
    m_tbRect->setIconSize(QSize(16, 16) * WinFullScreen::getScale());
    m_tbRect->setToolTip(tr("Rectangle"));
    m_tbRect->setChecked(false);

    m_tbFilledRect->setIcon(QIcon(":/resources/icons/normal/fill-rect.svg"));
    m_tbFilledRect->setIconSize(QSize(16, 16) * WinFullScreen::getScale());
    m_tbFilledRect->setToolTip(tr("Filled Rectangle"));
    m_tbFilledRect->setChecked(false);

    //
    setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout();
    const int margin = 0;
    hLayout->setContentsMargins(margin, margin, margin, margin);
    hLayout->setSpacing(0);
    hLayout->addWidget(m_tbRect);
    hLayout->addWidget(m_tbFilledRect);

    setLayout(hLayout);
}

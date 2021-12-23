//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/23.
//
#include "sublinetoolbar.h"
#include <QBoxLayout>
#include <QLabel>

SubLineToolBar::SubLineToolBar(QWidget *parent)
    : QWidget(parent)
    , m_spBorder(new QSpinBox(this))
{
    initUI();
}

void SubLineToolBar::initUI()
{
    setContentsMargins(0, 0, 0, 0);
    const int margin = 0;
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setObjectName("hLayout");
    hLayout->setContentsMargins(margin, margin, margin, margin);
    hLayout->setSpacing(0);
}

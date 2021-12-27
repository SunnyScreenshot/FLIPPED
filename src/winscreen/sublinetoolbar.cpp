//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/23.
//
#include "sublinetoolbar.h"
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QIcon>

SubLineToolBar::SubLineToolBar(QWidget *parent)
    : QWidget(parent)
    , m_cbEnds(new QComboBox(this))
    , m_cbDashes(new QComboBox(this))
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
    hLayout->setSpacing(4);

    QStringList listEnds;
    listEnds << "SolidLine" << "One-wayArrow" << "Two-wayArrow";
    for (QString name : listEnds) {
        QString path = QString(":/resources/icons/line/") + name + ".svg";
        m_cbEnds->addItem(QIcon(path), "");
    }
    hLayout->addWidget(new QLabel(tr("Ends"), this), Qt::AlignLeft);
    hLayout->addWidget(m_cbEnds, 2);

    QStringList listDashes;
    listDashes << "DashLine" << "DotLine";
    for (QString name : listDashes) {
        QString path = QString(":/resources/icons/line/") + name + ".svg";
        m_cbDashes->addItem(QIcon(path), "");
    }
    hLayout->addWidget(new QLabel(tr("Dashes"), this), Qt::AlignLeft);
    hLayout->addWidget(m_cbDashes, 2);

    hLayout->addStretch(2);
    setLayout(hLayout);
}

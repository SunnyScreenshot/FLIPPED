/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.03
 * Description: 偏好设置界面的 Main UI
 ******************************************************************/
#include "preference.h"
#include "../xglobal.h"
#include "../widget/xhorizontalline.h"
#include "../screen/drawhelper.h"
#include <QPushButton>
#include <QTabWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFile>
#include <QByteArray>
#include <QDomElement>
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QToolButton>

Preference::Preference(QWidget *parent)
    : QWidget(parent)
    , m_scale(1)
    , m_resetBtn(nullptr)
{
    initUI();
}

void Preference::initUI()
{
    // main layout
    setContentsMargins(PRE_MARGIN_HOR, PRE_MARGIN_VER_TOP, PRE_MARGIN_HOR, PRE_MARGIN_VER_BOTTOM);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setMargin(0);
    setLayout(vLayout);


    // tabPages
    QTabWidget* tabPages = new QTabWidget(this);
    tabPages->setMovable(true);
    tabPages->setObjectName("tabPages");
    tabPages->addTab(generalPage(), tr("General"));
    tabPages->addTab(shortcutPage(), tr("Shortcut"));
    tabPages->addTab(aboutPage(), tr("About"));



//    QHBoxLayout* layRestBtn = new QHBoxLayout(this);
//    layRestBtn->setMargin(0);
//    layRestBtn->addStretch(7);
//    layRestBtn->addWidget(m_resetBtn, 1, Qt::AlignRight);

    vLayout->addWidget(tabPages);
}

QWidget *Preference::generalPage()
{
//    QWidget* page = new QWidget(this);


    return new QPushButton("generalPage");
}

QWidget *Preference::shortcutPage()
{
    return new QPushButton("shortcutPage");
}

QWidget *Preference::aboutPage()
{
    QWidget* page = new QWidget(nullptr);
    QVBoxLayout *vLay = new QVBoxLayout(page);
    vLay->setMargin(0);
    vLay->addSpacing(30 * m_scale);

    QSize size(48 * 1.2, 48 * 1.2);
    QToolButton* tb = new QToolButton();
    const double scal = 2; //XHelp::getScale();
    tb->setIconSize(size * scal);
    tb->setFixedSize(size * scal);
    tb->setIcon(QIcon(":/resources/logo.svg"));
    tb->setAutoRaise(true);
    tb->setCheckable(false);
    tb->setChecked(false);
    tb->setDisabled(true);

    vLay->addWidget(tb, 4, Qt::AlignCenter);
//    vLay->addSpacing(10 * m_scale);

//    ADD_DEFINITIONS(-DPROJECT_NAME=\"${PROJECT_NAME}\")
//    ADD_DEFINITIONS(-DPROJECT_VERSION=\"${PROJECT_VERSION}\")

    QFont font;
    QLabel* project = new QLabel(tr("%1").arg(PROJECT_NAME));
    font.setPointSizeF(14);
    font.setBold(true);
    project->setFont(font);
    QLabel* buildTime = new QLabel(tr("%1-Beta x64 (2022.07.03)").arg(PROJECT_VERSION));
    font.setPointSizeF(9);
    font.setBold(false);
    buildTime->setFont(font);
    QLabel* detail = new QLabel(tr("PicShot is a cross-platform screenshot tool."));
    font.setPointSizeF(10);
    detail->setFont(font);

    vLay->addWidget(project, 1, Qt::AlignCenter);
    vLay->addSpacing(8 * m_scale);
    vLay->addWidget(buildTime, 1, Qt::AlignCenter);
    vLay->addSpacing(14 * m_scale);
    vLay->addWidget(detail, 1, Qt::AlignCenter);
    vLay->addSpacing(22 * m_scale);

    QLabel* authorL = new QLabel(tr("Author:"));
    font.setPointSizeF(10);
    detail->setFont(font);
    QLabel* authorR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli> XMuli</a>"));
    authorR->setOpenExternalLinks(true);
    QLabel* copyrightL = new QLabel(tr("Copyright ©"));
    QLabel* copyrightR = new QLabel(tr("2021-2022 XMuli"));

    QGridLayout* gird1 = new QGridLayout();
    gird1->setMargin(0);
    gird1->setSpacing(GL_SPACING_VER);
    gird1->setColumnStretch (0, 7);
    gird1->setColumnStretch (1, 9);
    gird1->addWidget(authorL, 0, 0, Qt::AlignRight);
    gird1->addWidget(authorR, 0, 1, Qt::AlignLeft);
    gird1->addWidget(copyrightL, 1, 0, Qt::AlignRight);
    gird1->addWidget(copyrightR, 1, 1, Qt::AlignLeft);
    vLay->addLayout(gird1);

    vLay->addSpacing(30 * m_scale);
    vLay->addWidget(new XHorizontalLine(contentsRect().width()), 1, Qt::AlignCenter);
    vLay->addSpacing(20 * m_scale);


    QLabel* ackmtR = new QLabel(tr("Acknowledgements"));
    font.setPointSizeF(11);
    font.setBold(true);
    ackmtR->setFont(font);
    QLabel* ghR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli/PicShot>PicShot</a>"));
    ghR->setOpenExternalLinks(true);
    QLabel* blogR = new QLabel(tr("<a style='color: green;' href=https://ifmet.cn>ifmet.cn</a>"));
    blogR->setOpenExternalLinks(true);

    QGridLayout* gird2 = new QGridLayout();
    gird2->setMargin(0);
    gird2->setSpacing(GL_SPACING_VER);
    gird2->setColumnStretch (0, 7);
    gird2->setColumnStretch (1, 9);
    gird2->addWidget(ackmtR, 0, 0, 1, 2, Qt::AlignCenter);
    gird2->addWidget(new QLabel("GitHub:"), 1, 0, Qt::AlignRight);
    gird2->addWidget(ghR, 1, 1, Qt::AlignLeft);
    gird2->addWidget(new QLabel(" "), 2, 0, Qt::AlignRight);
    gird2->addWidget(blogR, 2, 1, Qt::AlignLeft);
    gird2->addWidget(new QLabel("UI:"), 3, 0, Qt::AlignRight);
    gird2->addWidget(new QLabel("iCancely"), 3, 1, Qt::AlignLeft);
    gird2->addWidget(new QLabel("Contributor:"), 4, 0, Qt::AlignRight);
    gird2->addWidget(new QLabel("Tom Jerry"), 4, 1, Qt::AlignLeft);
    gird2->addWidget(new QLabel("Translators:"), 5, 0, Qt::AlignRight);
    gird2->addWidget(new QLabel("Tom Jerry"), 5, 1, Qt::AlignLeft);


    vLay->addLayout(gird2, 6);
    vLay->addSpacing(35 * m_scale);
//    vLay->addStretch(0);  // 设置策略，更改为弹簧效果

    return page;
}

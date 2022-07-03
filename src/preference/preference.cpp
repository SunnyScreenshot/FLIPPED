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
#include "../widget/xkeysequenceedit.h"
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
#include <QDateTime>

// test
#include <QDebug>

Preference::Preference(QWidget *parent)
    : QWidget(parent)
    , m_scale(1)
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
    tabPages->addTab(tabGeneral(), tr("General"));
    tabPages->addTab(tabHotkeys(), tr("HotKeys"));
    tabPages->addTab(tabAbout(), tr("About"));

    vLayout->addWidget(tabPages);
}

QWidget *Preference::tabGeneral()
{
//    QWidget* page = new QWidget(this);
    return new QPushButton("generalPage");
}

QWidget *Preference::tabHotkeys()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(20 * m_scale, 0, 20 * m_scale, 0);
    vLay->addSpacing(20 * m_scale);

    // 快捷键框
    const int keyEditMinWidget = 200;
    auto hotSrn1 = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_Shift + Qt::Key_Y));
    hotSrn1->setMinimumWidth(keyEditMinWidget);

    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(TSG_SPACING_VER);
    grid->setColumnStretch (0, 3);
    grid->setColumnStretch (1, 7);
    grid->addWidget(new QLabel(tr("Active Window:")), 0, 0, Qt::AlignRight);
    grid->addWidget(hotSrn1, 0, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Window / Object")), 1, 0, Qt::AlignRight);
    grid->addWidget(new QLabel(tr("hotkey")), 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Delay Capture:")), 2, 0, Qt::AlignRight);
    grid->addWidget(new QLabel(tr("hotkey")), 2, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Full Screen:")), 3, 0, Qt::AlignRight);
    grid->addWidget(new QLabel(tr("hotkey")), 3, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Fixd-Size Region:")), 4, 0, Qt::AlignRight);
    grid->addWidget(new QLabel(tr("hotkey")), 4, 1, Qt::AlignLeft);

    grid->addWidget(new XHorizontalLine(contentsRect().width()), 5, 0, 1, grid->columnCount(), Qt::AlignCenter);

    grid->addWidget(new QLabel(tr("Paste:")), 6, 0, Qt::AlignRight);
    grid->addWidget(new QLabel(tr("hotkey")), 6, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Hide/Show all images:")), 7, 0, Qt::AlignRight);
    grid->addWidget(new QLabel(tr("hotkey")), 7, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Switch to another image group:")), 8, 0, Qt::AlignRight);
    grid->addWidget(new QLabel(tr("hotkey")), 8, 1, Qt::AlignLeft);

    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(4);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->setContentsMargins(20, 0, 20, 20);
    hLay->addSpacing(0);
    hLay->addStretch(7);
    hLay->addWidget(new QPushButton(tr("Reset")), 1, Qt::AlignRight);
    vLay->addLayout(hLay, 1);

    return page;
}

QWidget *Preference::tabAbout()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
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

    vLay->addWidget(tb, 1, Qt::AlignCenter);
    QFont font;
    QLabel* project = new QLabel(tr("%1").arg(_PROJECT_NAME));
    font.setPointSizeF(14);
    font.setBold(true);
    project->setFont(font);
    QLabel* buildTime = new QLabel(tr("%1-Beta %2 (%3)")
                                   .arg(_PROJECT_VERSION)
                                   .arg(_BIT_ARCH)
                                   .arg(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss")));  // hh:mm:ss
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

    QGridLayout* grid1 = new QGridLayout();
    grid1->setMargin(0);
    grid1->setSpacing(TAG_SPACING_VER);
    grid1->setColumnStretch (0, 7);
    grid1->setColumnStretch (1, 9);
    grid1->addWidget(authorL, 0, 0, Qt::AlignRight);
    grid1->addWidget(authorR, 0, 1, Qt::AlignLeft);
    grid1->addWidget(copyrightL, 1, 0, Qt::AlignRight);
    grid1->addWidget(copyrightR, 1, 1, Qt::AlignLeft);
    vLay->addLayout(grid1, grid1->rowCount());

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

    QGridLayout* grid2 = new QGridLayout();
    grid2->setMargin(0);
    grid2->setSpacing(TAG_SPACING_VER);
    grid2->setColumnStretch (0, 7);
    grid2->setColumnStretch (1, 9);
    grid2->addWidget(ackmtR, 0, 0, 1, 2, Qt::AlignCenter);
    grid2->addWidget(new QLabel("GitHub:"), 1, 0, Qt::AlignRight);
    grid2->addWidget(ghR, 1, 1, Qt::AlignLeft);
    grid2->addWidget(new QLabel(" "), 2, 0, Qt::AlignRight);
    grid2->addWidget(blogR, 2, 1, Qt::AlignLeft);
    grid2->addWidget(new QLabel("UI:"), 3, 0, Qt::AlignRight);
    grid2->addWidget(new QLabel("iCancely"), 3, 1, Qt::AlignLeft);
    grid2->addWidget(new QLabel("Contributor:"), 4, 0, Qt::AlignRight);
    grid2->addWidget(new QLabel("Tom Jerry"), 4, 1, Qt::AlignLeft);
    grid2->addWidget(new QLabel("Translators:"), 5, 0, Qt::AlignRight);
    grid2->addWidget(new QLabel("Tom Jerry"), 5, 1, Qt::AlignLeft);
    vLay->addLayout(grid2, grid2->rowCount());

    vLay->addStretch(6);
    vLay->addSpacing(35 * m_scale);

    return page;
}

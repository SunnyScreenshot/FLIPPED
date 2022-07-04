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
#include <QKeySequence>

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
    //QKeySequence(QKeySequence::Print);
    //QKeySequence(tr("Ctrl+P"));
    //QKeySequence(tr("Ctrl+p"));
    //QKeySequence(Qt::CTRL + Qt::Key_P);

    const int keyEditMinWidget = 240;  // h:hotkey s: srn  --> hotSrn 缩写
    auto hAW = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_A));  // 出现乱码是因为使用了 QKeySequence(Qt::CTRL + Qt::Key_Shift + Qt::Key_Y)
    hAW->setMinimumWidth(keyEditMinWidget);
    auto hWO = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W));
    hWO->setMinimumWidth(keyEditMinWidget);
    auto hDC = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_L));
    hDC->setMinimumWidth(keyEditMinWidget);
    auto hFS = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    hFS->setMinimumWidth(keyEditMinWidget);
    auto hFR = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));
    hFR->setMinimumWidth(keyEditMinWidget);

    // 贴图相关
    auto hP = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));
    hP->setMinimumWidth(keyEditMinWidget);
    auto hHSAI = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_H));
    hHSAI->setMinimumWidth(keyEditMinWidget);
    auto hSCG = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_X));
    hSCG->setMinimumWidth(keyEditMinWidget);

    int i = 0;
    int j = 0;
    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(TSG_SPACING_VER);
    grid->setColumnStretch(0, 7);
    grid->setColumnStretch(1, 9);
    grid->addWidget(new QLabel(tr("Active Window:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(hAW, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Window / Object")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(hWO, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Delay Capture:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(hDC, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Full Screen:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(hFS, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Fixd-Size Region:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(hFR, i++, j + 1, 1, 1, Qt::AlignLeft);

    grid->addWidget(new XHorizontalLine(contentsRect().width()), i++, j, 1, grid->columnCount(), Qt::AlignCenter);

    grid->addWidget(new QLabel(tr("Paste:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(hP, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Hide/Show all images:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(hHSAI, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Switch curent group:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(hSCG,i++, j + 1, 1, 1, Qt::AlignLeft);

    qDebug() << "tabHotkeys:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->setContentsMargins(20, 0, 20, 0);
    hLay->addSpacing(0);
    hLay->addStretch(7);
    hLay->addWidget(new QPushButton(tr("Reset")), 1, Qt::AlignRight);
    vLay->addLayout(hLay, 1);
    vLay->addSpacing(20 * m_scale);

    return page;
}

QWidget *Preference::tabAbout()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(20 * m_scale, 0, 20 * m_scale, 0);
    vLay->addSpacing(30 * m_scale);

    int i = 0;
    int j = 0;
    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(TAG_SPACING_VER);
    grid->setColumnStretch(0, 7);
    grid->setColumnStretch(1, 9);

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
    grid->addWidget(tb, i++, j, 2, 2, Qt::AlignCenter);  // 0, 0

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

    QLabel* authorL = new QLabel(tr("Author:"));
    font.setPointSizeF(10);
    detail->setFont(font);
    QLabel* authorR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli> XMuli</a>"));
    authorR->setOpenExternalLinks(true);
    QLabel* copyrightL = new QLabel(tr("Copyright ©"));
    QLabel* copyrightR = new QLabel(tr("2021-2022 XMuli"));

    i = 2;
    grid->addWidget(project, i++, j, 1, 2, Qt::AlignCenter);
    grid->addWidget(buildTime, i++, j, 1, 2, Qt::AlignCenter);  
    grid->addWidget(detail, i++, j, 1, 2, Qt::AlignCenter);

    grid->addWidget(authorL, i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(authorR, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(copyrightL, i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(copyrightR, i++, j + 1, 1, 1, Qt::AlignLeft);

    grid->addWidget(new XHorizontalLine(contentsRect().width()), i++, j, 1, grid->columnCount(), Qt::AlignCenter);

    QLabel* ackmtR = new QLabel(tr("Acknowledgements"));
    font.setPointSizeF(11);
    font.setBold(true);
    ackmtR->setFont(font);
    QLabel* ghR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli/PicShot>PicShot</a>"));
    ghR->setOpenExternalLinks(true);
    QLabel* blogR = new QLabel(tr("<a style='color: green;' href=https://ifmet.cn>ifmet.cn</a>"));
    blogR->setOpenExternalLinks(true);

    grid->addWidget(ackmtR, i, j, 1, 2, Qt::AlignCenter);
    grid->addWidget(new QLabel("GitHub:"), ++i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(ghR, i, j + 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(" "), ++i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(blogR, i, j + 1, Qt::AlignLeft);
    grid->addWidget(new QLabel("UI:"), ++i, j, Qt::AlignRight);
    grid->addWidget(new QLabel("iCancely"), i, j + 1, Qt::AlignLeft);
    grid->addWidget(new QLabel("Contributor:"), ++i, j, Qt::AlignRight);
    grid->addWidget(new QLabel("Tom Jerry"), i, j + 1, Qt::AlignLeft);
    grid->addWidget(new QLabel("Translators:"), ++i, j, Qt::AlignRight);
    grid->addWidget(new QLabel("Tom Jerry"), i, j + 1, Qt::AlignLeft);

    qDebug() << "tabAbout:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addSpacing(35 * m_scale);

    return page;
}

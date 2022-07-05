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

#include <QHotkey>

// test
#include <QDebug>
#include <QVector>

Preference::Preference(QWidget *parent)
    : QWidget(parent)
    , m_scale(XHelp::getScale())
{
    initUI();
}

void Preference::initUI()
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    // main layout
    //setContentsMargins(0, 0, 0, 0);
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
    return new QPushButton("generalPage");
}

QWidget *Preference::tabHotkeys()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(THG_MARGIN_HOR * m_scale, 0, THG_MARGIN_HOR * m_scale, 0);
    vLay->addSpacing(THV_SPACING_VER * m_scale);

    // 快捷键框
    //QKeySequence(QKeySequence::Print);
    //QKeySequence(tr("Ctrl+P"));
    //QKeySequence(tr("Ctrl+p"));
    //QKeySequence(Qt::CTRL + Qt::Key_P);

    QVector<XKeySequenceEdit*> vHotkey;
    QMap<QString, int> map = { {"h0_AW", Qt::CTRL + Qt::SHIFT + Qt::Key_Y},     // 截图相关
                               {"h1_WO", Qt::CTRL + Qt::SHIFT + Qt::Key_W},
                               {"h2_DC", Qt::CTRL + Qt::SHIFT + Qt::Key_L},
                               {"h3_FS", Qt::CTRL + Qt::SHIFT + Qt::Key_S},
                               {"h4_FSR", Qt::CTRL + Qt::SHIFT + Qt::Key_F},
                               {"h5_P", Qt::CTRL + Qt::SHIFT + Qt::Key_T},      // 贴图相关
                               {"h6_HSAI", Qt::CTRL + Qt::SHIFT + Qt::Key_H},
                               {"h7_SCG", Qt::CTRL + Qt::SHIFT + Qt::Key_X}};

    
    for (auto it = map.begin(); it != map.end(); ++it) {
        auto hotKey = new XKeySequenceEdit(QKeySequence(it.value()));           // 出现乱码是因为使用了 QKeySequence(Qt::CTRL + Qt::Key_Shift + Qt::Key_Y)
        hotKey->setObjectName(it.key());
        hotKey->setMinimumWidth(106 * m_scale);                                           // 估的一个数值
        vHotkey.push_back(hotKey);
        //connect(hotKey, &XKeySequenceEdit::keySequenceChanged, )
    }

    int id = 0;
    int i = 0;
    int j = 0;
    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(THG_SPACING_VER);
    grid->setColumnStretch(0, 7);
    grid->setColumnStretch(1, 9);
    grid->addWidget(new QLabel(tr("Active Window:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(vHotkey[id++], i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Window / Object")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(vHotkey[id++], i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Delay Capture:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(vHotkey[id++], i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Full Screen:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(vHotkey[id++], i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Fixd-Size Region:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(vHotkey[id++], i++, j + 1, 1, 1, Qt::AlignLeft);

    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - THG_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);

    grid->addWidget(new QLabel(tr("Paste:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(vHotkey[id++], i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Hide/Show all images:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(vHotkey[id++], i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(tr("Switch current group:")), i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(vHotkey[id++],i++, j + 1, 1, 1, Qt::AlignLeft);


    auto lab = new QLabel(tr("Switch current group:"));
    qDebug() << "tabHotkeys:grid->rowCount():" << grid->rowCount() << "   font->" << lab->font().pointSizeF();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->setContentsMargins(0, 0, 0, 0);
    hLay->addSpacing(0);
    hLay->addStretch(7);
    hLay->addWidget(new QPushButton(tr("Reset")), 1, Qt::AlignRight);
    hLay->addSpacing(THG_MARGIN_HOR * m_scale);  // 设计图距离
    vLay->addLayout(hLay, 1);
    vLay->addSpacing(THG_MARGIN_VER * m_scale);

    return page;
}

QWidget *Preference::tabAbout()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(TAG_MARGIN_HOR * m_scale, 0, TAG_MARGIN_HOR * m_scale, 0);
    vLay->addSpacing(TAV_SPACING_VER * m_scale);

    int i = 0;
    int j = 0;
    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(TAG_SPACING_VER);
    grid->setColumnStretch(0, 7);
    grid->setColumnStretch(1, 9);

    QSize size(34, 34);
    QToolButton* tb = new QToolButton();
    tb->setIconSize(size * m_scale);
    tb->setFixedSize(size * m_scale);
    tb->setIcon(QIcon(":/resources/logo.svg"));
    tb->setAutoRaise(true);
    tb->setCheckable(false);
    tb->setChecked(false);
    tb->setDisabled(true);
    grid->addWidget(tb, i++, j, 2, 2, Qt::AlignCenter);  // 0, 0

    QFont font;
    QLabel* project = new QLabel(tr("%1").arg(_PROJECT_NAME));
    font.setPointSizeF(11);
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
    detail->setFont(font);

    QLabel* authorL = new QLabel(tr("Author:"));
    authorL->setFont(font);
    QLabel* authorR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli> XMuli</a>"));
    authorR->setOpenExternalLinks(true);
    authorR->setFont(font);
    QLabel* copyrightL = new QLabel(tr("Copyright ©"));
    copyrightL->setFont(font);
    QLabel* copyrightR = new QLabel(tr("2021-2022 XMuli"));
    copyrightR->setFont(font);

    i = 2;
    grid->addWidget(project, i++, j, 1, 2, Qt::AlignCenter);
    grid->addWidget(buildTime, i++, j, 1, 2, Qt::AlignCenter);  
    grid->addWidget(detail, i++, j, 1, 2, Qt::AlignCenter);

    grid->addWidget(authorL, i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(authorR, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addWidget(copyrightL, i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(copyrightR, i++, j + 1, 1, 1, Qt::AlignLeft);

    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TAG_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);

    QLabel* ackmtR = new QLabel(tr("Acknowledgements"));
    font.setPointSizeF(9);
    font.setBold(true);
    ackmtR->setFont(font);

    QLabel* ghL =new QLabel("GitHub:");
    font.setPointSizeF(8);
    ghL->setFont(font);
    QLabel* ghR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli/PicShot>PicShot</a>"));
    ghR->setOpenExternalLinks(true);
    ghR->setFont(font);
    QLabel* blogR = new QLabel(tr("<a style='color: green;' href=https://ifmet.cn>ifmet.cn</a>"));
    blogR->setOpenExternalLinks(true);
    font.setPointSizeF(8);
    blogR->setFont(font);

    QLabel* UIL = new QLabel("UI:");
    UIL->setFont(font);
    QLabel* UIR = new QLabel("iCancely");
    UIR->setFont(font);
    QLabel* ctL = new QLabel("Contributor:");
    ctL->setFont(font);
    QLabel* ctR = new QLabel("Tom Jerry");
    ctR->setFont(font);
    QLabel* tsL = new QLabel("Translators:");
    tsL->setFont(font);
    QLabel* tsR = new QLabel("Tom Jerry");
    tsR->setFont(font);

    grid->addWidget(ackmtR, i, j, 1, 2, Qt::AlignCenter);
    grid->addWidget(ghL, ++i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(ghR, i, j + 1, Qt::AlignLeft);
    grid->addWidget(new QLabel(" "), ++i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(blogR, i, j + 1, Qt::AlignLeft);
    grid->addWidget(UIL, ++i, j, Qt::AlignRight);
    grid->addWidget(UIR, i, j + 1, Qt::AlignLeft);
    grid->addWidget(ctL, ++i, j, Qt::AlignRight);
    grid->addWidget(ctR, i, j + 1, Qt::AlignLeft);
    grid->addWidget(tsL, ++i, j, Qt::AlignRight);
    grid->addWidget(tsR, i, j + 1, Qt::AlignLeft);

    qDebug() << "tabAbout:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addSpacing(TAG_MARGIN_VER* m_scale);

    return page;
}

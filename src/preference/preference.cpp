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
#include "../tool/base/colorparabar.h"
#include "../screen/drawhelper.h"
#include "../screen/tray.h"
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
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>

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
    setContentsMargins(PRE_MARGIN_HOR, PRE_MARGIN_VER, PRE_MARGIN_HOR, PRE_MARGIN_VER);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    setLayout(vLayout);

    // tabPages
    QTabWidget* tabPages = new QTabWidget(this);
    tabPages->setMovable(true);
    tabPages->setObjectName("tabPages");
    tabPages->addTab(tabGeneral(), tr("General"));
    tabPages->addTab(tabInterface(), tr("Interface"));
    tabPages->addTab(tabOutput(), tr("Output"));
    tabPages->addTab(tabPin(), tr("Pin"));
    tabPages->addTab(tabHotkeys(), tr("HotKeys"));
    tabPages->addTab(tabAbout(), tr("About"));

    vLayout->addWidget(tabPages);
}

QHBoxLayout* Preference::creatResetBtn()
{
    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->setContentsMargins(0, 0, 0, 0);
    hLay->addSpacing(0);
    hLay->addStretch(7);
    hLay->addWidget(new QPushButton(tr("Reset")), 1, Qt::AlignRight);
    return hLay;
}

QWidget *Preference::tabGeneral()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(TGV_MARGIN_HOR, TGV_MARGIN_VER_TOP, TGV_MARGIN_HOR, TGV_MARGIN_VER_BOTTOM);

    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setVerticalSpacing(TGG_SPACING_VER);
    grid->setHorizontalSpacing(TGG_SPACING_HOR);
    grid->setColumnStretch(0, 3);
    grid->setColumnStretch(1, 5);

    QFont font;
    font.setPointSizeF(9);
    QLabel* lanuage = new QLabel(tr("Lanuage:"));
    QLabel* launch = new QLabel(tr("Launch:"));
    QLabel* logLevel = new QLabel(tr("Log Level:"));
    QLabel* update = new QLabel(tr("Update:"));
    lanuage->setFont(font);
    launch->setFont(font);
    logLevel->setFont(font);
    update->setFont(font);

    int i = 0;
    int j = 0;
    grid->addWidget(lanuage, i, j, Qt::AlignRight);
    grid->addWidget(new QComboBox(this), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(launch, i, j, Qt::AlignRight);
    grid->addWidget(new QCheckBox(tr("Run on system startup")), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(new QCheckBox(tr("As administrator")), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(logLevel, i, j, Qt::AlignRight);
    grid->addWidget(new QComboBox(this), i++, j + 1, Qt::AlignLeft);

    grid->addItem(new QSpacerItem(0, 38, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j); // 添加弹簧
    grid->addWidget(update, i, j, Qt::AlignRight);
    QHBoxLayout* tHLay = new QHBoxLayout();
    tHLay->setContentsMargins(0, 0, 0, 0);
    tHLay->addSpacing(0);
    tHLay->addWidget(new QCheckBox(tr("Automatic check")), 1, Qt::AlignLeft);
    tHLay->addSpacing(20);
    tHLay->addWidget(new QPushButton(tr("Update")), 1, Qt::AlignLeft);
    tHLay->addStretch(8);
    grid->addLayout(tHLay, i, j + 1, Qt::AlignLeft);

    qDebug() << "tabGeneral:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);

    vLay->addLayout(creatResetBtn(), 1);

    insSettings->beginGroup(INIT_GENERAL);
    insSettings->setValue("Lanuage", "English");
    insSettings->setValue("Log Level", "debug");
    insSettings->endGroup();

    return page;
}

QWidget* Preference::tabInterface()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(TIV_MARGIN_HOR, TIV_MARGIN_VER_TOP, TIV_MARGIN_HOR, TIV_MARGIN_VER_BOTTOM);

    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setVerticalSpacing(TIG_SPACING_VER);
    grid->setHorizontalSpacing(TIG_SPACING_HOR);
    grid->setColumnStretch(0, 7);
    grid->setColumnStretch(1, 9);

    QFont font;
    font.setPointSizeF(9);
    QLabel* srnBorderStyle = new QLabel(tr("Screen Border Style:"));
    srnBorderStyle->setFont(font);
    QLabel* borderColor = new QLabel(tr("Border Color:"));
    borderColor->setFont(font);
    QLabel* borderWidth = new QLabel(tr("Border Width:"));
    borderWidth->setFont(font);
    QLabel* srnCrosshair = new QLabel(tr("Screen Crosshair:"));
    srnCrosshair->setFont(font);
    QLabel* srnCrosshairWidth = new QLabel(tr("Screen Crosshair Width:"));
    srnCrosshairWidth->setFont(font);

    int i = 0;
    int j = 0;
    grid->addWidget(srnBorderStyle, i, j, Qt::AlignRight);
    grid->addWidget(new QComboBox(this), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(borderColor, i, j, Qt::AlignRight);
    grid->addWidget(new ColorParaBar(ColorParaBarMode::CPB_HighLight), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(borderWidth, i, j, Qt::AlignRight);
    grid->addWidget(new QSpinBox(), i++, j + 1, Qt::AlignLeft);


    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TIV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);
    grid->addWidget(srnCrosshair, i, j, Qt::AlignRight);
    grid->addWidget(new ColorParaBar(ColorParaBarMode::CPB_HighLight), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(srnCrosshairWidth, i, j, Qt::AlignRight);
    grid->addWidget(new QSpinBox(this), i++, j + 1, Qt::AlignLeft);

    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TIV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);

    grid->addWidget(new QCheckBox(tr("Enable smart window")), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(new QCheckBox(tr("Show cursor")), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(new QCheckBox(tr("Automatically copy to clipboard")), i++, j + 1, Qt::AlignLeft);

    qDebug() << "tabInterface:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addLayout(creatResetBtn(), 1);

    return page;
}

QWidget* Preference::tabOutput()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(TOV_MARGIN_HOR, TOV_MARGIN_VER_TOP, TOV_MARGIN_HOR, TOV_MARGIN_VER_BOTTOM);

    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setVerticalSpacing(TOG_SPACING_VER);
    grid->setHorizontalSpacing(TOG_SPACING_HOR);
    grid->setColumnStretch(0, 2);
    grid->setColumnStretch(1, 5);

    QLabel* imageQuailty = new QLabel(tr("Image quailty:"));
    QLabel* fileName = new QLabel(tr("File Name:"));
    QLabel* quickSavePath = new QLabel(tr("Quick save path:"));
    QLabel* autoAavePath = new QLabel(tr("Auto save path:"));
    QLabel* configurePath = new QLabel(tr("Configure path:"));

    int i = 0;
    int j = 0;
    grid->addWidget(imageQuailty, i, j, Qt::AlignRight);
    grid->addWidget(new QSpinBox(this), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TOV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);
    grid->addWidget(fileName, i, j, Qt::AlignRight);
    grid->addWidget(new QLineEdit(this), i++, j + 1, Qt::AlignLeft);

    grid->addWidget(quickSavePath, i, j, Qt::AlignRight);
    QHBoxLayout* hLay1 = new QHBoxLayout();
    hLay1->setMargin(0);
    hLay1->addSpacing(0);
    hLay1->addWidget(new QLineEdit(this), 4, Qt::AlignLeft);
    hLay1->addWidget(new QPushButton(tr("Change path"), this), 1, Qt::AlignLeft);
    grid->addLayout(hLay1, i++, j + 1, Qt::AlignLeft);

    grid->addWidget(autoAavePath, i, j, Qt::AlignRight);
    QHBoxLayout* hLay2 = new QHBoxLayout();
    hLay2->setMargin(0);
    hLay2->addSpacing(0);
    hLay2->addWidget(new QLineEdit(this), 4, Qt::AlignLeft);
    hLay2->addWidget(new QPushButton(tr("Change path"), this), 1, Qt::AlignLeft);
    grid->addLayout(hLay2, i++, j + 1, Qt::AlignLeft);

    grid->addWidget(configurePath, i, j, Qt::AlignRight);
    QHBoxLayout* hLay3 = new QHBoxLayout();
    hLay3->setMargin(0);
    hLay3->addSpacing(0);
    hLay3->addWidget(new QLineEdit(this), 4, Qt::AlignLeft);
    hLay3->addWidget(new QPushButton(tr("Change path"), this), 1, Qt::AlignLeft);
    grid->addLayout(hLay3, i++, j + 1, Qt::AlignLeft);

    qDebug() << "tabOutput:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addLayout(creatResetBtn(), 1);

    return page;
}

QWidget* Preference::tabPin()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(TPV_MARGIN_HOR, TPV_MARGIN_VER_TOP, TPV_MARGIN_HOR, TPV_MARGIN_VER_BOTTOM);

    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setVerticalSpacing(TPG_SPACING_VER);
    grid->setHorizontalSpacing(TPG_SPACING_HOR);
    grid->setColumnStretch(0, 7);
    grid->setColumnStretch(1, 9);

    QLabel* shade = new QLabel(tr("Window shade:"));
    QLabel* opacity = new QLabel(tr("Opacity:"));
    QLabel* maxSize = new QLabel(tr("Maximum size:"));

    int i = 0;
    int j = 0;
    grid->addWidget(shade, i, j, Qt::AlignRight);
    grid->addWidget(new QCheckBox(tr("Enable window shadow"), this), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(opacity, i, j, Qt::AlignRight);
    grid->addWidget(new QSpinBox(this), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(maxSize, i, j, Qt::AlignRight);
    grid->addWidget(new QSpinBox(this), i++, j + 1, Qt::AlignLeft);

    qDebug() << "tabPin:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addLayout(creatResetBtn(), 1);

    return page;
}

#include "hotkeyswidget.h"
QWidget *Preference::tabHotkeys()
{
    return new HotkeysWidget();
//    QWidget* page = new QWidget(nullptr);
//    page->setContentsMargins(0, 0, 0, 0);
//    QVBoxLayout* vLay = new QVBoxLayout(page);
//    vLay->setContentsMargins(THV_MARGIN_HOR, THV_MARGIN_VER_TOP, THV_MARGIN_HOR, THV_MARGIN_VER_BOTTOM);

//    // 快捷键框。若是出现乱码则因为混用了 QKeySequence(Qt::CTRL + Qt::Key_Shift + Qt::Key_Y)
//    //QKeySequence(QKeySequence::Print);
//    //QKeySequence(tr("Ctrl+P"));
//    //QKeySequence(tr("Ctrl+p"));
//    //QKeySequence(Qt::CTRL + Qt::Key_P);


//    int i = 0;
//    int j = 0;
//    QGridLayout* grid = new QGridLayout();
//    grid->setMargin(0);
//    grid->setVerticalSpacing(THG_SPACING_VER);
//    grid->setHorizontalSpacing(THG_SPACING_HOR);
//    grid->setColumnStretch(0, 7);
//    grid->setColumnStretch(1, 9);

//    for (auto& it : Tray::instance().getVHotKeys()) {
//        // TODO 2022.07.17: 虽然是值传递，但 std::get<0>(it) 为空，以后研究下
//        QString& hotkey = std::get<1>(it);
//        QString& describe = std::get<2>(it);
//        XKeySequenceEdit* pEdit = new XKeySequenceEdit(QKeySequence(hotkey));
//        pEdit->setObjectName(describe);
//        pEdit->setMinimumWidth(110 * m_scale);                // 估的一个数值

//        connect(pEdit, &XKeySequenceEdit::sigKeySeqChanged, &(Tray::instance()), &Tray::onKeySequenceChanged);

//        grid->addWidget(new QLabel(describe + ":"), i, j, 1, 1, Qt::AlignRight);
//        grid->addWidget(pEdit, i++, j + 1, 1, 1, Qt::AlignLeft);

//        if (i == 5)
//            grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - THV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);
//    }

//    qDebug() << "tabHotkeys:grid->rowCount():" << grid->rowCount();
//    vLay->addLayout(grid, grid->rowCount());
//    vLay->addStretch(3);
//    vLay->addLayout(creatResetBtn(), 1);

//    return page;
}

QWidget *Preference::tabAbout()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(TAV_MARGIN_HOR, TAV_MARGIN_VER_TOP, TAV_MARGIN_HOR, TAV_MARGIN_VER_BOTTOM);

    int i = 0;
    int j = 0;
    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setVerticalSpacing(TAG_SPACING_VER);
    grid->setHorizontalSpacing(TAG_SPACING_VER);
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
    grid->addWidget(tb, i++, j, 2, 2, Qt::AlignCenter);  //占据两格


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

    i = grid->rowCount();
    grid->addItem(new QSpacerItem(0, 7, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(project, i++, j, 1, 2, Qt::AlignCenter);
    grid->addItem(new QSpacerItem(0, 6, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(buildTime, i++, j, 1, 2, Qt::AlignCenter);
    grid->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(detail, i++, j, 1, 2, Qt::AlignCenter);
    grid->addItem(new QSpacerItem(0, 16, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);

    grid->addWidget(authorL, i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(authorR, i++, j + 1, 1, 1, Qt::AlignLeft);
    grid->addItem(new QSpacerItem(0, 6, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(copyrightL, i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(copyrightR, i++, j + 1, 1, 1, Qt::AlignLeft);

    grid->addItem(new QSpacerItem(0, 21, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TAV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);
    grid->addItem(new QSpacerItem(0, 14, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);

    QLabel* ackmtR = new QLabel(tr("Acknowledgements"));
    font.setPointSizeF(9);
    font.setBold(true);
    ackmtR->setFont(font);
    font.setBold(false);

    QLabel* ghL =new QLabel("GitHub:");
    font.setPointSizeF(8);
    ghL->setFont(font);
    QLabel* ghR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli/PicShot>PicShot</a>"));
    ghR->setOpenExternalLinks(true);
    ghR->setFont(font);
    QLabel* blogR = new QLabel(tr("<a style='color: green;' href=https://ifmet.cn>ifmet.cn</a>"));
    blogR->setOpenExternalLinks(true);
    font.setPointSizeF(9);
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
    grid->addItem(new QSpacerItem(0, 7, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(ghL, ++i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(ghR, i, j + 1, Qt::AlignLeft);
    grid->addItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(new QLabel(" "), ++i, j, 1, 1, Qt::AlignRight);
    grid->addWidget(blogR, i, j + 1, Qt::AlignLeft);
    grid->addItem(new QSpacerItem(0, 6, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(UIL, ++i, j, Qt::AlignRight);
    grid->addWidget(UIR, i, j + 1, Qt::AlignLeft);
    grid->addItem(new QSpacerItem(0, 6, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(ctL, ++i, j, Qt::AlignRight);
    grid->addWidget(ctR, i, j + 1, Qt::AlignLeft);
    grid->addItem(new QSpacerItem(0, 6, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j);
    grid->addWidget(tsL, ++i, j, Qt::AlignRight);
    grid->addWidget(tsR, i, j + 1, Qt::AlignLeft);

    qDebug() << "tabAbout:grid->rowCount():" << grid->rowCount();  // 创建一个空 grid 时，便默认有一个 1 item，添加第一个后，依旧计数 1 个；
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addSpacing(TAV_MARGIN_VER_BOTTOM);

    return page;
}

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
#include "hotkeyswidget.h"
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
#include <QStandardPaths>
#include <QSizePolicy>

// test
#include <QDebug>
#include <QPushButton>
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
    insSettings->beginGroup(INIT_GENERAL);
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

    auto cbLanuage = new QComboBox(this);
    auto cbLogLevel = new QComboBox(this);
    int i = 0;
    int j = 0;
    grid->addWidget(lanuage, i, j, Qt::AlignRight);
    grid->addWidget(cbLanuage, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(launch, i, j, Qt::AlignRight);
    grid->addWidget(new QCheckBox(tr("Run on system startup")), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(new QCheckBox(tr("As administrator")), i++, j + 1, Qt::AlignLeft);
    grid->addWidget(logLevel, i, j, Qt::AlignRight);
    grid->addWidget(cbLogLevel, i++, j + 1, Qt::AlignLeft);

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

    QStringList lLanuage = {tr("English"), tr("中文")};
    QStringList lLogLevel = {tr("trace"), tr("debug"), tr("info"), tr("warn"), tr("error"), tr("critical"), tr("off")};
    cbLanuage->addItems(lLanuage);
    cbLogLevel->addItems(lLogLevel);
    cbLanuage->setCurrentText(insSettings->value("Lanuage", lLanuage[0]).toString());
    cbLogLevel->setCurrentText(insSettings->value("Log Level", lLogLevel[1]).toString());
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
    QLabel* srnBorderStyle = new QLabel(tr("Border Style:"));
    QLabel* borderColor = new QLabel(tr("Border Color:"));
    QLabel* borderWidth = new QLabel(tr("Border Width:"));
    QLabel* srnCrosshair = new QLabel(tr("Crosshair Color:"));
    QLabel* srnCrosshairWidth = new QLabel(tr("Crosshair Width:"));
    srnBorderStyle->setFont(font);
    borderColor->setFont(font);
    borderWidth->setFont(font);
    srnCrosshair->setFont(font);
    srnCrosshairWidth->setFont(font);

    QStringList lStyleTheme = { "picshot", "black and white", "blue" };
    auto cbBorderStyle = new QComboBox(this);
    cbBorderStyle->addItems(lStyleTheme);
    cbBorderStyle->setCurrentText(lStyleTheme.at(0));
    auto cpbHighLight = new ColorParaBar(ColorParaBarMode::CPB_HighLight);
    auto spBorder = new QSpinBox(this);
    spBorder->setRange(1, 100);
    spBorder->setValue(2);
    auto cpbCrosshair = new ColorParaBar(ColorParaBarMode::CPB_HighLight);
    auto spCrosshair = new QSpinBox(this);
    spCrosshair->setRange(1, 100);
    spCrosshair->setValue(2);
    auto cbEnableSamrtWindow = new QCheckBox(tr("Enable smart window"));
    auto cbEnableShowCursor = new QCheckBox(tr("Show cursor"));
    auto cbEnableAutoCopy = new QCheckBox(tr("Automatically copy to clipboard"));

    int i = 0;
    int j = 0;
    grid->addWidget(srnBorderStyle, i, j, Qt::AlignRight);
    grid->addWidget(cbBorderStyle, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(borderColor, i, j, Qt::AlignRight);
    grid->addWidget(cpbHighLight, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(borderWidth, i, j, Qt::AlignRight);
    grid->addWidget(spBorder, i++, j + 1, Qt::AlignLeft);


    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TIV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);
    grid->addWidget(srnCrosshair, i, j, Qt::AlignRight);
    grid->addWidget(cpbCrosshair, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(srnCrosshairWidth, i, j, Qt::AlignRight);
    grid->addWidget(spCrosshair, i++, j + 1, Qt::AlignLeft);

    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TIV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);

    grid->addWidget(cbEnableSamrtWindow, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(cbEnableShowCursor, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(cbEnableAutoCopy, i++, j + 1, Qt::AlignLeft);

    qDebug() << "tabInterface:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addLayout(creatResetBtn(), 1);

    insSettings->beginGroup(INIT_INTERFACE);
    //insSettings->setValue(srnBorderStyle->text().chopped(1), cbBorderStyle->currentText());
    //insSettings->setValue(borderColor->text().chopped(1), cpbHighLight->getCurColor().name());
    //insSettings->setValue(borderWidth->text().chopped(1), spBorder->value());

    //insSettings->setValue(srnCrosshair->text().chopped(1), cpbCrosshair->getCurColor().name());
    //insSettings->setValue(srnCrosshairWidth->text().chopped(1), spCrosshair->value());

    //insSettings->setValue(cbEnableSamrtWindow->text(), false);
    //insSettings->setValue(cbEnableShowCursor->text(), false);
    //insSettings->setValue(cbEnableAutoCopy->text(), false);

    cbBorderStyle->setCurrentText(insSettings->value(srnBorderStyle->text().chopped(1), "picshot").toString());
    cpbHighLight->setCurColor(QColor(insSettings->value(borderColor->text().chopped(1), QColor("#db000f")).toString()));
    spBorder->setValue(insSettings->value(borderWidth->text().chopped(1), 2).toInt());
    cpbCrosshair->setCurColor(QColor(insSettings->value(srnCrosshair->text().chopped(1), QColor("#db000f")).toString()));
    spCrosshair->setValue(insSettings->value(srnCrosshairWidth->text().chopped(1), 2).toInt());

    cbEnableSamrtWindow->setChecked(insSettings->value(cbEnableSamrtWindow->text(), false).toBool());
    cbEnableShowCursor->setChecked(insSettings->value(cbEnableShowCursor->text(), false).toBool());
    cbEnableAutoCopy->setChecked(insSettings->value(cbEnableAutoCopy->text(), false).toBool());

    insSettings->endGroup();

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

    auto imageQuailty = new QLabel(tr("Image quailty:"));
    auto fileName = new QLabel(tr("File Name:"));
    auto quickSavePath = new QLabel(tr("Quick save path:"));
    auto autoAavePath = new QLabel(tr("Auto save path:"));
    auto configurePath = new QLabel(tr("Config path:"));
    auto sbImageQuailty = new QSpinBox(this);
    auto editFileName = new QLineEdit(this);
    auto editQuickSavePath = new QLineEdit(this);
    auto editAutoSavePath = new QLineEdit(this);
    auto editConfigPath = new QLineEdit(this);
    auto changeFileName = new QPushButton(tr("Hint"), this);
    auto changeQuickSavePath = new QPushButton(tr("Change path"), this);
    auto changeAutoSavePath = new QPushButton(tr("Change path"), this);
    auto changeConfigPath = new QPushButton(tr("Change path"), this);

    sbImageQuailty->setRange(-1, 100);
    sbImageQuailty->setFixedWidth(80);

    int i = 0;
    int j = 0;
    grid->addWidget(imageQuailty, i, j, Qt::AlignRight);
    grid->addWidget(sbImageQuailty, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TOV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);
    grid->addWidget(fileName, i, j, Qt::AlignRight);
    QHBoxLayout* hLay0 = new QHBoxLayout();
    hLay0->setMargin(0);
    hLay0->addSpacing(0);
    hLay0->addWidget(editFileName, 4);
    hLay0->addWidget(changeFileName, 1);
    grid->addLayout(hLay0, i++, j + 1);

    grid->addWidget(quickSavePath, i, j, Qt::AlignRight);
    QHBoxLayout* hLay1 = new QHBoxLayout();
    hLay1->setMargin(0);
    hLay1->addSpacing(0);
    hLay1->addWidget(editQuickSavePath, 4);    // 去掉位置，即可自动策略为伸长
    hLay1->addWidget(changeQuickSavePath, 1);
    grid->addLayout(hLay1, i++, j + 1);

    grid->addWidget(autoAavePath, i, j, Qt::AlignRight);
    QHBoxLayout* hLay2 = new QHBoxLayout();
    hLay2->setMargin(0);
    hLay2->addSpacing(0);
    hLay2->addWidget(editAutoSavePath, 4);
    hLay2->addWidget(changeAutoSavePath, 1);
    grid->addLayout(hLay2, i++, j + 1);

    grid->addWidget(configurePath, i, j, Qt::AlignRight);
    QHBoxLayout* hLay3 = new QHBoxLayout();
    hLay3->setMargin(0);
    hLay3->addSpacing(0);
    hLay3->addWidget(editConfigPath, 4);
    hLay3->addWidget(changeConfigPath, 1);
    grid->addLayout(hLay3, i++, j + 1);

    qDebug() << "tabOutput:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addLayout(creatResetBtn(), 1);

    insSettings->beginGroup(INIT_OUTPUT);
    bool ok = false;
    sbImageQuailty->setValue(insSettings->value(imageQuailty->text().chopped(1), "-1").toInt(&ok));
    editFileName->setText(insSettings->value(fileName->text().chopped(1), "PicShot_xxxx.png").toString());
    editQuickSavePath->setText(insSettings->value(quickSavePath->text().chopped(1), QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)).toString());
    editAutoSavePath->setText(insSettings->value(autoAavePath->text().chopped(1), QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)).toString());
    editConfigPath->setText(insSettings->value(autoAavePath->text().chopped(1), QStandardPaths::standardLocations(QStandardPaths::ConfigLocation)).toString());
    insSettings->endGroup();

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

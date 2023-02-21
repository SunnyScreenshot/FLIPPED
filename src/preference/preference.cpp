// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

#include "preference.h"
#include "../xglobal.h"
#include "../widget/xhorizontalline.h"
#include "../tool/base/colorparabar.h"
#include "../screen/drawhelper.h"
#include "../core/xlog.h"
#include "appellation.h"
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
#include <QFontDialog>

// test
#include <QCoreApplication>
#include <QDebug>
#include <QPushButton>
#include <QTranslator>
#include <QVector>
#include <QFileDialog>

Preference::Preference(QWidget *parent)
    : QWidget(parent)
    , m_scale(XHelper::instance().getScale())
{
    initUI();
}

void Preference::initUI()
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    // main layout
    setContentsMargins(PRE_MARGIN_HOR, PRE_MARGIN_VER + 10, PRE_MARGIN_HOR, PRE_MARGIN_VER);

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
    setWindowTitle(tr("Flipped Preferences"));
    this->setMinimumWidth(800);

//    qDebug()<<"--->"<<tabPages->palette().window().color();
//    QPalette pal = this->palette();
//    pal.setColor(QPalette::Window, QColor("#EFEFEF")/*tabPages->palette().window().color()*/);  // tabPages->palette()
//    setAutoFillBackground(true);
//    this->setPalette(pal);
}

QHBoxLayout* Preference::creatResetBtn(QString objectName)
{
    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->setContentsMargins(0, 0, 0, 0);
    hLay->addSpacing(0);
    hLay->addStretch(7);
    auto btn = new QPushButton(tr("Reset"), this);
    if (btn) {
        btn->setObjectName(objectName);
        hLay->addWidget(btn, 1, Qt::AlignRight);
        connect(btn, &QPushButton::released, this, &Preference::onReset);
    }
    return hLay;
}

bool Preference::checkBoxState2Bool(int state)
{
    return state == Qt::Checked ? true : false;
}

void Preference::translateUI()
{
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

    NEW_OBJECT_AND_TEXT(lanuage, QLabel, tgLanuage, tr("Lanuage:"));
    NEW_OBJECT_AND_TEXT(font, QLabel, tgFont, tr("Font:"));
    NEW_OBJECT_AND_TEXT(launch, QLabel, tgSelfStarting, tr("Launch:"));
    NEW_OBJECT_AND_TEXT(logLevel, QLabel, tgLogLevel, tr("Log Level:"));
    NEW_OBJECT_AND_TEXT(update, QLabel, tgUpdate, tr("Update:"));

    NEW_OBJECT(cbLanuage, QComboBox, tgLanuage);
    NEW_OBJECT(btnFont, QPushButton, tgFont);
    NEW_OBJECT_AND_TEXT(cbSelfStart, QCheckBox, tgSelfStarting, tr("Run on system startup"));
    NEW_OBJECT_AND_TEXT(cbAsAdmin, QCheckBox, tgAsAdmin, tr("As administrator"));
    NEW_OBJECT(cbLogLevel, QComboBox, tgLogLevel);
    NEW_OBJECT_AND_TEXT(cbAutoCheck, QCheckBox, tgUpdate, tr("Automatic check"));
    NEW_OBJECT_AND_TEXT(pbUpdate, QPushButton, tgUpdate, tr("Update"));

    int i = 0;
    int j = 0;
    grid->addWidget(lanuage, i, j, Qt::AlignRight);
    grid->addWidget(cbLanuage, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(font, i, j, Qt::AlignRight);
    grid->addWidget(btnFont, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(launch, i, j, Qt::AlignRight);
    grid->addWidget(cbSelfStart, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(cbAsAdmin, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(logLevel, i, j, Qt::AlignRight);
    grid->addWidget(cbLogLevel, i++, j + 1, Qt::AlignLeft);

    grid->addItem(new QSpacerItem(0, 38, QSizePolicy::Expanding, QSizePolicy::Fixed), i++, j); // 添加弹簧
    grid->addWidget(update, i, j, Qt::AlignRight);
    QHBoxLayout* tHLay = new QHBoxLayout();
    tHLay->setContentsMargins(0, 0, 0, 0);
    tHLay->addSpacing(0);
    tHLay->addWidget(cbAutoCheck, 1, Qt::AlignLeft);
    tHLay->addSpacing(20);
    tHLay->addWidget(pbUpdate, 1, Qt::AlignLeft);
    tHLay->addStretch(8);
    grid->addLayout(tHLay, i, j + 1, Qt::AlignLeft);

    // #ToBeImproved
    cbSelfStart->setDisabled(true);
    cbAsAdmin->setDisabled(true);
    cbAutoCheck->setDisabled(true);
    pbUpdate->setDisabled(true);
    launch->hide();
    cbSelfStart->hide();
    cbAsAdmin->hide();
    update->hide();
    pbUpdate->hide();
    cbAutoCheck->hide();
    pbUpdate->hide();

//    qDebug() << "tabGeneral:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(8);
    vLay->addLayout(creatResetBtn(tgReset), 1);

    QMap<QString, QString> mapLanuage = { {tr("English"), "en_US"},
                                          {tr("简体中文"), "zh_CN"},
                                          {tr("繁體中文"), "zh_TW"} };

    for (auto it = mapLanuage.cbegin(); it != mapLanuage.cend(); ++it )
        cbLanuage->addItem(it.key(), it.value());
    btnFont->setStyleSheet("text-align: left;");
    QStringList lLogLevel = {tr("trace"), tr("debug"), tr("info"), tr("warn"), tr("error"), tr("critical"), tr("off")};
    cbLogLevel->addItems(lLogLevel);

    settingIni->beginGroup(INIT_GENERAL);
    cbLanuage->setCurrentText(mapLanuage.key(settingIni->value(tgLanuage, mapLanuage.key("English")).toString()));
    auto tt = settingIni->value(tgFont, "SimSun2,19").toString();
    btnFont->setText(settingIni->value(tgFont, "SimSun,9").toString());
    cbSelfStart->setChecked(settingIni->value(tgSelfStarting, false).toBool());
    cbAsAdmin->setChecked(settingIni->value(tgAsAdmin, false).toBool());
    cbLogLevel->setCurrentText(settingIni->value(tgLogLevel, "debug").toString());
    cbAutoCheck->setChecked(settingIni->value(tgAutoCheckUpdate, false).toBool());
    settingIni->endGroup();

    connect(cbLanuage, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, &Preference::onLanuageChange);
    connect(btnFont, &QPushButton::released, this, &Preference::onFontChange);
    connect(cbSelfStart, &QCheckBox::stateChanged, this, &Preference::onSelfStart);
    connect(cbAsAdmin, &QCheckBox::stateChanged, this, &Preference::onAsAdmin);
    connect(cbLogLevel, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, &Preference::onLogLevelChange);
    connect(cbAutoCheck, &QCheckBox::stateChanged, this, &Preference::onAutoCheck);
    connect(pbUpdate, &QPushButton::released, this, &Preference::onUpdate);
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

    NEW_OBJECT_AND_TEXT(srnBorderStyle, QLabel, tiBorderStyle, tr("Border Style:"));
    NEW_OBJECT_AND_TEXT(borderColor, QLabel, tiBorderColor, tr("Border Color:"));
    NEW_OBJECT_AND_TEXT(borderWidth, QLabel, tiBorderWidth, tr("Border Width:"));
    NEW_OBJECT_AND_TEXT(srnCrosshair, QLabel, tiCrosshairColor, tr("Crosshair Color:"));
    NEW_OBJECT_AND_TEXT(srnCrosshairWidth, QLabel, tiCrosshairWidth, tr("Crosshair Width:"));

    NEW_OBJECT(cbBorderStyle, QComboBox, tiBorderStyle);
    auto cpbHighLight = new ColorParaBar(ColorParaBarMode::CPB_HighLight);
    NEW_OBJECT(spBorder, QSpinBox, tiBorderWidth);
    auto cpbCrosshair = new ColorParaBar(ColorParaBarMode::CPB_HighLight);
    NEW_OBJECT(spCrosshair, QSpinBox, tiCrosshairWidth);
    NEW_OBJECT_AND_TEXT(cbEnableSamrtWindow, QCheckBox, tiSmartWindow, tr("Smart window"));
    NEW_OBJECT_AND_TEXT(cbEnableCrosshair, QCheckBox, tiCrosshair, tr("Crosshair"));
    NEW_OBJECT_AND_TEXT(cbEnableShowCursor, QCheckBox, tiShowCursor, tr("Show cursor"));
    NEW_OBJECT_AND_TEXT(cbEnableAutoCopy, QCheckBox, tiAutoCopyToClipboard, tr("Auto copy to clipboard"));

    cpbHighLight->setObjectName(tiBorderColor);
    cpbCrosshair->setObjectName(tiCrosshairColor);
    spBorder->setRange(1, 100);
    spCrosshair->setRange(1, 100);

    QStringList styles;
    styles << "flipped" << "black_white" << "light_blue";

    for (const auto &text : styles)
        cbBorderStyle->addItem(text, text);

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
    grid->addWidget(cbEnableCrosshair, i++, j + 1, Qt::AlignLeft);
    //grid->addWidget(cbEnableShowCursor, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(cbEnableAutoCopy, i++, j + 1, Qt::AlignLeft);

    // #ToBeImproved
    cbEnableShowCursor->setDisabled(true);
    cbEnableShowCursor->setVisible(false);

//    qDebug() << "tabInterface:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addLayout(creatResetBtn(tiReset), 1);

    settingIni->beginGroup(INIT_INTERFACE);
    cbBorderStyle->setCurrentText(settingIni->value(tiBorderStyle, "flipped").toString());
    cpbHighLight->setCurColor(QColor(settingIni->value(tiBorderColor, QColor("#0e70ff")).toString()));
    spBorder->setValue(settingIni->value(tiBorderWidth, 2).toInt());
    cpbCrosshair->setCurColor(QColor(settingIni->value(tiCrosshairColor, QColor("#db000f")).toString()));
    spCrosshair->setValue(settingIni->value(tiCrosshairWidth, 1).toInt());
    cbEnableSamrtWindow->setChecked(settingIni->value(tiSmartWindow, true).toBool());
    cbEnableCrosshair->setChecked(settingIni->value(tiCrosshair, false).toBool());
    cbEnableShowCursor->setChecked(settingIni->value(tiShowCursor, false).toBool());
    cbEnableAutoCopy->setChecked(settingIni->value(tiAutoCopyToClipboard, true).toBool());
    settingIni->endGroup();

    connect(cbBorderStyle, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), this, &Preference::onBorderStyle);
    connect(cpbHighLight, &ColorParaBar::sigColorChange, this, &Preference::onBorderColor);
    connect(spBorder, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preference::onBorderWidth);
    connect(cpbCrosshair, &ColorParaBar::sigColorChange, this, &Preference::onCrosshairColor);
    connect(spCrosshair, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preference::onCrosshairWidth);
    connect(cbEnableSamrtWindow, &QCheckBox::stateChanged, this, &Preference::onSmartWindow);
    connect(cbEnableCrosshair, &QCheckBox::stateChanged, this, &Preference::onShowCrosshair);
    connect(cbEnableShowCursor, &QCheckBox::stateChanged, this, &Preference::onShowCursor);
    connect(cbEnableAutoCopy, &QCheckBox::stateChanged, this, &Preference::onAutoCopyToClip);

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
    grid->setColumnStretch(0, 3);
    grid->setColumnStretch(1, 8);
//    grid->setColumnStretch(2, 0);

    NEW_OBJECT_AND_TEXT(imageQuailty, QLabel, toImageQuailty, tr("Image quailty:"));
    NEW_OBJECT_AND_TEXT(fileName, QLabel, toFileName, tr("File Name:"));
    NEW_OBJECT_AND_TEXT(quickSavePath, QLabel, toQuickSavePath, tr("Quick save path:"));
    NEW_OBJECT_AND_TEXT(autoAavePath, QLabel, toAutoSavePath, tr("Auto save path:"));
    NEW_OBJECT_AND_TEXT(configurePath, QLabel, toConfigPath, tr("Config path:"));

    NEW_OBJECT(sbImageQuailty, QSpinBox, toImageQuailty);
    NEW_OBJECT(editFileName, QLineEdit, toFileName);
    NEW_OBJECT(editQuickSavePath, QLineEdit, toQuickSavePath);
    NEW_OBJECT(editAutoSavePath, QLineEdit, toAutoSavePath);
    NEW_OBJECT(editConfigPath, QLineEdit, toConfigPath);

//    changeFileName = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);
    NEW_OBJECT_AND_TEXT(changeFileName, QPushButton, toFileName, tr("Hint"));
    NEW_OBJECT_AND_TEXT(changeQuickSavePath, QPushButton, toQuickSavePath, tr("Change path"));
    NEW_OBJECT_AND_TEXT(changeAutoSavePath, QPushButton, toAutoSavePath, tr("Change path"));
    NEW_OBJECT_AND_TEXT(changeConfigPath, QPushButton, toConfigPath, tr("Change path"));

    sbImageQuailty->setRange(-1, 100);
    sbImageQuailty->setMinimumWidth(70);

    int i = 0;
    int j = 0;
    grid->addWidget(imageQuailty, i, j, Qt::AlignRight);
    grid->addWidget(sbImageQuailty, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TOV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);

    auto creatPathEdit = [&](QLabel* label, QLineEdit* edit, QPushButton* btn) {
        grid->addWidget(label, i, j, Qt::AlignRight);
        btn->setMinimumWidth(100);
        QHBoxLayout* hLay = new QHBoxLayout();
        hLay->setMargin(0);
        hLay->addSpacing(0);
        hLay->addWidget(edit, 7);  // 去掉位置，即可自动策略为伸长
        hLay->addWidget(btn, 1);
        hLay->addSpacerItem(new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed));
        grid->addLayout(hLay, i++, j + 1);
    };

    creatPathEdit(fileName, editFileName, changeFileName);
    creatPathEdit(quickSavePath, editQuickSavePath, changeQuickSavePath);
    creatPathEdit(autoAavePath, editAutoSavePath, changeAutoSavePath);
    creatPathEdit(configurePath, editConfigPath, changeConfigPath);
    sbImageQuailty->setToolTip(tr("Range [0,100] or -1.\nSpecify 0 to obtain small compressed files, 100 for large uncompressed files.\nand -1 to use the default settings."));

//    qDebug() << "tabOutput:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addLayout(creatResetBtn(toReset), 1);

    settingIni->beginGroup(INIT_OUTPUT);
    bool ok = false;
    sbImageQuailty->setValue(settingIni->value(toImageQuailty, -1).toInt(&ok));
    editFileName->setText(settingIni->value(toFileName, "flipped_$yyyyMMdd_hhmmss$.png").toString());
    editQuickSavePath->setText(settingIni->value(toQuickSavePath, QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first()).toString());
    editAutoSavePath->setText(settingIni->value(toAutoSavePath, QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()).toString());
    editConfigPath->setText(settingIni->value(toConfigPath, qApp->applicationDirPath() + "/config").toString());
    settingIni->endGroup();

    connect(sbImageQuailty, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preference::onImageQuailty);
    connect(editFileName, &QLineEdit::textChanged, this, &Preference::onFileName);
    connect(editQuickSavePath, &QLineEdit::textChanged, this, &Preference::onQuickSavePath);
    connect(editAutoSavePath, &QLineEdit::textChanged, this, &Preference::onAutoSavePath);
    connect(editConfigPath, &QLineEdit::textChanged, this, &Preference::onConfigPath);
    connect(changeQuickSavePath, &QPushButton::released, this, &Preference::onChoosePath);
    connect(changeAutoSavePath, &QPushButton::released, this, &Preference::onChoosePath);
    connect(changeConfigPath, &QPushButton::released, this, &Preference::onChoosePath);

    const QString tooltip = XHelper::instance().formatToName(editFileName->text());
    editFileName->setToolTip(tooltip);

    // 临时屏蔽功能
    quickSavePath->hide();
    editQuickSavePath->hide();
    changeQuickSavePath->hide();


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

    NEW_OBJECT_AND_TEXT(shade, QLabel, tpWindowShadow, tr("Window Shadow:"));
    NEW_OBJECT_AND_TEXT(opacity, QLabel, tpOpacity, tr("Opacity:"));
    NEW_OBJECT_AND_TEXT(maxSize, QLabel, tpMaxSize, tr("Maximum size:"));

    NEW_OBJECT_AND_TEXT(cbWindowShadow, QCheckBox, tpWindowShadow, tr("Enable"));
    NEW_OBJECT(sbOpacity, QSpinBox, tpOpacity);
    NEW_OBJECT(sbMaxSize, QSpinBox, tpMaxSize);

    sbOpacity->setSuffix("%");
    sbOpacity->setSingleStep(10);
    sbOpacity->setRange(0, 100);
    sbOpacity->setMinimumWidth(70);
    sbOpacity->setAlignment(Qt::AlignHCenter);
    sbMaxSize->setRange(100, 100000);
    sbMaxSize->setMinimumWidth(80);
    sbMaxSize->setAlignment(Qt::AlignHCenter);

    int i = 0;
    int j = 0;
    grid->addWidget(shade, i, j, Qt::AlignRight);
    grid->addWidget(cbWindowShadow, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(opacity, i, j, Qt::AlignRight);
    grid->addWidget(sbOpacity, i++, j + 1, Qt::AlignLeft);
    grid->addWidget(maxSize, i, j, Qt::AlignRight);
    grid->addWidget(sbMaxSize, i++, j + 1, Qt::AlignLeft);

    // #ToBeImproved
    cbWindowShadow->setDisabled(true);
    shade->hide();
    cbWindowShadow->hide();

//    qDebug() << "tabPin:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addLayout(creatResetBtn(tpReset), 1);

    settingIni->beginGroup(INIT_PIN);
    cbWindowShadow->setChecked(settingIni->value(tpWindowShadow, true).toBool());
    sbOpacity->setValue(settingIni->value(tpOpacity, sbOpacity->maximum()).toInt());
    sbMaxSize->setValue(settingIni->value(tpMaxSize, sbMaxSize->maximum()).toInt());
    settingIni->endGroup();

    connect(cbWindowShadow, &QCheckBox::stateChanged, this, &Preference::onWindowShadow);
    connect(sbOpacity, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preference::onOpacity);
    connect(sbMaxSize, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preference::onMaxSize);

    return page;
}

QWidget *Preference::tabHotkeys()
{
    auto w = new HotkeysWidget();
    if (w) {
        auto btnReset = w->findChild<QPushButton *>(thReset);
        if (btnReset)
            connect(btnReset, &QPushButton::released, this, &Preference::onReset);

        return w;
    } else {
        return nullptr;
    }
}

QWidget *Preference::tabAbout()
{
    QWidget* page = new QWidget(nullptr);
    page->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *vLay = new QVBoxLayout(page);
    vLay->setContentsMargins(TAV_MARGIN_HOR, TAV_MARGIN_VER_TOP / 3, TAV_MARGIN_HOR, TAV_MARGIN_VER_BOTTOM);

    int i = 0;
    int j = 0;
    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setVerticalSpacing(TAG_SPACING_VER);
    grid->setHorizontalSpacing(TAG_SPACING_HOR);
    grid->setColumnStretch(0, 7);
    grid->setColumnStretch(1, 9);

    QLabel* logo = new QLabel();
    logo->setPixmap(QIcon(":/resources/logo.png").pixmap(QSize(48, 48) * m_scale));
    grid->addWidget(logo, i++, j, 2, 2, Qt::AlignCenter);  //占据两格

#if defined(Q_OS_MAC)
    const double ponitSize = 14;
#else
    const double ponitSize = 14;
#endif
    QString bit;
    if (_BIT_ARCH == 4)
        bit = "x86";
    else if (_BIT_ARCH == 8)
        bit = "x64";

    QLabel* project = new QLabel(tr("<a style='color: black;' href=https://github.com/XMuli/Flipped> %1</a>").arg(_PROJECT_NAME));
    project->setOpenExternalLinks(true);
    QFont font;
    font.setPointSizeF(ponitSize);
    font.setBold(true);
    project->setFont(font);
    QString time = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    QLabel* buildTime = new QLabel(tr("%1-Beta %2 (%3)")
                                   .arg(_PROJECT_VERSION)
                                   .arg(bit)
                                   .arg(time.left(time.indexOf(" "))));
    font.setPointSizeF(ponitSize - 4);
    font.setBold(false);
    buildTime->setFont(font);
    buildTime->setStyleSheet("QLabel{color:rgba(0, 0, 0, 0.6);}");
    buildTime->setToolTip(time.right(time.count() - time.indexOf(" ") - 1));
    QLabel* detail = new QLabel(tr("simple and beautiful cross-platform screenshot software."));
    detail->setFont(font);

    QLabel* authorL = new QLabel(tr("Author:"));
    authorL->setFont(font);
    QLabel* authorR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli> XMuli</a>"));
    authorR->setOpenExternalLinks(true);
    authorR->setFont(font);
    QLabel* copyrightL = new QLabel(tr("Copyright ©"));
    copyrightL->setFont(font);
    QLabel* copyrightR = new QLabel(tr("2021-2023 ") + tr("<a style='color: green;' href=https://github.com/XMuli> XMuli</a>"));
    copyrightR->setOpenExternalLinks(true);
    copyrightR->setFont(font);

    grid->addWidget(project, grid->rowCount(), j, 1, 2, Qt::AlignCenter);
    grid->addItem(new QSpacerItem(0, 8, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
    grid->addWidget(buildTime, grid->rowCount(), j, 1, 2, Qt::AlignCenter);
    grid->addItem(new QSpacerItem(0, 14, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
    grid->addWidget(detail, grid->rowCount(), j, 1, 2, Qt::AlignCenter);
    grid->addItem(new QSpacerItem(0, 14, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);

    grid->addWidget(authorL, grid->rowCount(), j, 1, 1, Qt::AlignRight);
    grid->addWidget(authorR, grid->rowCount() - 1, j + 1, 1, 1, Qt::AlignLeft);
    grid->addItem(new QSpacerItem(0, 8, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
    grid->addWidget(copyrightL, grid->rowCount(), j, 1, 1, Qt::AlignRight);
    grid->addWidget(copyrightR, grid->rowCount() - 1, j + 1, 1, 1, Qt::AlignLeft);

    grid->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
    grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - TAV_MARGIN_HOR * m_scale * 2), grid->rowCount(), j, 1, grid->columnCount(), Qt::AlignCenter);
    grid->addItem(new QSpacerItem(0, 5, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);

    QLabel* ackmtR = new QLabel(tr("Acknowledgements"));
    font.setPointSizeF(ponitSize - 3);
    font.setBold(true);
    ackmtR->setFont(font);
    font.setBold(false);

    QLabel* ghL =new QLabel("GitHub:");
    font.setPointSizeF(ponitSize - 5);
    ghL->setFont(font);
    QLabel* ghR = new QLabel(tr("<a style='color: green;' href=https://github.com/XMuli/Flipped>Flipped</a>"));
    ghR->setOpenExternalLinks(true);
    ghR->setFont(font);
    QLabel* blogR = new QLabel(tr("<a style='color: green;' href=https://ifmet.cn>ifmet.cn</a>"));
    blogR->setOpenExternalLinks(true);
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

    // 临时屏蔽
    authorL->hide();
    authorR->hide();
    ctL->hide();
    ctR->hide();
    tsL->hide();
    tsR->hide();
    ghL->hide();
    ghR->hide();
    blogR->hide();

    grid->addWidget(ackmtR, grid->rowCount(), j + 1, 1, 1, Qt::AlignLeft);
//    grid->addItem(new QSpacerItem(0, 7, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
//    grid->addWidget(ghL, grid->rowCount(), j, 1, 1, Qt::AlignRight);
//    grid->addWidget(ghR, grid->rowCount() - 1, j + 1, Qt::AlignLeft);
//    grid->addItem(new QSpacerItem(0, 1, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
//    grid->addWidget(new QLabel(" "), grid->rowCount(), j, 1, 1, Qt::AlignRight);
//    grid->addWidget(blogR, grid->rowCount() - 1, j + 1, Qt::AlignLeft);
    grid->addItem(new QSpacerItem(0, 6, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
    grid->addWidget(UIL, grid->rowCount(), j, Qt::AlignRight);
    grid->addWidget(UIR, grid->rowCount() - 1, j + 1, Qt::AlignLeft);
//    grid->addItem(new QSpacerItem(0, 6, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
//    grid->addWidget(ctL, grid->rowCount(), j, Qt::AlignRight);
//    grid->addWidget(ctR, grid->rowCount() - 1, j + 1, Qt::AlignLeft);
//    grid->addItem(new QSpacerItem(0, 6, QSizePolicy::Expanding, QSizePolicy::Fixed), grid->rowCount(), j);
//    grid->addWidget(tsL, grid->rowCount(), j, Qt::AlignRight);
//    grid->addWidget(tsR, grid->rowCount() - 1, j + 1, Qt::AlignLeft);

//    qDebug() << "tabAbout:grid->rowCount():" << grid->rowCount();  // 创建一个空 grid 时，便默认有一个 1 item，添加第一个后，依旧计数 1 个；
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);
    vLay->addSpacing(TAV_MARGIN_VER_BOTTOM);

    return page;
}

void Preference::onReset()
{
    // 所有页面的 reset 集中到此处理
    auto btn = qobject_cast<QPushButton*>(sender());
    if (!btn)
        return;

    auto name = btn->objectName();
    if (name == tgReset) {
        auto t = findChild<QComboBox *>(tgLanuage);
        if (t) t->setCurrentText("English");
        t = findChild<QComboBox *>(tgLogLevel);
        if (t) t->setCurrentText("Debug");
    } else if (name == tiReset) {
        auto t = findChild<QComboBox *>(tiBorderStyle);
        if (t) t->setCurrentText("flipped");

        auto selColor = this->findChild<ColorParaBar *>(tiBorderColor);
        if (selColor) {
            selColor->setCurColor(QColor("#0e70ff"));
            selColor->update();
        }
        selColor = this->findChild<ColorParaBar *>(tiCrosshairColor);
        if (selColor) {
            selColor->setCurColor(QColor("#db000f"));
            selColor->update();
        }

        auto width = findChild<QSpinBox *>(tiBorderWidth);
        if (width) width->setValue(2);
        width = findChild<QSpinBox *>(tiCrosshairWidth);
        if (width) width->setValue(1);

        std::map<QString, bool> map;
        map.emplace(std::make_pair(tiSmartWindow, true));
        map.emplace(std::make_pair(tiCrosshair, false));
        map.emplace(std::make_pair(tiShowCursor, false));
        map.emplace(std::make_pair(tiAutoCopyToClipboard, true));
        for (const auto& it : map) {
            auto tCheckBox = findChild<QCheckBox*>(it.first);
            if (tCheckBox) tCheckBox->setChecked(it.second);
        }
    } else if (name == toReset) {
        auto spinBox = findChild<QSpinBox *>(toImageQuailty);
        if (spinBox) spinBox->setValue(-1);

        std::map<QString, QString> map;
        map.emplace(std::make_pair(toFileName, "flipped_$yyyyMMdd_hhmmss$.png"));
        map.emplace(std::make_pair(toQuickSavePath, QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first()));
        map.emplace(std::make_pair(toAutoSavePath, QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()));
        map.emplace(std::make_pair(toConfigPath, qApp->applicationDirPath() + "/data/config"));
        for (const auto& it : map) {
            auto edit = findChild<QCheckBox*>(it.first);
            if (edit) edit->setText(it.second);
        }
    } else if (name == tpReset) {
        auto tCheckBox = findChild<QCheckBox*>(tpWindowShadow);
        if (tCheckBox) tCheckBox->setChecked(true);

        auto spinbox = findChild<QSpinBox*>(tpOpacity);
        if (spinbox) spinbox->setValue(100);
        spinbox = findChild<QSpinBox*>(tpMaxSize);
        if (spinbox) spinbox->setValue(100000);
    } else {
    }
}

void Preference::onLanuageChange(const QString &language)
{
    auto bt = qobject_cast<QComboBox *>(sender());
    if (!bt)
        return;

    qDebug() << qApp->applicationDirPath() + "/config/config.ini";
    settingIni->beginGroup(INIT_GENERAL);
    settingIni->setValue(tgLanuage, bt->itemData(bt->currentIndex()).toString());
    settingIni->endGroup();

    auto lang = bt->itemData(bt->currentIndex()).toString();

//    QTranslator trans;
//    QString qmPath(QCoreApplication::applicationDirPath());
//    QString(_COMPILER_ID).compare("MSVC") == 0 ? qmPath += "/../../src/" + lang + ".qm" : qmPath = "/Users/winks/Desktop/projects/Flipped/src/i18n/" + lang + ".qm";
//    qDebug()<<"[*.qm path]:" << qmPath << _COMPILER_ID << "   "<< bool(QString(_COMPILER_ID).compare("MSVC") == 0);
//    trans.load(qmPath);
//    qApp->installTranslator(&trans);

    QTranslator trans;
    trans.load("/Users/winks/Desktop/projects/Flipped/src/i18n/zh_CN.qm");  // QLocale::system().name()
    qApp->installTranslator(&trans);
}

void Preference::onFontChange()
{
    QString t("SimSun");
    t = READ_CONFIG_INI(INIT_GENERAL, tgFont, t).toString();
    qDebug() << "t:" << t;

    bool ret = false;
    QFont font = QFontDialog::getFont(&ret, QFont(t.split(',').at(0)), this, tr("Select Font"));
    if (!ret)
        return;
        
    qApp->setFont(font);
    auto btnFont = findChild<QPushButton*>(tgFont);
    QString text = QString("%1, %2").arg(font.family()).arg(font.pointSize());
    btnFont->setText(text);

    WRITE_CONFIG_INI(INIT_GENERAL, tgFont, text);
    qDebug("当前选择的字体是[%s]-是否加粗[%d]-是否倾斜[%d]-字号[%d]", font.family().toUtf8().data(), font.bold(), font.italic(), font.pointSize());
}

void Preference::onSelfStart(int sta)
{
    WRITE_CONFIG_INI(INIT_GENERAL, tgSelfStarting, checkBoxState2Bool(sta));
}

void Preference::onAsAdmin(int sta)
{
    WRITE_CONFIG_INI(INIT_GENERAL, tgAsAdmin, checkBoxState2Bool(sta));
}

void Preference::onLogLevelChange(const QString& language)
{
    settingIni->beginGroup(INIT_GENERAL);
    settingIni->setValue(tgLogLevel, language);
    settingIni->endGroup();

    XLog::instance()->setLevel(language.toStdString());
}

void Preference::onAutoCheck(int sta)
{
    WRITE_CONFIG_INI(INIT_GENERAL, tgAutoCheckUpdate, checkBoxState2Bool(sta));
}

void Preference::onUpdate()
{
    // TODO 检查更新
}

void Preference::onBorderStyle(const QString& style)
{
    auto bt = qobject_cast<QComboBox *>(sender());
    if (!bt)
        return;

    const QString text = bt->itemData(bt->currentIndex()).toString();
    XHelper::instance().setBoardStyle(text);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiBorderStyle, text);
}

void Preference::onBorderColor(const QColor& color)
{
    auto t = color.name();
    XHelper::instance().setBorderColor(color);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiBorderColor, color.name());
}

void Preference::onBorderWidth(int val)
{
    XHelper::instance().setBorderWidth(val);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiBorderWidth, val);
}

void Preference::onCrosshairColor(const QColor& color)
{
    auto t = color.name();
    XHelper::instance().setCrosshairColor(color);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiCrosshairColor, color.name());
}

void Preference::onCrosshairWidth(int val)
{
    XHelper::instance().setCrosshairWidth(val);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiCrosshairWidth, val);
}


void Preference::onSmartWindow(int val)
{
    const bool b = checkBoxState2Bool(val);
    XHelper::instance().setSmartWindow(b);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiSmartWindow, b);
}

void Preference::onShowCrosshair(int val)
{
    const bool b = checkBoxState2Bool(val);
    XHelper::instance().setCrosshair(b);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiCrosshair, b);
}

void Preference::onShowCursor(int val)
{
    const bool b = checkBoxState2Bool(val);
    XHelper::instance().setShowCursor(b);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiShowCursor, b);
}

void Preference::onAutoCopyToClip(int val)
{
    const bool b = checkBoxState2Bool(val);
    XHelper::instance().setAutoCpoyClip(b);
    WRITE_CONFIG_INI(INIT_INTERFACE, tiAutoCopyToClipboard, b);
}

void Preference::onImageQuailty(int val)
{
    XHelper::instance().setImgQuailty(val);
    WRITE_CONFIG_INI(INIT_OUTPUT, toImageQuailty, val);
}

void Preference::onFileName(const QString& name)
{
    XHelper::instance().setPath(toFileName, name);
    WRITE_CONFIG_INI(INIT_OUTPUT, toFileName, name);
}

void Preference::onQuickSavePath(const QString& path)
{
    XHelper::instance().setPath(toQuickSavePath, path);
    WRITE_CONFIG_INI(INIT_OUTPUT, toQuickSavePath, path);
}

void Preference::onAutoSavePath(const QString& path)
{
    XHelper::instance().setPath(toAutoSavePath, path);
    WRITE_CONFIG_INI(INIT_OUTPUT, toAutoSavePath, path);
}

void Preference::onConfigPath(const QString& path)
{
    XHelper::instance().setPath(toConfigPath, path);
    WRITE_CONFIG_INI(INIT_OUTPUT, toConfigPath, path);
}

void Preference::onChoosePath()
{
    auto btn = qobject_cast<QPushButton*>(sender());
    auto btnQuickSavePath = findChild<QPushButton*>(toQuickSavePath);
    auto btnAutoSavePath = findChild<QPushButton*>(toAutoSavePath);
    auto btnConfigPath = findChild<QPushButton*>(toConfigPath);
    auto editQuickSavePath = findChild<QLineEdit*>(toQuickSavePath);
    auto editAutoSavePath = findChild<QLineEdit*>(toAutoSavePath);
    auto editConfigPath = findChild<QLineEdit*>(toConfigPath);

    if (!btn || !btnQuickSavePath || !btnAutoSavePath || !btnConfigPath
        || !editQuickSavePath || !editAutoSavePath || !editConfigPath)
        return;

    QString defPath;
    QString selPath;
    if (btn == btnQuickSavePath) {
        defPath = editQuickSavePath->text();
        selPath = QFileDialog::getExistingDirectory(this, tr("select a path"), defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            editQuickSavePath->setText(selPath);
    } else if (btn == btnAutoSavePath) {
        defPath = editAutoSavePath->text();
        selPath = QFileDialog::getExistingDirectory(this, tr("select a path"), defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            editAutoSavePath->setText(selPath);
    } else if (btn == btnConfigPath) {
        defPath = editConfigPath->text();
        selPath = QFileDialog::getExistingDirectory(this, tr("select a path"), defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            editConfigPath->setText(selPath);
    }
}

void Preference::onWindowShadow(int sta)
{
    bool checked = checkBoxState2Bool(sta);
    XHelper::instance().setWinShadow(checked);
    WRITE_CONFIG_INI(INIT_PIN, tpWindowShadow, checked);
}

void Preference::onOpacity(int val)
{
    XHelper::instance().setPinOpacity(val);
    WRITE_CONFIG_INI(INIT_PIN, tpOpacity, val);
}

void Preference::onMaxSize(int val)
{
    XHelper::instance().setPinMaxSize(val);
    WRITE_CONFIG_INI(INIT_PIN, tpMaxSize, val);
}

void Preference::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        translateUI();
        break;
    default:
        QWidget::changeEvent(e);
    }
}

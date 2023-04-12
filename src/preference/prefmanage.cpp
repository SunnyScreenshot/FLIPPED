#include "prefmanage.h"
#include "ui_prefmanage.h"

#include <map>
//#include <QUrl>
#include <QIcon>
#include <QString>

#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QtGlobal>
#include <QFontDialog>
#include <QFont>
#include <QApplication>
#include <QTranslator>
#include <QFileDialog>
//#include <QDesktopServices>
#include "../screen/tray.h"
#include "../screen/datamaid.h"
#include "../tool/base/colorparabar.h"
#include "../widget/xkeysequenceedit.h"

PrefManage::PrefManage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrefManage)
{
    ui->setupUi(this);
    initUI();

    connect(ui->tabWidget,  &QTabWidget::currentChanged, [this](int index){
        this->show();
        this->update();

//        ui->tabGeneral->adjustSize();
//        ui->tabInterface->adjustSize();
//        ui->tabOutput->adjustSize();
//        ui->tabHotkeys->adjustSize();
//        ui->tabPin->adjustSize();
//        ui->tabAbout->adjustSize();
        this->adjustSize();
    });
}

PrefManage::~PrefManage()
{
    delete ui;
}

void PrefManage::initUIGeneral()
{
    m_lanuages = { {"en_US", "English"}
                  ,{"zh_CN", "简体中文"}
                  ,{"zh_TW", "繁體中文"}};

    for (const auto& it : m_lanuages)
        ui->cbLanuage->addItem(it.second, it.first);

    const auto it = m_lanuages.find(DATAMAID->paraValue("lanuage").value<QString>());
    const QString strLanuage = (it != m_lanuages.end()) ? it->second : m_lanuages.at("en_US");

    ui->cbLanuage->setCurrentText(strLanuage);
    ui->btnFont->setText(DATAMAID->paraValue("font").value<QString>());
    ui->cbAutoRun->setChecked(DATAMAID->paraValue("autoRun").value<bool>());
    ui->cbLogLevel->setCurrentText(DATAMAID->paraValue("logLevel").value<QString>());
}

void PrefManage::initUIInterface()
{
    m_borderStyle = {  {tr("Flipped"), "Flipped"}
                      ,{tr("BalckAndWhite"), "BalckAndWhite"}
                      ,{tr("LightBlue"), "LightBlue"}};
    for (const auto& it : m_borderStyle)
        ui->cbBorderStyle->addItem(it.first, it.second);

    auto cpbHighLight = new ColorParaBar(ColorParaBarMode::CPB_HighLight);
    auto cpbCrosshair = new ColorParaBar(ColorParaBarMode::CPB_HighLight);
    cpbHighLight->setObjectName(tiBorderColor);
    cpbCrosshair->setObjectName(tiCrosshairColor);
    ui->interfaceLayout->addWidget(cpbHighLight, 1, 1);
    ui->interfaceLayout->addWidget(cpbCrosshair, 4, 1);

    ui->cbBorderStyle->setCurrentText(DATAMAID->paraValue("borderStyle").value<QString>());
    cpbHighLight->setCurColor(DATAMAID->paraValue("borderColor").value<QString>());
    ui->spBorderWidth->setValue(DATAMAID->paraValue("borderWidth").value<int>());
    cpbCrosshair->setCurColor(DATAMAID->paraValue("crosshairColor").value<QString>());
    ui->spCrosshairWidth->setValue(DATAMAID->paraValue("crosshairWidth").value<int>());
    ui->cbAutoDetectWindows->setChecked(DATAMAID->paraValue("smartWindow").value<bool>());
    ui->cbCrosshair->setChecked(DATAMAID->paraValue("crosshair").value<bool>());
    ui->cbAutoCopy2clipboard->setChecked(DATAMAID->paraValue(tiAutoCopy2Clipboard).value<bool>());

    connect(cpbHighLight, &ColorParaBar::sigColorChange, this, &PrefManage::onBorderColor);
    connect(cpbCrosshair, &ColorParaBar::sigColorChange, this, &PrefManage::onCrosshairColor);
}

void PrefManage::initUIOutput()
{
    ui->sbImageQuailty->setValue(DATAMAID->paraValue(toImageQuailty).value<int>());
    ui->leFileName->setText(DATAMAID->paraValue(toFileName).toString());
    ui->leConfig->setText(DATAMAID->paraValue(toConfigPath).toString());
    ui->leQuickSave->setText(DATAMAID->paraValue(toQuickSavePath).toString());
    ui->leAutoSave->setText(DATAMAID->paraValue(toAutoSavePath).toString());

    const bool quickSaveEnable = DATAMAID->paraValue(toQuickSave).toBool();
    ui->cbQuickSave->setChecked(quickSaveEnable);
    ui->btnQuickSave->setEnabled(quickSaveEnable);
    ui->leQuickSave->setEnabled(quickSaveEnable);
    const bool autoSaveEnable = DATAMAID->paraValue(toAutoSave).toBool();
    ui->cbAutoSave->setChecked(autoSaveEnable);
    ui->btnAutoSave->setEnabled(autoSaveEnable);
    ui->leAutoSave->setEnabled(autoSaveEnable);

    const QString tooltip = DATAMAID->formatToFileName(ui->leFileName->text());
    ui->leFileName->setToolTip(tooltip);

//    QString tip = "Shift + F4";
//    cbQuickSave->setToolTip(tip);
}

void PrefManage::initUIPin()
{
    ui->sbOpacity->setValue(DATAMAID->paraValue(tpOpacity).value<int>());
    ui->sbMaxSize->setValue(DATAMAID->paraValue(tpMaxSize).value<int>());
}

void PrefManage::initUIHotkeys()
{
    int row = 0;
    int col = 2;
//    QStringList list = {  DATAMAID->paraValue(thScrnCapture).toString()
//                         ,DATAMAID->paraValue(thDelayCapture).toString()
//                         ,DATAMAID->paraValue(thFullScreen).toString()};
//    std::map<XKeySequenceEdit*, const QString> vHkEdit;
//    int idx = 0;
    for (auto& it : Tray::instance().getVHotKeys()) {
        auto& hk = std::get<0>(it);
        QString& hotkey = std::get<1>(it);
        QString& describe = std::get<2>(it);
        qDebug() << "----->t0:" << hk << "    hotkey:" << hotkey << "    describe:" << describe;

        XKeySequenceEdit* pEdit = new XKeySequenceEdit(QKeySequence(hotkey));
        ui->hotkeyLayout->addWidget(pEdit, row++, col);

//        vHkEdit.insert(std::make_pair(pEdit, list.at(idx++)));
        pEdit->setObjectName(describe);
        pEdit->setMinimumWidth(110 * DATAMAID->scale());
        const bool reg = hk->isRegistered();
        if (reg) {
            pEdit->setStyleSheet("background-color: #98fb98;");
        } else {
            pEdit->setStyleSheet("background-color: #ff7f50;");
        }

        connect(pEdit, &XKeySequenceEdit::sigKeySeqChanged, &(Tray::instance()), &Tray::onKeySequenceChanged);
    }
}

void PrefManage::initUIAbout()
{
    ui->labLogo->setPixmap(QIcon(":/resources/logo.png").pixmap(QSize(200, 200)));

    QString bit;
    if (_BIT_ARCH == 4) bit = "x86";
    else if (_BIT_ARCH == 8) bit = "x64";
    ui->labVersion->setText(tr("%1-Beta %2 (%3)")
                            .arg(_PROJECT_VERSION)
                            .arg(bit)
                            .arg(_VERSION_BUILD_TIME));

}

void PrefManage::initUI()
{
    initUIGeneral();
    initUIInterface();
    initUIOutput();
    initUIPin();
    initUIHotkeys();
    initUIAbout();
}

bool PrefManage::cbState2Bool(int state)
{
    return state == Qt::Checked ? true : false;
}

void PrefManage::onBorderColor(const QColor &color)
{
    DATAMAID->setParaValue(tiBorderColor, color.name());
}

void PrefManage::onCrosshairColor(const QColor &color)
{
    DATAMAID->setParaValue(tiCrosshairColor, color.name());
}

void PrefManage::on_cbBorderStyle_currentTextChanged(const QString &style)
{
    Q_UNUSED(style);
    auto btn = ui->cbBorderStyle;
    const QString dataStyle = btn->itemData(btn->currentIndex()).toString();
    DATAMAID->setParaValue(tiBorderStyle, dataStyle);
}


void PrefManage::on_spBorderWidth_valueChanged(int val)
{
    DATAMAID->setParaValue(tiBorderWidth, val);
}


void PrefManage::on_spCrosshairWidth_valueChanged(int val)
{
    DATAMAID->setParaValue(tiCrosshairWidth, val);
}


void PrefManage::on_cbAutoDetectWindows_stateChanged(int val)
{
    DATAMAID->setParaValue(tiSmartWindow, cbState2Bool(val));
}


void PrefManage::on_cbCrosshair_stateChanged(int val)
{
    DATAMAID->setParaValue(tiCrosshair, cbState2Bool(val));
}


void PrefManage::on_cbAutoCopy2clipboard_stateChanged(int val)
{
    DATAMAID->setParaValue(tiAutoCopy2Clipboard, cbState2Bool(val));
}


QString PrefManage::onSelectPath()
{
    QString selPath = "";
    auto btn = qobject_cast<QPushButton*>(sender());
    if (!btn) selPath;

    QString defPath = "";
    const QString title(tr("Select a path"));
    if (btn == ui->btnQuickSave) {
        defPath = ui->leQuickSave->text();
        selPath = QFileDialog::getExistingDirectory(this, title, defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            ui->leQuickSave->setText(selPath);
    } else if (btn == ui->btnAutoSave) {
        defPath = ui->leAutoSave->text();
        selPath = QFileDialog::getExistingDirectory(this, title, defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            ui->leAutoSave->setText(selPath);
    } else if (btn == ui->btnConfig) {
        defPath = ui->leConfig->text();
        selPath = QFileDialog::getExistingDirectory(this, title, defPath, QFileDialog::ShowDirsOnly);

        if (!selPath.isEmpty() && defPath != selPath)
            ui->leConfig->setText(selPath);
    }

    return selPath;
}

void PrefManage::on_sbOpacity_valueChanged(int val)
{
    DATAMAID->setParaValue(tpOpacity, val);
}

void PrefManage::on_spMaxSize_valueChanged(int val)
{
    DATAMAID->setParaValue(tpMaxSize, val);
}

void PrefManage::on_generalReset_released()
{
    qDebug() << "-----------1";

//    cbLanuage
//    btnFont
//    cbAutoRun
//    cbLogLevel

}


void PrefManage::on_interfaceReset_released()
{
    qDebug() << "-----------2";
//    cbBorderStyle
//    spBorderWidth
//    spCrosshairWidth
//    cbAutoDetectWindows
//    cbCrosshair
//    cbAutoCopy2clipboard
}


void PrefManage::on_outputReset_released()
{
    qDebug() << "-----------3";
//    spImageQuailty
//    leFileName
//    leConfig
//    leQuickSave
//    leAutoSave

// btnConfig
// btnQuickSave
// btnAutoSave

// cbQuickSave
// cbAutoSave
}


void PrefManage::on_pinReset_released()
{
    qDebug() << "-----------4";
//    spOpacity
//    spMaxSize
}


void PrefManage::on_hotkeysReset_released()
{
    qDebug() << "-----------5";
//    构建时，需自行标注对象名称
//    hkCapture
//    hkDelayCapture
    //    hkFullScreCapture
}


void PrefManage::on_sbImageQuailty_valueChanged(int val)
{
    DATAMAID->setParaValue(toImageQuailty, val);
}


void PrefManage::on_leFileName_textChanged(const QString &name)
{
    DATAMAID->setParaValue(toFileName, name);
}


void PrefManage::on_leConfig_textChanged(const QString &path)
{
    DATAMAID->setParaValue(toConfigPath, onSelectPath());
}


void PrefManage::on_leQuickSave_textChanged(const QString &path)
{
    DATAMAID->setParaValue(toQuickSavePath, onSelectPath());
}


void PrefManage::on_leAutoSave_textChanged(const QString &path)
{
    DATAMAID->setParaValue(toAutoSavePath, onSelectPath());
}


void PrefManage::on_cbQuickSave_stateChanged(int val)
{
    const bool enbale = cbState2Bool(val);
    DATAMAID->setParaValue(toQuickSave, enbale);

    ui->btnQuickSave->setEnabled(enbale);
    ui->leQuickSave->setEnabled(enbale);
}


void PrefManage::on_cbAutoSave_stateChanged(int val)
{
    const bool enbale = cbState2Bool(val);
    DATAMAID->setParaValue(toAutoSave, enbale);

    ui->btnAutoSave->setEnabled(enbale);
    ui->leAutoSave->setEnabled(enbale);
}


void PrefManage::on_cbLanuage_currentTextChanged(const QString &language)
{
    qDebug() << qApp->applicationDirPath() + "/config/config.ini" << " language:" << language;
    auto btn = ui->cbLanuage;
    const QString strLanuage = btn->itemData(btn->currentIndex()).toString();
    DATAMAID->setParaValue("lanuage", strLanuage);

    QTranslator trans;
    QString qmPath(qApp->applicationDirPath());
#if defined(Q_OS_MAC)
    qmPath += "/../../../../bin/translations/" + strLanuage + ".qm";
#else
    qmPath += "/translations/" + strLanuage + ".qm";
#endif
    trans.load(qmPath);
    qApp->installTranslator(&trans);

    ui->retranslateUi(this);
}


void PrefManage::on_btnFont_released()
{
    bool ret = false;
    const auto fontFamily = DATAMAID->paraValue("font").toString();
    QFont font = QFontDialog::getFont(&ret, QFont(fontFamily.split(',').at(0)), this, tr("Select Font"));
    if (!ret) return;

    qApp->setFont(font);
    QString text = QString("%1, %2").arg(font.family()).arg(font.pointSize());
    ui->btnFont->setText(text);

    DATAMAID->setParaValue(tgFont, text);
    qDebug("当前选择的字体是[%s]-是否加粗[%d]-是否倾斜[%d]-字号[%d]", font.family().toUtf8().data(), font.bold(), font.italic(), font.pointSize());
}


void PrefManage::on_cbAutoRun_stateChanged(int sta)
{
    DATAMAID->setParaValue(tgAutoRun, cbState2Bool(sta));
    DATAMAID->setAutoRun();
}


void PrefManage::on_cbLogLevel_currentTextChanged(const QString &level)
{
    DATAMAID->setParaValue(tgLogLevel, level);
}


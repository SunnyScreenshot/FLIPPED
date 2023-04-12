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

bool PrefManage::cbState2Bool(int state)
{
    return state == Qt::Checked ? true : false;
}

void PrefManage::onLanuage(const QString &language)
{
    qDebug() << qApp->applicationDirPath() + "/config/config.ini";
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

void PrefManage::onFont()
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

void PrefManage::onAutoRun(int sta)
{
    DATAMAID->setParaValue(tgAutoRun, cbState2Bool(sta));
    DATAMAID->setAutoRun();
}

void PrefManage::onLogLevel(const QString &language)
{
    DATAMAID->setParaValue(tgLogLevel, language);
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


    connect(ui->cbLanuage, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, &PrefManage::onLanuage);
    connect(ui->btnFont, &QPushButton::released, this, &PrefManage::onFont);
    connect(ui->cbAutoRun, &QCheckBox::stateChanged, this, &PrefManage::onAutoRun);
    connect(ui->cbLogLevel, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, &PrefManage::onLogLevel);
}

void PrefManage::initUIInterface()
{
    ui->interfaceLayout->addWidget(new ColorParaBar(ColorParaBarMode::CPB_HighLight), 1, 1);
    ui->interfaceLayout->addWidget(new ColorParaBar(ColorParaBarMode::CPB_HighLight), 4, 1);
}

void PrefManage::initUIOutput()
{

}

void PrefManage::initUIPin()
{

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

//    QDesktopServices::openUrl(QUrl(QString("https://www.baidu.com")));


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



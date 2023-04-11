#include "prefmanage.h"
#include "ui_prefmanage.h"

#include <map>
//#include <QUrl>
#include <QIcon>
#include <QString>
//#include <QDesktopServices>
#include "../../screen/tray.h"
#include "../../screen/datamaid.h"
#include "../../tool/base/colorparabar.h"
#include "../../widget/xkeysequenceedit.h"

PrefManage::PrefManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrefManage)
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

void PrefManage::initUIInterface()
{
    ui->interfaceLayout->addWidget(new ColorParaBar(ColorParaBarMode::CPB_HighLight), 1, 1);
    ui->interfaceLayout->addWidget(new ColorParaBar(ColorParaBarMode::CPB_HighLight), 4, 1);
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
    initUIInterface();
    initUIHotkeys();
    initUIAbout();
}

//    QDesktopServices::openUrl(QUrl(QString("https://www.baidu.com")));


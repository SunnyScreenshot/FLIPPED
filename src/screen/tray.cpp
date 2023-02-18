// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

#include "tray.h"
#include "../preference/preference.h"
#include "../widget/xkeysequenceedit.h"
#include "../pluginsinterface/iplugininterface.h"
#include "../screen/drawhelper.h"
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfoList>
#include <QKeySequence>
#include <QHotkey>
#include <QMetaEnum>

#include "QThread"
//#include "../../pluginsimpl/watemark/pluginwatemark.h"

/*!
 * \brief WinFullScreen::instance 单例的实现
 * \return 返回单例的引用
 * \note 问：类的static变量在什么时候初始化？函数的static变量在什么时候初始化？
 * \li 答：类的静态成员变量在类实例化之前就已经存在了，并且分配了内存。函数的static变量在执行此函数时进行初始化。
 */
Tray& Tray::instance()
{
    static Tray tray;
    return tray;
}

std::vector<std::tuple<QHotkey*, QString, QString, CaptureHelper::CaptureType>> Tray::getVHotKeys() const
{
    return m_vHotKeys;
}

Tray::Tray(QObject *parent)
    : QObject(parent)
	, m_pSrnShot(nullptr)
    , m_pPref(nullptr)
	, m_trayIconMenu(new QMenu())
    , m_trayIcon(new QSystemTrayIcon(this))
    //, m_hkManage(new HotkeySvs(this))
{
    init();
    initGlobalHotKeys();




//	QString t = QApplication::instance()->applicationDirPath() + "/../../pluginsimpl/watemark/RelWithDebInfo";
//    QDir pluginsDir(t);
//        QStringList nameFilters;
//        nameFilters<<"*.so"<<"*.dylib"<<"*.dll";
//        QFileInfoList plugins = pluginsDir.entryInfoList(nameFilters, QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
//
//        foreach(QFileInfo plugin, plugins){
//            QPluginLoader pluginLoader(plugin.absoluteFilePath(), this);
//            IPluginInterface *pPlugin=qobject_cast<IPluginInterface *>(pluginLoader.instance());
//            if (pPlugin){
//                QString temp= pPlugin->plugName();
////                QAction *action = new QAction(plugin_ptr->name());
////                editMenu->addAction(action);
////                editToolBar->addAction(action);
////                editPlugins[plugin_ptr->name()]=plugin_ptr;
////                connect(action,SIGNAL(triggered(bool)),this,SLOT(pluginPerform()));
//
////                pPlugin.unload();
//            }else{
//                qDebug()<<"bad plugin:"<<plugin.absoluteFilePath();
//            }
//        }
}

Tray::~Tray()
{
    if (m_pSrnShot)
        m_pSrnShot->deleteLater();

    if (m_pPref)
        m_pPref->deleteLater();

    delete m_trayIconMenu;
    m_trayIconMenu = nullptr;
}

void Tray::init()
{
    QAction* srnShot = new QAction(tr("ScreenShot"), this);
    QAction* preference = new QAction(tr("Preference"), this);
    QAction* quit = new QAction(tr("Quit"), this);

	m_trayIconMenu->addAction(srnShot);
    m_trayIconMenu->addAction(preference);
	m_trayIconMenu->addSeparator();
	m_trayIconMenu->addAction(quit);

    m_trayIcon->setIcon(QIcon(":/resources/logo.png"));
    m_trayIcon->setToolTip(tr(_PROJECT_NAME));
	m_trayIcon->setContextMenu(m_trayIconMenu);

	connect(srnShot, &QAction::triggered, this, &Tray::onSrnShot);
    connect(preference, &QAction::triggered, this, &Tray::onPreference);
	connect(quit, &QAction::triggered, [](){qApp->quit();});

#ifdef Q_OS_WIN
    // Ensure proper removal of tray icon when program quits on Windows.
    connect(qApp, &QCoreApplication::aboutToQuit, m_trayIcon, &QSystemTrayIcon::hide);
#endif

    m_trayIcon->show();
}


void Tray::initGlobalHotKeys()
{
    m_vHotKeys = {
        std::make_tuple(nullptr, "Ctrl+Shift+A", tr("Active Window"), CaptureHelper::SST_ActionWindow),
        //std::make_tuple(nullptr, "Ctrl+Shift+W", tr("Scrolling Window"), ScrnShotType::SST_ScrollingWindow),
        std::make_tuple(nullptr, "Ctrl+Shift+L", tr("Delay Capture"), CaptureHelper::SST_DelayCapture),
        std::make_tuple(nullptr, "Ctrl+Shift+S", tr("Full Screen"), CaptureHelper::SST_FullScreen),
//        std::make_tuple(nullptr, "Ctrl+Shift+F", tr("Fixd-Size Region"), ScrnShotType::SST_FixdSizeRegion),
//        std::make_tuple(nullptr, "Ctrl+Shift+T", tr("Paste"), ScrnShotType::SST_Paste),
//        std::make_tuple(nullptr, "Ctrl+Shift+H", tr("Hide/Show all images"), ScrnShotType::SST_HideShowAllImages),
//        std::make_tuple(nullptr, "Ctrl+Shift+X", tr("Switch current group"), ScrnShotType::SST_SwitchCurrentGroup)
    };

    settingIni->beginGroup(INIT_HOTKEYS);
    for (auto& it : m_vHotKeys) {
        auto& pHK = std::get<0>(it);    // QHotkey*& 指针的引用类型
        QString& hotkey = std::get<1>(it);
        QString& describe = std::get<2>(it);
        const CaptureHelper::CaptureType sst = std::get<3>(it);
        const QString strHotKey = settingIni->value(describe, hotkey).toString();
        if (!strHotKey.isEmpty())
            hotkey = strHotKey;

        pHK =  new QHotkey(QKeySequence(hotkey), true, qApp);
        QMetaEnum enumSst = QMetaEnum::fromType<CaptureHelper::CaptureType>();
        pHK->setObjectName(enumSst.valueToKey(sst));
        connect(pHK, &QHotkey::activated, this, &Tray::onSrnShot);
        
        qDebug() << "pHK: " << pHK << Qt::endl
            << "std::get<0>(it):" << std::get<0>(it) << Qt::endl
            << "hotkey:" << hotkey << "    hk Is Registered:" << pHK->isRegistered() << Qt::endl;
    }
    settingIni->endGroup();
}

void Tray::onSrnShot()
{
    // 第二次就是野指针, 故使用 QPointer 解决 https://blog.csdn.net/luoyayun361/article/details/90199081
    if (!m_pSrnShot)
        m_pSrnShot = new ScreenShot();

    QHotkey* hk = qobject_cast<QHotkey*>(sender());
    const auto act = qobject_cast<QAction*>(sender());
    if (!hk && !act)
        return;

    int sst = CaptureHelper::SST_Unknow;
    if (hk) {
        bool ok = false;
        QMetaEnum metaSst = QMetaEnum::fromType<CaptureHelper::CaptureType>();
        sst = metaSst.keyToValue(hk->objectName().toStdString().c_str(), &ok);
    }

    if (act || sst == CaptureHelper::SST_ActionWindow
        || sst == CaptureHelper::SST_DelayCapture
        || sst == CaptureHelper::SST_FullScreen)
        m_pSrnShot->launchCapture(static_cast<CaptureHelper::CaptureType>(sst));
}

void Tray::onPreference(bool checked)
{
    Q_UNUSED(checked);

    if (!m_pPref) m_pPref = new Preference();
    m_pPref->setVisible(true);
}

void Tray::onKeySequenceChanged(const QKeySequence& keySequence)
{
    XKeySequenceEdit* editor = qobject_cast<XKeySequenceEdit*>(sender());
    if (!editor)
        return;

    settingIni->beginGroup(INIT_HOTKEYS);
    for (auto& it : m_vHotKeys) {
        auto& pHK = std::get<0>(it);                                          // QHotkey*& 指针的引用类型
//        QString& hotkey = std::get<1>(it);
        QString& describe = std::get<2>(it);

        if (editor->objectName() == describe) {
            auto prev = pHK->shortcut();
            qDebug() << "----->prev:" << prev << "   keySequence:" << keySequence;
            if (prev == keySequence || keySequence.isEmpty())
                return;

            pHK->setShortcut(keySequence, true);

            qDebug() << "#editor->keySequence():" << editor->keySequence() << "  keySequence:" << keySequence 
                << "  pHK:" << pHK << "  pHK->shortcut():" << pHK->shortcut();

            if (pHK->isRegistered()) {
                settingIni->setValue(describe, keySequence.toString());
                editor->setStyleSheet("background-color: #98fb98;");
            } else {
                //pHK->setShortcut(prev);
                editor->setStyleSheet("background-color: #ff7f50;");
            }
        } else {
            continue;
        }
    }

    settingIni->endGroup();
}

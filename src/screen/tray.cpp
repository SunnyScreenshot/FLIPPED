/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.10.08
 * Description:
 ******************************************************************/
#include "tray.h"
#include "../preference/preference.h"
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
#include "../pluginsinterface/iplugininterface.h"
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

Tray::Tray(QObject *parent)
    : QObject(parent)
	, m_pSrnShot(nullptr)
    , m_pPref(nullptr)
	, m_trayIconMenu(new QMenu())
    , m_trayIcon(new QSystemTrayIcon(this))
    //, m_hkManage(new HotkeySvs(this))
{
    init();

    QHotkey* m_hkAW = new QHotkey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Y), true, qApp);                   // Active Window 截图（手动矩形）
    QHotkey* m_hkWO = new QHotkey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W), true, qApp);                   // Window / Object 窗口/对象截图

    connect(m_hkAW, &QHotkey::activated, this, &Tray::onSrnShot);
    connect(m_hkWO, &QHotkey::activated, this, &Tray::onSrnShot);

    qDebug() << "m_hkAW Is Registered: " << m_hkAW->isRegistered();
    qDebug() << "m_hkWO Is Registered: " << m_hkWO->isRegistered();




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

    m_trayIcon->setIcon(QIcon(":/resources/logo.svg"));
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


void Tray::onSrnShot()
{
    // 第二次就是野指针, 故使用 QPointer 解决 https://blog.csdn.net/luoyayun361/article/details/90199081
    if (!m_pSrnShot)
        m_pSrnShot = new ScreenShot();

    m_pSrnShot->getScrnShots();
    m_pSrnShot->activateWindow();
    m_pSrnShot->setFocus();
}

void Tray::onPreference(bool checked)
{
    Q_UNUSED(checked);
    if (!m_pPref)
        m_pPref = new Preference();

    m_pPref->setVisible(true);
}

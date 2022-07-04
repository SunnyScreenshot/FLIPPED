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
#include "winresetbtn.h"
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfoList>
#include "../pluginsinterface/iplugininterface.h"
//#include "../../pluginsimpl/watemark/pluginwatemark.h"

/*!
 * \brief WinFullScreen::instance 单例的实现
 * \return 返回单例的引用
 * \note 问：类的static变量在什么时候初始化？函数的static变量在什么时候初始化？
 * \li 答：类的静态成员变量在类实例化之前就已经存在了，并且分配了内存。函数的static变量在执行此函数时进行初始化。
 */
// 此准备改写为单例。
Tray& Tray::instance()
{
    static Tray tray;
    return tray;
}

Tray::Tray(QObject *parent)
    : QObject(parent)
	, m_pSrnShot(nullptr)
	, m_trayIconMenu(new QMenu())
    , m_trayIcon(new QSystemTrayIcon(this))
    //, m_winMain(nullptr)
{
	init();

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

    delete m_trayIconMenu;
    m_trayIconMenu = nullptr;
}

void Tray::init()
{
    QAction* srnShot = new QAction(tr("ScreenShot"), this);
    //QAction* setting = new QAction(tr("Config"), this);
    QAction* quit = new QAction(tr("Quit"), this);

	m_trayIconMenu->addAction(srnShot);
	//m_menuTary->addAction(setting);
	m_trayIconMenu->addSeparator();
	m_trayIconMenu->addAction(quit);

    m_trayIcon->setIcon(QIcon(":/resources/logo.svg"));
	m_trayIcon->setToolTip(tr("PicShot"));
	m_trayIcon->setContextMenu(m_trayIconMenu);
	
    //m_winMain = new WinSetting();

	connect(srnShot, &QAction::triggered, this, &Tray::onSrnShot);
	//connect(setting, &QAction::triggered, this, &Tray::onSetting);
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

void Tray::onSetting(bool checked)
{
	Q_UNUSED(checked)
    //m_winMain->show();
}

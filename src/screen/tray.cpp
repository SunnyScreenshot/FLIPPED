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

Tray::Tray(QObject *parent)
    : QObject(parent)
	, m_pSrnShot(nullptr)
	, m_screenShot(nullptr)
	, m_showWinConfig(nullptr)
	, m_quit(nullptr)
	, m_menuTary(nullptr)
    , m_sysTary(nullptr)
    , m_winMain(nullptr)
{
	init();

	QString t = QApplication::instance()->applicationDirPath() + "/../../pluginsimpl/watemark/RelWithDebInfo";
    QDir pluginsDir(t);
        QStringList nameFilters;
        nameFilters<<"*.so"<<"*.dylib"<<"*.dll";
        QFileInfoList plugins = pluginsDir.entryInfoList(nameFilters, QDir::NoDotAndDotDot | QDir::Files, QDir::Name);


        foreach(QFileInfo plugin,plugins){
            QPluginLoader pluginLoader(plugin.absoluteFilePath(), this);
            IPluginInterface *pPlugin=qobject_cast<IPluginInterface *>(pluginLoader.instance());
            if (pPlugin){
                QString temp= pPlugin->plugName();
//                QAction *action = new QAction(plugin_ptr->name());
//                editMenu->addAction(action);
//                editToolBar->addAction(action);
//                editPlugins[plugin_ptr->name()]=plugin_ptr;
//                connect(action,SIGNAL(triggered(bool)),this,SLOT(pluginPerform()));

//                pPlugin.unload();
            }else{
                qDebug()<<"bad plugin:"<<plugin.absoluteFilePath();
            }


        }
    //调用部分
//    IPluginInterface* pDll = nullptr;
//    QPluginLoader plugin_loader("../pluginsimpl/watemark/watemark.dll");
//    QObject* plugin = plugin_loader.instance();
//    if(plugin)
//    {
//        pDll = qobject_cast<IPluginInterface*>(plugin);
//        if(pDll)
//        {
//            QString temp= pDll->plugName();
//        }
//        //delete plugin;
//        plugin_loader.unload();
//    }

}

Tray::~Tray()
{
}

void Tray::init()
{
	m_screenShot = new QAction(tr("ScreenShot"), this);
	m_showWinConfig = new QAction(tr("Config"), this);
	m_quit = new QAction(tr("Quit"), this);

	m_menuTary = new QMenu();
	m_menuTary->addAction(m_screenShot);
	m_menuTary->addAction(m_showWinConfig);
	m_menuTary->addSeparator();
	m_menuTary->addAction(m_quit);

	m_sysTary = new QSystemTrayIcon(this);
	m_sysTary->setIcon(QIcon(":/resources/PicShot_32.svg"));
	m_sysTary->setToolTip(tr("PicShot"));
	m_sysTary->setContextMenu(m_menuTary);
	m_sysTary->setVisible(true);

    //m_winMain = new WinSetting();

	connect(m_screenShot, &QAction::triggered, this, &Tray::onScreenShot);
	connect(m_showWinConfig, &QAction::triggered, this, &Tray::onShowWinConfig);
	connect(m_quit, &QAction::triggered, []() {
		qApp->quit();
		});
}

void Tray::onScreenShot()
{
    // 第二次就是野指针, 故使用 QPointer 解决 https://blog.csdn.net/luoyayun361/article/details/90199081
    if (!m_pSrnShot)
        m_pSrnShot = new ScreenShot();

    m_pSrnShot->getScrnShots();
    m_pSrnShot->activateWindow();
    m_pSrnShot->setFocus();
}

void Tray::onShowWinConfig(bool checked)
{
	Q_UNUSED(checked)
    //m_winMain->show();
}

//
// Created by xmuli on 2021/10/08.
//

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


Tray::Tray(QObject *parent)
    : QObject(parent)
	, m_screenShot(nullptr)
	, m_showWinConfig(nullptr)
	, m_quit(nullptr)
	, m_menuTary(nullptr)
    , m_sysTary(nullptr)
    , m_winMain(nullptr)
{
	init();

    QDir pluginsDir(QApplication::instance()->applicationDirPath()+"/../pluginsimpl/watemark");
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
	m_winMain->deleteLater();
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

    m_winMain = new WinSetting();

	connect(m_screenShot, &QAction::triggered, this, &Tray::onScreenShot);
	connect(m_showWinConfig, &QAction::triggered, this, &Tray::onShowWinConfig);
	connect(m_quit, &QAction::triggered, []() {
		qApp->quit();
		});
}

void Tray::onScreenShot()
{
	// TODO 2021-10-08 模拟按下快捷键或后续的封装函数
    ScreenShot::instance().getScrnShots();
}

void Tray::onShowWinConfig(bool checked)
{
	Q_UNUSED(checked)
    m_winMain->show();
}

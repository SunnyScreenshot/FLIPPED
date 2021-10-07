//
// Created by xmuli on 2021/10/08.
//

#include "tary.h"
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QIcon>


Tary::Tary(QObject *parent)
    : QObject(parent)
	, m_screenShot(nullptr)
	, m_showWinConfig(nullptr)
	, m_quit(nullptr)
	, m_menuTary(nullptr)
	, m_sysTary(nullptr)
{
	init();
}

Tary::~Tary()
{

}

void Tary::init()
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
}

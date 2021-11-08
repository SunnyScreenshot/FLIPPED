//
// Created by xmuli on 2021/10/08.
//

#ifndef TARY_H
#define TRAY_H

#include <QObject>
#include <QHotkey>
#include "winmain.h"
#include "winfullscreen.h"

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;
class QAction;
class QMenu;
QT_END_NAMESPACE


class Tray : public QObject
{
    Q_OBJECT
public:
    explicit Tray(QObject *parent = nullptr);
    virtual ~Tray();

private:
	void init();

public slots:
	void onScreenShot();
	void onShowWinConfig(bool checked);
	
private:
	QAction* m_screenShot;
	QAction* m_showWinConfig;
	QAction* m_quit;
	QMenu* m_menuTary;
	QSystemTrayIcon* m_sysTary;

    WinMain* m_winMain;
};

#endif // TARY_H

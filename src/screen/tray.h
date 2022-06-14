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
#ifndef TRAY_H
#define TRAY_H

#include <QObject>
#include <QHotkey>
#include <QPointer>
#include "winsetting.h"
#include "screenshot.h"

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;
class QAction;
class QMenu;
QT_END_NAMESPACE

class Tray : public QObject
{
    Q_OBJECT
public:
	static Tray& instance();

private:
    explicit Tray(QObject* parent = nullptr);
    virtual ~Tray();
	void init();

public slots:
	void onSrnShot();
	void onSetting(bool checked);
	
private:
	QPointer<ScreenShot> m_pSrnShot;
	QMenu* m_trayIconMenu;
    QPointer<QSystemTrayIcon> m_trayIcon;
    //WinSetting* m_winMain;
};

#endif // TARY_H
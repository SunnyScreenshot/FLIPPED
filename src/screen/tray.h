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
#include "screenshot.h"
#include "hotkeysvs.h"

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;
class QAction;
class QMenu;
QT_END_NAMESPACE

class QHotkey;
class Preference;

class Tray : public QObject
{
    Q_OBJECT
    friend class HotkeySvs;

public:
	static Tray& instance();

private:
    explicit Tray(QObject* parent = nullptr);
    virtual ~Tray();
	void init();

public slots:
	void onSrnShot();
    void onPreference(bool checked);
	
private:
	QPointer<ScreenShot> m_pSrnShot;           // 前台截图 UI
    QPointer<Preference> m_pPref;              // 偏好设置 UI 
	QMenu* m_trayIconMenu;
    QPointer<QSystemTrayIcon> m_trayIcon;

    //QPointer<HotkeySvs> m_hkManage;
};

#endif // TARY_H

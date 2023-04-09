// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

#ifndef TRAY_H
#define TRAY_H

#include <QObject>
#include <QHotkey>
#include <QPointer>
#include <QSystemTrayIcon>
#include "screenshot.h"

class QSystemTrayIcon;
class QAction;
class QMenu;
class QHotkey;
class Preference;

class Tray : public QObject
{
    Q_OBJECT
public:
	static Tray& instance();
    std::vector<std::tuple<QHotkey*, QString, QString, CaptureHelper::CaptureType>> getVHotKeys() const;

private:
    explicit Tray(QObject* parent = nullptr);
    virtual ~Tray();
	void init();
    void initGlobalHotKeys();

public slots:
	void onSrnShot();
    void onPreference(bool checked);
    void onKeySequenceChanged(const QKeySequence& keySequence);

    void onNotificQuickSave(const bool bSaveOk, const QString& pathName);
    void onNotificHotkeyRegisteredFail(std::map<const QString, const QString> map);

    void onActivated(QSystemTrayIcon::ActivationReason reason);
    // tabGeneral
    //void onLanuageChange(const QString& language);
    //void onSelfStart(int sta);
    //void onAsAdmin(int sta);
    //void onAutoCheck(int sta);
    //void onUpdate();

private:
	QPointer<ScreenShot> m_pSrnShot;           // 前台截图 UI
    QPointer<Preference> m_pPref;              // 偏好设置 UI 
	QMenu* m_trayIconMenu;
    QPointer<QSystemTrayIcon> m_trayIcon;

    std::vector<std::tuple<QHotkey*, QString, QString, CaptureHelper::CaptureType>> m_vHotKeys;
};

#endif // TARY_H

/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.05
 * Description: singletion hotkeys 创建热键的单例
 ******************************************************************/
#include "hotkeysvs.h"
#include <QKeySequence>
#include <QCoreApplication>
#include <QHotkey>
#include "tray.h"

HotkeySvs::HotkeySvs(Tray *tray, QObject *parent)
    : QObject(parent)
    , m_tray(tray)
    , m_hkAW(new QHotkey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Y), true, qApp))
    , m_hkWO(new QHotkey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W), true, qApp))
//    , m_hkDC(new QHotkey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_L), true, qApp))
//    , m_hkFS(new QHotkey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), true, qApp))
//    , m_hkFSR(new QHotkey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F), true, qApp))
{
    init();
}

void HotkeySvs::init()
{
    if (!m_tray)
        return;

    connect(m_hkAW, &QHotkey::activated, m_tray, &Tray::onSrnShot);
    connect(m_hkWO, &QHotkey::activated, m_tray, &Tray::onSrnShot);

//    connect(m_hkDC, &QHotkey::activated, this, &WinSetting::onScrnShotDelay);
//    connect(m_hkFS, &QHotkey::activated, this, &WinSetting::onScrnShotFixedSize);
//    connect(m_hkFSR, &QHotkey::activated, this, &WinSetting::onScrnShotFullScreen);

    qDebug() << "m_hkAW Is Registered: " << m_hkAW->isRegistered();
    qDebug() << "m_hkWO Is Registered: " << m_hkWO->isRegistered();
}


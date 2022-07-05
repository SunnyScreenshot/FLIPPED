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
#ifndef HOTKEYSVS_H
#define HOTKEYSVS_H

#include <QObject>

class Tray;
class QHotkey;
class HotkeySvs: public QObject
{
    Q_OBJECT
public:
    explicit HotkeySvs(Tray* tray = nullptr, QObject *parent=nullptr);
    ~HotkeySvs() = default;

    void init();

private:
    Tray* m_tray;
    QHotkey* m_hkAW;                   // Active Window 截图（手动矩形）
    QHotkey* m_hkWO;                   // Window / Object 窗口/对象截图
//    QHotkey* m_hkDC;                   // Delay Capture 延时截图
//    QHotkey* m_hkFS;                   // Full Screen 整个屏幕截图
//    QHotkey* m_hkFSR;                  // Fixd-Size Region固定区域截图
};

#endif // HOTKEYSVS_H

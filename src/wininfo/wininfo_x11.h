/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.05.11
 * Description: Linux 系统下智能窗口识别
 ******************************************************************/
#ifndef WININFO_X11_H
#define WININFO_X11_H

#include "iwininfo.h"
#include <QImage>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <list>
#include <string>


class WinInfo_x11 : public IWinInfo
{
public:
    virtual void setWinFilter(WinID target) override;
    virtual void getWinInfoFromPoint(WinData &winData, QPoint pt, bool bPrevCache) override;

public:
    WinInfo_x11();
    ~WinInfo_x11();

    std::list<Window> getWindowsList();
    void test();

    bool bSupportQuery();
    void getTopLevelWindows();
    std::string getWindowAtom(Window win, const char *atom);
    inline std::string getWindowName(Window win);

private:
    Display* m_pDisplay;
    std::list<Window> m_listWin;

    QListWidget *m_listWidget = nullptr;
};

#endif // WININFO_X11_H

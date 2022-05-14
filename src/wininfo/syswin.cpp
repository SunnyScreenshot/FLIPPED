#include "syswin.h"

// pt 光标当前位置；bPrevCache: true 获取所有窗口模式: false 实时获取窗口模式
WinData SysWin::targWinInfo(X_POINT pt, bool bPrevCache)
{
    return WinData();
}

X_RECT SysWin::targWinRect(X_POINT pt, bool bPrevCache)
{
    WinData date = targWinInfo(pt, bPrevCache);
    return date.rect;
}

void SysWin::winInfoAllCache()
{
}

void SysWin::winInfoRealTime()
{
}

SysWin::SysWin()
    : m_bGetAllCache(false)
    , m_pos(0, 0)
{
}

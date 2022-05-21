#ifndef WININFO_H
#define WININFO_H

#include "../core/isingleton.h"
#include "iwininfo.h"

class WinInfo : public ISingleton<WinInfo>
{
    FRIEND_CREAT_SINGLETION(WinInfo);

public:
    const WinData*  targWinInfo(WinID hWnd, bool bPrevCache);
    const QRect targWinRect(WinID hWnd, bool bPrevCache = false);

private:
    void winInfoAllCache();
    void winInfoRealTime();

private:
    WinInfo();

private:
    bool m_bGetAllCache;
    IWinInfo* m_pWinList;
};

#endif // WININFO_H

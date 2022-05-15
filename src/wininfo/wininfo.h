#ifndef WININFO_H
#define WININFO_H

#include "../core/isingleton.h"
#include "xtype.h"
#include "iwininfo.h"

#include <vector>



class WinInfo : public ISingleton<WinInfo>
{
    FRIEND_CREAT_SINGLETION(WinInfo);

public:
    const WinData*  targWinInfo(void* hWnd, bool bPrevCache);
    const X_RECT targWinRect(void* hWnd, bool bPrevCache = false);

    void test();

private:
    void winInfoAllCache();
    void winInfoRealTime();

private:
    WinInfo();

private:
    bool m_bGetAllCache;
//    X_POINT m_pos;
//    std::vector<WinData> m_vData;

    IWinInfo* m_pWinList;
};

#endif // WININFO_H

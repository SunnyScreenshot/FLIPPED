#ifndef SYSWIN_H
#define SYSWIN_H

#include "../core/isingleton.h"
#include "xtype.h"
#include <string>

#ifdef _WIN32
    //define something for Windows (32-bit and 64-bit, this part is common)
    #include "wininfo_win.h"
#elif __APPLE__
#elif __linux__
    // linux
    #include "wininfo_x11.h"
#else
    # error "Unknown compiler"
#endif

struct WinData
{
    WinData()
        : rect(0, 0, 0, 0)
        , hWnd(0)
        , path(L"")
        , name(L"")
        , title(L"")
        , reserved(L"")
        , level(-1)
        , childCount(0)
        , prev(nullptr)
        , next(nullptr) {}

    X_RECT rect;
    unsigned long hWnd;
    std::wstring path;
    std::wstring name;
    std::wstring title;
    std::wstring reserved;
    int level;
    int childCount;
    WinData* prev;
    WinData* next;
};

class SysWin : public ISingleton<SysWin>
{
    FRIEND_CREAT_SINGLETION(SysWin);

public:
    WinData targWinInfo(X_POINT pt, bool bPrevCache = false);
    X_RECT targWinRect(X_POINT pt, bool bPrevCache = false);

private:
    void winInfoAllCache();
    void winInfoRealTime();
    

private:
    SysWin();

private:
    bool m_bGetAllCache;
    X_POINT m_pos;
    std::vector<WinData> m_vData;
};

#endif // SYSWIN_H

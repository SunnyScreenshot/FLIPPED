#include "wininfo.h"

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

// pt 光标当前位置；bPrevCache: true 获取所有窗口模式: false 实时获取窗口模式
const WinData* WinInfo::targWinInfo(void* hWnd, bool bPrevCache)
{
    if (!m_pWinList)
        return nullptr;

    WinData* pData = new WinData();
    X_POINT pt;

#ifdef _WIN32
    POINT pos;
    ::GetCursorPos(&pos);
    pt.fromPOINT(pos);
#elif __APPLE__
#elif __linux__
#else
#endif

    if (m_pWinList) {
        m_pWinList->setWinFilter(hWnd);
        m_pWinList->getWinInfoFromPoint(*pData, pt, bPrevCache);
    }

    return pData;
}

const X_RECT WinInfo::targWinRect(void* hWnd, bool bPrevCache)
{
    const WinData* pDate = targWinInfo(hWnd, bPrevCache);

    if (pDate)
        return pDate->rect;
    else
        return X_RECT();
}

void WinInfo::test()
{
    if (!m_pWinList) {
//        m_pWinList->m_vWinList;
    }
}

void WinInfo::winInfoAllCache()
{
}

void WinInfo::winInfoRealTime()
{
}

WinInfo::WinInfo()
    : m_bGetAllCache(false)
    , m_pWinList(nullptr)
{
#ifdef _WIN32
    m_pWinList = new WinInfo_Win();
#elif __APPLE__
#elif __linux__
#else
#endif
}

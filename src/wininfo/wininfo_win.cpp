/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.01.30
 * Description: Windows 系统下智能窗口识别
 ******************************************************************/
#include "wininfo_win.h"
//#include <Windows.h>
//#include <atlstr.h>
//#include <string>
#include <psapi.h>
#include <iostream>

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif

HWND WinInfo_Win::m_hWndTarget = NULL;

WinInfo_Win::WinInfo_Win()
{
}

WinInfo_Win::~WinInfo_Win()
{
}

// cout <<用
void WinInfo_Win::getAllWinInfoCache()
{
    m_vWinList.clear();
    m_hWndTarget = nullptr;
    ::EnumWindows(WinInfo_Win::EnumWindowsProc, 0); // 0 为 z 序最顶层的

//    int i = 0;
//    setlocale(LC_ALL, "");
//    for (auto it = IWinInfo::m_vWinList.cbegin(); it != IWinInfo::m_vWinList.cend(); ++it) {
//        //std::string str = CT2A(it->procPath.GetString());
//        tcout << i++ << _T("   ") << it->hWnd
//            << _T("   ") << it->index
//            << _T("   ") << it->path
//            << _T("   ") << it->name
//            << _T("   ") << it->title
//            << _T("   (") << it->rect.left << _T(", ") << it->rect.top << _T(", ")
//            << it->rect.right - it->rect.left << _T(" * ") << it->rect.bottom - it->rect.top << _T(")")
//            << std::endl;
//    }
}

HWND WinInfo_Win::getAllWinInfoRealTime(POINT pt)
{ 
    m_vWinList.clear();
    m_hWndTarget = nullptr;
    ::EnumWindows(WinInfo_Win::EnumRealTimeWindowsProc, MAKELPARAM(pt.x, pt.y));

//    int i = 0;
//    setlocale(LC_ALL, "");
//    for (auto it = IWinInfo::m_vWinList.cbegin(); it != IWinInfo::m_vWinList.cend(); ++it) {
//        //std::string str = CT2A(it->procPath.GetString());
//        tcout << i++ << _T("   ") << it->hWnd
//            << _T("   ") << it->index
//            << _T("   ") << it->path
//            << _T("   ") << it->name
//            << _T("   ") << it->title
//            << _T("   (") << it->rect.left << _T(", ") << it->rect.top << _T(", ")
//            << it->rect.right - it->rect.left << _T(" * ") << it->rect.bottom - it->rect.top << _T(")")
//            << std::endl;
//    }

    // 其中第一个为 Z 序最上层;
    if (m_vWinList.size())
        m_hWndTarget = static_cast<HWND>(m_vWinList.at(0).id._hWnd);  // m_vWinList.size() -1

//    for (const auto& it : m_vWinList) {
//        HWND hwnd1 = ::GetWindow((HWND)it.hWnd, GW_HWNDFIRST);
//        HWND hwnd2 = ::GetTopWindow((HWND)it.hWnd);
//		HWND hwnd3 = ::GetTopWindow(nullptr);
//        HWND hwnd4 = ::GetNextWindow((HWND)it.hWnd, GW_HWNDFIRST);
//        HWND hwnd5 = ::GetNextWindow((HWND)it.hWnd, GW_HWNDLAST);
//        std::wcout << L"it.hWnd:" << (HWND)it.hWnd
//			<< L"  " << hwnd1 << L"  " << hwnd2
//			<< L"  " << hwnd3 << L"  " << hwnd4
//			<< L"  " << hwnd5 << std::endl;
//	}
    return m_hWndTarget;
}

// 设置窗口过滤
void WinInfo_Win::setWinFilter(WinID target /*= nullptr*/)
{
//    if (target)
        m_vHWndFilter.push_back(target);
}

void WinInfo_Win::getWinInfoFromPoint(WinData& winData, QPoint pt, bool bPrevCache /*= false*/)
{
    POINT pos;
    pos.x = pt.x();
    pos.y = pt.y();

    HWND hWndTarget = nullptr;

    if (bPrevCache) {
        getAllWinInfoCache();
        hWndTarget = getWinInfoFromCache(pos);
    } else {
        hWndTarget = getAllWinInfoRealTime(pos);
    }

    for (WinData& it : m_vWinList) {
        if (hWndTarget && hWndTarget == (HWND)(it.id._hWnd)) {
            winData = it;
            break;
        }
    }
}

static int g_index = 0;

BOOL WinInfo_Win::EnumRealTimeWindowsProc(HWND hWnd, LPARAM lParam)
{
	POINT pos;
    pos.x = ((int)(short)LOWORD(lParam));
    pos.y = ((int)(short)HIWORD(lParam));

	if (WindowsContainsPoint(hWnd, pos) && WindowsFilter(hWnd)) {
        RECT rt;
		DWORD processId;
		int32_t nLevel = 0;
		TCHAR windowTitle[MAX_PATH] = _T("");
        ::GetWindowRect(hWnd, &rt);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
		CString procPath = getWindowPath(processId);
		CString procName = windowPath2Name(procPath);

        m_hWndTarget = hWnd;
        WinID winId;
        winId._hWnd = (void *)hWnd;
        m_vWinList.push_back(WinData(QRect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top)
                                          , winId
                                          , QString::fromStdWString(procPath.GetString())
                                          , QString::fromStdWString(procName.GetString())
                                          , QString::fromWCharArray(windowTitle)
                                          , ""
                                          , nLevel
                                          , g_index++
                                          , 0));

		EnumChildWindows(hWnd, EnumChildRealTimeWindowsProc, lParam);
		return FALSE;
	}

	return TRUE;
}

BOOL WinInfo_Win::EnumChildRealTimeWindowsProc(HWND hWnd, LPARAM lParam)
{
	POINT pos;
	pos.x = ((int)(short)LOWORD(lParam));
	pos.y = ((int)(short)HIWORD(lParam));

	if (WindowsContainsPoint(hWnd, pos) && WindowsFilter(hWnd)) {
        RECT rt;
		DWORD processId;
		int32_t nLevel = 0;
		TCHAR windowTitle[MAX_PATH] = _T("");
        ::GetWindowRect(hWnd, &rt);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
		CString procPath = getWindowPath(processId);
		CString procName = windowPath2Name(procPath);

        m_hWndTarget = hWnd;
        WinID winId;
        winId._hWnd = (void *)hWnd;
        m_vWinList.push_back(WinData(QRect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top)
                                     , winId
                                     , QString::fromStdWString(procPath.GetString())
                                     , QString::fromStdWString(procName.GetString())
                                     , QString::fromWCharArray(windowTitle)
                                     , ""
                                     , nLevel
                                     , g_index++
                                     , 0));

		EnumChildWindows(hWnd, EnumChildRealTimeWindowsProc, lParam);
		return FALSE;
	}

	return TRUE;
}

BOOL WinInfo_Win::EnumChildWindowsProc(HWND hWnd, LPARAM level)
{
    //if (hWnd && WindowsFilter(hWnd)) {
        RECT rt;
		DWORD processId;
		int32_t nLevel = level;
		TCHAR windowTitle[MAX_PATH] = _T("");
        ::GetWindowRect(hWnd, &rt);  // GetWindowRect
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
		CString procPath = getWindowPath(processId);
		CString procName = windowPath2Name(procPath);

        WinID winId;
        winId._hWnd = (void *)hWnd;
        m_vWinList.push_back(WinData(QRect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top)
                                     , winId
                                     , QString::fromStdWString(procPath.GetString())
                                     , QString::fromStdWString(procName.GetString())
                                     , QString::fromWCharArray(windowTitle)
                                     , ""
                                     , nLevel
                                     , g_index++
                                     , 0));
//        m_vWinInfo.push_back(InfoDATA(hWnd, rect, nLevel, -2,  windowTitle, procPath, procName));


		//::EnumChildWindows(hWnd, EnumChildWindowsProc, ++nLevel);
    //}

    return TRUE;
}

// MSDN !!!: EnumWindows continues until the last top-level window is enumerated or the callback function returns FALSE.
BOOL WinInfo_Win::EnumWindowsProc(HWND hWnd, LPARAM level)
{
	if (hWnd && WindowsFilter(hWnd)) {  // 句柄存在且可见
        RECT rt;
		DWORD processId;
		int32_t nLevel = level;
		TCHAR windowTitle[MAX_PATH] = _T("");
        ::GetWindowRect(hWnd, &rt);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
		CString procPath = getWindowPath(processId);
		CString procName = windowPath2Name(procPath);

        WinID winId;
        winId._hWnd = (void *)hWnd;
        m_vWinList.push_back(WinData(QRect(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top)
                                     , winId
                                     , QString::fromStdWString(procPath.GetString())
                                     , QString::fromStdWString(procName.GetString())
                                     , QString::fromWCharArray(windowTitle)
                                     , ""
                                     , nLevel
                                     , g_index++
                                     , 0));

		//::EnumChildWindows(hWnd, EnumChildWindowsProc, ++nLevel); // 设置开关时候开启便利子窗口
	}

    return TRUE;
}


BOOL WinInfo_Win::WindowsContainsPoint(HWND hWnd, POINT pt)
{
    RECT rtWin = { 0, 0, 0, 0};
	::GetWindowRect(hWnd, &rtWin);
	return PtInRect(&rtWin, pt);
}

HWND WinInfo_Win::getWinInfoFromCache(POINT pt)
{
    WinData winData;

    for (const auto& it : m_vWinList) {
        RECT rt;
        rt.left = it.rect.left();
        rt.top = it.rect.top();
        rt.right = it.rect.right();
        rt.bottom = it.rect.bottom();


        if (PtInRect(&rt, pt)) {

            if (winData.index <= it.index) {
                winData = it;
            }
		}
	}

    return (HWND)winData.id._hWnd;
}

// 过滤不可见等的窗口
/* spy 抓的 不可见窗口类型 参考
WS_POPUP
WS_CLIPSIBLINGS
WS_EX_TOPMOST
WS_BORDER
WS_SIZEBOX

WS_MINIMIZEBOX
WS_SYSMENU
WS_CLIPCHILDREN

// 扩展类型
WS_EX_TOPMOST
WS_EX_TOOLWINDOW
WS_EX_PALETTEWINDOW
00080000
*/
BOOL WinInfo_Win::WindowsFilter(HWND hWnd)
{
    if (!hWnd)
        return FALSE;

    for (auto& it : m_vHWndFilter) {
        if (hWnd == static_cast<HWND>(it._hWnd))
            return FALSE;
    }

    RECT rect;
    ::GetWindowRect(hWnd, &rect);

    if (rect.left == -32000 && rect.top == -32000   // 最小化
		|| rect.bottom - rect.top <= 0 || rect.right - rect.left <= 0)     // 宽度不存在
        return FALSE;

    DWORD styles = ::GetWindowLong(hWnd, GWL_STYLE);
    DWORD ex_styles = ::GetWindowLong(hWnd, GWL_EXSTYLE);
    if (styles & WS_EX_TOOLWINDOW || ex_styles & WS_EX_TRANSPARENT) // TODO: 看下使用什么合适
        return FALSE;

    return ::IsWindowVisible(hWnd);
}

CString WinInfo_Win::getWindowPath(DWORD processId)
{
    TCHAR path[MAX_PATH] = _T("");
    HANDLE hProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);
    GetProcessImageFileName(hProc, path, MAX_PATH);
    ::CloseHandle(hProc);

    return path;
}

CString WinInfo_Win::windowPath2Name(CString path)
{
    auto pos = path.ReverseFind(_T('/'));
    if (pos == -1)
        pos = path.ReverseFind(_T('\\'));
    if (pos == -1)
        return _T("");

    return path.Right(path.GetLength() - (pos + 1));
}

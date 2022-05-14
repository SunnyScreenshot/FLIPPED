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

HWND WinInfoWin::m_hWndTarget = NULL;
HWND WinInfoWin::m_hWndFilter = NULL;
std::vector<WinInfo> WinInfoWin::m_vWinInfo;

WinInfoWin::WinInfoWin()
{
}

// cout <<用
void WinInfoWin::getAllWinInfoCache()
{
	::EnumWindows(WinInfoWin::EnumWindowsProc, 0); // 0 为 z 序最顶层的

	int i = 0;
	setlocale(LC_ALL, "");
	for (auto it = WinInfoWin::m_vWinInfo.cbegin(); it != WinInfoWin::m_vWinInfo.cend(); ++it) {
		//std::string str = CT2A(it->procPath.GetString());
		tcout << i++ << _T("   ") << it->hWnd
			<< _T("   ") << it->procPath.GetString()
			<< _T("   ") << it->procName.GetString()
			<< _T("   ") << it->procTitle.GetString()
			<< _T("   (") << it->rect.left << _T(", ") << it->rect.top << _T(", ")
			<< it->rect.right - it->rect.left << _T(" * ") << it->rect.bottom - it->rect.top << _T(")")
			<< std::endl;
	}
}

HWND WinInfoWin::getAllWinInfoRealTime(POINT pt)
{ 
	m_vWinInfo.clear();
	m_hWndTarget = nullptr;
	::EnumWindows(WinInfoWin::EnumRealTimeWindowsProc, MAKELPARAM(pt.x, pt.y));

	int i = 0;
	setlocale(LC_ALL, "");
	for (auto it = WinInfoWin::m_vWinInfo.cbegin(); it != WinInfoWin::m_vWinInfo.cend(); ++it) {
		//std::string str = CT2A(it->procPath.GetString());
		tcout << i++ << _T("   ") << it->hWnd
			<< _T("   ") << it->index
			<< _T("   ") << it->procPath.GetString()
			<< _T("   ") << it->procName.GetString()
			<< _T("   ") << it->procTitle.GetString()
			<< _T("   (") << it->rect.left << _T(", ") << it->rect.top << _T(", ")
			<< it->rect.right - it->rect.left << _T(" * ") << it->rect.bottom - it->rect.top << _T(")")
			<< std::endl;
	}

	//// 其中第一个为 Z 序最上层;
	//if (m_vWinInfo.size() >= 1)
	//	m_hWndTarget = m_vWinInfo.at(0).hWnd;

	for (const auto& it : m_vWinInfo) {
		HWND hwnd1 = ::GetWindow(it.hWnd, GW_HWNDFIRST);
		HWND hwnd2 = ::GetTopWindow(it.hWnd);
		HWND hwnd3 = ::GetTopWindow(nullptr);
		HWND hwnd4 = ::GetNextWindow(it.hWnd, GW_HWNDFIRST);
		HWND hwnd5 = ::GetNextWindow(it.hWnd, GW_HWNDLAST);
		std::wcout << L"it.hWnd:" << it.hWnd 
			<< L"  " << hwnd1 << L"  " << hwnd2 
			<< L"  " << hwnd3 << L"  " << hwnd4 
			<< L"  " << hwnd5 << std::endl;
	}
	return m_hWndTarget;
}

// 过滤指定窗口
void WinInfoWin::setWindowsFilter(HWND hWnd)
{
	if (hWnd)   
		m_hWndFilter = hWnd;
}

// bSmartDetection: true 及时自动获取； false 缓存所有获取
RECT WinInfoWin::getWindowsRectFromPoint(POINT pt, BOOL bSmartDetection)
{
	RECT rect = { 0, 0, 0, 0 };
	if (bSmartDetection) {
		HWND hWndTarget = getAllWinInfoRealTime(pt);

		if (hWndTarget)
			::GetWindowRect(hWndTarget, &rect);
	} else {
		getAllWinInfoCache();
		getRectFromCache(pt, rect);
	}


	return rect;
}

static int g_index = 0;

BOOL WinInfoWin::EnumRealTimeWindowsProc(HWND hWnd, LPARAM lParam)
{
	POINT pos;
    pos.x = ((int)(short)LOWORD(lParam));
    pos.y = ((int)(short)HIWORD(lParam));

	if (WindowsContainsPoint(hWnd, pos) && WindowsFilter(hWnd)) {
		RECT rect;
		DWORD processId;
		int32_t nLevel = 0;
		TCHAR windowTitle[MAX_PATH] = _T("");
		::GetWindowRect(hWnd, &rect);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
		CString procPath = getWindowPath(processId);
		CString procName = windowPath2Name(procPath);

		m_hWndTarget = hWnd;
		m_vWinInfo.push_back(WinInfo(hWnd, rect, nLevel, g_index ++, windowTitle, procPath, procName));
		EnumChildWindows(hWnd, EnumChildRealTimeWindowsProc, lParam);
		return FALSE;
	}

	return TRUE;
}

BOOL WinInfoWin::EnumChildRealTimeWindowsProc(HWND hWnd, LPARAM lParam)
{
	POINT pos;
	pos.x = ((int)(short)LOWORD(lParam));
	pos.y = ((int)(short)HIWORD(lParam));

	if (WindowsContainsPoint(hWnd, pos) && WindowsFilter(hWnd)) {
		RECT rect;
		DWORD processId;
		int32_t nLevel = 0;
		TCHAR windowTitle[MAX_PATH] = _T("");
		::GetWindowRect(hWnd, &rect);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
		CString procPath = getWindowPath(processId);
		CString procName = windowPath2Name(procPath);

		m_hWndTarget = hWnd;
		m_vWinInfo.push_back(WinInfo(hWnd, rect, nLevel, -4, windowTitle, procPath, procName));
		EnumChildWindows(hWnd, EnumChildRealTimeWindowsProc, lParam);
		return FALSE;
	}

	return TRUE;
}

BOOL WinInfoWin::EnumChildWindowsProc(HWND hWnd, LPARAM level)
{
    //if (hWnd && WindowsFilter(hWnd)) {
		RECT rect;
		DWORD processId;
		int32_t nLevel = level;
		TCHAR windowTitle[MAX_PATH] = _T("");
		::GetWindowRect(hWnd, &rect);  // GetWindowRect
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
		CString procPath = getWindowPath(processId);
		CString procName = windowPath2Name(procPath);

		m_vWinInfo.push_back(WinInfo(hWnd, rect, nLevel, -2,  windowTitle, procPath, procName));

		//::EnumChildWindows(hWnd, EnumChildWindowsProc, ++nLevel);
    //}

    return TRUE;
}

// MSDN !!!: EnumWindows continues until the last top-level window is enumerated or the callback function returns FALSE.
BOOL WinInfoWin::EnumWindowsProc(HWND hWnd, LPARAM level)
{
	
	if (hWnd && WindowsFilter(hWnd)) {  // 句柄存在且可见
		RECT rect;
		DWORD processId;
		int32_t nLevel = level;
		TCHAR windowTitle[MAX_PATH] = _T("");
		::GetWindowRect(hWnd, &rect);
		::GetWindowThreadProcessId(hWnd, &processId);
		::GetWindowText(hWnd, windowTitle, MAX_PATH);
		CString procPath = getWindowPath(processId);
		CString procName = windowPath2Name(procPath);

		m_vWinInfo.push_back(WinInfo(hWnd, rect, nLevel, g_index++, windowTitle, procPath, procName));
		//::EnumChildWindows(hWnd, EnumChildWindowsProc, ++nLevel); // 设置开关时候开启便利子窗口
	}

    return TRUE;
}


BOOL WinInfoWin::WindowsContainsPoint(HWND hWnd, POINT pt)
{
    RECT rtWin = { 0, 0, 0, 0};
	::GetWindowRect(hWnd, &rtWin);
	return PtInRect(&rtWin, pt);
}

BOOL WinInfoWin::getRectFromCache(POINT pt, RECT & rect)
{
	WinInfo winInfo;

	for (const auto& it : m_vWinInfo) {
		if (winInfo.hWnd && it.level <= winInfo.level)
			break;

		if (PtInRect(&it.rect, pt)) {
			if (winInfo.hWnd == NULL) {
				winInfo = it;
			} else {
				if (it.level > winInfo.level)
					winInfo = it;;
			}
		}
	}

	if (winInfo.hWnd) {
		rect = winInfo.rect;
		return TRUE;
	}

	return FALSE;
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
BOOL WinInfoWin::WindowsFilter(HWND hWnd)
{
    if (!hWnd)
        return FALSE;

	if (hWnd == m_hWndFilter)
		return FALSE;

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

#include <Windows.h>
#include <atlstr.h>
CString WinInfoWin::getWindowPath(DWORD processId)
{
    TCHAR path[MAX_PATH] = _T("");
    HANDLE hProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);
    GetProcessImageFileName(hProc, path, MAX_PATH);
    ::CloseHandle(hProc);

    return path;
}

CString WinInfoWin::windowPath2Name(CString path)
{
    auto pos = path.ReverseFind(_T('/'));
    if (pos == -1)
        pos = path.ReverseFind(_T('\\'));
    if (pos == -1)
        return _T("");

    return path.Right(path.GetLength() - (pos + 1));
}

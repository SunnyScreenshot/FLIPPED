#include "wininfo_win.h"
//#include <Windows.h>
//#include <atlstr.h>
//#include <string>
#include <psapi.h>
//#include <iostream>

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif

HWND WinInfoWin::m_hWndTarget = NULL;
HWND WinInfoWin::m_hWndFilter = NULL;
std::vector<WinInfo> WinInfoWin::m_vWinInfo;

//CString WinInfoWin::getWindowNameT(HWND hWnd, DWORD processId)
//{
//	TCHAR exename[MAX_PATH] = _T("");
//	HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);
//	GetProcessImageFileName(hProc, exename, MAX_PATH);
//	CloseHandle(hProc);
//	CString windowName(exename);
//	auto pos = windowName.ReverseFind(_T('/'));
//	if (pos == -1)
//		pos = windowName.ReverseFind(_T('\\'));
//	if (pos == -1)
//		return _T("");

//	//return windowName.Right(windowName.GetLength() - pos);
//	return windowName;
//}

//bool WinInfoWin::checkWindowValid(HWND hWnd)
//{
//	RECT rect;
//	GetClientRect(hWnd, &rect);
//	if (rect.bottom == 0 || rect.right == 0)
//		return false;

//	DWORD ex_styles = (DWORD)GetWindowLongPtr(hWnd, GWL_EXSTYLE);
//	DWORD styles = (DWORD)GetWindowLongPtr(hWnd, GWL_STYLE);
//	if (ex_styles & WS_EX_TOOLWINDOW || styles & WS_CHILD)
//		return false;

//	return IsWindowVisible(hWnd);
//}

//bool WinInfoWin::checkWindowTitleValid(CString tile)
//{
//	TCHAR storeName[] = _T("Microsoft Store");
//	if (tile.IsEmpty() || _tcscmp(tile, storeName) == 0
//		|| _tcscmp(tile, _T("设置")) == 0
//		|| _tcscmp(tile, _T("Nahimic")) == 0
//		|| _tcscmp(tile, _T("Microsoft Text Input Application")) == 0)
//		return false;
//	else
//		return true;
//}

//void WinInfoWin::getAllWinInfo(std::vector<WinInfo>& vec)
//{
//	HWND desktopHwnd = GetDesktopWindow();
//	TCHAR windowTitle[MAX_PATH] = _T("");
//	CString exename = _T("");
//	HWND hWnd = GetWindow(desktopHwnd, GW_CHILD);
//	while (hWnd != NULL) {
//		RECT rect;
//		DWORD processId;
//		GetWindowThreadProcessId(hWnd, &processId);
//		GetWindowRect(hWnd, &rect);
//		GetWindowText(hWnd, windowTitle, MAX_PATH);
//		exename = getWindowNameT(hWnd, processId);

//		if (exename.IsEmpty() || !checkWindowValid(hWnd) || !checkWindowTitleValid(windowTitle)) {
//			hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
//			continue;
//		}
//		vec.push_back(WinInfo(rect, hWnd, exename));
//		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);

//		static int i = 0;
//		tcout << i++ << _T("   windowTitle:") << windowTitle << "  hWnd:" << hWnd
//			<< _T("   (") << rect.left << _T(", ") << rect.top << _T(", ") << rect.right - rect.left << _T(" * ") << rect.bottom - rect.top << _T(")")
//			<< std::endl;
//		//std::cout << "windowTitle:" << windowTitle << std::endl;
//		//std::cout << "exename:" << exename << std::endl;
//	}
//}

//void WinInfoWin::getAllWinInfo()
//{

//}


BOOL WinInfoWin::EnumChildWindowsProc(HWND hWnd, LPARAM level)
{
    if (!hWnd || !WindowsFilter(hWnd))
        return false;

    RECT rect;
    DWORD processId;
    int32_t nLevel = level;
    ::GetClientRect(hWnd, &rect);
    ::GetWindowThreadProcessId(hWnd, &processId);
    CString exePath = getWindowPath(processId);
    CString exeName = windowPath2Name(exePath);

    m_vWinInfo.push_back(WinInfo(hWnd, rect, nLevel, exePath, exeName));

    ::EnumChildWindows(hWnd, EnumChildWindowsProc, ++nLevel);

    return true;
}

BOOL WinInfoWin::EnumWindowsProc(HWND hWnd, LPARAM level)
{
    if (!hWnd || !WindowsFilter(hWnd))
        return false;

    RECT rect;
    DWORD processId;
    int32_t nLevel = level;
    ::GetClientRect(hWnd, &rect);
    ::GetWindowThreadProcessId(hWnd, &processId);
    CString exePath = getWindowPath(processId);
    CString exeName = windowPath2Name(exePath);

    m_vWinInfo.push_back(WinInfo(hWnd, rect, nLevel, exePath, exeName));

    ::EnumChildWindows(hWnd, EnumChildWindowsProc, ++nLevel);

    return true;
}


// 过滤不可见等的窗口
bool WinInfoWin::WindowsFilter(HWND hWnd)
{
    if (hWnd && hWnd == m_hWndFilter)
        return false;

    RECT rect;
    ::GetClientRect(hWnd, &rect);
    if (rect.bottom == 0 || rect.right == 0)
        return false;

    DWORD styles = ::GetWindowLong(hWnd, GWL_STYLE);
    DWORD ex_styles = ::GetWindowLong(hWnd, GWL_EXSTYLE);
    if (styles & WS_VISIBLE || ex_styles & WS_EX_TOOLWINDOW)
        return false;

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

    return path.Right(path.GetLength() - pos);
}

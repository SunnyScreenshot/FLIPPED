/*******************************************************************
 * Copyright (C)  2021~2022  偕臧  All rights reserved.
 *
 * GitHub:  https://github.com/xmuli
 * Author:  偕臧 <xmulitech@gmail.com>
 *
 * Create: 2021.10.15
 * Modify: 2022.01.14
 * Description: 获取 Windows 下桌面的所有窗口矩形信息
 * Analysis: 参考 https://blog.csdn.net/yp18792574062/article/details/109279577
 *                https://www.cnblogs.com/zjutlitao/p/3889900.html
 *
 ******************************************************************/
#include "syswininfo_win.h"
#include <tchar.h>

SysWinInfo::SysWinInfo()
{
}

SysWinInfo::~SysWinInfo()
{
}

// 获取桌面下的所有窗口（条件过滤）
void SysWinInfo::getWindowList()
{
	HWND desktopWin = ::GetDesktopWindow();
	TCHAR winTitle[MAX_PATH] = { 0 };
	DWORD nSize = 0;
	CString exeName;
	HWND hwnd = ::GetWindow(desktopWin, GW_CHILD);

	if (hwnd != NULL) {
		DWORD procId;
		RECT rect;
		::GetWindowThreadProcessId(hwnd, &procId);
		::GetWindowRect(hwnd, &rect);
		::GetWindowText(hwnd, winTitle, MAX_PATH);

		exeName = getWindowName(hwnd, procId);
		//TCHAR name[MAX_PATH] = { 0 };
		//_tcscpy(name, exeName);
		//if (exeName.IsEmpty() || !checkWindowValid(hwnd) /*|| !checkWindowTitleValid(name)*/)
		//	continue;

	}


	while (hwnd != NULL) {
		hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
		DWORD procId;
		RECT rect;
		::GetWindowThreadProcessId(hwnd, &procId);
		::GetWindowRect(hwnd, &rect);
		::GetWindowText(hwnd, winTitle, MAX_PATH);

		exeName = getWindowName(hwnd, procId);
		//TCHAR name[MAX_PATH] = { 0 };
		//_tcscpy(name, exeName);
		//if (!exeName.IsEmpty() || !checkWindowValid(hwnd) /*|| !checkWindowTitleValid(name)*/)
		//	continue;

		std::cout << "windowTitle:" << winTitle << std::endl;
		//std::cout << "exename:" << exeName.GetBuffer() << std::endl;

	}
}

// 获取当前窗口名称
CString SysWinInfo::getWindowName(HWND hwnd, DWORD processId)
{
	CString rt = _T("");
	if (!hwnd)
		return rt;

	TCHAR exeName[MAX_PATH] = { 0 };
	DWORD nSize = 0;
	HANDLE hProc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);
	::GetProcessImageFileName(hProc, exeName, MAX_PATH);
	::CloseHandle(hProc);

	rt.Format(_T("%s"), exeName);
	auto pos = rt.ReverseFind(_T('/'));

	int nn = rt.GetLength();
	if (pos == -1)
		pos = rt.ReverseFind(_T('\\'));

	if (pos == -1)
		return  _T("");
	else
		return rt;
}

// 过滤不可见的窗体
bool SysWinInfo::checkWindowValid(HWND hwnd)
{
	if (!hwnd)
		return false;

	RECT rect;
	::GetClientRect(hwnd, &rect); // 获取客户区域

	if (rect.right - rect.left <= 0 || rect.bottom - rect.top <= 0)
		return false;

	DWORD exStyles = static_cast<DWORD>(::GetWindowLongPtr(hwnd, GWL_EXSTYLE));
	DWORD styles = static_cast<DWORD>(::GetWindowLongPtr(hwnd, GWL_STYLE));
	if (exStyles & WS_EX_TOOLWINDOW || styles & WS_CHILD)
		return false;

	if (!IsWindowVisible(hwnd))
		return false;
	else
		return true;
}

// 过滤指定标题的窗口
bool SysWinInfo::checkWindowTitleValid(TCHAR * winName)
{
	if (!winName || _tcslen(winName) || _tcscmp(winName, _T("Microsoft Store")))
		return false;

	return false;
}
#include "wininfo_win.h"
#include <Windows.h>
#include <atlstr.h>

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif

CString WinInfoWin::getWindowNameT(HWND hwnd, DWORD processId)
{
	TCHAR exename[MAX_PATH] = _T("");
	HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, processId);
	GetProcessImageFileName(hProc, exename, MAX_PATH);
	CloseHandle(hProc);
	CString windowName(exename);
	auto pos = windowName.ReverseFind(_T('/'));
	if (pos == -1)
		pos = windowName.ReverseFind(_T('\\'));
	if (pos == -1)
		return _T("");

	//return windowName.Right(windowName.GetLength() - pos);
	return windowName;
}

bool WinInfoWin::checkWindowValid(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	if (rect.bottom == 0 || rect.right == 0)
		return false;

	DWORD ex_styles = (DWORD)GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	DWORD styles = (DWORD)GetWindowLongPtr(hwnd, GWL_STYLE);
	if (ex_styles & WS_EX_TOOLWINDOW || styles & WS_CHILD)
		return false;

	return IsWindowVisible(hwnd);
}

bool WinInfoWin::checkWindowTitleValid(CString tile)
{
	TCHAR storeName[] = _T("Microsoft Store");
	if (/*tile.IsEmpty() ||*/ _tcscmp(tile, storeName) == 0)
		return false;
	else
		return true;
}

void WinInfoWin::getAllWinInfo(std::vector<WinInfo>& vec)
{
	HWND desktopHwnd = GetDesktopWindow();
	TCHAR windowTitle[MAX_PATH] = _T("");
	CString exename = _T("");
	HWND hwnd = GetWindow(desktopHwnd, GW_CHILD);
	while (hwnd != NULL) {
		RECT rect;
		DWORD processId;
		GetWindowThreadProcessId(hwnd, &processId);
		GetWindowRect(hwnd, &rect);
		GetWindowText(hwnd, windowTitle, MAX_PATH);
		exename = getWindowNameT(hwnd, processId);

		if (exename.IsEmpty() || !checkWindowValid(hwnd) || !checkWindowTitleValid(windowTitle)) {
			hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
			continue;
		}
		vec.push_back(WinInfo(rect, hwnd, exename));
		hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);

		static int i = 0;
		tcout << i++ << _T("   windowTitle:") << windowTitle << "  hwnd:" << hwnd
			<< _T("   (") << rect.left << _T(", ") << rect.top << _T(", ") << rect.right - rect.left << _T(" * ") << rect.bottom - rect.top << _T(")")
			<< std::endl;
		//std::cout << "windowTitle:" << windowTitle << std::endl;
		//std::cout << "exename:" << exename << std::endl;
	}
}

#ifndef WININFO_WIN_H
#define WININFO_WIN_H
#include "iwininfo.h"
#include "../core/tsingleton.h"

#include <windows.h>
#include <atlstr.h>
#include <vector>
// 窗口信息_win 版本
class WinInfoWin : public TSingleton<WinInfoWin>// , IWinInfo
{
//private:
//	CString getWindowNameT(HWND hwnd, DWORD processId);      // 获取窗口名称
//	bool checkWindowValid(HWND hwnd);                        // 过滤可见窗口
//	bool checkWindowTitleValid(CString tile);                // 过滤指定名称窗口

//public:
//    void getAllWinInfo();


//protected:
//    friend class TSingleton<WinInfoWin>;

protected:
    static BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM level);
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM level);

    static bool WindowsFilter(HWND hWnd);
    static CString getWindowPath(DWORD processId);
    static CString windowPath2Name(CString path);

//    error: LNK2005: "protected: static struct HWND__ * WinInfoWin::m_hWndTarget" (?m_hWndTarget@WinInfoWin@@1PAUHWND__@@A) already defined in mocs_compilation.cpp.obj

    // 静态成员变量只能在 cpp 中初始化，坑了一会 https://stackoverflow.com/questions/40991522
private:
    static HWND m_hWndTarget;
    static HWND m_hWndFilter;
    static std::vector<WinInfo> m_vWinInfo;
};


#endif // WININFO_WIN_H

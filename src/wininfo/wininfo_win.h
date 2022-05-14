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
#ifndef WININFO_WIN_H
#define WININFO_WIN_H
#include "../core/isingleton.h"

#include <windows.h>
#include <atlstr.h>
#include <vector>

class WinInfo
{
public:
    WinInfo() : hWnd(nullptr)
      , rect({0, 0, 0, 0})
      , level(-1)
      , index(-1)
      , procTitle(_T(""))
      , procPath(_T(""))
      , procName(_T("")) {
    }

    WinInfo(const HWND hWnd, const RECT rect, const int32_t level, const int32_t index,
        const CString procTitle, const CString procPath, const CString procName) {
        this->rect = rect;
        this->hWnd = hWnd;
        this->level = level;
        this->procTitle = procTitle;
        this->procPath = procPath;
        this->procName = procName;
    }

    HWND hWnd;
    RECT rect;
    int32_t level;
    int32_t index;
    CString procTitle;
    CString procName;
    CString procPath;
};

// 窗口信息_win 版本
class WinInfoWin : public ISingleton<WinInfoWin>// , IWinInfo
{
private:
	WinInfoWin();

    void getAllWinInfoCache();
	HWND getAllWinInfoRealTime(POINT pt);

public:
	void setWindowsFilter(HWND hWnd);
	RECT getWindowsRectFromPoint(POINT pt, BOOL bSmartDetection = TRUE);

//
protected:
	friend class ISingleton<WinInfoWin>;
//
//protected:
public:
	static BOOL CALLBACK EnumRealTimeWindowsProc(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK EnumChildRealTimeWindowsProc(HWND hWnd, LPARAM lParam);

    static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM level);
    static BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM level);

    static BOOL WindowsContainsPoint(HWND hWnd, POINT pt);
	static BOOL getRectFromCache(POINT pt, RECT& rect);
    static BOOL WindowsFilter(HWND hWnd);
    static CString getWindowPath(DWORD processId);
    static CString windowPath2Name(CString path);

//    error: LNK2005: "protected: static struct HWND__ * WinInfoWin::m_hWndTarget" (?m_hWndTarget@WinInfoWin@@1PAUHWND__@@A) already defined in mocs_compilation.cpp.obj

    // 静态成员变量只能在 cpp 中初始化，坑了一会 https://stackoverflow.com/questions/40991522
//private:
    static HWND m_hWndTarget;
    static HWND m_hWndFilter;
    static std::vector<WinInfo> m_vWinInfo;
};

#endif // WININFO_WIN_H

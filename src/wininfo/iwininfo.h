/*******************************************************************
 * Copyright (C)  2022  偕臧  All rights reserved.
 *
 * Author: 偕臧 <xmulitech@gmail.com>
 * GitHub: https://github.com/xmuli
 *
 * Create: 2022.01.29
 * Modify: 2022.01.29
 * Description: 实现一个模板的静态单例
 ******************************************************************/
#ifndef IWININFO_H
#define IWININFO_H

#if _WIN32
    #include <windows.h>
    #include <atlstr.h>
#else
    //
#endif

#include <vector>

class WinInfo
{
public:
    WinInfo() : hWnd(nullptr)
      , rect({0, 0, 0, 0})
      , level(-1)
      , procPath(_T(""))
      , procName(_T("")) {
    }

    WinInfo(const HWND hWnd, const RECT rect, const int32_t level, 
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
    CString procTitle;
    CString procName;
    CString procPath;
};

//class IWinInfo
//{
//public:
//    virtual void getAllWinInfo(std::vector<WinInfo>& vec) = 0;
//};
#endif // IWININFO_H


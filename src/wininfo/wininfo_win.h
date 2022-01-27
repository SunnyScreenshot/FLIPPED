#pragma once
#include "iwininfo.h"
#include <iostream>
#include <string>
#include <psapi.h>

// 窗口信息_win 版本
class WinInfoWin :
    public ISysWinInfo
{
private:
	CString getWindowNameT(HWND hwnd, DWORD processId);      // 获取窗口名称
	bool checkWindowValid(HWND hwnd);                        // 过滤可见窗口
	bool checkWindowTitleValid(CString tile);                // 过滤指定名称窗口

public:
    virtual void getAllWinInfo(std::vector<WinInfo>& vec) override;
};


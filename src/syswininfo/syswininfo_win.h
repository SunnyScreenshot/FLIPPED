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

#ifndef SYSWININFO_WIN_H
#define SYSWININFO_WIN_H

 //#ifdef _WIN32
 ////#include <windows.h>
 ////#include <winuser.h>
 //#endif

#include <atlstr.h>
#include <psapi.h>
#include <iostream>


class SysWinInfo
{
public:
	SysWinInfo();
	~SysWinInfo();

	void getWindowList();                                 // 获取所有桌面的窗口

private:
	CString getWindowName(HWND hwnd, DWORD processId);    // 获取窗口名称
	bool checkWindowValid(HWND hwnd);                     // 窗口客户区域不为零且可见
	bool checkWindowTitleValid(TCHAR* winName);
};


#endif // SYSWININFO_WIN_H

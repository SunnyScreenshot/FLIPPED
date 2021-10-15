//
// Created by xmuli on 2021/10/15.
//

#ifndef SYSWININFO_WIN_H
#define SYSWININFO_WIN_H

#include <windows.h>
#include <stdio.h>
//#include <tchar.h>
//#include <string.h>
//#include <iostream>
//using namespace std;

// https://blog.csdn.net/vevenlcf/article/details/80206457
//    EnumWindows(EnumWindowsProc, NULL);
//EnumDesktopWindows(hDesktop, EnumWindowsProc, NULL);  // 在 mac 上或许可用该函数?

BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM lParam);
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

void getAllTopWinRect();

bool isValue(RECT rect);


//HWND hd = GetDesktopWindow();        //得到桌面窗口
//hd = GetWindow(hd, GW_CHILD);        //得到屏幕上第一个子窗口
//char s[200] = { 0 };
//int num = 1;
//while (hd != NULL)                    //循环得到所有的子窗口
//{
//    memset(s, 0, 200);
//    GetWindowText(hd, s, 200);
//    cout << num++ << ": " << s << endl;
//    hd = GetNextWindow(hd, GW_HWNDNEXT);
//}


#endif // SYSWININFO_WIN_H

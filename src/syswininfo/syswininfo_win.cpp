//
// Created by xmuli on 2021/10/15.
//
#include "syswininfo_win.h"
////#include <winuser.h>
//#include <math.h>

//INT Pnum = 0;
//INT Cnum = 0; //父窗口数量，每一级父窗口的子窗口数量
//RECT g_Rect;

//// EnumChildWindows 回调函数，hwnd为指定的父窗口
//BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM lParam)
//{
//    char WindowTitle[100] = { 0 };
//    Cnum++;
//    RECT rect;
//    ::GetWindowText(hWnd, WindowTitle, 100);
//    ::GetWindowRect(hWnd, &rect);
//    if (isValue(rect))
//        printf("--|%d :%s %d %d %d %d\n", Cnum, WindowTitle, rect.left, rect.top, rect.right, rect.bottom);

//    return true;
//}


//// EnumWindows 回调函数，hwnd为发现的顶层窗口
//BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
//{
//    if (GetParent(hWnd) == NULL && IsWindowVisible(hWnd)) {  //判断是否顶层窗口并且可见
//        Pnum++;
//        Cnum = 0;
//        char WindowTitle[100] = { 0 };
//        RECT rect;
//        ::GetWindowText(hWnd, WindowTitle, 100);
//        ::GetWindowRect(hWnd, &g_Rect);

//        if (isValue(g_Rect)) {
//            printf("-------------------------------------------\n");

//            printf("%d: %s %d %d %d %d\n", Pnum, WindowTitle, g_Rect.left, g_Rect.top, rect.right, rect.bottom);
//            EnumChildWindows(hWnd, EnumChildWindowsProc, NULL); //获取父窗口的所有子窗口
//        }
//    }

//    return true;
//}

//// 获取所有顶层窗口矩形，包含子窗口（0*0大小则略过）
//void getAllTopWinRect()
//{
//    // 获取屏幕上所有的顶层窗口,每发现一个窗口就调用回调函数一次
//    EnumWindows(EnumWindowsProc, NULL);
//}

//// 判断该矩形是否有效
//bool isValue(RECT rect)
//{
//    if (rect.right - rect.left > 0 && rect.bottom - rect.top > 0)
//        return true;
//    else
//        return false;
//}

/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.05.14
 * Description: 定义几个基础通用的变量
 ******************************************************************/
#ifndef XTYPE_H
#define XTYPE_H

//#include <string>

//#ifdef _WIN32
//    #include <windows.h>
//#elif __APPLE__
//#elif __linux__
//#else
//#endif

//template<class T>
//struct _X_POINT
//{
//    _X_POINT() : x(0), y(0) {}
//    _X_POINT(T _x, T _y) : x(_x), y(_y) {}

//#ifdef _WIN32
//    _X_POINT(const POINT& pt) : x(pt.x), y(pt.y) {}

//    _X_POINT fromPOINT(const POINT& pt) {
//        x = pt.x;
//        y = pt.y;
//        return *this;
//    }

//    POINT toPOINT() {
//        POINT pt;
//        pt.x = x;
//        pt.y = y;

//        return pt;
//    }
//#elif __APPLE__
//#elif __linux__
//#else
//#endif

//    T  x;
//    T  y;
//};

//typedef _X_POINT<int> X_POINT;
////typedef _X_POINT<float> FX_POINT;

//template<class T>
//struct _QRect
//{
//    _QRect() : left(), top(), right(), bottom() {}
//    _QRect(T left, T top, T right, T bottom) :
//        left(left), top(top), right(right), bottom(bottom) {}


//#ifdef _WIN32
//    _QRect(const RECT& rt) : left(rt.left), top(rt.top), right(rt.right), bottom(rt.bottom) {}

//    RECT toRECT() {
//        RECT rt;
//        rt.left = left;
//        rt.top = top;
//        rt.right = right;
//        rt.bottom = bottom;

//        return rt;
//    }
//#elif __APPLE__
//#elif __linux__
//#else
//#endif


//    template<class Point>
//    _QRect(Point p, T width, T height) :
//      left(p.x), top(p.y), right(p.x + width), bottom(p.y + height) {}

//    T left;
//    T top;
//    T right;
//    T bottom;
//};

//typedef _QRect<int> QRect;
////typedef _QRect<float> FX_RECT;

//#include <QRect>
//#include <QString>

//struct WinData
//{
//    WinData(QRect tRect
//            , void* tHWnd
//            , QString tPath
//            , QString tName
//            , QString tTitle
//            , QString tReserved
//            , int tLevel
//            , int tIndex
//            , int tChildCount)
//        : rect(tRect)
//        , hWnd(tHWnd)
//        , path(tPath)
//        , name(tName)
//        , title(tTitle)
//        , reserved(tReserved)
//        , level(tLevel)
//        , index(tIndex)
//        , childCount(tChildCount){}

//    WinData()
//        : rect(0, 0, 0, 0)
//        , hWnd(nullptr)
//        , path("")
//        , name(L"")
//        , title(L"")
//        , reserved(L"")
//        , level(0)
//        , index(0)
//        , childCount(0) {}

////    WinData(const WinData &) = default;
////    WinData& operator=(const WinData&) = default;

//    QRect rect;

//    union hwnd {
//        void* _hWnd;             // Windows OS
//        unsigned long _xWindow   // X11 OS
//    };

//    QString path;
//    QString name;
//    QString title;
//    QString reserved;
//    int level;
//    int index;
//    int childCount;
//};



#endif // XTYPE_H

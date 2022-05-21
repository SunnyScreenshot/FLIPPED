/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.01.29
 * Description:
 ******************************************************************/
#ifndef IWININFO_H
#define IWININFO_H

//#include "xtype.h"


#include <QRect>
#include <QPoint>
#include <QString>
#include <vector>

union WinID {
    void* _hWnd;              // NT OS
    unsigned long _xWindow;   // X11 OS
};

struct WinData
{
    WinData(WinID tId
            , bool bTFilter
            , QRect tRect
            , QString tPath
            , QString tName
            , QString tTitle
            , QString tReserved
            , int tLevel
            , int tIndex
            , int tChildCount)
        : id(tId)
        , bFilter(bTFilter)
        , rect(tRect)
        , path(tPath)
        , name(tName)
        , title(tTitle)
        , reserved(tReserved)
        , level(tLevel)
        , index(tIndex)
        , childCount(tChildCount){}

    WinData()
        : id()  // todo
        , bFilter(false)
        , rect(0, 0, 0, 0)
        , path("")
        , name("")
        , title("")
        , reserved("")
        , level(0)
        , index(0)
        , childCount(0) {}

    WinID id;
    bool bFilter;
    QRect rect;
    QString path;
    QString name;
    QString title;
    QString reserved;
    int level;
    int index;
    int childCount;
};

class IWinInfo
{
public:
    IWinInfo() = default;
    virtual ~IWinInfo() = default;

    virtual void setWinIdFilter(WinID target) = 0;
    virtual WinData* getWinInfoFromPoint(QPoint pt, bool bPrevCache = false) = 0;

//protected:

    static std::vector<WinData> m_vWinData;
    static std::vector<WinID> m_vWinIdFilter;
};



#endif // IWININFO_H


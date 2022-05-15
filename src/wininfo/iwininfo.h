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

#include "xtype.h"
#include <vector>

class IWinInfo
{
public:
    IWinInfo() = default;
    virtual ~IWinInfo() = default;

    virtual void setWinFilter(void* target = nullptr) = 0;
    virtual void getWinInfoFromPoint(WinData& winData, X_POINT pt, bool bPrevCache = false) = 0;

//protected:

    static std::vector<WinData> m_vWinList;
    static std::vector<void *> m_vHWndFilter;
};



#endif // IWININFO_H


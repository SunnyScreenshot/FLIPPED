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

class IWinInfo
{
public:
    virtual void setWinFilter(void* target = nullptr) = 0;
    virtual X_RECT getWinRectFromPoint(X_POINT pt, bool bPrevCache = false) = 0;
};

#endif // IWININFO_H


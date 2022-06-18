/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.18
 * Description: 管理工具栏类
 ******************************************************************/
#ifndef MANAGETOOLBAR_H
#define MANAGETOOLBAR_H

#include "frametoolbar.h"

class ManageToolBar : public FrameToolBar
{
    Q_OBJECT
public:
    explicit ManageToolBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~ManageToolBar();
};

#endif // MANAGETOOLBAR_H

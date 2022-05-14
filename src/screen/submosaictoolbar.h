/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.02.12
 * Description:
 ******************************************************************/
#ifndef SUBMOSAICTOOLBAR_H
#define SUBMOSAICTOOLBAR_H

#include "subabstoolbar.h"
#include <QObject>

class SubMosaicToolBar : public SubAbsToolBar
{
    Q_OBJECT
public:
    SubMosaicToolBar(QWidget *parent = nullptr);

    virtual void initUI() override;

    static const QPixmap* setMosaicSmooth(QPixmap* pixmap, int px);      // 毛玻璃马赛克
	static const QImage setMosaicPixlelated(QPixmap* pixmap, int px = 20);  // 像素级马赛克
};

#endif // SUBMOSAICTOOLBAR_H

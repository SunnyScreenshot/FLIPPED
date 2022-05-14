/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.12.04
 * Description:
 ******************************************************************/
#ifndef WINCOLORPICK_H
#define WINCOLORPICK_H

#include <QWidget>

/*!
 * \brief The WinColorPick class 通用的取色
 */
class WinColorPick : public QWidget
{
    Q_OBJECT
public:
    explicit WinColorPick(QWidget *parent = nullptr);

signals:

};

#endif // WINCOLORPICK_H

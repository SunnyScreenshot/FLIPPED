/*******************************************************************
 * Copyright (c) 2021-2022 ÙÉê°  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.12.14
 * Description:
 ******************************************************************/
#ifndef SUBELLIPSETOOLBAR_H
#define SUBELLIPSETOOLBAR_H

#include "subabstoolbar.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
class QToolButton;
class QSpinBox;
QT_END_NAMESPACE

class SubEllipseToolBar : public SubAbsToolBar
{
    Q_OBJECT
public:
    explicit SubEllipseToolBar(QWidget *parent = nullptr);

    virtual void initUI() override;

private:

    QSpinBox* m_spBorder;
};

#endif // SUBELLIPSETOOLBAR_H

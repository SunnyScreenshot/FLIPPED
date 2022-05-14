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
#ifndef WINDRAWRECT_H
#define WINDRAWRECT_H

#include "drawhelper.h"
#include "subabstoolbar.h"
#include <QComboBox>
#include <QSpinBox>
#include <QStringList>
#include <QToolButton>
#include <QVector>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QToolButton;
class QSpinBox;
QT_END_NAMESPACE

class SubRectToolBar : public SubAbsToolBar
{
    Q_OBJECT
public:
    explicit SubRectToolBar(QWidget *parent = nullptr);

	virtual void initUI() override;
private:
    QSpinBox* m_spBorder;
//    QToolButton* m_tbRoundCorners;
//    QComboBox* m_cbDashes;
//    QSpinBox* m_spOutLine;
//    QToolButton* m_tbColorOutLine;
//    QSlider* m_sliderOpacity;
//    QToolButton* m_tbShadow;
//    QToolButton* m_tbShadowSet
};

#endif // WINDRAWRECT_H

﻿/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.22
 * Description: 绘画工具栏 下面那个具体参数 ToolBar
 ******************************************************************/
#ifndef PARAMETERBAR_H
#define PARAMETERBAR_H

#include "../widget/xframewidget.h"
#include "base/widthparabar.h"
#include "base/colorparabar.h"
#include "base/managebar.h"
#include <QPointer>

QT_BEGIN_NAMESPACE
class QBoxLayout;
class XComboBox;
QT_END_NAMESPACE

class ParameterBar : public XFrameWidget
{
    Q_OBJECT
public:
    explicit ParameterBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~ParameterBar() = default;

private:
    void initUI();
    void addWidget(QWidget *w);
    void addSpacer();

    bool creatorRectBar();

private:
    int m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;

    QPointer<WidthParaBar> m_widthBar;         // 选择画笔宽度
    QPointer<ColorParaBar> m_colorBar;         // 取色板
    QPointer<XComboBox>    m_serialBar;        // 序号
                                               
    QPointer<ManageBar>    m_rectBar;          // 参数-矩形
    //QPointer<ManageBar>    m_ellipseBar;       // 参数-圆形
    //QPointer<ManageBar>    m_mosaicBar;        // 参数-马赛克
    //QPointer<ManageBar>    m_arrowsBar;        // 参数-箭头
};

#endif // PARAMETERBAR_H

/*******************************************************************
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
#include <QPointer>

QT_BEGIN_NAMESPACE
class QBoxLayout;
QT_END_NAMESPACE

class ParameterBar : public XFrameWidget
{
    Q_OBJECT
public:
    explicit ParameterBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~ParameterBar();

private:
    void initUI();
    void addWidget(QWidget *w);
    void addSpacer();

private:
    double m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;

    QPointer<WidthParaBar> m_widthBar;
    QPointer<ColorParaBar> m_colorBar;
};

#endif // PARAMETERBAR_H

/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.18
 * Description: 工具栏框架类，负责绘画
 ******************************************************************/
#ifndef FRAMETOOLBAR_H
#define FRAMETOOLBAR_H

#include <QWidget>
#include "../../widget/xframewidget.h"

QT_BEGIN_NAMESPACE
class QBoxLayout;
QT_END_NAMESPACE

class FrameToolBar : public XFrameWidget
{
    Q_OBJECT
public:
    explicit FrameToolBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~FrameToolBar();

    void addWidget(QWidget *w);
    void addSpacer();

    void setMargin(int margin);
    void setSpacing(int space);

private:
    void initUI();

//protected:
//    virtual void paintEvent(QPaintEvent *event) override;

private:
    int m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;
};

#endif // FRAMETOOLBAR_H

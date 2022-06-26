/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.22
 * Description: 基础底部 widget
 ******************************************************************/
#ifndef XFRAMEWIDGET_H
#define XFRAMEWIDGET_H

#include <QWidget>

class XFrameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XFrameWidget(QWidget *parent = nullptr);
    ~XFrameWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // XFRAMEWIDGET_H

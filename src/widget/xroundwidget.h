/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2021.11.06
 * Description: 圆角矩形窗口
 ******************************************************************/
#ifndef XROUNDWIDGET_H
#define XROUNDWIDGET_H

#include <QBoxLayout>
#include <QWidget>

class XRoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XRoundWidget(QWidget *parent = nullptr);
    virtual ~XRoundWidget();

    void initUI();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // XROUNDWIDGET_H

// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

#ifndef XFRAMEWIDGET_H
#define XFRAMEWIDGET_H

#include <QWidget>

class XFrameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XFrameWidget(QWidget *parent = nullptr);
    virtual ~XFrameWidget() = default;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // XFRAMEWIDGET_H

// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

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

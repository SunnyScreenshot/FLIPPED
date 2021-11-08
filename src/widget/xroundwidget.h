//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/06.
//

#ifndef XROUNDWIDGET_H
#define XROUNDWIDGET_H

#include <QWidget>

class XRoundWidget : public QWidget
{
    Q_OBJECT
public:
    XRoundWidget(QWidget *parent = nullptr);
    virtual ~XRoundWidget();

    void init();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // XROUNDWIDGET_H

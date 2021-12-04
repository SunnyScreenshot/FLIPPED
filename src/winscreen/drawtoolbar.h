#ifndef WINDRAWTOOL_H
#define WINDRAWTOOL_H

#include "../widget/xroundwidget.h"

class DrawToolBar : public XRoundWidget
{
    Q_OBJECT
public:
    explicit DrawToolBar(QWidget *parent = nullptr);

    void init();
};

#endif // WINDRAWTOOL_H

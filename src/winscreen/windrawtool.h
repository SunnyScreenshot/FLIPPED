#ifndef WINDRAWTOOL_H
#define WINDRAWTOOL_H

#include "../widget/xroundwidget.h"

class WinDrawTool : public XRoundWidget
{
    Q_OBJECT
public:
    explicit WinDrawTool(QWidget *parent = nullptr);

    void init();
};

#endif // WINDRAWTOOL_H

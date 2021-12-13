#ifndef WINDRAWTOOL_H
#define WINDRAWTOOL_H

#include "../widget/xroundwidget.h"
#include "subrecttoolbar.h"
#include "subellipsetoolbar.h"

class DrawToolBar : public XRoundWidget
{
    Q_OBJECT
public:
    explicit DrawToolBar(QWidget *parent = nullptr);

    void initUI();


private:
    SubRectToolBar* m_subRectTb;
    SubEllipseToolBar* m_subEllipseTb;
};

#endif // WINDRAWTOOL_H

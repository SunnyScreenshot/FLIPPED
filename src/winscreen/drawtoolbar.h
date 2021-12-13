#ifndef WINDRAWTOOL_H
#define WINDRAWTOOL_H

#include "../widget/xroundwidget.h"
#include "drawhelper.h"
#include "subgraptoolbar.h"
#include "subrecttoolbar.h"
#include "subellipsetoolbar.h"

class XHorizontalLine;
class DrawToolBar : public XRoundWidget
{
    Q_OBJECT
public:
    explicit DrawToolBar(QWidget *parent = nullptr);

    void initUI();

public slots:
    void onDrawShape(XDrawShape shape, bool checked);

private:
    SubGrapToolBar* m_subGrapTb;
    SubRectToolBar* m_subRectTb;
    SubEllipseToolBar* m_subEllipseTb;

    QVBoxLayout* m_vLayout;
    XHorizontalLine* m_hLine;
};

#endif // WINDRAWTOOL_H

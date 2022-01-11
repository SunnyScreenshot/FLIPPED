#ifndef WINDRAWTOOL_H
#define WINDRAWTOOL_H

#include "../widget/xroundwidget.h"
#include "drawhelper.h"
#include "subgraptoolbar.h"
#include "subrecttoolbar.h"
#include "subellipsetoolbar.h"
#include "subarrowtoolbar.h"
#include "submosaictoolbar.h"

class XHorizontalLine;
class DrawToolBar : public XRoundWidget
{
    Q_OBJECT
public:
    explicit DrawToolBar(QWidget *parent = nullptr);

    void initUI();

private:
    void insertSubBar(QWidget* subBar, int index = 1);
    void removeSubBar(QWidget* subBar);
    void removeAllSubBar();

signals:
    // SubGrapToolBar
    void sigDrawStart();
    void sigDrawEnd();
    void sigDrawShape(XDrawShape shape, bool checked);
    void sigUndo();
    void sigRedo();
    void sigDownload();
    void sigCopy();

    //
    void sigIsFill(bool bFill);

    // this class
    void sigDrawContexChange(XDrawStep step); // 切换画笔、宽度等

public slots:
    void onDrawShape(XDrawShape shape, bool checked);

private:
    SubGrapToolBar* m_subGrapBar;
    SubRectToolBar* m_subRectBar;
    SubEllipseToolBar* m_subEllipseBar;
    // TODO: 2021.12.25 SubRectToolBar SubEllipseToolBar 行为相似，可以优化为同一个类的两个对象（或者继承类）
    SubArrowToolBar* m_subArrowBar;
    SubMosaicToolBar* m_subMosaicBar;

    QVBoxLayout* m_vLayout;
    XHorizontalLine* m_hLine;
};

#endif // WINDRAWTOOL_H

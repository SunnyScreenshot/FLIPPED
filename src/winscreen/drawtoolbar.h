#ifndef WINDRAWTOOL_H
#define WINDRAWTOOL_H

#include "../widget/xroundwidget.h"
#include "drawhelper.h"
#include "subgraptoolbar.h"
#include "subrecttoolbar.h"
#include "subellipsetoolbar.h"
#include "subabslinetoolbar.h"
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
    void sigDrawShape(DrawShape shape, bool checked);
    void sigUndo();
    void sigRedo();
    void sigDownload();
    void sigCopy();

	// m_subArrowBar、m_subBrushBar
	void sigLineEndsChange(LineEnds end);
	void sigLineDasheChange(Qt::PenStyle dashe);

    //
    void sigIsFill(bool bFill);

    // this class
    void sigDrawContexChange(XDrawStep step); // 切换画笔、宽度等

public slots:
    void onDrawShape(DrawShape shape, bool checked);

private:
    SubGrapToolBar* m_subGrapBar;
    SubRectToolBar* m_subRectBar;
    SubEllipseToolBar* m_subEllipseBar;
    SubAbsLineToolBar* m_subArrowBar;
	SubAbsLineToolBar* m_subBrushBar;
    SubMosaicToolBar* m_subMosaicBar;

    QVBoxLayout* m_vLayout;
    XHorizontalLine* m_hLine;
};

#endif // WINDRAWTOOL_H

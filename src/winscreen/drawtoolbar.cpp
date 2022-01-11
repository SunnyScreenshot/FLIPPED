#include "drawtoolbar.h"
#include "../widget/xhorizontalline.h"
#include <QBoxLayout>
#include <QVector>

DrawToolBar::DrawToolBar(QWidget *parent)
    : XRoundWidget(parent)
    , m_subGrapBar(new SubGrapToolBar(this))
    , m_subRectBar(new SubRectToolBar(this))
    , m_subEllipseBar(new SubEllipseToolBar(this))
    , m_subArrowBar(new SubArrowToolBar(this))
    , m_subMosaicBar(new SubMosaicToolBar(this))
    , m_vLayout(new QVBoxLayout())
    , m_hLine(nullptr)
{
    initUI();

    // SubGrapToolBar 的信号传递
    connect(m_subGrapBar, &SubGrapToolBar::sigDownload, this, &DrawToolBar::sigDownload);
    connect(m_subGrapBar, &SubGrapToolBar::sigCopy, this, &DrawToolBar::sigCopy);

    connect(m_subGrapBar, &SubGrapToolBar::sigDrawStart, this, &DrawToolBar::sigDrawStart);
    connect(m_subGrapBar, &SubGrapToolBar::sigDrawEnd, this, &DrawToolBar::sigDrawEnd);
    connect(m_subGrapBar, &SubGrapToolBar::sigDrawShape, this, &DrawToolBar::sigDrawShape);
    connect(m_subGrapBar, &SubGrapToolBar::sigUndo, this, &DrawToolBar::sigUndo);
    connect(m_subGrapBar, &SubGrapToolBar::sigRedo, this, &DrawToolBar::sigRedo);

    // SubRectToolBar* m_subRectBar;
    connect(m_subRectBar, &SubRectToolBar::sigIsFill, this, &DrawToolBar::sigIsFill);
    connect(m_subEllipseBar, &SubEllipseToolBar::sigIsFill, this, &DrawToolBar::sigIsFill);
	connect(m_subMosaicBar, &SubMosaicToolBar::sigIsFill, this, &DrawToolBar::sigIsFill);
	
}

void DrawToolBar::initUI()
{
    const int margin = 4;
    setContentsMargins(0, 0, 0, 0);
    m_vLayout->setContentsMargins(margin, margin, margin, margin);
    m_vLayout->setSpacing(0);
    m_vLayout->addWidget(m_subGrapBar);
    m_subGrapBar->show();
    setLayout(m_vLayout);

    m_subRectBar->hide();
    m_subEllipseBar->hide();
    m_subArrowBar->hide();
    m_subMosaicBar->hide();
    m_hLine = new XHorizontalLine(m_subGrapBar->width() - margin * 2, this);
    m_hLine->hide();

    connect(m_subGrapBar, &SubGrapToolBar::sigDrawShape, this, &DrawToolBar::onDrawShape);
}

// 默认插在第二个
void DrawToolBar::insertSubBar(QWidget *subBar, int index)
{
    if (!subBar)
        return;

    if (m_vLayout) {
        m_vLayout->insertWidget(index, subBar);
        subBar->show();
    }
}

void DrawToolBar::removeSubBar(QWidget* subBar)
{
    if (!subBar)
        return;

    if (subBar && m_vLayout) {
        m_vLayout->removeWidget(subBar);
        subBar->hide();
    }
}

void DrawToolBar::removeAllSubBar()
{
    // m_subGrapTb 不移除
    QVector<QWidget *> vec = {m_subRectBar
                             , m_subEllipseBar
                             , m_subArrowBar
                             , m_subMosaicBar};

    for (auto subBar : vec) {
        if (subBar && m_vLayout)
            removeSubBar(subBar);
    }
}

void DrawToolBar::onDrawShape(XDrawShape shape, bool checked)
{
    removeAllSubBar();


    switch (shape) {
    case XDrawShape::Rectangles: {
        if (checked)
            insertSubBar(m_subRectBar);
        break;
    }
    case XDrawShape::Ellipses: {
        if (checked)
            insertSubBar(m_subEllipseBar);
        break;
    }
    case XDrawShape::Arrows: {
        if (checked)
            insertSubBar(m_subArrowBar);
        break;
    }
    case XDrawShape::Mosaics: {
        if (checked)
            insertSubBar(m_subMosaicBar);
        break;
    }
    default:
        break;
    }

    // TODO 2021.12.14:可优化，暂固定死不同状态的高度
    setFixedHeight(50);

    if (checked) {
        insertSubBar(m_hLine);
        setFixedHeight(100);
    } else {
        removeSubBar(m_hLine);
    }
}

#include "drawtoolbar.h"
#include "subgraptoolbar.h"
#include "../widget/xhorizontalline.h"
#include <QBoxLayout>

DrawToolBar::DrawToolBar(QWidget *parent)
    : XRoundWidget(parent)
    , m_subRectTb(new SubRectToolBar(this))
    , m_subEllipseTb(new SubEllipseToolBar(this))
{
    initUI();
}

void DrawToolBar::initUI()
{
    setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *vLayout = new QVBoxLayout();
    const int margin = 4;
    vLayout->setContentsMargins(margin, margin, margin, margin);
    vLayout->setSpacing(0);
    SubGrapToolBar* subGrapTb = new SubGrapToolBar(this);
    vLayout->addWidget(subGrapTb);
    subGrapTb->show();

    vLayout->addWidget(new XHorizontalLine(subGrapTb->width() - margin * 2, this));
    vLayout->addWidget(m_subRectTb);
    vLayout->addWidget(m_subEllipseTb);
    setLayout(vLayout);
}

#include "subrecttoolbar.h"
#include "drawtoolbar.h"
#include "subgraptoolbar.h"
#include "../widget/xhorizontalline.h"
#include <QBoxLayout>

DrawToolBar::DrawToolBar(QWidget *parent)
    : XRoundWidget(parent)
{
    init();
}

void DrawToolBar::init()
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
    vLayout->addWidget(new SubRectToolBar(this));
    QFrame* frame = new QFrame(this);
    frame->setFixedWidth(10);
    vLayout->addWidget(frame);
    setLayout(vLayout);
}

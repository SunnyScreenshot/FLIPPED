#include "windrawrect.h"
#include "windrawtool.h"
#include "subgraptoolbar.h"
#include <QBoxLayout>

WinDrawTool::WinDrawTool(QWidget *parent)
    : XRoundWidget(parent)
{
    init();
}

void WinDrawTool::init()
{
    setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *vLayout = new QVBoxLayout();
    const int margin = 4;
    vLayout->setContentsMargins(margin, margin, margin, margin);
    vLayout->setSpacing(0);
    vLayout->addWidget(new SubGrapToolBar(this));
    vLayout->addWidget(new WinDrawRect(this));
    QFrame* frame = new QFrame(this);
    frame->setFixedWidth(10);
    vLayout->addWidget(frame);
    setLayout(vLayout);
}

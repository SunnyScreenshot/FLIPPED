#include "windrawrect.h"
#include "subrecttoolBar.h"
#include "subgraptoolbar.h"
#include <QBoxLayout>

SubRectToolBar::SubRectToolBar(QWidget *parent)
    : XRoundWidget(parent)
{
    init();
}

void SubRectToolBar::init()
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

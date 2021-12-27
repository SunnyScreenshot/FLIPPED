#include "submosaictoolbar.h"

#include <QLabel>
#include <QSlider>

SubMosaicToolBar::SubMosaicToolBar(QWidget *parent)
    : SubAbsToolBar(parent)
{
    initAbsTb(":/resources/icons/mosaic/smooth.svg", tr("Smooth"));
    initFilledAbsTb(":/resources/icons/mosaic/pixelated.svg", tr("Pixelated"));
//    initUI(); // 会警告

    addWidget(new QLabel(tr("Strength:"), this));
    QSlider* slider = new QSlider(Qt::Horizontal, this);
    slider->setTickPosition(QSlider::TicksAbove);
    addWidget(slider);
    addStretch(0);
}

void SubMosaicToolBar::initUI()
{
}

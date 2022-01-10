#include "submosaictoolbar.h"

#include <QLabel>
#include <QSlider>
#include <QImage>

SubMosaicToolBar::SubMosaicToolBar(QWidget *parent)
    : SubAbsToolBar(parent)
{
	initUI();
}

void SubMosaicToolBar::initUI()
{
	initAbsTb(":/resources/icons/mosaic/smooth.svg", tr("Smooth"));
	initFilledAbsTb(":/resources/icons/mosaic/pixelated.svg", tr("Pixelated"));

	//addWidget(new QLabel(tr("Strength:"), this));
	//QSlider* slider = new QSlider(Qt::Horizontal, this);
	//slider->setTickPosition(QSlider::TicksAbove);
	//addWidget(slider);
	addStretch(0);
}

QImage SubMosaicToolBar::setMosaicSmooth(QPixmap * pixmap, int px)
{
	// TODO 2022-01-09:  QGraphicsBlurEffect
	// key: qt 截图 毛玻璃效果
	return QImage();
}

QImage SubMosaicToolBar::setMosaicPixlelated(QPixmap* pixmap, int px)
{
	if (!pixmap)
		return QImage();

	const QImage& image = pixmap->toImage();
	QImage* pImage = const_cast<QImage *>(&image);

	const int width = image.width();
	const int height = image.height();

	for (int i = 0; i < width; i += px) {
		for (int j = 0; j < height; j += px) {
			QSize size(px, px);
			if (width - i < px)
				size.setWidth(width - i);
			if (height - j < px)
				size.setHeight(height - j);

			const QPoint topLeft(i, j);
			const QRect rt(topLeft, size);

			//            qInfo()<<"--------->>i:"<< i << "  j:" << j << "  rt:" << rt;
			QColor color = pImage->pixelColor(rt.topLeft());
			for (int x = rt.x(); x <= rt.right(); ++x) {
				for (int y = rt.y(); y <= rt.bottom(); ++y)
					pImage->setPixelColor(x, y, color);
			}
		}
	}

	return *pImage;  // TODO 可优化: 值传递
}

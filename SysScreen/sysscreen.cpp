//
// Created by xmuli on 2021/9/25.
//

#include "sysscreen.h"
#include <QScreen>
#include <QApplication>
#include <QDebug>

void SysScreen::display() {
    for (QScreen* it: m_screens) {
        qDebug()<<"------------------------------------------\n"
        <<"count:"<<it<<"  devicePixelRatio:"<<it->devicePixelRatio()<<"  manufacturer:"<<it->manufacturer()
        <<"  model:"<<it->model()<<"  name:"<<it->name()<<"  physicalSize:"<<it->physicalSize()<<"  refreshRate:"<<it->refreshRate ()
        <<"  serialNumber:"<<it->serialNumber()<<"  size:"<<it->size()<<"  Scale:"<< getScale(it)<<"  virtualGeometry:"<<it->virtualGeometry()<<"\n"
        "Physical DPI:"<<it->physicalDotsPerInch()<<"  DPIX:"<<it->physicalDotsPerInchX()<<"  DPIY:"<<it->physicalDotsPerInchY()<<"\n"
        <<"Logical DPI:"<<it->logicalDotsPerInch()<<"  DPIX:"<<it->logicalDotsPerInchX()<<"  DPIY:"<<it->logicalDotsPerInchY()<<"\n";
    }
}

double SysScreen::getScale()
{
	return getScale(m_primaryScreen);
}

double SysScreen::getScale(QScreen * screen)
{
	double scale = screen->logicalDotsPerInch() / 96.0;
	if (scale < 1.25)
		return 1;
	else if (1.25 <= scale && scale < 1.5)
		return 1.25;
	else if (1.5 <= scale && scale < 1.75)
		return 1.5;
	else if (1.75 <= scale && scale < 2)
		return 1.75;
	else if (2 <= scale && scale < 2.25)
		return 2;
	else if (2.25 <= scale && scale < 2.5)
		return 2.25;
	else if (2.5 <= scale && scale < 3)
		return 2.5;
	else if (3 <= scale && scale < 3.5)
		return 3;
	else if (3.5 <= scale && scale < 4)
		return 3.5;
	else
		return scale;
}

SysScreen::SysScreen()
    : m_primaryScreen(nullptr)
{
    m_primaryScreen = QApplication::primaryScreen();
    m_screens = QApplication::screens();
}
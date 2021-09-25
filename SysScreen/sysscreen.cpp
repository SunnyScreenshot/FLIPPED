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
        <<"  serialNumber:"<<it->serialNumber()<<"  size:"<<it->size()<<"  virtualGeometry:"<<it->virtualGeometry()<<"\n"
        "Physical DPI:"<<it->physicalDotsPerInch()<<"  DPIX:"<<it->physicalDotsPerInchX()<<"  DPIY:"<<it->physicalDotsPerInchY()<<"\n"
        <<"Logical DPI:"<<it->logicalDotsPerInch()<<"  DPIX:"<<it->logicalDotsPerInchX()<<"  DPIY:"<<it->logicalDotsPerInchY()<<"\n";
    }
}

SysScreen::SysScreen()
    : m_primaryScreen(nullptr)
{
    m_primaryScreen = QApplication::primaryScreen();
    m_screens = QApplication::screens();
}
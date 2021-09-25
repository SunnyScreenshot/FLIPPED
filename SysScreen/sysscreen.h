//
// Created by xmuli on 2021/9/25.
//

#ifndef PICSHOT_SYSSCREEN_H
#define PICSHOT_SYSSCREEN_H

#include <QList>

QT_BEGIN_NAMESPACE
class QScreen;
QT_END_NAMESPACE

class SysScreen
{
public:
    SysScreen();
    void display();
	double getScale();
	double getScale(QScreen *screen);

private:
    QList<QScreen *> m_screens;
    QScreen * m_primaryScreen;
};

#endif //PICSHOT_SYSSCREEN_H

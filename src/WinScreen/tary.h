//
// Created by xmuli on 2021/10/08.
//

#ifndef TARY_H
#define TARY_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QSystemTrayIcon;
class QAction;
class QMenu;
QT_END_NAMESPACE

class Tary : public QObject
{
    Q_OBJECT
public:
    explicit Tary(QObject *parent = nullptr);
    ~Tary();

private:
	void init();

private:
	QAction* m_screenShot;
	QAction* m_showWinConfig;
	QAction* m_quit;
	QMenu* m_menuTary;
	QSystemTrayIcon* m_sysTary;
};

#endif // TARY_H

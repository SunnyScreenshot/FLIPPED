//
// Created by XMuli <xmulitech@gmail.com> on 2021/10/05.
//

#ifndef WINMAIN_H
#define WINMAIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QKeySequenceEdit;
class QLabel;
QT_END_NAMESPACE

class WinMain : public QWidget
{
    Q_OBJECT
public:
    explicit WinMain(QWidget *parent = nullptr);
	~WinMain();

private:
	void init();
	QWidget* tabShortcuts();
	//void setLayout(QWidget *parent = nullptr);

signals:


private:
	QKeySequenceEdit* m_scrnShot;        // 截图
	QKeySequenceEdit* m_scrnShotLater;   // 延时截图
	QKeySequenceEdit* m_scrnShotRect;    // 矩形区域截图
	QKeySequenceEdit* m_scrnShotWhole;   // 整个屏幕截图

	QLabel* m_labScrnShot;
	QLabel* m_labScrnShotLater;
	QLabel* m_labScrnShotRect;
	QLabel* m_labScrnShotWhole;
};

#endif // WINMAIN_H

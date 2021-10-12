//
// Created by XMuli <xmulitech@gmail.com> on 2021/10/05.
//

#ifndef WINMAIN_H
#define WINMAIN_H

#include <QWidget>
#include "../widget/xkeysequenceedit.h"

QT_BEGIN_NAMESPACE
class XKeySequenceEdit;
class QLabel;
class QLineEdit;
class QToolButton;
QT_END_NAMESPACE

class WinMain : public QWidget
{
    Q_OBJECT
public:
    explicit WinMain(QWidget *parent = nullptr);
	~WinMain();

private:
	void init();
	QWidget* tabScreenShot();
	QWidget* tabOutput();
	QWidget* tabShortcuts();
	QWidget* tabAbout();

	//void setLayout(QWidget *parent = nullptr);

signals:

public slots:
    void onScrnShot(const QKeySequence &keySequence);

private:
	// tab shortcut
    XKeySequenceEdit* m_scrnShot;        // 截图
    XKeySequenceEdit* m_scrnShotLater;   // 延时截图
    XKeySequenceEdit* m_scrnShotRect;    // 矩形区域截图
    XKeySequenceEdit* m_scrnShotWhole;   // 整个屏幕截图
	QLabel* m_labScrnShot;
	QLabel* m_labScrnShotLater;
	QLabel* m_labScrnShotRect;
	QLabel* m_labScrnShotWhole;

	// tab optput
	QLineEdit* m_leFileName;        // 文件名称
	QLineEdit* m_leScrnPath;        // 截图路径
	QLineEdit* m_leConfPath;        // 配置路径
	QLineEdit* m_leLogPath;         // 日志路径
	QToolButton* m_tbFileName;
	QToolButton* m_tbScrnPath;
	QToolButton* m_tbConfPath;
	QToolButton* m_tbLogPath;
};

#endif // WINMAIN_H

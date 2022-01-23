//
// Created by XMuli <xmulitech@gmail.com> on 2021/10/05.
//

#ifndef WINMAIN_H
#define WINMAIN_H

#include <QWidget>
#include "../widget/xkeysequenceedit.h"

class QHotkey;

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
    virtual ~WinMain();

private:
	void initUI();
    void hotKeyInit();
	bool hotKeyStateUpdate(QLabel* lab, QHotkey* hotKey);
	QWidget* tabScreenShot();
	QWidget* tabOutput();
	QWidget* tabShortcuts();
	QWidget* tabAbout();

	//void setLayout(QWidget *parent = nullptr);

public slots:
    void onScrnShot();
	void onScrnShotDelay();
	void onScrnShotFixedSize();
	void onScrnShotFullScreen();

    void onKeySeqChanged(const QKeySequence &keySequence);

private:
	// tab shortcut
	XKeySequenceEdit* m_scrnShot;            // 截图（手动矩形）
	XKeySequenceEdit* m_scrnShotWinObj;      // 窗口/对象截图
	XKeySequenceEdit* m_scrnShotDelay;       // 延时截图
	XKeySequenceEdit* m_scrnShotFixedSize;   // 固定区域截图
    XKeySequenceEdit* m_scrnShotFullScreen;  // 整个屏幕截图

	// tab optput
	QLineEdit* m_leFileName;                 // 文件名称
	QLineEdit* m_leScrnPath;                 // 截图路径
	QLineEdit* m_leConfPath;                 // 配置路径
	QLineEdit* m_leLogPath;                  // 日志路径
	QToolButton* m_tbFileName;
	QToolButton* m_tbScrnPath;
	QToolButton* m_tbConfPath;
	QToolButton* m_tbLogPath;

    // hotKey
	QHotkey* m_hkScrnShot;                   // 截图（手动矩形）
	QHotkey* m_hkScrnShotWinObj;             // 窗口/对象截图
	QHotkey* m_hkScrnShotDelay;              // 延时截图
	QHotkey* m_hkScrnShotFixedSize;          // 固定区域截图
	QHotkey* m_hkScrnShotFullScreen;         // 整个屏幕截图



public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // WINMAIN_H

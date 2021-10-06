//
// Created by XMuli <xmulitech@gmail.com> on 2021/10/06.
//

#ifndef WINRESETBTN_H
#define WINRESETBTN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class WinResetBtn : public QWidget
{
public:
	explicit WinResetBtn(QWidget *parent = nullptr);
	~WinResetBtn();

	QPushButton* getBtnReset();

private:
	void init();


private:
	QPushButton* m_btnReset;
};


#endif // WINRESETBTN_H

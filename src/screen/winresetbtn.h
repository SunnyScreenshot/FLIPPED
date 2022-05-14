/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.09.29
 * Description:
 ******************************************************************/
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

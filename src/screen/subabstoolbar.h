/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.12.27
 * Description:
 ******************************************************************/
#ifndef SUBABSTOOLBAR_H
#define SUBABSTOOLBAR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE

class SubAbsToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit SubAbsToolBar(QWidget *parent = nullptr);
    void addWidget(QWidget *widget);
    void addStretch(int stretch);

    virtual void initUI() = 0;

protected:
    void initAbsTb(QString tbPath, QString tbToolTip);
    void initFilledAbsTb(QString tbPath, QString tbToolTip);
	
signals:
    void sigIsFill(bool bFill);

public slots:
    void onClicked(bool checked);
	void onToggleOnlyOneBtn(bool bFill);

private:
    QToolButton* m_tbAbs;
    QToolButton* m_tbFilledAbs;
};

#endif // SUBABSTOOLBAR_H

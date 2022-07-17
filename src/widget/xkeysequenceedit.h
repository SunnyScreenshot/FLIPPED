/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2021.10.10
 * Description: 快捷键编辑框
 ******************************************************************/
#ifndef XKEYSEQUENCEEDIT_H
#define XKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>

class XKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    explicit XKeySequenceEdit(QWidget *parent = nullptr);
    explicit XKeySequenceEdit(const QKeySequence &keySequence, QWidget *parent = nullptr);
    virtual ~XKeySequenceEdit() = default;

private:
    void initUI();
//	void test();

signals:
	void sigKeySeqChanged(const QKeySequence &keySequence);

//public slots:
	//void onEditingFinished();
	//void onKeySequenceChanged(const QKeySequence &keySequence);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    //void mousePressEvent(QMouseEvent* event) override;
};



#endif // XKEYSEQUENCEEDIT_H

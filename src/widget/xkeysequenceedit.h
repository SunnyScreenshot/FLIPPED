// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

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

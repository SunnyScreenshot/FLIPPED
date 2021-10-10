//
// Created by xmuli on 2021/10/10.
//

#ifndef XKEYSEQUENCEEDIT_H
#define XKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>

class XKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    XKeySequenceEdit(QWidget *parent = 0);
    XKeySequenceEdit(const QKeySequence &keySequence, QWidget *parent = nullptr);
    ~XKeySequenceEdit();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // XKEYSEQUENCEEDIT_H

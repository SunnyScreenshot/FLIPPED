//
// Created by xmuli on 2021/10/10.
//

#include "xkeysequenceedit.h"
#include <QWidget>

/*!
 * \brief The XKeySequenceEdit class 设置仅接受一个快捷键组合；删除清空
 */

XKeySequenceEdit::XKeySequenceEdit(QWidget *parent)
    : QKeySequenceEdit(parent)
{

}

XKeySequenceEdit::XKeySequenceEdit(const QKeySequence &keySequence, QWidget *parent)
    : QKeySequenceEdit(keySequence, parent)
{

}

XKeySequenceEdit::~XKeySequenceEdit()
{

}

void XKeySequenceEdit::keyPressEvent(QKeyEvent *event)
{
    QKeySequenceEdit::keyPressEvent(event);

    QKeySequence keySeq = keySequence();
    if ( keySeq.count() <= 0)
        return;

    int key = keySeq[0];
    if (key == Qt::Key_Backspace || key == Qt::Key_Delete)
        key = 0;

//    其中，int key = keySeq[0]; 更准确的写法应该是 int key = keySeq[keySeq.count()-1];
//    ，但实际两句的效果一样，原因在于setKeySequence函数的内部实现。
    setKeySequence(key);
}

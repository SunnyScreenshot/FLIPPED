//
// Created by xmuli on 2021/12/14.
//
#ifndef SUBELLIPSETOOLBAR_H
#define SUBELLIPSETOOLBAR_H

#include "subabstoolbar.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
class QToolButton;
class QSpinBox;
QT_END_NAMESPACE

class SubEllipseToolBar : public SubAbsToolBar
{
    Q_OBJECT
public:
    explicit SubEllipseToolBar(QWidget *parent = nullptr);

    virtual void initUI() override;

private:

    QSpinBox* m_spBorder;
};

#endif // SUBELLIPSETOOLBAR_H

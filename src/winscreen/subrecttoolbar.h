//
// Created by xmuli on 2021/12/04.
//
#ifndef WINDRAWRECT_H
#define WINDRAWRECT_H

#include "drawhelper.h"
#include "subabstoolbar.h"
#include <QComboBox>
#include <QSpinBox>
#include <QStringList>
#include <QToolButton>
#include <QVector>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QToolButton;
class QSpinBox;
QT_END_NAMESPACE

class SubRectToolBar : public SubAbsToolBar
{
    Q_OBJECT
public:
    explicit SubRectToolBar(QWidget *parent = nullptr);

	virtual void initUI() override;
private:

//    QToolButton* m_tbColorBorder;
    QSpinBox* m_spBorder;
//    QToolButton* m_tbRoundCorners;
//    QComboBox* m_cbDashes;
//    QSpinBox* m_spOutLine;
//    QToolButton* m_tbColorOutLine;
//    QSlider* m_sliderOpacity;
//    QToolButton* m_tbShadow;
//    QToolButton* m_tbShadowSet
};

#endif // WINDRAWRECT_H

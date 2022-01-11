//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/23.
//
#ifndef SUBARROWTOOLBAR_H
#define SUBARROWTOOLBAR_H

#include <QSpinBox>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

// 矩形和椭圆等的线型、宽度
class SubArrowToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit SubArrowToolBar(QWidget *parent = nullptr);

private:
    void initUI();
signals:

private:
    QComboBox* m_cbEnds;
    QComboBox* m_cbDashes;

};

#endif // SUBLINETOOLBAR_H

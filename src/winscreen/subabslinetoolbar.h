//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/23.
//
#ifndef SUBABSLINETOOLBAR_H
#define SUBABSLINETOOLBAR_H

#include <QSpinBox>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

// 矩形和椭圆等的线型、宽度
class SubAbsLineToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit SubAbsLineToolBar(QWidget *parent = nullptr);

private:
    void initUI();
signals:

private:
    QComboBox* m_cbEnds;
    QComboBox* m_cbDashes;

};

#endif // SUBLINETOOLBAR_H
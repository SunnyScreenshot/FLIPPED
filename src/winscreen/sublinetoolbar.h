//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/23.
//
#ifndef SUBLINETOOLBAR_H
#define SUBLINETOOLBAR_H

#include <QSpinBox>
#include <QWidget>

// 矩形和椭圆等的线型、宽度
class SubLineToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit SubLineToolBar(QWidget *parent = nullptr);

private:
    void initUI();
signals:

private:
    QSpinBox* m_spBorder;

};

#endif // SUBLINETOOLBAR_H

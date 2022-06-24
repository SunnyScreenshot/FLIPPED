/*******************************************************************
 * Copyright (c) 2021-2022 Ÿ…Í∞  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.25
 * Description: ≤‚ ‘
 ******************************************************************/
#ifndef EXWIDGET_H
#define EXWIDGET_H

#include <QColor>
#include <QMap>
#include <QString>
#include <QWidget>
 //#include "../screen/drawhelper.h"

// test
#include <QMessageBox>
#include <QDebug>
#include <QPushButton>

class XLabel;
class QGridLayout;
class ExWidget : public QWidget
{
    Q_OBJECT
public:
    ExWidget(Qt::Orientations orien = Qt::Horizontal, QWidget* parent = nullptr);
    virtual ~ExWidget();
    void init();

signals:
    void sigPickColor(XLabel*, QColor);

public slots:
    void onPickColor(XLabel* lab, QColor col);
    void onBtnTest();

public:
    //virtual bool eventFilter(QObject* watched, QEvent* event) override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    int m_scal;
    Qt::Orientations m_orien;
    QGridLayout* m_layout;
    QMap<QString, QString> m_labMap;
    XLabel* m_curLab;
    QColor m_curColor;
};

#endif // EXWIDGET_H
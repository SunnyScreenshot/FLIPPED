/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.21
 * Description: 取色器面板
 ******************************************************************/
#ifndef COLORPARABAR_H
#define COLORPARABAR_H

#include "frametoolbar.h"
#include <QColor>
#include <QMap>
#include <QString>
#include <QWidget>
//#include "../screen/drawhelper.h"

class XLabel;
class QGridLayout;
class ColorParaBar : public QWidget
{
    Q_OBJECT
public:
    ColorParaBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~ColorParaBar();
    void init();

signals:
    void sigPickColor(XLabel*, QColor);

public slots:
    void onPickColor(XLabel* lab, QColor col);

public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    int m_scal;
    Qt::Orientations m_orien;
    QGridLayout* m_layout;
    QMap<QString, QString> m_labMap;
    XLabel* m_curLab;
    QColor m_curColor;
};

#endif // COLORPARABAR_H

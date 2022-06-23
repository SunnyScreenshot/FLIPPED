/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.22
 * Description: Line Widt
 ******************************************************************/
#ifndef WIDTHPARABAR_H
#define WIDTHPARABAR_H

#include "../../widget/xframewidget.h"

QT_BEGIN_NAMESPACE
class QBoxLayout;
QT_END_NAMESPACE

class WidthParaBar : public QWidget
{
    Q_OBJECT
public:
    explicit WidthParaBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~WidthParaBar();

    void addWidget(QWidget *w);
    void addSpacer();

private:
    void initUI();

//protected:
//    virtual void paintEvent(QPaintEvent *event) override;

private:
    double m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;
};

#endif // WIDTHPARABAR_H

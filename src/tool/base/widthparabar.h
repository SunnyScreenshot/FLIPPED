/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.22
 * Description: Line Widt, archive
 ******************************************************************/
#ifndef WIDTHPARABAR_H
#define WIDTHPARABAR_H

#include "../../widget/xframewidget.h"

QT_BEGIN_NAMESPACE
class QBoxLayout;
class XLabel;
QT_END_NAMESPACE

class WidthParaBar : public QWidget
{
    Q_OBJECT
public:
    explicit WidthParaBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~WidthParaBar();

private:
    void initUI();
    void addWidget(QWidget* w);
    void addSpacer();
    void setChecked(XLabel* selLab, bool reset = false);

public:
    bool eventFilter(QObject* watched, QEvent* event) override;


private:
    int m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;
};

#endif // WIDTHPARABAR_H

/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.22
 * Description: 工具栏框架类，负责选择画各种图形的 ToolBar
 ******************************************************************/
#ifndef SELECTBAR_H
#define SELECTBAR_H

#include "base/frametoolbar.h"
#include "../screen/drawhelper.h"
#include <QVector>

QT_BEGIN_NAMESPACE
class QToolButton;
class QStringList;
QT_END_NAMESPACE

// rectangle
// ellipse
// line
// arrow
// pen
// mosaic  、 smooth
// text
// serialnumber
// gif 暂不添加
// revocation
// renewal
// save
// cancel
// finish
class SelectBar : public XFrameWidget
{
    Q_OBJECT
public:
    SelectBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~SelectBar();

private:
    void initUI();
    void addWidget(QWidget *w);
    void addSpacer();

public slots:
    void onToolBtn();

signals:
    void sigEnableDraw(bool enable);
    void sigDrawStart();       // 处于绘画状态
    void sigDrawEnd();         // 处于绘画状态
    void sigSelShape(DrawShape shape, bool checked);
    void sigRevocation();
    void sigRenewal();
    void sigSave();
    void sigCancel();
    void sigFinish();

private:
    int m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;

    QStringList m_tbName;
    QStringList m_tbOnlyClickName;
    QVector<QToolButton *> m_vItem;
};

#endif // SELECTBAR_H

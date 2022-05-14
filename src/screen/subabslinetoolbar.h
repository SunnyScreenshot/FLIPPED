/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.12.23
 * Description:
 ******************************************************************/
#ifndef SUBABSLINETOOLBAR_H
#define SUBABSLINETOOLBAR_H

#include "drawhelper.h"
#include <QSpinBox>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
QT_END_NAMESPACE

// 抽象绘画线：直线和画刷线
class SubAbsLineToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit SubAbsLineToolBar(QWidget *parent = nullptr);

	static QPainterPath getArrowHead(QPoint p1, QPoint p2, const int thickness = 10);
	static QLine getShorterLine(QPoint p1, QPoint p2, const int thickness = 10);

private:
    void initUI();

signals:
	void sigLineEndsChange(LineEnds ends);
	void sigLineDasheChange(Qt::PenStyle dashes);

private:
    QComboBox* m_cbEnds;
    QComboBox* m_cbDashes;
};

#endif // SUBLINETOOLBAR_H

/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.26
 * Description: 参数工具栏的一部分，作为管理类：矩形、椭圆、箭头、马赛克等
 ******************************************************************/
#ifndef MANAGEBAR_H
#define MANAGEBAR_H

#include <QWidget>

class QBoxLayout;
class ManageBar : public QWidget
{
    Q_OBJECT
public:
    explicit ManageBar(Qt::Orientations orien = Qt::Horizontal, QWidget* parent = nullptr);
    virtual ~ManageBar() = default;
    void addWidget(QWidget* w);
    void addSpacer();

private:
    double m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;
};

#endif // MANAGEBAR_H

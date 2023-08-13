// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

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

// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

#ifndef MAGNIFIER_H
#define MAGNIFIER_H

#include <QWidget>

class Magnifier : public QWidget
{
    Q_OBJECT
public:
    explicit Magnifier(QWidget* parent = nullptr);
    ~Magnifier() = default;
};

#endif // MAGNIFIER_H

/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.02
 * Description: 放大镜
 ******************************************************************/

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

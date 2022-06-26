/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.25
 * Description: 自定义的下拉列表框
 ******************************************************************/
#ifndef XCOMBOBOX_H
#define XCOMBOBOX_H

#include <QComboBox>

class XComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit XComboBox(QWidget* parent = nullptr);
    virtual ~XComboBox();
};

#endif // XCOMBOBOX_H

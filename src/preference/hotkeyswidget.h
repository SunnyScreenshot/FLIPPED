/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.17
 * Description: 偏好设置界面的 hotkeys UI；为了规避掉 编辑框 鼠标点击外面却仍有焦点
 ******************************************************************/
#ifndef HOTKEYSWIDGET_H
#define HOTKEYSWIDGET_H

#include <QWidget>

class HotkeysWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HotkeysWidget(QWidget *parent = nullptr);
    virtual ~HotkeysWidget() = default;

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // HOTKEYSWIDGET_H

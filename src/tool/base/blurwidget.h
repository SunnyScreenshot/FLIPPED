/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.16
 * Description: 模拟 亚克力/透明磨砂 效果（此处取巧）
 ******************************************************************/
#ifndef BLURWIDGET_H
#define BLURWIDGET_H

#include <QWidget>

class QPixmap;
class QGraphicsBlurEffect;

class BlurWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlurWidget(QWidget *parent = nullptr);
    virtual ~BlurWidget() = default;

    virtual void setPixmap(const QPixmap& pix, double blurRadius);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pix;
    QGraphicsBlurEffect* m_pBlurEffect;
};

#endif // BLURWIDGET_H

// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

#ifndef BLURWIDGET_H
#define BLURWIDGET_H

#include <memory>
#include <QWidget>
#include <QGraphicsBlurEffect>

// 模拟 亚克力/透明磨砂 效果（此处取巧）
class QPixmap;
class BlurWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlurWidget(QWidget *parent = nullptr);
    virtual ~BlurWidget() = default;
    void setPixmap(const QPixmap& pix, double blurRadius);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pix;
//    std::unique_ptr<QGraphicsBlurEffect> m_pBlurEffect;
};

#endif // BLURWIDGET_H

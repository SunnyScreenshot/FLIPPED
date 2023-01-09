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

#include "../screen/drawhelper.h"
#include <memory>
#include "base/blurwidget.h"

class BlurWidget;
class QToolButton;
class QStringList;
class QBoxLayout;
class QAbstractButton;
class QButtonGroup;

// rectangle
// ellipse
// arrow
// pen
// mosaic、smooth
// text
// serialnumber、
// pin
// gif 暂不添加
// revocation
// renewal
// save
// cancel
// finish
class SelectBar : public QWidget
{
    Q_OBJECT
public:
    SelectBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~SelectBar() = default;
    void setBlurBackground(const QPixmap& pix, double blurRadius);

private:
    void initUI();
    void addWidget(QWidget *w);
    void addSpacer();

public slots:
    void onToolBtn();

signals:
    void sigEnableDraw(bool enable);
    void sigSelShape(DrawShape shape, bool checked);
    void sigRevocation();
    void sigRenewal();
    void sigPin();
    void sigSave();
    void sigCancel();
    void sigFinish();
    void sigInterruptEdit(const QPoint& pos);

protected: 
    void enterEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;   // 仅单独适配 MAC 效果

private:
    double m_scal;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;
    std::unique_ptr<BlurWidget> m_blur;
};

#endif // SELECTBAR_H

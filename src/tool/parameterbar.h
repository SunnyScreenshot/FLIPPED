/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.22
 * Description: 绘画工具栏 下面那个具体参数 ToolBar
 ******************************************************************/
#ifndef PARAMETERBAR_H
#define PARAMETERBAR_H

#include "../screen/drawhelper.h"
#include "base/colorparabar.h"
#include "base/managebar.h"
#include <QPointer>

class XComboBox;
class BlurWidget;
class QBoxLayout;
class QAbstractButton;
class QToolButton;

class ParameterBar : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~ParameterBar() = default;
    void setBlurBackground(const QPixmap& pix, double blurRadius);

private:
    void initUI();
    void addWidget(QWidget *w);
    void addSpacer();

    void creatorParaBar(QPointer<ManageBar>& manageBar, QString& path, QStringList items);                    // 比如矩形等
    void initRectBar();
    void initEllipseBar();
    void initMosaicBar();
    void initArrowBar();
    void initLineWidthBar();

    void removeAllBar();                                   // 移除所有的参数 Bar

signals:
    void sigParaBtnId(DrawShape shap, QToolButton* tb);    // ToolButton 发射的信号
    void sigSelColor(QColor);                              // XLabel 发射的信号

public slots:
    void onTBReleased(QAbstractButton* btn);
    void onEnableDraw(bool enable);
    void onSelShape(DrawShape shape, bool checked);

protected:
    void enterEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    double m_scal;
    BlurWidget* m_blur;
    Qt::Orientations m_orien;
    QBoxLayout* m_layout;

    QPointer<ColorParaBar> m_colorBar;                     // 取色板
    QPointer<XComboBox>    m_serialBar;                    // 序号
                                                           
    QPointer<ManageBar>    m_rectBar;                      // 参数-矩形     TODO 2022.05.26 此几个变量也可以省略掉， 通过slot 里面的 传入 btn 的 partent来控制即可
    QPointer<ManageBar>    m_ellipseBar;                   // 参数-圆形
    QPointer<ManageBar>    m_mosaicBar;                    // 参数-马赛克
    QPointer<ManageBar>    m_arrowBar;                     // 参数-箭头
    QPointer<ManageBar>    m_lienWidthBar;                 // 参数-线宽
};

#endif // PARAMETERBAR_H

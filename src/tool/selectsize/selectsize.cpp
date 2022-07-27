/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.02
 * Description: 选中矩形区域的尺寸大小，显示在左上角
 ******************************************************************/
#include "selectsize.h"
#include "../../xglobal.h"
#include "../../screen/drawhelper.h"
#include <QPainter>

SelectSize::SelectSize(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
    : QLabel(parent, f)
{
    initUI();
}

SelectSize::SelectSize(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
    : QLabel(text, parent, f)
{
    initUI();
}

void SelectSize::onTextChanged(QString text)
{
    QFontMetrics fm(this->font());
    QRect bound = fm.boundingRect(QRect(), Qt::AlignCenter, text);

    setMinimumWidth(bound.width());  // TODO 2022.07.02: 需要优化，x 坐标比较大时候，则右侧留白会过长
    update(rect());
}

void SelectSize::initUI()
{
    setContentsMargins(SS_MARGIN_LEFT, SS_MARGIN_TOP, SS_MARGIN_RIGHT, SS_MARGIN_BOTTOM);
    connect(this, &SelectSize::sigTextChanged, this, &SelectSize::onTextChanged);
}

void SelectSize::paintEvent(QPaintEvent* e)
{
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor col(XHelp::borderColor());
    pa.setPen(Qt::NoPen);
    pa.setBrush(col);
    pa.drawRoundedRect(rect(), SS_RADIRS, SS_RADIRS);

    pa.setPen("#FFFFFF");
    pa.setBrush(Qt::NoBrush);
    pa.drawText(contentsRect(), text());
}

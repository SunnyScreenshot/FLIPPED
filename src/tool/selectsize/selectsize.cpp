// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

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
    text = this->text();
    QFontMetrics fm(font());
    QRect bound = fm.boundingRect(QRect(), Qt::AlignCenter, text);
//    qDebug() << "SelectSize::onTextChanged()" << text << "  " <<bound << "  " << rect();
    setFixedSize(bound.size());
//    qDebug() <<bound << "  " << rect();
}

void SelectSize::initUI()
{
    setContentsMargins(SS_MARGIN_LEFT, SS_MARGIN_TOP, SS_MARGIN_RIGHT, SS_MARGIN_BOTTOM);
    connect(this, &SelectSize::sigTextChanged, this, &SelectSize::onTextChanged);
}

//void SelectSize::showEvent(QShowEvent *e)
//{
//    QWidget::showEvent(e);
//    onTextChanged("");

//    qDebug() << " rect:" << rect() << "   " << this->text();
//}

void SelectSize::paintEvent(QPaintEvent* e)
{
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor col(XHelper::instance().borderColor());
    pa.setPen(Qt::NoPen);
    pa.setBrush(col);
    pa.drawRoundedRect(rect(), SS_RADIRS, SS_RADIRS);

    pa.setPen("#FFFFFF");
    pa.setBrush(Qt::NoBrush);
    pa.drawText(contentsRect(), Qt::AlignCenter, text());
}


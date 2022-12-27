/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.01.16
 * Description: 文字编辑窗口
 ******************************************************************/
#include "xtextwidget.h"
#include <QFontMetrics>
#include <QTextCharFormat>
#include <QPen>
#include <QDebug>
#include "../core/xlog.h"

XTextWidget::XTextWidget(QWidget *parent)
    : QTextEdit(parent)
    , m_baseSize()
    , m_minSize()
{
    XLOG_DEBUG("rect({}, {}, {} * {})", rect().left(), rect().top(), rect().width(), rect().height());
    
    //setStyleSheet(QStringLiteral("XTextWidget { background: transparent; }"));
    connect(this, &XTextWidget::textChanged, this, &XTextWidget::adjustSize);
    connect(this, &XTextWidget::textChanged, this, &XTextWidget::emitTextUpdated);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::NoContextMenu);
}

XTextWidget::~XTextWidget()
{
    XLOG_DEBUG("rect({}, {}, {} * {})", rect().left(), rect().top(), rect().width(), rect().height());
}

// https://blog.csdn.net/kenfan1647/article/details/115171891
void XTextWidget::adjustSize()
{
    XLOG_DEBUG("--a1--rect({}, {}, {} * {})", rect().left(), rect().top(), rect().width(), rect().height());

    QString&& text = this->toPlainText();

    QFontMetrics fm(font());
    QRect bounds = fm.boundingRect(QRect(), Qt::AlignLeft, text);  // 需要再研究下
    int pixelsWide = bounds.width() + fm.lineSpacing();
    int pixelsHigh = bounds.height() * 1.15 + fm.lineSpacing();
    if (pixelsWide < m_minSize.width()) {
        pixelsWide = m_minSize.width();
    }
    if (pixelsHigh < m_minSize.height()) {
        pixelsHigh = m_minSize.height();
    }

    XLOG_DEBUG("--a2--rect({}, {}, {} * {})", rect().left(), rect().top(), rect().width(), rect().height());
    this->setFixedSize(pixelsWide, pixelsHigh);

    XLOG_DEBUG("--a3--rect({}, {}, {} * {})", rect().left(), rect().top(), rect().width(), rect().height());
    XLOG_DEBUG("bounds({}, {}, {} * {})  fm.lineSpacing[{}]  pixelsWide[{}]  pixelsHigh[{}]  m_minSize({}, {})", bounds.left(), bounds.top(), bounds.width(), bounds.height()
        , fm.lineSpacing(), pixelsWide, pixelsHigh , m_minSize.width(), m_minSize.height());
}

void XTextWidget::setFont(const QFont & font)
{
    XLOG_DEBUG("rect({}, {}, {} * {})", rect().left(), rect().top(), rect().width(), rect().height());
    QTextEdit::setFont(font);
    adjustSize();
}

void XTextWidget::showEvent(QShowEvent *e)
{
    QFont font;
    QFontMetrics fm(font);  // 赋值一个初始的宽高

    XLOG_DEBUG("--a1--rect({}, {}, {} * {})  size({}，{})   fm.lineSpacing[{}]", rect().left(), rect().top(), rect().width(), rect().height(), size().width(), size().height(), fm.lineSpacing());

    setFixedWidth(fm.lineSpacing() * 6);
    setFixedHeight(fm.lineSpacing() * 2.5);
    m_baseSize = size();
    m_minSize = m_baseSize;

    QTextEdit::showEvent(e);
    adjustSize();

    XLOG_DEBUG("--a2--rect({}, {}, {} * {})  size({}，{})   fm.lineSpacing[{}]", rect().left(), rect().top(), rect().width(), rect().height(), size().width(), size().height(), fm.lineSpacing());
}

// 当 widget 大小被重新设置的时候就会被调用
void XTextWidget::resizeEvent(QResizeEvent *e)
{
    XLOG_DEBUG("--a1--rect({}, {}, {} * {})  size({}，{})", rect().left(), rect().top(), rect().width(), rect().height(), size().width(), size().height());
    m_minSize.setHeight(qMin(m_baseSize.height(), height()));
    m_minSize.setWidth(qMin(m_baseSize.width(), width()));
    QTextEdit::resizeEvent(e);

    XLOG_DEBUG("--a2--rect({}, {}, {} * {})  size({}，{})", rect().left(), rect().top(), rect().width(), rect().height(), size().width(), size().height());
}

void XTextWidget::setTextColor(const QColor & c)
{
    QString s(QStringLiteral("XTextWidget { background: transparent; color: %1; }"));
    setStyleSheet(s.arg(c.name()));
}

void XTextWidget::setAlignment(Qt::AlignmentFlag alignment)
{
    QTextEdit::setAlignment(alignment);
    adjustSize();
}

void XTextWidget::emitTextUpdated()
{
    XLOG_DEBUG("--a1--rect({}, {}, {} * {})  size({}，{})", rect().left(), rect().top(), rect().width(), rect().height(), size().width(), size().height());
    emit textUpdated(this->toPlainText());

    XLOG_DEBUG("--a2--rect({}, {}, {} * {})  size({}，{})", rect().left(), rect().top(), rect().width(), rect().height(), size().width(), size().height());
}

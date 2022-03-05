//
// Created by XMuli <xmulitech@gmail.com> on 2022/01/16.
//
#include "xtextwidget.h"
#include <QFontMetrics>
#include <QDebug>

XTextWidget::XTextWidget(QWidget *parent)
	: QTextEdit(parent)
	, m_baseSize()
	, m_minSize()
{
    qInfo() << "###############> [XTextWidget::resizeEvent()]";
    //setStyleSheet(QStringLiteral("XTextWidget { background: transparent; }"));
	connect(this, &XTextWidget::textChanged, this, &XTextWidget::adjustSize);
	connect(this, &XTextWidget::textChanged, this, &XTextWidget::emitTextUpdated);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setContextMenuPolicy(Qt::NoContextMenu);
}

XTextWidget::~XTextWidget()
{
    qInfo() << "###############> [XTextWidget::~XTextWidget()]";
}

// https://blog.csdn.net/kenfan1647/article/details/115171891
void XTextWidget::adjustSize()
{
    qInfo() << "###############> [XTextWidget::adjustSize()]";
    const QString& text = this->toPlainText();
	QFontMetrics fm(this->font());
	QRect bound = fm.boundingRect(QRect(), Qt::AlignLeft, text); // 需要再研究下

	const int lineSpacing = fm.lineSpacing();
	int pixelsWide = bound.width() + lineSpacing;
	int pixelsHigh = bound.height() + lineSpacing;

	if (pixelsWide < m_minSize.width())
		pixelsWide = m_minSize.width();

	if (pixelsHigh < m_minSize.height())
		pixelsHigh = m_minSize.height();

	qInfo() << "----------pixelsWide:" << pixelsWide << "  pixelsHigh:" << pixelsHigh;
	setFixedSize(pixelsWide, pixelsHigh);
}

void XTextWidget::setFont(const QFont & font)
{
	QTextEdit::setFont(font);
	adjustSize();
}


void XTextWidget::showEvent(QShowEvent *e)
{
    QFont font;
    QFontMetrics fm(font);  // 赋值一个初始的宽高
	qInfo() << "###############> [XTextWidget::showEvent()]" << size() << fm.lineSpacing();
    setFixedWidth(fm.lineSpacing() * 6);
    setFixedHeight(fm.lineSpacing() * 2.5);
    m_baseSize = size();
    m_minSize = m_baseSize;

	QTextEdit::showEvent(e);
	adjustSize();

	qInfo() << "###############> [XTextWidget::showEvent()]  rect:" << rect();
}

// 当 widget 大小被重新设置的时候就会被调用
void XTextWidget::resizeEvent(QResizeEvent *e)
{
    qInfo() << "###############> [XTextWidget::resizeEvent()]";
	m_minSize.setHeight(qMin(m_baseSize.height(), height()));
	m_minSize.setWidth(qMin(m_baseSize.width(), width()));
	QTextEdit::resizeEvent(e);
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
    emit textUpdated(this->toPlainText());
}

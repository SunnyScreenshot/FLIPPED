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
	//setStyleSheet(QStringLiteral("TextWidget { background: transparent; }"));
	connect(this, &XTextWidget::textChanged, this, &XTextWidget::adjustSize);
	// connect(this, &TextWidget::textChanged, this, &TextWidget::emitTextUpdated);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setContextMenuPolicy(Qt::NoContextMenu);
}

XTextWidget::~XTextWidget()
{
}

// 
// https://blog.csdn.net/kenfan1647/article/details/115171891
void XTextWidget::adjustSize()
{
	qInfo() << "###############> adjustSize";
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
	qInfo() << "###############> showEvent";
	QTextEdit::showEvent(e);
	adjustSize();
}

void XTextWidget::resizeEvent(QResizeEvent *e)
{
	qInfo() << "###############> resizeEvent";
	m_minSize.setHeight(qMin(m_baseSize.height(), height()));
	m_minSize.setWidth(qMin(m_baseSize.width(), width()));
	QTextEdit::resizeEvent(e);
}

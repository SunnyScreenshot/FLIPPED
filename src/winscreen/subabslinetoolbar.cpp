//
// Created by XMuli <xmulitech@gmail.com> on 2021/12/23.
//
#include "subabslinetoolbar.h"
#include "drawhelper.h"
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QIcon>
#include <QVariant>
#include <QDebug>

Q_DECLARE_METATYPE(LineEnds)
Q_DECLARE_METATYPE(Qt::PenStyle)

const int ArrowWidth = 10;
const int ArrowHeight = 18;

SubAbsLineToolBar::SubAbsLineToolBar(QWidget *parent)
    : QWidget(parent)
    , m_cbEnds(new QComboBox(this))
    , m_cbDashes(new QComboBox(this))
{
    initUI();

	connect(m_cbEnds, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [&](int index) {
		emit sigLineEndsChange(m_cbEnds->itemData(index).value<LineEnds>());
	});
	connect(m_cbDashes, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [&](int index) {
		emit sigLineDasheChange(m_cbDashes->itemData(index).value<Qt::PenStyle>());
	});
}

QPainterPath SubAbsLineToolBar::getArrowHead(QPoint p1, QPoint p2, const int thickness)
{
	QLineF base(p1, p2);
	// Create the vector for the position of the base  of the arrowhead
	QLineF temp(QPoint(0, 0), p2 - p1);
	int val = ArrowHeight + thickness * 4;
	if (base.length() < val) {
		val = static_cast<int>(base.length() + thickness * 2);
	}
	temp.setLength(base.length() + thickness * 2 - val);
	// Move across the line up to the head
	QPointF bottomTranslation(temp.p2());

	// Rotate base of the arrowhead
	base.setLength(ArrowWidth + thickness * 2);
	base.setAngle(base.angle() + 90);
	// Move to the correct point
	QPointF temp2 = p1 - base.p2();
	// Center it
	QPointF centerTranslation((temp2.x() / 2), (temp2.y() / 2));

	base.translate(bottomTranslation);
	base.translate(centerTranslation);

	QPainterPath path;
	path.moveTo(p2);
	path.lineTo(base.p1());
	path.lineTo(base.p2());
	path.lineTo(p2);
	return path;
}

QLine SubAbsLineToolBar::getShorterLine(QPoint p1, QPoint p2, const int thickness)
{
	QLineF l(p1, p2);
	int val = ArrowHeight + thickness * 4;
	if (l.length() < val) {
		val = static_cast<int>(l.length() + thickness * 2);
	}
	l.setLength(l.length() + thickness * 2 - val);
	return l.toLine();
}

void SubAbsLineToolBar::initUI()
{
    setContentsMargins(0, 0, 0, 0);
    const int margin = 0;
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setObjectName("hLayout");
    hLayout->setContentsMargins(margin, margin, margin, margin);
    hLayout->setSpacing(4);

	QMap<QString, LineEnds> mapEnds = { {"EmptyToEmpty", LineEnds::EmptyToEmpty}
		, {"EmptyToRecesedArrow", LineEnds::EmptyToRecesedArrow}
		, {"DotToRecesedArrow", LineEnds::DotToRecesedArrow}
		, {"EmptyToArrow", LineEnds::EmptyToArrow}
		, {"DotToArrow", LineEnds::DotToArrow}
		, {"EmptyToDot", LineEnds::EmptyToDot}
		, {"DotToEmpty", LineEnds::DotToEmpty}
		, {"DotToDot", LineEnds::DotToDot}
		, {"RecesedArrowToRecesedArrow", LineEnds::RecesedArrowToRecesedArrow}
		, {"ArrowToArrow", LineEnds::ArrowToArrow}
	};

	for (auto it = mapEnds.constBegin(); it != mapEnds.constEnd(); ++it) {
        QString path = QString(":/resources/icons/line/") + it.key() + ".svg";
        m_cbEnds->addItem(QIcon(path), it.key(), QVariant::fromValue(it.value()));
    }
    hLayout->addWidget(new QLabel(tr("Ends"), this), Qt::AlignLeft);
    hLayout->addWidget(m_cbEnds, 2);

	QMap<QString, Qt::PenStyle> mapDashes = {{"SolidLine", Qt::SolidLine}
		, {"DashLine", Qt::DashLine}
		, {"DotLine", Qt::DotLine}
		, {"DashDotLine", Qt::DashDotLine}
		, {"DashDotDotLine", Qt::DashDotDotLine}
		, {"CustomDashLine", Qt::CustomDashLine}
	};

	for (auto it = mapDashes.constBegin(); it != mapDashes.constEnd(); ++it) {
		QString path = QString(":/resources/icons/line/") + it.key() +".svg";
		m_cbDashes->addItem(QIcon(path), it.key(), QVariant::fromValue(it.value()));
	}
    hLayout->addWidget(new QLabel(tr("Dashes"), this), Qt::AlignLeft);
    hLayout->addWidget(m_cbDashes, 2);

    hLayout->addStretch(2);
    setLayout(hLayout);
}

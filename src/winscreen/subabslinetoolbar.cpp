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
Q_DECLARE_METATYPE(LineDashes)

SubAbsLineToolBar::SubAbsLineToolBar(QWidget *parent)
    : QWidget(parent)
    , m_cbEnds(new QComboBox(this))
    , m_cbDashes(new QComboBox(this))
{
    initUI();
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

	QMap<QString, LineDashes> mapDashes = {{"SolidLine", LineDashes::SolidLine}
		, {"DashLine", LineDashes::DashLine}
		, {"DotLine", LineDashes::DotLine}
		, {"DashDotLine", LineDashes::DashDotLine}
		, {"DashDotDotLine", LineDashes::DashDotDotLine}
		, {"CustomDashLine", LineDashes::CustomDashLine}
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

//
// Created by XMuli <xmulitech@gmail.com> on 2021/10/05.
//

#include "winmain.h"
#include "winresetbtn.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QKeySequence>
#include <QStackedWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QKeySequenceEdit>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QToolButton>

WinMain::WinMain(QWidget *parent) 
	: QWidget(parent)
	, m_scrnShot(nullptr)
	, m_scrnShotLater(nullptr)
	, m_scrnShotRect(nullptr)
	, m_scrnShotWhole(nullptr)
	, m_labScrnShot(nullptr)
	, m_labScrnShotLater(nullptr)
	, m_labScrnShotRect(nullptr)
	, m_labScrnShotWhole(nullptr)
	, m_leFileName(nullptr)
	, m_leScrnPath(nullptr)
	, m_leConfPath(nullptr)
	, m_leLogPath(nullptr)
	, m_tbFileName(nullptr)
	, m_tbScrnPath(nullptr)
	, m_tbConfPath(nullptr)
	, m_tbLogPath(nullptr)
{
	init();
	resize(850, 410);
}

WinMain::~WinMain()
{
}

void WinMain::init()
{
	QTabWidget* tabWidget = new QTabWidget();
	tabWidget->addTab(tabOutput(), tr("Output"));
	tabWidget->addTab(tabShortcuts(), tr("Screenshots"));

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->addWidget(tabWidget);
}

QWidget * WinMain::tabShortcuts()
{
	// tabOneShortcuts 是第一页 widget
	QWidget* tabOneShortcuts = new QWidget();
	QVBoxLayout* mainVLayout = new QVBoxLayout();

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(tr("Global Shortcuts 1")), Qt::AlignLeft);
	hLayout->addWidget(new QLabel(tr("ⓘ")), Qt::AlignLeft);
	hLayout->addStretch(3);
	mainVLayout->addLayout(hLayout);

	QFrame* line = new QFrame();
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	mainVLayout->addWidget(line);

	QGridLayout* girdLayout = new QGridLayout();
	girdLayout->addWidget(new QLabel(tr("Screenshots")), 0, 0, Qt::AlignLeft);
	m_scrnShot = new QKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F1));
	girdLayout->addWidget(m_scrnShot, 0, 1, Qt::AlignLeft);
	m_labScrnShot = new QLabel("✔");
	girdLayout->addWidget(m_labScrnShot, 0, 2, Qt::AlignLeft);
	girdLayout->addWidget(new QLabel(tr("Time-lapse Screenshot")), 1, 0, Qt::AlignLeft);
	m_scrnShotLater = new QKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F2));
	girdLayout->addWidget(m_scrnShotLater, 1, 1, Qt::AlignLeft);
	m_labScrnShotLater = new QLabel("✔");
	girdLayout->addWidget(m_labScrnShotLater, 1, 2, Qt::AlignLeft);
	girdLayout->addWidget(new QLabel(tr("Rectangular Area")), 2, 0, Qt::AlignLeft);
	m_scrnShotRect = new QKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F3));
	girdLayout->addWidget(m_scrnShotRect, 2, 1, Qt::AlignLeft);
	m_labScrnShotRect = new QLabel("✔");
	girdLayout->addWidget(m_labScrnShotRect, 2, 2, Qt::AlignLeft);
	girdLayout->addWidget(new QLabel(tr("Whole Screen")), 3, 0, Qt::AlignLeft);
	m_scrnShotWhole = new QKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F4));
	girdLayout->addWidget(m_scrnShotWhole, 3, 1, Qt::AlignLeft);
	m_labScrnShotWhole = new QLabel("✔");
	girdLayout->addWidget(m_labScrnShotWhole, 3, 2, Qt::AlignLeft);
	mainVLayout->addLayout(girdLayout);
	//girdLayout->setColumnStretch(0, 2);
	//girdLayout->setColumnStretch(1, 7);
	//girdLayout->setColumnStretch(2, 1);

	mainVLayout->addStretch(0);
	mainVLayout->addWidget(new WinResetBtn());


	tabOneShortcuts->setLayout(mainVLayout);
	QStackedWidget* stack = new QStackedWidget();
	stack->addWidget(tabOneShortcuts); // 添加一页
	qInfo() << "添加第一页的控件完成";

	//stack->setCurrentIndex(0);  // 设置 QStackedWidget 翻页
	return stack;
}


QWidget * WinMain::tabOutput()
{
	QWidget* outputWidget = new QWidget();
	QVBoxLayout* mainVLayout = new QVBoxLayout(outputWidget);

	QGridLayout* girdLayout = new QGridLayout();
	girdLayout->addWidget(new QLabel(tr("File Name")), 0, 0, Qt::AlignLeft);
	m_leFileName = new QLineEdit("PicShot_xxxxx.png");
	girdLayout->addWidget(m_leFileName, 0, 1, Qt::AlignLeft);
	m_tbFileName = new QToolButton();
	girdLayout->addWidget(m_tbFileName, 0, 2, Qt::AlignLeft);
	girdLayout->addWidget(new QLabel(tr("Screen Path")), 1, 0, Qt::AlignLeft);
	m_leScrnPath = new QLineEdit("path/1111");
	girdLayout->addWidget(m_leScrnPath, 1, 1, Qt::AlignLeft);
	m_tbScrnPath = new QToolButton();
	girdLayout->addWidget(m_tbScrnPath, 1, 2, Qt::AlignLeft);
	girdLayout->addWidget(new QLabel(tr("Config Path")), 2, 0, Qt::AlignLeft);
	m_leConfPath = new QLineEdit("path/1112");
	girdLayout->addWidget(m_leConfPath, 2, 1, Qt::AlignLeft);
	m_tbConfPath = new QToolButton();
	girdLayout->addWidget(m_tbConfPath, 2, 2, Qt::AlignLeft);
	girdLayout->addWidget(new QLabel(tr("Log Path")), 3, 0, Qt::AlignLeft);
	m_leLogPath = new QLineEdit("path/1113");
	girdLayout->addWidget(m_leLogPath, 3, 1, Qt::AlignLeft);
	m_tbLogPath = new QToolButton();
	girdLayout->addWidget(m_tbLogPath, 3, 2, Qt::AlignLeft);

	mainVLayout->addLayout(girdLayout);
	mainVLayout->addStretch(0);
	mainVLayout->addWidget(new WinResetBtn());

	return outputWidget;
}

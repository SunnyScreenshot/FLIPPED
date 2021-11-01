//
// Created by XMuli <xmulitech@gmail.com> on 2021/10/05.
//

#include "winmain.h"
#include "winresetbtn.h"
#include "winfullscreen.h"
#include "../widget/xkeysequenceedit.h"
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
#include <QTextEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QToolButton>
#include <QSizePolicy>
#include <QHotkey>
#include <QCoreApplication>

WinMain::WinMain(QWidget *parent) 
	: QWidget(parent)
	, m_scrnShot(nullptr)
	, m_scrnShotLater(nullptr)
	, m_scrnShotRect(nullptr)
	, m_scrnShotWhole(nullptr)
	, m_leFileName(nullptr)
	, m_leScrnPath(nullptr)
	, m_leConfPath(nullptr)
	, m_leLogPath(nullptr)
	, m_tbFileName(nullptr)
	, m_tbScrnPath(nullptr)
	, m_tbConfPath(nullptr)
	, m_tbLogPath(nullptr)
    , m_hkScrnShot(nullptr)
    , m_hkScrnShotLater(nullptr)
    , m_hkScrnShotRect(nullptr)
    , m_hkScrnShotWhole(nullptr)
{
	init();
    hotKeyInit();
    resize(800, 400);
}

WinMain::~WinMain()
{
}

void WinMain::init()
{
	QTabWidget* tabWidget = new QTabWidget();
	tabWidget->addTab(tabScreenShot(), tr("ScreenShot"));
	tabWidget->addTab(tabOutput(), tr("Output"));
    tabWidget->addTab(tabShortcuts(), tr("Shortcuts"));
	tabWidget->addTab(tabAbout(), tr("About"));

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	//vLayout->setMargin(4);
	//vLayout->setSpacing(0);
    vLayout->addWidget(tabWidget);
}

void WinMain::hotKeyInit()
{
    // hotKey connect ------------------------------------
//    qDebug() << "Is Registered: " << m_hkScnShot->isRegistered();
    m_hkScrnShot = new QHotkey(QKeySequence("ctrl+alt+t"), true, qApp);
    connect(m_hkScrnShot, &QHotkey::activated, this, &WinMain::onScrnShot);
    m_hkScrnShotLater = new QHotkey(QKeySequence("ctrl+alt+y"), true, qApp);
    connect(m_hkScrnShotLater, &QHotkey::activated, this, &WinMain::onScrnShot);
    m_hkScrnShotRect = new QHotkey(QKeySequence("ctrl+alt+t"), true, qApp);
    connect(m_hkScrnShotRect, &QHotkey::activated, this, &WinMain::onScrnShot);
    m_hkScrnShotWhole = new QHotkey(QKeySequence("ctrl+alt+y"), true, qApp);
    connect(m_hkScrnShotWhole, &QHotkey::activated, this, &WinMain::onScrnShot);

    connect(m_scrnShot, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
    connect(m_scrnShotLater, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
    connect(m_scrnShotRect, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
    connect(m_scrnShotWhole, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
}

QWidget * WinMain::tabScreenShot()
{
	// 边框颜色组 ------------------------------------
	QHBoxLayout* vlBorderCol = new QHBoxLayout();
	vlBorderCol->setMargin(0);
	vlBorderCol->addWidget(new QLabel(tr("Border Color")));
	vlBorderCol->addWidget(new QLabel(tr("Col")));
	QHBoxLayout* vlArchorCol = new QHBoxLayout();
	vlArchorCol->setMargin(0);
	vlArchorCol->addWidget(new QLabel(tr("Archor Color")));
	vlArchorCol->addWidget(new QLabel(tr("Col")));
	QHBoxLayout* vlCrossCurveCol = new QHBoxLayout();
	vlCrossCurveCol->setMargin(0);
	vlCrossCurveCol->addWidget(new QLabel(tr("Cross Curve Color")));
	vlCrossCurveCol->addWidget(new QLabel(tr("Col")));

	QSpinBox* sbBorderWidth = new QSpinBox();
	sbBorderWidth->setMinimum(0);
	sbBorderWidth->setSingleStep(1);
	sbBorderWidth->setValue(2);
	QSpinBox* sbArchorCount = new QSpinBox();
	sbArchorCount->setMinimum(0);
	sbArchorCount->setMaximum(8);
	sbArchorCount->setSingleStep(4);
	sbArchorCount->setValue(0);
	QSpinBox* sbvlCrossCurveWidget = new QSpinBox();
	sbvlCrossCurveWidget->setMinimum(0);
	sbvlCrossCurveWidget->setSingleStep(1);
	sbvlCrossCurveWidget->setValue(1);

	QGridLayout* girdLayout = new QGridLayout();
	girdLayout->setMargin(0);
	girdLayout->addLayout(vlBorderCol, 0, 0);
	girdLayout->addLayout(vlArchorCol, 1, 0);
	girdLayout->addLayout(vlCrossCurveCol, 2, 0);
	// TODO 2021-10-07: (0, 1) 插入一个弹簧
	girdLayout->addWidget(new QLabel(tr("Border Widget")), 0, 2);
	girdLayout->addWidget(new QLabel(tr("Archor Count")), 1, 2);
	girdLayout->addWidget(new QLabel(tr("Cross Curve Widget")), 2, 2);
	girdLayout->addWidget(sbBorderWidth, 0, 3);
	girdLayout->addWidget(sbArchorCount, 1, 3);
	girdLayout->addWidget(sbvlCrossCurveWidget, 2, 3);

	QHBoxLayout* hlGirdParent = new QHBoxLayout();
	hlGirdParent->setMargin(0);
	hlGirdParent->addLayout(girdLayout);
	hlGirdParent->addStretch(0);
	
	// checkbox 组 ------------------------------------
	QCheckBox* cbWinDetection = new QCheckBox(tr("enable window detection"));
	QCheckBox* cbCatchCurcose = new QCheckBox(tr("catch curcose"));
	QCheckBox* cbAutoCpoyClipboard = new QCheckBox(tr("auto cpoy clipboard"));
	QVBoxLayout* vlCheckBox = new QVBoxLayout();
	vlCheckBox->setMargin(0);
	vlCheckBox->addWidget(cbWinDetection);
	vlCheckBox->addWidget(cbCatchCurcose);
	vlCheckBox->addWidget(cbAutoCpoyClipboard);

	// 整体布局 ------------------------------------
	QWidget* tabScreenShot = new QWidget();
	QVBoxLayout* mainVLayout = new QVBoxLayout(tabScreenShot);
	mainVLayout->addLayout(hlGirdParent);
	mainVLayout->addLayout(vlCheckBox);
	mainVLayout->addStretch(0);
	mainVLayout->addWidget(new WinResetBtn());

	return tabScreenShot;
}

QWidget * WinMain::tabOutput()
{
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

	// 整体布局 ------------------------------------
	QWidget* outputWidget = new QWidget();
	QVBoxLayout* mainVLayout = new QVBoxLayout(outputWidget);
	girdLayout->addWidget(m_tbLogPath, 3, 2, Qt::AlignLeft);
	mainVLayout->addLayout(girdLayout);
	mainVLayout->addStretch(0);
	mainVLayout->addWidget(new WinResetBtn());

	return outputWidget;
}

QWidget * WinMain::tabShortcuts()
{
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setMargin(0);
	hLayout->addWidget(new QLabel(tr("Global Shortcuts 1")), Qt::AlignLeft);
	hLayout->addWidget(new QLabel(tr("ⓘ")), Qt::AlignLeft);
	hLayout->addStretch(3);

	QFrame* line = new QFrame();
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);


	// https://jishurizhi.com/p-74.html
	QGridLayout* girdLayout = new QGridLayout();
    girdLayout->setObjectName("tabShortcutsGirdLayout");
	girdLayout->setMargin(0);
    const int keyEditWidget = 300;
	girdLayout->addWidget(new QLabel(tr("Screenshots")), 0, 0, Qt::AlignLeft);
    m_scrnShot = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F1));
    m_scrnShot->setMinimumWidth(keyEditWidget);
	girdLayout->addWidget(m_scrnShot, 0, 1, Qt::AlignLeft);
	QLabel* labScrnShot = new QLabel("✔");
	girdLayout->addWidget(labScrnShot, 0, 2, Qt::AlignLeft);
	girdLayout->addWidget(new QLabel(tr("Time-lapse Screenshot")), 1, 0, Qt::AlignLeft);
    m_scrnShotLater = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F2));
    m_scrnShotLater->setMinimumWidth(keyEditWidget);
	girdLayout->addWidget(m_scrnShotLater, 1, 1, Qt::AlignLeft);
	QLabel* labScrnShotLater = new QLabel("✔");
	girdLayout->addWidget(labScrnShotLater, 1, 2, Qt::AlignLeft);
	girdLayout->addWidget(new QLabel(tr("Rectangular Area")), 2, 0, Qt::AlignLeft);
    m_scrnShotRect = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F3));
    m_scrnShotRect->setMinimumWidth(keyEditWidget);
	girdLayout->addWidget(m_scrnShotRect, 2, 1, Qt::AlignLeft);
	QLabel* labScrnShotRect = new QLabel("✔");
	girdLayout->addWidget(labScrnShotRect, 2, 2, Qt::AlignLeft);
    girdLayout->addWidget(new QLabel(tr("Rectangular Area222")), 3, 0, Qt::AlignLeft);
	m_scrnShotWhole = new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F3));
	m_scrnShotWhole->setMinimumWidth(keyEditWidget);
    girdLayout->addWidget(m_scrnShotWhole, 3, 1, Qt::AlignLeft);
	QLabel* labScrnShotWhole = new QLabel("✔");
    girdLayout->addWidget(labScrnShotWhole, 3, 2, Qt::AlignLeft);

    m_scrnShot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_scrnShotLater->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_scrnShotRect->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_scrnShotWhole->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//    girdLayout->setColumnStretch(0, 0);
//    girdLayout->setColumnStretch(1, 0);
//    girdLayout->setColumnStretch(2, 0);

	// 整体布局 ------------------------------------
	QVBoxLayout* mainVLayout = new QVBoxLayout();
    mainVLayout->addLayout(hLayout);
    mainVLayout->addWidget(line);
	mainVLayout->addLayout(girdLayout);
    mainVLayout->addStretch(0);
    mainVLayout->addWidget(new WinResetBtn());

    QWidget* tabOneShortcuts = new QWidget();
	tabOneShortcuts->setLayout(mainVLayout);
    QStackedWidget* stack = new QStackedWidget();
    stack->addWidget(tabOneShortcuts); // 添加一页
	qInfo() << "添加第一页的控件完成";

	//stack->setCurrentIndex(0);  // 设置 QStackedWidget 翻页

    return stack;
}

QWidget* WinMain::tabAbout()
{
	QWidget* aboutWidget = new QWidget();
	QHBoxLayout* mainHLayout = new QHBoxLayout(aboutWidget);
	mainHLayout->setMargin(0);
	mainHLayout->setSpacing(0);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setMargin(0);
	vLayout->addWidget(new QLabel(tr("后面替换为 LOGO 图标")));
	vLayout->addWidget(new QLabel(tr("PicShot")));
	vLayout->addWidget(new QLabel(tr("一款开源且极简的跨平台截图软件")));
	vLayout->addWidget(new QLabel(tr("版本：V1.0 (2021.xx.xx)")));
	vLayout->addWidget(new QLabel(tr("版权© 2021 偕臧/XMuli")));
	vLayout->addStretch(0);
	mainHLayout->addLayout(vLayout, 2);
	QTextEdit* textEdit = new QTextEdit("鸣谢");
	mainHLayout->addWidget(textEdit, 3);

    return aboutWidget;
}

void WinMain::onScrnShot()
{
    WinFullScreen::instance().getScrnShots();
}

void WinMain::onKeySeqChanged(const QKeySequence &keySequence)
{
    XKeySequenceEdit* obj = qobject_cast<XKeySequenceEdit *>(sender());
    if (!obj)
        return;

    QGridLayout* girdLayout = obj->parent()->findChild<QGridLayout*>("tabShortcutsGirdLayout");
    if (!girdLayout)
        return;

    int row = -1;
    int col = -1;
    int rowSpan = -1;
    int columnSpan = -1;
    girdLayout->getItemPosition(girdLayout->indexOf(obj), &row, &col, &rowSpan, &columnSpan);
    qInfo()<<"----->"<<obj<<obj->parent()<<obj->parentWidget()<<girdLayout;
    qInfo()<<"----->"<<girdLayout->indexOf(obj);
    QLayoutItem* item = girdLayout->itemAtPosition(row, col + 1);
    QLabel* lab = qobject_cast<QLabel *>(item->widget());

	QHotkey* hotkey = nullptr;
	if (obj == m_scrnShot) {
		hotkey = m_hkScrnShot;
	} else if (obj == m_scrnShotLater) {
		hotkey = m_hkScrnShotLater;
	} else if (obj == m_scrnShotRect) {
		hotkey = m_hkScrnShotRect;
	} else if (obj == m_scrnShotWhole) {
		hotkey = m_hkScrnShotWhole;
	} else {
	}

	if (!hotkey || !lab)
		return;

	hotkey->resetShortcut();
	hotkey->setShortcut(keySequence, true);

    if (hotkey->isRegistered()) {
        lab->setText(tr("✔️12"));
    } else {
        lab->setText(tr("❌23"));
    }

    qInfo() << "Is Registered22: " << hotkey << hotkey->isRegistered()<<obj->parent();
    qInfo()<< "sender():" << sender()<<"keySequence:"<<keySequence<< "keySequence:" << keySequence.count();
}

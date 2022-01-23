//
// Created by XMuli <xmulitech@gmail.com> on 2021/10/05.
//

#include "winmain.h"
#include "winresetbtn.h"
#include "screenshot.h"
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
#include <QTimer>
#include <QColorDialog>

WinMain::WinMain(QWidget *parent) 
	: QWidget(parent)
	, m_scrnShot(new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_Shift + Qt::Key_T)))
	, m_scrnShotWinObj(new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F2)))
	, m_scrnShotDelay(new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F3)))
	, m_scrnShotFixedSize(new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F4)))
	, m_scrnShotFullScreen(new XKeySequenceEdit(QKeySequence(Qt::CTRL + Qt::Key_F5)))
	, m_leFileName(nullptr)
	, m_leScrnPath(nullptr)
	, m_leConfPath(nullptr)
	, m_leLogPath(nullptr)
	, m_tbFileName(nullptr)
	, m_tbScrnPath(nullptr)
	, m_tbConfPath(nullptr)
	, m_tbLogPath(nullptr)
    , m_hkScrnShot(new QHotkey(QKeySequence("ctrl+alt+t"), true, qApp))
    , m_hkScrnShotWinObj(new QHotkey(QKeySequence("ctrl+f2"), true, qApp))
	, m_hkScrnShotDelay(new QHotkey(QKeySequence("ctrl+alt+f3"), true, qApp))
	, m_hkScrnShotFixedSize(new QHotkey(QKeySequence("ctrl+4"), true, qApp))
	, m_hkScrnShotFullScreen(new QHotkey(QKeySequence("ctrl+5"), true, qApp))
{
    hotKeyInit();
	initUI();
    resize(800, 400);
}

WinMain::~WinMain()
{
}

void WinMain::initUI()
{
	QTabWidget* tabWidget = new QTabWidget();
	tabWidget->addTab(tabScreenShot(), tr("ScreenShot"));
	tabWidget->addTab(tabOutput(), tr("Output"));
    tabWidget->addTab(tabShortcuts(), tr("Shortcuts"));
	tabWidget->addTab(tabAbout(), tr("About"));

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->setMargin(0);
	vLayout->setSpacing(0);
    vLayout->addWidget(tabWidget);
}

void WinMain::hotKeyInit()
{
    // hotKey connect ------------------------------------
//    qDebug() << "Is Registered: " << m_hkScnShot->isRegistered();
    connect(m_hkScrnShot, &QHotkey::activated, this, &WinMain::onScrnShot);
	connect(m_hkScrnShotWinObj, &QHotkey::activated, this, &WinMain::onScrnShot);
    connect(m_hkScrnShotDelay, &QHotkey::activated, this, &WinMain::onScrnShotDelay);
    connect(m_hkScrnShotFixedSize, &QHotkey::activated, this, &WinMain::onScrnShotFixedSize);
    connect(m_hkScrnShotFullScreen, &QHotkey::activated, this, &WinMain::onScrnShotFullScreen);

    connect(m_scrnShot, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
	connect(m_scrnShotWinObj, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
	connect(m_scrnShotDelay, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
	connect(m_scrnShotFixedSize, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
	connect(m_scrnShotFullScreen, &XKeySequenceEdit::sigKeySeqChanged, this, &WinMain::onKeySeqChanged);
}
// 更新当前热键状态时候注册成功
bool WinMain::hotKeyStateUpdate(QLabel* lab, QHotkey* hotKey)
{
	if (!lab || !hotKey)
		return false;

	if (hotKey->isRegistered())
		lab->setText("✔");
	else
		lab->setText("✘");

	return true;
}

QWidget * WinMain::tabScreenShot()
{
	// 边框颜色组 ------------------------------------
	QHBoxLayout* vlBorderCol = new QHBoxLayout();
	vlBorderCol->setMargin(0);
	vlBorderCol->addWidget(new QLabel(tr("Border Color")));
    QLabel* borderColor = new QLabel(this);
    borderColor->setObjectName("borderColor");
    borderColor->setFixedWidth(30 * ScreenShot::getScale());
    borderColor->installEventFilter(this);
    borderColor->setAutoFillBackground(true);
    vlBorderCol->addWidget(borderColor);
	QHBoxLayout* vlArchorCol = new QHBoxLayout();
	vlArchorCol->setMargin(0);
	vlArchorCol->addWidget(new QLabel(tr("Archor Color")));
    QLabel* archorColor = new QLabel(this);
    archorColor->setObjectName("archorColor");
    archorColor->setFixedWidth(30 * ScreenShot::getScale());
    archorColor->installEventFilter(this);
    archorColor->setAutoFillBackground(true);
    vlArchorCol->addWidget(archorColor);
	QHBoxLayout* vlCrossCurveCol = new QHBoxLayout();
	vlCrossCurveCol->setMargin(0);
	vlCrossCurveCol->addWidget(new QLabel(tr("Cross Curve Color")));
    QLabel* crossCurveColor = new QLabel(this);
    crossCurveColor->setObjectName("crossCurveColor");
    crossCurveColor->setFixedWidth(30 * ScreenShot::getScale());
    crossCurveColor->installEventFilter(this);
    crossCurveColor->setAutoFillBackground(true);
    vlCrossCurveCol->addWidget(crossCurveColor);

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
	girdLayout->addWidget(new QLabel(tr("File Name")), 0, 0);  //  Qt::AlignLeft 带上的话，就不会拉伸；坑死了，查好久
	m_leFileName = new QLineEdit("PicShot_xxxxx.png");
	//m_leFileName->setToolTip("testTooltip");  // 显示预览效果
	girdLayout->addWidget(m_leFileName, 0, 1);
	m_tbFileName = new QToolButton();
	girdLayout->addWidget(m_tbFileName, 0, 2);
	girdLayout->addWidget(new QLabel(tr("Screen Path")), 1, 0);
	m_leScrnPath = new QLineEdit("path/1111");
	girdLayout->addWidget(m_leScrnPath, 1, 1);
	m_tbScrnPath = new QToolButton();
	girdLayout->addWidget(m_tbScrnPath, 1, 2);
	girdLayout->addWidget(new QLabel(tr("Config Path")), 2, 0);
	m_leConfPath = new QLineEdit("path/1112");
	girdLayout->addWidget(m_leConfPath, 2, 1);
	m_tbConfPath = new QToolButton();
	girdLayout->addWidget(m_tbConfPath, 2, 2);
	girdLayout->addWidget(new QLabel(tr("Log Path")), 3, 0);
	m_leLogPath = new QLineEdit("path/1113");
	girdLayout->addWidget(m_leLogPath, 3, 1);
	m_tbLogPath = new QToolButton();
    girdLayout->addWidget(m_tbLogPath, 3, 2);

	girdLayout->setColumnStretch(0, 1);
	girdLayout->setColumnStretch(1, 10);
	girdLayout->setColumnStretch(2, 2);
	girdLayout->setColumnStretch(3, 1);

	// 整体布局 ------------------------------------
	QWidget* outputWidget = new QWidget();
	QVBoxLayout* mainVLayout = new QVBoxLayout(outputWidget);
	//mainVLayout->setMargin(0);
	//mainVLayout->setSpacing(0);
	mainVLayout->addLayout(girdLayout);
	mainVLayout->addStretch(0);
	mainVLayout->addWidget(new WinResetBtn());

	return outputWidget;
}

QWidget * WinMain::tabShortcuts()
{
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setMargin(0);
	hLayout->addWidget(new QLabel(tr("Global Shortcuts 1")));
	hLayout->addWidget(new QLabel(tr("ⓘ")));
	hLayout->addStretch(0);

	QFrame* line = new QFrame();
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);

	// m_scrnShot;
	// m_scrnShotWinObj;
	// m_scrnShotDelay;
	// m_scrnShotFixedSize;
	// m_scrnShotFullScreen;
	// https://jishurizhi.com/p-74.html
	QGridLayout* girdLayout = new QGridLayout();
    girdLayout->setObjectName("tabShortcutsGirdLayout");
	girdLayout->setMargin(0);
    const int keyEditMinWidget = 200;
	girdLayout->addWidget(new QLabel(tr("Rectangular Region")), 0, 0);
    m_scrnShot->setMinimumWidth(keyEditMinWidget);
	girdLayout->addWidget(m_scrnShot, 0, 1);
	QLabel* labShot = new QLabel("?");
	girdLayout->addWidget(labShot, 0, 2);
	girdLayout->addWidget(new QLabel(tr("Window / Object")), 1, 0);
	m_scrnShotWinObj->setMinimumWidth(keyEditMinWidget);
	girdLayout->addWidget(m_scrnShotWinObj, 1, 1);
	QLabel* labShotWinObj = new QLabel("?");
	girdLayout->addWidget(labShotWinObj, 1, 2);

	girdLayout->addWidget(new QLabel(tr("Delay Capture")), 2, 0);
	m_scrnShotDelay->setMinimumWidth(keyEditMinWidget);
	girdLayout->addWidget(m_scrnShotDelay, 2, 1);
	QLabel* labShotDelay = new QLabel("?");
	girdLayout->addWidget(labShotDelay, 2, 2);

	girdLayout->addWidget(new QLabel(tr("Fixed-Size Region")), 3, 0);
	m_scrnShotFixedSize->setMinimumWidth(keyEditMinWidget);
	girdLayout->addWidget(m_scrnShotFixedSize, 3, 1);
	QLabel* labtShotFixedSize = new QLabel("?");
	girdLayout->addWidget(labtShotFixedSize, 3, 2);

	girdLayout->addWidget(new QLabel(tr("Full Screen")), 4, 0);
	m_scrnShotFullScreen->setMinimumWidth(keyEditMinWidget);
	girdLayout->addWidget(m_scrnShotFullScreen, 4, 1);
	QLabel* labShotFullScreen = new QLabel("?");
	girdLayout->addWidget(labShotFullScreen, 4, 2);

	hotKeyStateUpdate(labShot, m_hkScrnShot);
	hotKeyStateUpdate(labShotWinObj, m_hkScrnShotWinObj);
	hotKeyStateUpdate(labShotDelay, m_hkScrnShotDelay);
	hotKeyStateUpdate(labtShotFixedSize, m_hkScrnShotFixedSize);
	hotKeyStateUpdate(labShotFullScreen, m_hkScrnShotFullScreen);

    girdLayout->setColumnStretch(0, 1);
    girdLayout->setColumnStretch(1, 10);
    girdLayout->setColumnStretch(2, 2);
	girdLayout->setColumnStretch(3, 1); // 整列添加弹簧

	// 整体布局 ------------------------------------
	QVBoxLayout* mainVLayout = new QVBoxLayout();
	//mainVLayout->setMargin(0);
	//mainVLayout->setSpacing(0);
	mainVLayout->addLayout(hLayout);
	mainVLayout->addWidget(line);
	mainVLayout->addLayout(girdLayout);
    mainVLayout->addStretch(0);
    mainVLayout->addWidget(new WinResetBtn());

    QWidget* tabOneShortcuts = new QWidget();
	tabOneShortcuts->setLayout(mainVLayout);
    QStackedWidget* stack = new QStackedWidget();
    stack->addWidget(tabOneShortcuts); // 添加一页
	qInfo() << "添加第一页的控件完成" << tabOneShortcuts->contentsMargins() << stack->contentsMargins();

    return tabOneShortcuts;
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
    ScreenShot::instance().getScrnShots();
}

void WinMain::onScrnShotDelay()
{
	ScreenShot& instance = ScreenShot::instance();
	instance.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags()); // 去掉标题栏 + 置顶
	instance.setFixedSize(QApplication::desktop()->size());

    QTimer::singleShot(3000, [&]() {
		instance.getScrnShots();
	});
}

void WinMain::onScrnShotFixedSize()
{
	//TODO:2021.11.2 截图预设的固定大小的矩形
}

void WinMain::onScrnShotFullScreen()
{
	ScreenShot& instance = ScreenShot::instance();
	instance.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags()); // 去掉标题栏 + 置顶
	instance.setFixedSize(QApplication::desktop()->size());
	instance.getScrnShots();
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
	} else if (obj == m_scrnShotDelay) {
		hotkey = m_hkScrnShotDelay;
	} else if (obj == m_scrnShotWinObj) {
		hotkey = m_hkScrnShotFixedSize;
	} else if (obj == m_scrnShotFullScreen) {
		hotkey = m_hkScrnShotFullScreen;
	} else {
	}

	if (!hotkey || !lab)
		return;

	hotkey->resetShortcut();
	hotkey->setShortcut(keySequence, true);

    if (hotkey->isRegistered()) {
        lab->setText(tr("✔️"));
    } else {
        lab->setText(tr("❌"));
    }

    qInfo() << "Is Registered22: " << hotkey << hotkey->isRegistered()<<obj->parent();
    qInfo()<< "sender():" << sender()<<"keySequence:"<<keySequence<< "keySequence:" << keySequence.count();
}

// see: 用法 https://blog.csdn.net/xiezhongyuan07/article/details/79992099
bool WinMain::eventFilter(QObject *watched, QEvent *event)
{
    QLabel* lab = qobject_cast<QLabel *>(watched);

    if (!lab)
        return QWidget::eventFilter(watched, event);

    if (event->type() == QEvent::MouseButtonRelease) {
        if (lab->objectName() == "borderColor"
                || lab->objectName() == "archorColor"
                || lab->objectName() == "crossCurveColor") {
            QColor color = QColorDialog::getColor(lab->palette().color(QPalette::Background), this, tr("选择文本颜色"));
            QPalette palette;
            palette.setColor(QPalette::Background, color);
            lab->setPalette(palette);
        }

        return true;
    }

    return QWidget::eventFilter(watched, event);
}

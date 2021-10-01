//
// Created by xmuli on 2021/9/29.
//

#include "winfullscreen.h"
#include <QScreen>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTime>
#include <QDebug>

WinFullScreen::WinFullScreen(QWidget *parent)
	: QWidget(parent)
	, m_primaryScreen(nullptr)
	, m_currPixmap(nullptr)
	, m_blurPixmap(nullptr)
	, m_basePixmap(nullptr)
	, m_rectCalcu()
	, m_cursorType(CursorType::UnknowCursorType)
	, m_cursorArea(CursorArea::UnknowCursorArea)
{
	m_primaryScreen = QApplication::primaryScreen();
	m_screens = QApplication::screens();

	setMouseTracking(true);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags()); // 去掉标题栏 + 置顶
	/*setFixedSize(QApplication::desktop()->size());*/
	setFixedSize(800, 600);
}

WinFullScreen::~WinFullScreen() 
{
	delete m_currPixmap;
	m_currPixmap = nullptr;
	delete m_blurPixmap;
	m_blurPixmap = nullptr;
	delete m_basePixmap;
	m_basePixmap = nullptr;
}

// 获取虚拟屏幕截图
void WinFullScreen::getVirtualScreen() 
{
	// TODO 2021-09-29:
	// 万一虚拟屏幕没开启，优先截取当前鼠标所在的屏幕

	QDesktopWidget *desktop = QApplication::desktop();  // 获取桌面的窗体对象
	m_currPixmap = new QPixmap(m_primaryScreen->grabWindow(desktop->winId(), 0, 0, desktop->width(), desktop->height()));

	QTime startTime = QTime::currentTime();
	//m_currPixmap->save("m_currPixmap.png");
	QTime stopTime = QTime::currentTime();
	int elapsed = startTime.msecsTo(stopTime);
	qDebug() << "save m_currPixmap tim =" << elapsed << "ms";
}

// 获取屏幕遮罩
QPixmap* WinFullScreen::getblurPixmap(QColor color)
{
	if (!m_currPixmap)
		return nullptr;

	m_blurPixmap = new QPixmap(m_currPixmap->size());
	m_blurPixmap->fill(color);
	//m_blurPixmap->save("m_blurPixmap.png");

	return m_blurPixmap;
}

// 获取当前屏幕截图 + 遮罩
QPixmap* WinFullScreen::getBasePixmap()
{
	if (!m_currPixmap)
		getVirtualScreen();

	if (!m_blurPixmap)
		getblurPixmap();

	m_basePixmap = new QPixmap(m_currPixmap->copy(m_currPixmap->rect()));
	QPainter pa(m_basePixmap);
	QTime startTime = QTime::currentTime();
	pa.drawPixmap(m_basePixmap->rect(), *m_blurPixmap);
	//m_basePixmap->save("m_basePixmap.png");
	//m_currPixmap->save("m_currPixmap2.png");
	QTime stopTime = QTime::currentTime();
	int elapsed = startTime.msecsTo(stopTime);
	qDebug() << "save m_basePixmap time 汉字测试 =" << elapsed << "ms";

	return m_basePixmap;
}

void WinFullScreen::paintEvent(QPaintEvent *event)
{
	QPainter pa(this);
	pa.drawPixmap(QApplication::desktop()->rect(), *m_basePixmap);

	pa.setPen(Qt::red);
	QRect selRect(m_rectCalcu.getSelRect());

	// 注意独立屏幕缩放比（eg: mac = 2）
	pa.drawPixmap(selRect, m_currPixmap->copy(QRect(selRect.topLeft() * getDevicePixelRatio(), selRect.size() * getDevicePixelRatio())));
	pa.drawRect(m_rectCalcu.getSelRect());
	qDebug() << "【paintEvent】  :" << m_rectCalcu.getSelRect() << "   " << m_rectCalcu.m_EndPos;

	update();
}

void WinFullScreen::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		close();
		qDebug() << "---Key_Escape---";
	}
}

void WinFullScreen::mousePressEvent(QMouseEvent *event)
{
	m_rectCalcu.clear();
	m_cursorType = CursorType::Select;

	switch (m_cursorType)
	{
	case Select: {
		m_rectCalcu.m_startPos = event->pos();
		m_rectCalcu.m_EndPos = event->pos();
		//m_rectCalcu.setClear(false);
		qDebug() << "【mousePressEvent】 Select :" << m_rectCalcu.m_startPos << "   " << m_rectCalcu.m_EndPos;
		break;
	}
	case MovePosition:
		break;
	case ModifWidth:
		break;
	case ModifHeight:
		break;
	case ModifSize:
		break;
	case Move:
		break;
	case Waiting: {
		break;
	}
	case UnknowCursorType:
		break;
	default:
		break;
	}
}

void WinFullScreen::mouseMoveEvent(QMouseEvent *event)
{
	switch (m_cursorType)
	{
	case Select: {
		m_rectCalcu.m_EndPos = event->pos();
		qDebug() << "【mouseMoveEvent】 Select :" << m_rectCalcu.m_startPos << "   " << m_rectCalcu.m_EndPos;
		break;
	}
	case MovePosition: {
		break;
	}
	case ModifWidth:
		break;
	case ModifHeight:
		break;
	case ModifSize:
		break;
	case Move:
		break;
	case Waiting: {
		
		break;
	}
	case UnknowCursorType:
		break;
	default:
		break;
	}
}

void WinFullScreen::mouseReleaseEvent(QMouseEvent *event)
{
	switch (m_cursorType)
	{
	case Select: {
		m_rectCalcu.m_EndPos = event->pos();
		//m_rectCalcu.clear();
		qDebug() << "【mouseReleaseEvent】 Select :" << m_rectCalcu.m_startPos << "   " << m_rectCalcu.m_EndPos;
		m_cursorType = CursorType::Waiting;
		break;
	}
	case MovePosition:
		break;
	case ModifWidth:
		break;
	case ModifHeight:
		break;
	case ModifSize:
		break;
	case Move:
		break;
	case Waiting: {
		break;
	}
	case UnknowCursorType:
		break;
	default:
		break;
	}
}



// 屏幕详细参数
void WinFullScreen::display()
{
	for (QScreen* it : m_screens) {
		qDebug() << "------------------------------------------\n"
			<< "count:" << it << "  devicePixelRatio:" << it->devicePixelRatio() << "  manufacturer:" << it->manufacturer()
			<< "  model:" << it->model() << "  name:" << it->name() << "  physicalSize:" << it->physicalSize() << "  refreshRate:" << it->refreshRate()
			<< "  serialNumber:" << it->serialNumber() << "  size:" << it->size() << "  Scale:" << getScale(it) << "  virtualGeometry:" << it->virtualGeometry() << "\n"
			"Physical DPI:" << it->physicalDotsPerInch() << "  DPIX:" << it->physicalDotsPerInchX() << "  DPIY:" << it->physicalDotsPerInchY() << "\n"
			<< "Logical DPI:" << it->logicalDotsPerInch() << "  DPIX:" << it->logicalDotsPerInchX() << "  DPIY:" << it->logicalDotsPerInchY() << "\n";
	}
}

double WinFullScreen::getDevicePixelRatio()
{
	return m_primaryScreen->devicePixelRatio();
}

double WinFullScreen::getDevicePixelRatio(QScreen * screen)
{
	if (!screen)
		return 0.0;
	else
		return screen->devicePixelRatio();
	
}

double WinFullScreen::getScale()
{
	return getScale(m_primaryScreen);
}

double WinFullScreen::getScale(QScreen * screen)
{
	double scale = screen->logicalDotsPerInch() / 96.0;
	if (scale < 1.25)
		return 1;
	else if (1.25 <= scale && scale < 1.5)
		return 1.25;
	else if (1.5 <= scale && scale < 1.75)
		return 1.5;
	else if (1.75 <= scale && scale < 2)
		return 1.75;
	else if (2 <= scale && scale < 2.25)
		return 2;
	else if (2.25 <= scale && scale < 2.5)
		return 2.25;
	else if (2.5 <= scale && scale < 3)
		return 2.5;
	else if (3 <= scale && scale < 3.5)
		return 3;
	else if (3.5 <= scale && scale < 4)
		return 3.5;
	else
		return scale;
}

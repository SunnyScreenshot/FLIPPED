//
// Created by XMuli <xmulitech@gmail.com> on 2021/9/29.
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
	, m_rtCalcu()
	, m_cursorArea(CursorArea::UnknowCursorArea)
{
	m_primaryScreen = QApplication::primaryScreen();
	m_screens = QApplication::screens();

	//setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags()); // 去掉标题栏 + 置顶
	//setFixedSize(QApplication::desktop()->size());
    resize(1920, 1080);
    QDesktopWidget* desktopWidget =QApplication::desktop();
    QRect deskRect =desktopWidget->availableGeometry();   //获取可用桌面大小
    QRect screenRect =desktopWidget->screenGeometry();  //获取设备屏幕大小

    qDebug()<<deskRect<<screenRect;
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

	//QTime startTime = QTime::currentTime();
	////m_currPixmap->save("m_currPixmap.png");
	//QTime stopTime = QTime::currentTime();
	//int elapsed = startTime.msecsTo(stopTime);
	//qDebug() << "save m_currPixmap tim =" << elapsed << "ms" << size;
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

// 修改拉伸选中矩形的大小
void WinFullScreen::modifyRectSize(QRect& rtSel)
{
	QRect rt(rtSel);
	int width = m_rtCalcu.getModifyWidth();
	int height = m_rtCalcu.getModifyHeight();
	if (m_cursorArea == CursorArea::CursorCrossLeft) {
		rtSel.setTopLeft(rt.topLeft() + QPoint(width, 0));
	} else if (m_cursorArea == CursorArea::CursorCrossRight) {
		rtSel.setBottomRight(rt.bottomRight() + QPoint(width, 0));
	} else if (m_cursorArea == CursorArea::CursorCrossTop) {
		rtSel.setTopLeft(rt.topLeft() + QPoint(0, height));
	} else if (m_cursorArea == CursorArea::CursorCrossBottom) {
		rtSel.setBottomRight(rt.bottomRight() + QPoint(0, height));
	} else if (m_cursorArea == CursorArea::CursorCrossTopLeft) {
		rtSel.setTopLeft(rt.topLeft() + QPoint(width, height));
	} else if (m_cursorArea == CursorArea::CursorCrossTopRight) {
		rtSel.setTopRight(rt.topRight() + QPoint(width, height));
	} else if (m_cursorArea == CursorArea::CursorCrossBottomLeft) {
		rtSel.setBottomLeft(rt.bottomLeft() + QPoint(width, height));
	} else if (m_cursorArea == CursorArea::CursorCrossBottomRight) {
		rtSel.setBottomRight(rt.bottomRight() + QPoint(width, height));
	} else {

	}
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
	pa.drawPixmap(m_basePixmap->rect(), *m_blurPixmap);
	//QTime startTime = QTime::currentTime();
	////m_basePixmap->save("m_basePixmap.png");
	////m_currPixmap->save("m_currPixmap2.png");
	//QTime stopTime = QTime::currentTime();
	//int elapsed = startTime.msecsTo(stopTime);
	//qDebug() << "save m_basePixmap time 汉字测试 =" << elapsed << "ms";

	return m_basePixmap;
}

void WinFullScreen::paintEvent(QPaintEvent *event)
{
	QPainter pa(this);
	pa.drawPixmap(QApplication::desktop()->rect(), *m_basePixmap);

	pa.setPen(Qt::red);
	QRect rtSel(m_rtCalcu.getSelRect().translated(m_rtCalcu.getMoveWidth(), m_rtCalcu.getMoveHeight()));

	 //拉伸边框矩形大小
	modifyRectSize(rtSel);
	m_rtCalcu.getSelRect();

	//qDebug() << "【paintEvent】  :" << m_rtCalcu.m_cursorType << m_rtCalcu.getSelRect() << rtSel << m_rtCalcu.getSelRect() << "   " << m_rtCalcu.m_EndPos << "  " << m_basePixmap << "  " << QRect();
	// 注意独立屏幕缩放比（eg: macox = 2）
	if (rtSel.width() > 0 && rtSel.height() > 0){
		pa.drawPixmap(rtSel, m_currPixmap->copy(QRect(rtSel.topLeft() * getDevicePixelRatio(), rtSel.size() * getDevicePixelRatio())));
		pa.drawRect(rtSel);

	}

	QRect rtOuter = m_rtCalcu.getOuterSelRect(rtSel);
	QRect rtInner = m_rtCalcu.getInnerSelRect(rtSel);
	int interval = (rtOuter.height() - rtInner.height()) / 2;

	QRect rtLeft(rtOuter.left(), rtInner.top(), interval, rtInner.height());
	QRect rtTop(rtInner.left(), rtOuter.top(), rtInner.width(), interval);
	QRect rtRight(rtInner.right(), rtInner.top(), interval, rtInner.height());
	QRect rtBottom(rtInner.left(), rtInner.bottom(), rtInner.width(), interval);
	QRect rtTopLeft(rtOuter.left(), rtOuter.top(), interval, interval);
	QRect rtTopRight(rtInner.right(), rtOuter.top(), interval, interval);
	QRect rtBottomLeft(rtOuter.left(), rtInner.bottom(), interval, interval);
	QRect rtBottomRight(rtInner.right(), rtInner.bottom(), interval, interval);

	pa.setBrush(Qt::blue);
	pa.drawRect(rtLeft);
	pa.drawRect(rtTop);
	pa.drawRect(rtRight);
	pa.drawRect(rtBottom);
	pa.setBrush(Qt::yellow);
	pa.drawRect(rtTopLeft);
	pa.drawRect(rtTopRight);
	pa.drawRect(rtBottomLeft);
	pa.drawRect(rtBottomRight);

	
	/*qDebug() << "【paintEvent】  :" << m_rtCalcu.m_cursorType << m_rtCalcu.getSelRect() << rtSel << m_rtCalcu.getSelRect() << "   " << m_rtCalcu.m_EndPos << "  " << m_basePixmap << "  " << QRect();*/
	//<< "外部矩形：" << rtOuter << "内部矩形：" << rtInner;


	update();
}

void WinFullScreen::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		close();
        qDebug() << "Key_Escape";
	}
}

void WinFullScreen::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "event->pos():" << event->pos() << "m_rtCalcu.m_cursorType:" << m_rtCalcu.m_cursorType
             << "m_rtCalcu.m_startPos:" << m_rtCalcu.m_startPos
             << "m_rtCalcu.m_EndPos:" << m_rtCalcu.m_EndPos;

	m_rtCalcu.m_cursorType = CursorType::Waiting;
	setMouseTracking(false);

	if (m_rtCalcu.getSelRect().isEmpty()) {
		m_rtCalcu.clear();
		m_rtCalcu.m_cursorType = CursorType::Select;
	} else {  // 可能是添加编辑画图、或者移动等模式
		if (event->button() == Qt::LeftButton)
		{
			if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorInner) {
				m_rtCalcu.m_cursorType = CursorType::Move;
			} else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossLeft) {
				m_rtCalcu.m_cursorType = CursorType::ModifWidth;
				m_cursorArea = CursorArea::CursorCrossLeft;
			} else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossRight) {
				m_rtCalcu.m_cursorType = CursorType::ModifWidth;
				m_cursorArea = CursorArea::CursorCrossRight;
			} else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossTop) {
				m_rtCalcu.m_cursorType = CursorType::ModifHeight;
				m_cursorArea = CursorArea::CursorCrossTop;
			} else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossBottom) {
				m_rtCalcu.m_cursorType = CursorType::ModifHeight;
				m_cursorArea = CursorArea::CursorCrossBottom;
			} else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossTopLeft) {
				m_rtCalcu.m_cursorType = CursorType::ModifyTLAndBR;
				m_cursorArea = CursorArea::CursorCrossTopLeft;
			} else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossTopRight) {
				m_rtCalcu.m_cursorType = CursorType::ModifyTRAndBL;
				m_cursorArea = CursorArea::CursorCrossTopRight;
			} else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossBottomLeft) {
				m_rtCalcu.m_cursorType = CursorType::ModifyTRAndBL;
				m_cursorArea = CursorArea::CursorCrossBottomLeft;
			} else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossBottomRight) {
				m_rtCalcu.m_cursorType = CursorType::ModifyTLAndBR;
				m_cursorArea = CursorArea::CursorCrossBottomRight;
			} else {

			}
		}

	}

    qDebug() << "event->pos():" << event->pos() << "m_rtCalcu.m_cursorType:" << m_rtCalcu.m_cursorType
             << "m_rtCalcu.m_startPos:" << m_rtCalcu.m_startPos
             << "m_rtCalcu.m_EndPos:" << m_rtCalcu.m_EndPos;

	switch (m_rtCalcu.m_cursorType)
	{
	case Select: {
		m_rtCalcu.m_startPos = event->pos();
        m_rtCalcu.m_EndPos = event->pos();
		break;
	}
	case MovePosition:
		break;
	case ModifWidth:
	case ModifHeight:
	case ModifyTLAndBR:
	case ModifyTRAndBL: {
		m_rtCalcu.m_modifyStartPos = event->pos();
		m_rtCalcu.m_modifyEndPos = event->pos();
		break;
	}
	case Move: {
		setCursor(Qt::ArrowCursor);
		m_rtCalcu.m_moveStartPos = event->pos();
		m_rtCalcu.m_moveEndPos = event->pos();
		break;
	}
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
    qDebug() << "event->pos():" << event->pos() << "m_rtCalcu.m_cursorType:" << m_rtCalcu.m_cursorType
             << "m_rtCalcu.m_startPos:" << m_rtCalcu.m_startPos
             << "m_rtCalcu.m_EndPos:" << m_rtCalcu.m_EndPos;

	switch (m_rtCalcu.m_cursorType)
	{
	case Select: {
		m_rtCalcu.m_EndPos = event->pos();
		setCursor(Qt::ArrowCursor);
		qDebug() << "【mouseMoveEvent】 Select :" << m_rtCalcu.m_startPos << "   " << m_rtCalcu.m_EndPos;
		break;
	}
	case MovePosition: {
		break;
	}
	case ModifWidth:
	case ModifHeight:
	case ModifyTLAndBR:
	case ModifyTRAndBL: {
		m_rtCalcu.m_modifyEndPos = event->pos();
		break;
	}
	case Move: {
        m_rtCalcu.m_moveEndPos = event->pos();
		break;
	}
	case Waiting: {
		switch (m_rtCalcu.getCursorArea(event->pos()))
		{
		case CursorCrossHorizontal:
            setCursor(Qt::SizeHorCursor);
			break;
		case CursorCrossVertical:
			setCursor(Qt::SizeVerCursor);
			break;
		case CursorCrossTL2BR:
			setCursor(Qt::SizeFDiagCursor);
			break;
		case CursorCrossTR2BL:
			setCursor(Qt::SizeBDiagCursor);
			break;
		case CursorInner:
			setCursor(Qt::SizeAllCursor);
			break;
		default:
			setCursor(Qt::ArrowCursor);
			break;
		}
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
    qDebug() << "event->pos():" << event->pos() << "m_rtCalcu.m_cursorType:" << m_rtCalcu.m_cursorType
             << "m_rtCalcu.m_startPos:" << m_rtCalcu.m_startPos
             << "m_rtCalcu.m_EndPos:" << m_rtCalcu.m_EndPos;

	switch (m_rtCalcu.m_cursorType)
	{
	case Select: {
		m_rtCalcu.m_EndPos = event->pos();
		//m_rectCalcu.clear();
		qDebug() << "【mouseReleaseEvent】 Select :" << m_rtCalcu.m_startPos << "   " << m_rtCalcu.m_EndPos;
		break;
	}
	case MovePosition:
		break;
	case ModifWidth:
	case ModifHeight:
	case ModifyTLAndBR:
	case ModifyTRAndBL: {
		modifyRectSize(m_rtCalcu.getSelRect());

		m_rtCalcu.m_modifyStartPos = QPoint();
		m_rtCalcu.m_modifyEndPos = QPoint();
		m_cursorArea = CursorArea::UnknowCursorArea;
		//qDebug() << "【mouseMoveEvent】ModifWidth :" << m_rtCalcu.getSelRect() << m_rtCalcu.getSelRect() << m_rtCalcu.getModifyWidth() << m_rtCalcu.getModifyHeight();
		break;
		break;
	}
	case Move: {
		m_rtCalcu.getSelRect().translate(m_rtCalcu.getMoveWidth(), m_rtCalcu.getMoveHeight());
		m_rtCalcu.m_moveStartPos = QPoint();
		m_rtCalcu.m_moveEndPos = QPoint();
		break;
	}
	case Waiting: {
		break;
	}
	case UnknowCursorType:
		break;
	default:
		break;
	}

	m_rtCalcu.m_cursorType = CursorType::Waiting;
	setMouseTracking(true);
}



// 屏幕详细参数
void WinFullScreen::display()
{
	for (QScreen* it : m_screens) {
        qInfo() << "----------------------------------------------------------------------------\n"
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
#ifdef Q_OS_WIN
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
#elif  defined(Q_OS_MAC)
    double scale = screen->logicalDotsPerInch() / 72.0;

#elif  defined(Q_OS_LINUX)
#endif
}

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
#include <QIcon>
#include <QTime>
#include <QDebug>
#include <QClipboard>
#include <QFileDialog>

#define CURR_TIME QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")

WinFullScreen::WinFullScreen(QWidget *parent)
	: QWidget(parent)
	, m_primaryScreen(nullptr)
	, m_currPixmap(nullptr)
	, m_blurPixmap(nullptr)
	, m_basePixmap(nullptr)
	, m_rtCalcu()
	, m_cursorArea(CursorArea::UnknowCursorArea)
    , m_toolBar(nullptr)
{
	m_primaryScreen = QApplication::primaryScreen();
	m_screens = QApplication::screens();

    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags()); // 去掉标题栏 + 置顶
	//setFixedSize(QApplication::desktop()->size());
	resize(1920, 1080);

    m_toolBar = new WinToolBar(this);
    connect(m_toolBar, &WinToolBar::sigDownload, this, &WinFullScreen::onDownload);
    connect(m_toolBar, &WinToolBar::sigCopy, this, &WinFullScreen::onCopy);

	connect(this, &WinFullScreen::sigClearScreen, this, &WinFullScreen::onClearScreen);
}

WinFullScreen::~WinFullScreen() 
{
	int a = 13;
}

// 清空截图内容（当关闭 Esc、或完成截图时）
void WinFullScreen::onClearScreen()
{
	//m_screens、m_primaryScreen 还保留

	delete m_currPixmap;
	m_currPixmap = nullptr;
	delete m_blurPixmap;
	m_blurPixmap = nullptr;
	delete m_basePixmap;
	m_basePixmap = nullptr;

	m_rtCalcu.clear();
    m_cursorArea = CursorArea::UnknowCursorArea;
}

void WinFullScreen::onDownload()
{
    if (m_savePixmap.isNull())
        return;

    QString fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files件(*.*)"));
    QString fileNmae = QFileDialog::getSaveFileName(this, tr("Save Files"), "PicShot_" + CURR_TIME + ".png", fileter);

    QTime startTime = QTime::currentTime();
    m_savePixmap.save(fileNmae);
    QTime stopTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(stopTime);
    qDebug() << "save m_savePixmap tim =" << elapsed << "ms" << m_savePixmap.size();

    emit sigClearScreen();
    hide();
}

void WinFullScreen::onCopy()
{
    if (m_savePixmap.isNull())
        return;

//    ScreenShots *screenShot = static_cast<ScreenShots *>(parent());
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(m_savePixmap);
    qDebug()<<"--------------onCopy"<<parent()<<parent()->parent();

    emit sigClearScreen();
    hide();
}

// 获取虚拟屏幕截图
void WinFullScreen::getVirtualScreen() 
{
	// TODO 2021-09-29:
	// 万一虚拟屏幕没开启，优先截取当前鼠标所在的屏幕
	if (!m_currPixmap) {
		QDesktopWidget *desktop = QApplication::desktop();  // 获取桌面的窗体对象
		m_currPixmap = new QPixmap(m_primaryScreen->grabWindow(desktop->winId(), 0, 0, desktop->width(), desktop->height()));
	}
}

// 获取屏幕遮罩
QPixmap* WinFullScreen::getblurPixmap(QColor color)
{
	if (!m_currPixmap)
		return nullptr;

	if (!m_blurPixmap) {
		m_blurPixmap = new QPixmap(m_currPixmap->size());
		m_blurPixmap->fill(color);
		//m_blurPixmap->save("m_blurPixmap.png");
	}

	return m_blurPixmap;
}

// 修改拉伸选中矩形的大小
void WinFullScreen::modifyRectSize(QRect& rt)
{
	int width = m_rtCalcu.getModifyWidth();
	int height = m_rtCalcu.getModifyHeight();
	if (m_cursorArea == CursorArea::CursorCrossLeft) {
		rt = m_rtCalcu.getRect(rt, width, m_cursorArea);
	} else if (m_cursorArea == CursorArea::CursorCrossRight) {
		rt = m_rtCalcu.getRect(rt, width, m_cursorArea);
	} else if (m_cursorArea == CursorArea::CursorCrossTop) {
		rt = m_rtCalcu.getRect(rt, height, m_cursorArea);
	} else if (m_cursorArea == CursorArea::CursorCrossBottom) {
		rt = m_rtCalcu.getRect(rt, height, m_cursorArea);
	} else if (m_cursorArea == CursorArea::CursorCrossTopLeft) {
		rt = m_rtCalcu.getRect(rt.topLeft() + QPoint(width, height), rt.bottomRight());
	} else if (m_cursorArea == CursorArea::CursorCrossTopRight) {
		rt = m_rtCalcu.getRect(rt.topRight() + QPoint(width, height), rt.bottomLeft());
	} else if (m_cursorArea == CursorArea::CursorCrossBottomLeft) {
		rt = m_rtCalcu.getRect(rt.bottomLeft() + QPoint(width, height), rt.topRight());
	} else if (m_cursorArea == CursorArea::CursorCrossBottomRight) {
		rt = m_rtCalcu.getRect(rt.bottomRight() + QPoint(width, height), rt.topLeft());
	} else {
	}
}

void WinFullScreen::drawBorderMac(QPainter & pa, QRect rt, int num, bool isRound)
{
	if (num == 0)
		return;

	pa.save();
	pa.setRenderHint(QPainter::Antialiasing, false);
	pa.setBrush(Qt::NoBrush);
	QPen penWhite(QColor(255, 255, 255, 1 * 255), 1);
	penWhite.setStyle(Qt::CustomDashLine);
	penWhite.setDashOffset(0);
    penWhite.setDashPattern(QVector<qreal>() << 4 * WinFullScreen::getScale() << 4 * WinFullScreen::getScale());
	penWhite.setCapStyle(Qt::FlatCap);
	pa.setPen(penWhite);
	pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.right(), rt.top()));
	pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.left(), rt.bottom()));
	pa.drawLine(QPoint(rt.left(), rt.bottom()), QPoint(rt.right(), rt.bottom()));
	pa.drawLine(QPoint(rt.right(), rt.top()), QPoint(rt.right(), rt.bottom()));

	QPen penBlack(penWhite);
	penBlack.setColor(QColor(0, 0, 0, 1 * 255));
	penBlack.setDashOffset(4);
	pa.setPen(penBlack);
	pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.right(), rt.top()));
	pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.left(), rt.bottom()));
	pa.drawLine(QPoint(rt.left(), rt.bottom()), QPoint(rt.right(), rt.bottom()));
	pa.drawLine(QPoint(rt.right(), rt.top()), QPoint(rt.right(), rt.bottom()));

	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	rt.getCoords(&x1, &y1, &x2, &y2);

	QVector<QPoint> ver = { QPoint(x1, y1),
							QPoint(x2, y1),
							QPoint(x1, y2),
							QPoint(x2, y2),
							QPoint((x1 + x2) / 2.0, y1),
							QPoint((x1 + x2) / 2.0, y2),
							QPoint(x1, (y1 + y2) / 2.0),
							QPoint(x2, (y1 + y2) / 2.0) };

	pa.setPen(QPen(Qt::white, 1.5));
	pa.setBrush(QColor(146, 146, 146, 1 * 255));
    QPoint offsetPos(HAIF_R_BORDER_MARK * WinFullScreen::getScale(), HAIF_R_BORDER_MARK * WinFullScreen::getScale());
	pa.setRenderHint(QPainter::Antialiasing, true);

	for (int i = 0; i < num; ++i)
		pa.drawEllipse(ver[i], offsetPos.x(), offsetPos.y());

	pa.restore();
}

// 样式一: 浅蓝色
void WinFullScreen::drawBorderBlue(QPainter& pa, QRect rt, int num, bool isRound)
{
    if (num == 0)
        return;

    pa.setPen(Qt::NoPen);
    pa.setBrush(Qt::NoBrush);

    QIcon icon(":/resources/icons/boardPoint_8px.svg");
    QPixmap pixmap = icon.pixmap(QSize(HAIF_R_BORDER_MARK, HAIF_R_BORDER_MARK) * 4 * WinFullScreen::getScale());
    pixmap.setDevicePixelRatio(getDevicePixelRatio());

    QPoint offsetPos(HAIF_R_BORDER_MARK * 2 * WinFullScreen::getScale(), HAIF_R_BORDER_MARK * 2 * WinFullScreen::getScale()) ;
    pa.drawPixmap(rt.topLeft() - offsetPos, pixmap);
    pa.drawPixmap(rt.topRight() - offsetPos, pixmap);
    pa.drawPixmap(rt.bottomLeft() - offsetPos, pixmap);
    pa.drawPixmap(rt.bottomRight() - offsetPos, pixmap);

    if (num <= 8) {
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;

        rt.getCoords(&x1, &y1, &x2, &y2);
        pa.drawPixmap(QPoint((x1 + x2) / 2, y1) - offsetPos, pixmap);
        pa.drawPixmap(QPoint((x1 + x2) / 2, y2) - offsetPos, pixmap);
        pa.drawPixmap(QPoint(x1, (y1 + y2) / 2) - offsetPos, pixmap);
        pa.drawPixmap(QPoint(x2, (y1 + y2) / 2) - offsetPos, pixmap);
	}
}


// 获取当前屏幕截图 + 遮罩
QPixmap* WinFullScreen::getBasePixmap()
{
	if (!m_currPixmap)
		getVirtualScreen();

	if (!m_blurPixmap)
		getblurPixmap();

	if (!m_basePixmap) {
		m_basePixmap = new QPixmap(m_currPixmap->copy(m_currPixmap->rect()));
		QPainter pa(m_basePixmap);
		pa.drawPixmap(m_basePixmap->rect(), *m_blurPixmap);
		//QTime startTime = QTime::currentTime();
		////m_basePixmap->save("m_basePixmap.png");
		////m_currPixmap->save("m_currPixmap2.png");
		//QTime stopTime = QTime::currentTime();
		//int elapsed = startTime.msecsTo(stopTime);
		//qDebug() << "save m_basePixmap time 汉字测试 =" << elapsed << "ms";
	}

	return m_basePixmap;
}

void WinFullScreen::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	if (!m_basePixmap)
		return;

	QPainter pa(this);
	QPen pen(QColor("#01bdff"));
    const int width = HAIF_INTERVAL * 2;  // 画笔宽度
    pen.setWidth(width);
	pa.setPen(pen);
	pa.setOpacity(1);
	pa.setBrush(Qt::transparent);
	pa.drawPixmap(QApplication::desktop()->rect(), *m_basePixmap);

	QRect rtSel(m_rtCalcu.getSelRect().translated(m_rtCalcu.getMoveWidth(), m_rtCalcu.getMoveHeight()));  // 移动选中矩形
	m_rtCalcu.limitBound(rtSel, rect());
	modifyRectSize(rtSel);  // 拉伸选中矩形大小

	//qDebug() << "【paintEvent】  :" << m_rtCalcu.m_cursorType << m_rtCalcu.getSelRect() << rtSel << m_rtCalcu.getSelRect() << "   " << m_rtCalcu.m_EndPos << "  " << m_basePixmap << "  " << QRect();
	// 注意独立屏幕缩放比（eg: macox = 2）
	if (rtSel.width() > 0 && rtSel.height() > 0){
        m_savePixmap = m_currPixmap->copy(QRect(rtSel.topLeft() * getDevicePixelRatio(), rtSel.size() * getDevicePixelRatio()));
        pa.drawPixmap(rtSel, m_savePixmap);

    #if 0
        drawBorderMac(pa, rtSel);
    #else
        pa.drawRect(rtSel);
        drawBorderBlue(pa, rtSel);
//        pen.setWidth(1);
//        pen.setColor(Qt::red);
//        pa.setPen(pen);
//        pa.drawRect(rtSel);
    #endif
		
		qInfo() << "--------------->rtSel:" << rtSel << "  m_rtCalcu.getSelRect:" << m_rtCalcu.getSelRect();
	}

    if (isVisible() && m_toolBar) {
        QPoint topLeft;
        const int space = 4;
        topLeft.setX(rtSel.bottomRight().x() - m_toolBar->width());
        topLeft.setY(rtSel.bottomRight().y() + width / 2 + space);
        m_toolBar->move(topLeft);
    }


//    if (m_rtCalcu.getSelRect().contains(pos(), false)
//            && m_toolBar->isToolBtnChecked()/*
//            && m_rtCalcu.m_cursorType == Waiting*/)
//        setCursor(Qt::CrossCursor);
//        m_rtCalcu.m_cursorType = Drawing;
//    else
//        m_rtCalcu.m_cursorType = Waiting;

#if 0
    QRect rtOuter = m_rtCalcu.getOuterSelRect(rtSel, width / 2);
    QRect rtInner = m_rtCalcu.getInnerSelRect(rtSel, width / 2);
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
#endif // 1


}

void WinFullScreen::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		qDebug() << "Key_Escape";
		emit sigClearScreen();
		hide();
		//close();   // 销毁会有问题,已经排查：1. tray 有关，改用 qpushbutton 和 close即可； 2.单例有关，该市建议修改为 new 指针的比较合适
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

	update();
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
    case Drawing: {
        setCursor(Qt::CrossCursor);
        break;
    }
	case UnknowCursorType:
		break;
	default:
		break;
	}

	update();
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
	}
	case Move: {
		m_rtCalcu.getSelRect().translate(m_rtCalcu.getMoveWidth(), m_rtCalcu.getMoveHeight());
		m_rtCalcu.limitBound(m_rtCalcu.getSelRect());
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

	update();
}

/*!
 * \brief WinFullScreen::instance 单例的实现
 * \return 返回单例的引用
 * \note 问：类的static变量在什么时候初始化？函数的static变量在什么时候初始化？
 * \li 答：类的静态成员变量在类实例化之前就已经存在了，并且分配了内存。函数的static变量在执行此函数时进行初始化。
 */
WinFullScreen &WinFullScreen::instance()
{
    static WinFullScreen m_instance;
    return m_instance;
}

void WinFullScreen::getScrnShots()
{
    this->getScrnInfo();
    this->getBasePixmap();
    this->show();
}

// 屏幕详细参数
void WinFullScreen::getScrnInfo()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect deskRect = desktopWidget->availableGeometry();   //获取可用桌面大小
	QRect screenRect = desktopWidget->screenGeometry();  //获取设备屏幕大小
	qInfo() << "availableGeometry(可用桌面大小):" << deskRect << "  screenGeometry(设备屏幕大小):"<< screenRect;

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

//
// Created by XMuli <xmulitech@gmail.com> on 2021/9/29.
//

#include "screenshot.h"
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
#include <QImage>

#define CURR_TIME QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")

ScreenShot::ScreenShot(QWidget *parent)
	: QWidget(parent)
	, m_primaryScreen(nullptr)
	, m_currPixmap(nullptr)
	, m_rtCalcu()
    , m_cursorArea(CursorArea::UnknowCursorArea)
    , m_tbDrawBar(new DrawToolBar(this))
{
	m_primaryScreen = QApplication::primaryScreen();
	m_screens = QApplication::screens();

	// 注意显示器摆放的位置不相同~；最大化的可能异常修复
 //   setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags()); // 去掉标题栏 + 置顶
 //   setFixedSize(m_screens[0]->virtualGeometry().size());
	//move(m_screens[0]->virtualGeometry().topLeft());

	//int x1 = 0;
	//int x2 = 0;
	//int y1 = 0;
	//int y2 = 0;
	//m_screens[0]->virtualGeometry().getRect(&x1, &y1, &x2, &y2);
	//QRect rt(x1, y1, x2, y2);
	//qInfo() << "-------->" << m_screens[0]->virtualGeometry() << "  " << QApplication::desktop()->rect() << "   " << rt;

    resize(1920, 1080);

//    m_draw = new XDraw(this);
    connect(m_tbDrawBar, &DrawToolBar::sigDownload, this, &ScreenShot::onDownload);
    connect(m_tbDrawBar, &DrawToolBar::sigCopy, this, &ScreenShot::onCopy);

    connect(m_tbDrawBar, &DrawToolBar::sigDrawStart, this, &ScreenShot::onDrawStart);
    connect(m_tbDrawBar, &DrawToolBar::sigDrawEnd, this, &ScreenShot::onDrawEnd);
    connect(m_tbDrawBar, &DrawToolBar::sigDrawShape, this, &ScreenShot::onDrawShape);
    connect(m_tbDrawBar, &DrawToolBar::sigUndo, this, &ScreenShot::onUndo);
    connect(m_tbDrawBar, &DrawToolBar::sigRedo, this, &ScreenShot::onRedo);

    connect(m_tbDrawBar, &DrawToolBar::sigIsFill, this, [&](bool bFill) {
        m_drawStep.bFill = bFill;
    });

	connect(this, &ScreenShot::sigClearScreen, this, &ScreenShot::onClearScreen);
}

ScreenShot::~ScreenShot() 
{
}

// 清空截图内容（当关闭 Esc、或完成截图时）
void ScreenShot::onClearScreen()
{
	//m_screens、m_primaryScreen 还保留
	delete m_currPixmap;
	m_currPixmap = nullptr;

	m_vDrawed.clear();
	m_vDrawUndo.clear();

	m_rtCalcu.clear();
    m_cursorArea = CursorArea::UnknowCursorArea;
};

void ScreenShot::onDrawShape(XDrawShape shape)
{
    m_drawStep.shape = shape;
    qDebug() << "--------@onDrawShape:" << int(m_drawStep.shape);
}

// 点击一次，撤销一步
void ScreenShot::onUndo()
{
    if (m_vDrawed.count() <= 0)
        return;

    m_vDrawUndo.push_back(*(m_vDrawed.end() - 1));
    m_vDrawed.pop_back();
    qDebug() << "---->m_vDrawRevoke:" << m_vDrawUndo.count() << "    m_vDraw:" << m_vDrawed.count();
    update();
}

void ScreenShot::onRedo()
{
    if (m_vDrawUndo.count() <= 0)
        return;

    m_vDrawed.push_back(*(m_vDrawUndo.end() - 1));
    m_vDrawUndo.pop_back();
    qDebug() << "---->m_vDrawRevoke:" << m_vDrawUndo.count() << "    m_vDraw:" << m_vDrawed.count();
    update();
}

void ScreenShot::onDownload()
{
    if (!m_savePixmap || !m_currPixmap)
        return;

    QPainter pa;
    pa.begin(m_currPixmap);
    for (XDrawStep& it : m_vDrawed)
        drawStep(pa, it);
    pa.end();

    QString fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files件(*.*)"));
    QString fileNmae = QFileDialog::getSaveFileName(this, tr("Save Files"), "PicShot_" + CURR_TIME + ".png", fileter);

    QTime startTime = QTime::currentTime();
    m_savePixmap.save(fileNmae);  // 绘画在 m_savePixmap 中，若在 m_savePixmap 会有 selRect 的左上角的点的偏移
    QTime stopTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(stopTime);
    qDebug() << "save m_savePixmap tim =" << elapsed << "ms" << m_savePixmap.size();

    emit sigClearScreen();
    hide();
}

void ScreenShot::onCopy()
{
    if (!m_savePixmap || !m_currPixmap)
        return;

    QPainter pa;
    pa.begin(m_currPixmap);
    for (XDrawStep& it : m_vDrawed)
        drawStep(pa, it);
    pa.end();

    // TODO: 2021.12.02, 为何添加 QFileDialog::getSaveFileName (此两行)即可顺利保存绘画后的文件，复制到剪切板中？
    // 可能？sleep(1000) x;  update(); 都不是原因，不对
//    QString fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files件(*.*)"));
//    QString fileNmae = QFileDialog::getSaveFileName(this, tr("Save Files"), "PicShot_" + CURR_TIME + ".png");

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(m_savePixmap);

    qDebug()<<"--------------onCopy"<<parent();

    emit sigClearScreen();
    hide();
}

void ScreenShot::onDrawStart()
{
    m_rtCalcu.m_cursorType = CursorType::Drawing;
    setMouseTracking(false);  // Fix: 鼠标移动中会被自动绘画矩形，副作用绘画状态的光标不完美了(选中框内外的光标被固定了)，严格不算 bug，一种外观特效
//    qInfo()<<"--------------onDrawStart"<<m_rtCalcu.m_cursorType;
}

void ScreenShot::onDrawEnd()
{
    m_rtCalcu.m_cursorType = CursorType::Waiting;
    setMouseTracking(true);  // 等待状态开启鼠标跟踪
//    qInfo()<<"--------------onDrawEnd"<<m_rtCalcu.m_cursorType;
}

// 获取虚拟屏幕截图
QPixmap* ScreenShot::getVirtualScreen()
{
	// TODO 2021-09-29:
	// 万一虚拟屏幕没开启，优先截取当前鼠标所在的屏幕
	if (!m_currPixmap) {
		QDesktopWidget *desktop = QApplication::desktop();  // 获取桌面的窗体对象
		m_currPixmap = new QPixmap(m_primaryScreen->grabWindow(desktop->winId(), 0, 0, desktop->width(), desktop->height()));
    }

    return m_currPixmap;
}

// 修改拉伸选中矩形的大小
void ScreenShot::modifyRectSize(QRect& rt)
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

void ScreenShot::drawBorderMac(QPainter & pa, QRect rt, int num, bool isRound)
{
	if (num == 0)
		return;

	pa.save();
	pa.setRenderHint(QPainter::Antialiasing, false);
	pa.setBrush(Qt::NoBrush);
	QPen penWhite(QColor(255, 255, 255, 1 * 255), 1);
	penWhite.setStyle(Qt::CustomDashLine);
	penWhite.setDashOffset(0);
    penWhite.setDashPattern(QVector<qreal>() << 4 * ScreenShot::getScale() << 4 * ScreenShot::getScale());
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
    QPoint offsetPos(HAIF_R_BORDER_MARK * ScreenShot::getScale(), HAIF_R_BORDER_MARK * ScreenShot::getScale());
	pa.setRenderHint(QPainter::Antialiasing, true);

	for (int i = 0; i < num; ++i)
		pa.drawEllipse(ver[i], offsetPos.x(), offsetPos.y());

    pa.restore();
}

// 绘画当前类型的一个图案形状; isUseOwn 为 true 使用自带的画笔等；false 使用上一个环境的
void ScreenShot::drawStep(QPainter& pa, XDrawStep& step, bool isUseOwn)
{
    if (XDrawShape::NoDraw == step.shape)
        return;

    if (isUseOwn) {
        QPen pen(step.pen);
        pen.setWidth(step.penWidth);
        QBrush brush(step.brush);
        QFont font(step.font);
        font.setPixelSize(step.fontSize);

        if (step.bFill){
            brush.setColor(step.pen.color());
            brush.setStyle(Qt::SolidPattern);
        } else {
            brush.setStyle(Qt::NoBrush);
        }

        pa.setPen(pen);
        pa.setBrush(brush);
        pa.setFont(font);
    }

    switch (step.shape) {
    case XDrawShape::Rectangles: {
		if (step.rt.isEmpty())
			break;

        pa.drawRect(step.rt);
        break;
    }
    case XDrawShape::Ellipses: {
		if (step.rt.isEmpty())
			break;

        pa.drawEllipse(step.rt.center(), step.rt.width() / 2, step.rt.height() / 2);
        break;
    }
    case XDrawShape::Arrows: {
        pa.drawLine(step.startPos, step.endPos);
        break;
    }
	case XDrawShape::Brush: {
		pa.drawLine(step.startPos, step.endPos);
		break;
	}
    case XDrawShape::Texts: {
        pa.drawText(step.rt.topLeft(), step.text);
        break;
    }
    case XDrawShape::Mosaics: {
		if (!m_currPixmap)
			return;

		if (!step.bFill) {
			// setMosaicSmooth
		} else {
			// TODO: 多绘画几个就略有点卡顿，一切到此处便会内存先增加后恢复
			if (step.rt.isEmpty())  // 优化，删除就很明显
				break;

			QPixmap mosaicPixmap = m_currPixmap->copy(QRect(step.rt.topLeft() * getDevicePixelRatio(), step.rt.size() * getDevicePixelRatio()));
            const QImage image = SubMosaicToolBar::setMosaicPixlelated(&mosaicPixmap, step.mscPx);
            pa.drawImage(step.rt, image);
		}
        break;
    }
    default:
        break;
    }
}

// 样式一: 浅蓝色
void ScreenShot::drawBorderBlue(QPainter& pa, QRect rt, int num, bool isRound)
{
    if (num == 0)
        return;

    pa.setPen(Qt::NoPen);
    pa.setBrush(Qt::NoBrush);

    QIcon icon(":/resources/icons/boardPoint_8px.svg");
    QPixmap pixmap = icon.pixmap(QSize(HAIF_R_BORDER_MARK, HAIF_R_BORDER_MARK) * 4 * ScreenShot::getScale());
    pixmap.setDevicePixelRatio(getDevicePixelRatio());

    QPoint offsetPos(HAIF_R_BORDER_MARK * 2 * ScreenShot::getScale(), HAIF_R_BORDER_MARK * 2 * ScreenShot::getScale()) ;
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

// 效果：绘画的顺序重要
void ScreenShot::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

    if (!m_currPixmap)
        getVirtualScreen();

    // 原始图案
    QPainter pa(this);
    pa.setRenderHint(QPainter::Antialiasing, true);
    const int width = HAIF_INTERVAL;  // 画笔宽度
    QPen pen(QColor("#01bdff"));
    pen.setWidth(width);
    pa.setPen(pen);
    pa.setOpacity(1);
    pa.setBrush(Qt::transparent);
    pa.drawPixmap(QApplication::desktop()->rect(), *m_currPixmap);

	QRect rtSel(m_rtCalcu.getSelRect().translated(m_rtCalcu.getMoveWidth(), m_rtCalcu.getMoveHeight()));  // 移动选中矩形
	m_rtCalcu.limitBound(rtSel, rect());
	modifyRectSize(rtSel);  // 拉伸选中矩形大小

	//qDebug() << "【paintEvent】  :" << m_rtCalcu.m_cursorType << m_rtCalcu.getSelRect() << rtSel << m_rtCalcu.getSelRect() << "   " << m_rtCalcu.m_EndPos << "  " << m_basePixmap << "  " << QRect();
    // 注意独立屏幕缩放比（eg: macox = 2）
	if (rtSel.width() > 0 && rtSel.height() > 0){
        m_savePixmap = m_currPixmap->copy(QRect(rtSel.topLeft() * getDevicePixelRatio(), rtSel.size() * getDevicePixelRatio()));
        pa.drawPixmap(rtSel, m_savePixmap);

        qInfo() << "m_currPixmap:" << m_currPixmap << "    &m_savePixmap:" << &m_savePixmap<< "    m_savePixmap:" << m_savePixmap;
        qInfo() << "--------------->rtSel:" << rtSel << "  m_rtCalcu.getSelRect:" << m_rtCalcu.getSelRect();
	}

	// 绘画图案
    for (XDrawStep& it : m_vDrawed)
        drawStep(pa, it);

	pen.setWidth(width / 2);
	pen.setColor(Qt::yellow);
	pa.setPen(pen);
	drawStep(pa, m_drawStep, false);

    // 屏幕遮罩
    QPainterPath path;
    path.addRect(QApplication::desktop()->rect());
    path.addRect(rtSel);
    path.setFillRule(Qt::OddEvenFill);
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(0, 0, 0, 0.5 * 255));
    pa.drawPath(path);

    // 边框
    if (rtSel.width() > 0 && rtSel.height() > 0){
        pen.setColor(QColor("#01bdff"));
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(width);
        pa.setPen(pen);
        pa.setBrush(Qt::NoBrush);

        QString str = QString("rtSel(%1, %2, %3 * %4)  m_savePixmap.rect:%5 * %6").arg(rtSel.left()).arg(rtSel.top()).arg(rtSel.width()).arg(rtSel.height())
                .arg(m_savePixmap.width()).arg(m_savePixmap.height());
        pa.drawText(rtSel.topLeft() + QPoint(0, -10), str);

        #if 0
            drawBorderMac(pa, rtSel);
        #else
            pa.drawRect(rtSel);
            drawBorderBlue(pa, rtSel);
        #endif
    }

    // 绘画工具栏
    if (isVisible() && m_tbDrawBar) {
        QPoint topLeft;
        const int space = 8;
        topLeft.setX(rtSel.bottomRight().x() - m_tbDrawBar->width());
        topLeft.setY(rtSel.bottomRight().y() + width + space);
        m_tbDrawBar->move(topLeft);
    }

#if 0
    QRect rtOuter = m_rtCalcu.getOuterSelRect(rtSel, width);
    QRect rtInner = m_rtCalcu.getInnerSelRect(rtSel, width);
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

void ScreenShot::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		qDebug() << "Key_Escape";
		emit sigClearScreen();
		hide();
		//close();   // 销毁会有问题,已经排查：1. tray 有关，改用 qpushbutton 和 close即可； 2.单例有关，该市建议修改为 new 指针的比较合适
	}
}

// 注意: 1. 按下、松开时候会切换状态；点击绘画按钮也会切换状态
//      2. 开启鼠标跟踪时机；点击绘画按钮也会相应开启/关闭
//      3. mousePressEvent、mouseMoveEvent、mouseReleaseEvent 合成整体来看；以及不忘记绘画按钮的槽函数
void ScreenShot::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "event->pos():" << event->pos() << "m_rtCalcu.m_cursorType:" << m_rtCalcu.m_cursorType
             << "m_rtCalcu.m_startPos:" << m_rtCalcu.m_startPos
             << "m_rtCalcu.m_EndPos:" << m_rtCalcu.m_EndPos;

    bool bDrawing = false;
    if (m_rtCalcu.m_cursorType == CursorType::Drawing) {
        bDrawing = true;
    } else {
        m_rtCalcu.m_cursorType = CursorType::Waiting;
    }

	setMouseTracking(false);

	if (m_rtCalcu.getSelRect().isEmpty()) {
		m_rtCalcu.clear();
		m_rtCalcu.m_cursorType = CursorType::Select;
	} else {  // 可能是添加编辑画图、或者移动等模式
		if (event->button() == Qt::LeftButton)
		{
            if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorInner && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::Move;
            } else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossLeft && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::ModifWidth;
				m_cursorArea = CursorArea::CursorCrossLeft;
            } else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossRight && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::ModifWidth;
				m_cursorArea = CursorArea::CursorCrossRight;
            } else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossTop && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::ModifHeight;
				m_cursorArea = CursorArea::CursorCrossTop;
            } else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossBottom && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::ModifHeight;
				m_cursorArea = CursorArea::CursorCrossBottom;
            } else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossTopLeft && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::ModifyTLAndBR;
				m_cursorArea = CursorArea::CursorCrossTopLeft;
            } else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossTopRight && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::ModifyTRAndBL;
				m_cursorArea = CursorArea::CursorCrossTopRight;
            } else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossBottomLeft && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::ModifyTRAndBL;
				m_cursorArea = CursorArea::CursorCrossBottomLeft;
            } else if (m_rtCalcu.getCursorArea(event->pos(), true) == CursorArea::CursorCrossBottomRight && !bDrawing) {
				m_rtCalcu.m_cursorType = CursorType::ModifyTLAndBR;
				m_cursorArea = CursorArea::CursorCrossBottomRight;
			} else {
			}
		}
	}

    qDebug() << "event->pos():" << event->pos() << "m_rtCalcu.m_cursorType:" << m_rtCalcu.m_cursorType
             << "m_rtCalcu.m_startPos:" << m_rtCalcu.m_startPos
             << "m_rtCalcu.m_EndPos:" << m_rtCalcu.m_EndPos << hasMouseTracking();

	switch (m_rtCalcu.m_cursorType)
	{
	case Select: {
		m_rtCalcu.m_startPos = event->pos();
        m_rtCalcu.m_EndPos = event->pos();
		break;
	}
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
    case Drawing: {
        m_drawStep.startPos = event->pos();
        m_drawStep.endPos = event->pos();
        break;
    }
	case UnknowCursorType:
		break;
	default:
		break;
	}

	update();
}

void ScreenShot::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "event->pos():" << event->pos() << "m_rtCalcu.m_cursorType:" << m_rtCalcu.m_cursorType
             << "m_rtCalcu.m_startPos:" << m_rtCalcu.m_startPos
             << "m_rtCalcu.m_EndPos:" << m_rtCalcu.m_EndPos << hasMouseTracking();;

	switch (m_rtCalcu.m_cursorType)
	{
	case Select: {
		m_rtCalcu.m_EndPos = event->pos();
		setCursor(Qt::ArrowCursor);
		qDebug() << "【mouseMoveEvent】 Select :" << m_rtCalcu.m_startPos << "   " << m_rtCalcu.m_EndPos;
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
       if (m_rtCalcu.getSelRect().contains(event->pos(), false))
          setCursor(Qt::CrossCursor);
       else
           setCursor(Qt::ArrowCursor);

       m_drawStep.endPos = event->pos();
       m_drawStep.rt = RectCalcu::getRect(m_drawStep.startPos, m_drawStep.endPos);
//       qDebug() << "-----MOVE------Drawing->:" << m_drawStep.rt << (int)m_drawStep.shape;
// << m_rtCalcu.getSelRect() << pos() << "【" << m_rtCalcu.getSelRect().contains(pos(), false) << cursor();
        break;
    }
	case UnknowCursorType:
		break;
	default:
		break;
	}

	update();
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "event->pos():" << event->pos() << "m_rtCalcu.m_cursorType:" << m_rtCalcu.m_cursorType
             << "m_rtCalcu.m_startPos:" << m_rtCalcu.m_startPos
             << "m_rtCalcu.m_EndPos:" << m_rtCalcu.m_EndPos << hasMouseTracking();

	switch (m_rtCalcu.m_cursorType)
	{
	case Select: {
		m_rtCalcu.m_EndPos = event->pos();
		//m_rectCalcu.clear();
		qDebug() << "【mouseReleaseEvent】 Select :" << m_rtCalcu.m_startPos << "   " << m_rtCalcu.m_EndPos;
		break;
	}
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
    case Drawing: {
        m_drawStep.endPos = event->pos();
        m_drawStep.rt = RectCalcu::getRect(m_drawStep.startPos, m_drawStep.endPos);
        m_vDrawed.push_back(m_drawStep);

        m_drawStep.clear();
        qInfo() << "--------------------------count>"<<m_vDrawed.count();

        int i = 1;
        for (XDrawStep it : m_vDrawed) {
            qDebug() << i++ << "  rt:" << it.rt << "  shape:" << int(it.shape) << endl;
        }
        break;
    }
	case UnknowCursorType:
		break;
	default:
		break;
	}

    if (m_rtCalcu.m_cursorType != CursorType::Drawing) {
        m_rtCalcu.m_cursorType = CursorType::Waiting;
        setMouseTracking(true);
    }

	update();
}

/*!
 * \brief WinFullScreen::instance 单例的实现
 * \return 返回单例的引用
 * \note 问：类的static变量在什么时候初始化？函数的static变量在什么时候初始化？
 * \li 答：类的静态成员变量在类实例化之前就已经存在了，并且分配了内存。函数的static变量在执行此函数时进行初始化。
 */
ScreenShot &ScreenShot::instance()
{
    static ScreenShot m_instance;
    return m_instance;
}

void ScreenShot::getScrnShots()
{
    this->getScrnInfo();
    this->show();
}

// 屏幕详细参数
void ScreenShot::getScrnInfo()
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

double ScreenShot::getDevicePixelRatio()
{
	return m_primaryScreen->devicePixelRatio();
}

double ScreenShot::getDevicePixelRatio(QScreen * screen)
{
	if (!screen)
		return 0.0;
	else
        return screen->devicePixelRatio();
}

double ScreenShot::getScale(QScreen * screen)
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

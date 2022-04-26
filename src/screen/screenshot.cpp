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
#include <QTextEdit>
#include "../core/xlog/xlog.h"

#define _MYDEBUG

#define CURR_TIME QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")

namespace Util {

#ifdef Q_OS_WIN
    bool getRectFromCurrentPoint(HWND hWndMySelf, QRect &outRect)
    {
        POINT pt;
        ::GetCursorPos(&pt);
        WinInfoWin::instance().setWindowsFilter(hWndMySelf);
        RECT rect = WinInfoWin::instance().getWindowsRectFromPoint(pt, TRUE);
        outRect = QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
        return outRect.isValid();
    }
#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
#endif

}

ScreenShot::ScreenShot(QWidget *parent)
	: QWidget(parent)
    , m_screens(QApplication::screens())
	, m_primaryScreen(QApplication::primaryScreen())
	, m_currPixmap(nullptr)
	, m_rtCalcu()
    , m_pCurrShape(nullptr)
    , m_tbDrawBar(new DrawToolBar(this))
	, m_textEdit(new XTextWidget(this))
    , m_rtVirDesktop(0, 0, 0, 0)
{
    XLOG_INFO("bootUniqueId[{}]", QSysInfo::bootUniqueId().data());
    XLOG_INFO("buildAbi[{}]", QSysInfo::buildAbi().toUtf8().data());
    XLOG_INFO("buildCpuArchitecture[{}]", QSysInfo::buildCpuArchitecture().toUtf8().data());
    XLOG_INFO("currentCpuArchitecture[{}]", QSysInfo::currentCpuArchitecture().toUtf8().data());
    XLOG_INFO("kernelType[{}]", QSysInfo::kernelType().toUtf8().data());
    XLOG_INFO("kernelVersion[{}]", QSysInfo::kernelVersion().toUtf8().data());
    XLOG_INFO("machineHostName[{}]", QSysInfo::machineHostName().toUtf8().data());
    XLOG_INFO("machineUniqueId[{}]", QSysInfo::machineUniqueId().data());
    XLOG_INFO("prettyProductName[{}]", QSysInfo::prettyProductName().toUtf8().data());
    XLOG_INFO("productType[{}]", QSysInfo::productType().toUtf8().data());
    XLOG_INFO("productVersion[{}]", QSysInfo::productVersion().toUtf8().data());


//    QPluginLoader plugin_loader("plugin.dll");
//    QObject* plugin = plugin_loader.instance();
//    if(plugin)
//    {
//        interface = qobject_cast<PluginInterface*>(plugin);
//        if(interface)
//        {
//            interface->SayHello(this);
//        }
//        //delete plugin;
//        plugin_loader.unload();
//    }

	QDesktopWidget *desktop = QApplication::desktop();  // 获取桌面的窗体对象
	const QRect geom = desktop->geometry();             // 多屏的矩形取并集

    //QFont font("STXingkai", 40); // 设置默认值
    //m_textEdit->setFont(font);
    m_textEdit->setTextColor(Qt::red);
    m_textEdit->hide();
	
	// 注意显示器摆放的位置不相同~；最大化的可能异常修复
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags()); // 去掉标题栏 + 置顶
    #ifdef _MYDEBUG
        if (m_screens.size() >= 2) {

            int index = 0;   // 使用主屏的屏幕作为 Debug 测试
            for (; index < m_screens.size(); ++index) {
                if (m_primaryScreen == m_screens.at(index))
                    break;
            }
            
            setFixedSize(m_screens.at(index)->size());
            //move(m_screens.at(index)->geometry().topLeft());

        } else {
            QSize size(m_screens.at(0)->size());
            resize(size.width() / 2.0, size.height());
            move(m_screens.at(0)->geometry().topLeft());
        }

        
    #else
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags()); // 去掉标题栏 + 置顶
        setFixedSize(geom.size());
        
    #endif

#else // Q_OS_MAC
    setWindowFlags(Qt::Window);  // 不设置则 mac 下 devicePixelRatio: 1
    #ifdef _MYDEBUG
        if (m_screens.size() >= 2) {
            int index = -1;   // 使用非主屏的一块屏幕作为 Debug 测试
            for (; index < m_screens.size(); ++index) {
                if (m_primaryScreen != m_screens.at(index))
                    break;
            }
            
        } else {
            QSize size(m_screens.at(0)->size());
            resize(size.width() / 2.0, size.height());
        }
    #else
        showFullScreen();
    #endif
#endif
        
    move(geom.topLeft());

    

	//int x1 = 0;
	//int x2 = 0;
	//int y1 = 0;
	//int y2 = 0;
	//m_screens[0]->virtualGeometry().getRect(&x1, &y1, &x2,  &y2) ;
	//QRect rt(x1, y1, x2, y2);

    setMouseTracking(true);
    m_rtCalcu.scrnType = ScrnType::Wait;
    

//    m_draw = new XDraw(this);
    connect(m_tbDrawBar, &DrawToolBar::sigDownload, this, &ScreenShot::onDownload);
    connect(m_tbDrawBar, &DrawToolBar::sigCopy, this, &ScreenShot::onCopy);

    connect(m_tbDrawBar, &DrawToolBar::sigDrawStart, this, &ScreenShot::onDrawStart);
    connect(m_tbDrawBar, &DrawToolBar::sigDrawEnd, this, &ScreenShot::onDrawEnd);
    connect(m_tbDrawBar, &DrawToolBar::sigDrawShape, this, &ScreenShot::onDrawShape);
    connect(m_tbDrawBar, &DrawToolBar::sigUndo, this, &ScreenShot::onUndo);
    connect(m_tbDrawBar, &DrawToolBar::sigRedo, this, &ScreenShot::onRedo);

    connect(m_tbDrawBar, &DrawToolBar::sigIsFill, this, [&](bool bFill) {
        m_step.bFill = bFill;
    });

	connect(m_tbDrawBar, &DrawToolBar::sigLineEndsChange, this, &ScreenShot::onLineEndsChange);
	connect(m_tbDrawBar, &DrawToolBar::sigLineDasheChange, this, &ScreenShot::onLineDasheChange);

	connect(this, &ScreenShot::sigClearScreen, this, &ScreenShot::onClearScreen);
}

ScreenShot::~ScreenShot() 
{
}

ScrnType ScreenShot::updateScrnType(const QPoint pos)
{
	CursorArea cursArea = m_rtCalcu.getCursorArea(pos);

	// 后面加上绘画和选择
	if (cursArea == CursorArea::Internal) {
		return ScrnType::Move;
	} else if (cursArea == CursorArea::External) {
		return ScrnType::Wait;
	} else if (cursArea == CursorArea::Border) {
		return ScrnType::Stretch;
    } else {
        return ScrnType::Wait; // 避免警告，不会运行到此
    }

}

void ScreenShot::updateCursorShape(const QPoint pos) 
{
    if (m_rtCalcu.scrnType == ScrnType::Draw) {
        setCursor(Qt::ArrowCursor);
        return;
    }
    
	CursorArea cursArea = m_rtCalcu.getCursorArea(pos, true);

    if (m_rtCalcu.scrnType == ScrnType::Move) {
        //if (cursArea == CursorArea::External)
        //    setCursor(Qt::ForbiddenCursor);
        //else
        //    setCursor(Qt::SizeAllCursor);
    } else if (m_rtCalcu.scrnType == ScrnType::Select) {
        setCursor(Qt::CrossCursor);
    } else if (m_rtCalcu.scrnType == ScrnType::Stretch) {

    } else if (m_rtCalcu.scrnType == ScrnType::Wait) {
        if (cursArea == CursorArea::External) {
            setCursor(Qt::ArrowCursor);
        } else if (cursArea == CursorArea::Internal){
            setCursor(Qt::SizeAllCursor);
        } else {
            updateBorderCursorShape(cursArea);
        }
    }
}

// 功能函数，仅考虑边框状态
void ScreenShot::updateBorderCursorShape(const CursorArea & cursArea)
{
    if (cursArea == CursorArea::Top || cursArea == CursorArea::Bottom)
        setCursor(Qt::SizeVerCursor);
    else if (cursArea == CursorArea::Left || cursArea == CursorArea::Right)
        setCursor(Qt::SizeHorCursor);
    else if (cursArea == CursorArea::TopLeft || cursArea == CursorArea::BottomRight)
        setCursor(Qt::SizeFDiagCursor);
    else if (cursArea == CursorArea::TopRight || cursArea == CursorArea::BottomLeft)
        setCursor(Qt::SizeBDiagCursor);
}

// 清空截图内容（当关闭 Esc、或完成截图时）
void ScreenShot::onClearScreen()
{
#ifdef Q_OS_WIN
    m_vec.clear();

    //if (m_rtCalcu.bSmartMonitor)  // 存储所需要全部窗口信息
    //    m_vec = WinInfoWin::instance().m_vWinInfo;

#elif  defined(Q_OS_MAC)
    setWindowFlags(Qt::SubWindow);
    showNormal();
#elif  defined(Q_OS_LINUX)
#endif
    
    m_vWholeScrn.clear();

	//m_screens、m_primaryScreen 还保留
	delete m_currPixmap;
	m_currPixmap = nullptr;

	m_vDrawed.clear();
	m_vDrawUndo.clear();

	m_rtCalcu.clear();

    m_rtAtuoMonitor = QRect();
    if (m_rtCalcu.scrnType == ScrnType::Wait) // 状态重置
        setMouseTracking(true);
};

void ScreenShot::onDrawShape(DrawShape shape)
{
    m_step.shape = shape;
    qDebug() << "--------@onDrawShape:" << int(m_step.shape);
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
    if (drawToCurrPixmap()) {
        QString fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files(*.*)"));
        QString fileNmae = QFileDialog::getSaveFileName(this, tr("Save Files"), "PicShot_" + CURR_TIME + ".png", fileter);

        QTime startTime = QTime::currentTime();
        m_savePixmap.save(fileNmae);  // 绘画在 m_savePixmap 中，若在 m_savePixmap 会有 selRect 的左上角的点的偏移
        QTime stopTime = QTime::currentTime();
        int elapsed = startTime.msecsTo(stopTime);
        qDebug() << "save m_savePixmap tim =" << elapsed << "ms" << m_savePixmap.size();
        m_currPixmap->save("a2.png");
    }

    emit sigClearScreen();
    hide();
}

void ScreenShot::onCopy()
{
    if (drawToCurrPixmap()) {
        if (!m_savePixmap.isNull())
            QApplication::clipboard()->setPixmap(m_savePixmap);
    }

    //qDebug()<<"--------------onCopy"<<parent() << "  " << m_savePixmap.size();
    emit sigClearScreen();
    hide();
}

void ScreenShot::onDrawStart()
{
    m_rtCalcu.scrnType = ScrnType::Draw;

    setMouseTracking(false);  // Fix: 鼠标移动中会被自动绘画矩形，副作用绘画状态的光标不完美了(选中框内外的光标被固定了)，严格不算 bug，一种外观特效
//    qInfo()<<"--------------onDrawStart"<<m_rtCalcu.scrnType;
}

void ScreenShot::onDrawEnd()
{
    m_rtCalcu.scrnType = ScrnType::Wait;
    setMouseTracking(true);  // 等待状态开启鼠标跟踪
//    qInfo()<<"--------------onDrawEnd"<<m_rtCalcu.scrnType;
}

void ScreenShot::onLineEndsChange(LineEnds ends)
{
	m_step.lineEnds = ends;
}

void ScreenShot::onLineDasheChange(Qt::PenStyle dashes)
{
	m_step.lineDashes = dashes;
}

// 获取虚拟屏幕截图
QPixmap* ScreenShot::getVirtualScreen()
{
	// TODO 2021-09-29:
	// 万一虚拟屏幕没开启，优先截取当前鼠标所在的屏幕
	if (!m_currPixmap) {
        QDesktopWidget* desktop = QApplication::desktop();  // 获取桌面的窗体对象

#if defined(Q_OS_MAC)
        if (!desktop)
            return nullptr;

        QRect rtScrn = desktop->screen(desktop->screenNumber(QCursor::pos()))->rect();
        m_currPixmap = new QPixmap(m_primaryScreen->grabWindow(desktop->winId(), rtScrn.x(), rtScrn.y(), rtScrn.width(), rtScrn.height()));

        XLOG_DEBUG("rtScrn({}, {}, {} * {})", rtScrn.x(), rtScrn.y(), rtScrn.width(), rtScrn.height());
#else
        const QRect geom = desktop->geometry();             // 多屏的矩形取并集
        m_currPixmap = new QPixmap(m_primaryScreen->grabWindow(desktop->winId(), geom.x(), geom.y(), desktop->width(), desktop->height()));

#endif

        //m_currPixmap->save("123456.png");
    }

    return m_currPixmap;
}

bool ScreenShot::drawToCurrPixmap()
{
    if (!m_savePixmap || !m_currPixmap)
        return false;

    QPainter pa;
    pa.begin(m_currPixmap);
    for (XDrawStep& it : m_vDrawed)
        drawStep(pa, it);
    pa.end();

    QRect rtSel(m_rtCalcu.getSelRect());
    const QRect rtAppVirDesktop(QPoint(0, 0), m_rtVirDesktop.size());
    m_rtCalcu.limitBound(rtSel, rtAppVirDesktop);
    if (rtSel.width() > 0 && rtSel.height() > 0)
        m_savePixmap = m_currPixmap->copy(QRect(rtSel.topLeft() * getDevicePixelRatio(), rtSel.size() * getDevicePixelRatio()));  // 注意独立屏幕缩放比（eg: macox = 2）

    return (!m_savePixmap.isNull() && m_currPixmap);
}

// 获取已绘画形状的矩形，准备拖曳移动其
bool ScreenShot::getDrawedShapeRect()
{
    return false;
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

// 绘画当前类型的一个图案形状; isUseOwn 为 true 使用自带的画笔等；true 使用上一个环境的
void ScreenShot::drawStep(QPainter& pa, XDrawStep& step, bool isUseEnvContext)
{
    if (DrawShape::NoDraw == step.shape || (m_pCurrShape == &step && m_rtCalcu.scrnType == ScrnType::Move)) // 选中图形的处于移动状态，跳过这不绘画
        return;

    if (!isUseEnvContext) {
        QPen pen(step.pen);
        pen.setWidth(step.penWidth);
		pen.setStyle(step.lineDashes);
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
    case DrawShape::Rectangles: {
		if (step.rt.isEmpty())
			break;

        pa.drawRect(step.rt);
        break;
    }
    case DrawShape::Ellipses: {
		if (step.rt.isEmpty())
			break;

        pa.drawEllipse(step.rt.center(), step.rt.width() / 2, step.rt.height() / 2);
        break;
    }
    case DrawShape::Arrows: {
		step.brush.setColor(step.pen.color());   // TODO 2022.01.16: 可以优化到上面去的 if 判断
		step.brush.setStyle(Qt::SolidPattern);
		pa.setBrush(step.brush);

		QPainterPath arrowPath = SubAbsLineToolBar::getArrowHead(step.pos1, step.pos2);
		pa.fillPath(arrowPath, step.brush);
		pa.drawLine(SubAbsLineToolBar::getShorterLine(step.pos1, step.pos2));

		//QPoint offset(0, 20);
		//pa.setPen(Qt::green);
		//pa.drawLine(step.startPos + offset, step.endPos + offset);
        break;
    }
	case DrawShape::Brush: {

		pa.drawPolyline(step.custPath.data(), step.custPath.size());
		break;
	}
    case DrawShape::Text: {
        // 记住：这是每一个 step 都会绘画的
        if (step.bTextComplete && !step.bDisplay && step.text > 0) {
            QFontMetrics metrics(m_textEdit->font());

            //QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
            //QPoint pos(step.editPos.x(), step.editPos.y() + metrics.ascent() + metrics.leading()); // 偏移得到视觉的“正确”

            int flags = Qt::TextWordWrap; // 自动换行
            QRect textBoundingRect = metrics.boundingRect(QRect(0, 0, m_rtVirDesktop.width(), 0), flags, step.text);

            //http://qtdebug.com/qtbook-paint-text
            //https://doc.qt.io/qt-5/qpainter.html#drawText-5
            const QFont font(pa.font());
            pa.setFont(m_textEdit->font());
            pa.drawText(QRect(step.editPos, textBoundingRect.size()), flags, step.text);
            pa.setFont(font);
        }   
        break;
    }
    case DrawShape::Mosaics: {
		if (!m_currPixmap || step.rt.isEmpty())  // 优化，删除就很明显
			return;
        
        QPixmap mosaicPixmap = m_currPixmap->copy(QRect(mapFromGlobal(step.rt.topLeft()) * getDevicePixelRatio(), step.rt.size() * getDevicePixelRatio()));
        if (step.bFill) { 
            const QImage img = SubMosaicToolBar::setMosaicPixlelated(&mosaicPixmap, step.mscPx);
            pa.drawImage(step.rt, img);
		} else {
            const QPixmap* pix = SubMosaicToolBar::setMosaicSmooth(&mosaicPixmap, step.mscPx);
            pa.drawPixmap(step.rt, *pix);
		}

        break;
    }
    default:
        break;
    }
}

bool ScreenShot::isDrawShape(XDrawStep& step)
{
    if (step.pos1 == step.pos2)
        return false;

    //if (step.shape == DrawShape::Rectangles) {
    //} else if (step.shape == DrawShape::Ellipses) {

    //} else if (step.shape == DrawShape::Brush) {

    //} else if (step.shape == DrawShape::Arrows) {

    //} else if (step.shape == DrawShape::Mosaics) {

    //} else if (step.shape == DrawShape::Text || step.shape == DrawShape::NoDraw) {
    //    return false; // 特殊处理，或者不处理
    //} else {
    //    return true; // 避免警告
    //}

    return true;
}


void ScreenShot::whichShape()
{
    bool bChecked = false; // 点击是否选中
    const int r = 3;
    QPoint pos(QCursor::pos());
    for (auto it = m_vDrawed.rbegin(); it != m_vDrawed.rend(); ++it) {
        const QRect& rt = it->rt;

        if (it->shape == DrawShape::Rectangles) {
            const QRect& rtIn = rt.adjusted(r, r, -r, -r);
            const QRect& rtOut = rt.adjusted(-r, -r, r, r);

            bool bFirst = rtOut.contains(pos) && !rtIn.contains(pos, true);
            bool bOther = rtOut.contains(pos) && m_pCurrShape == &(*it);

            if (bFirst || bOther){
                m_pCurrShape = &(*it);
                bChecked = true;
                return;
            }
        }
    }

    m_pCurrShape = nullptr;  // 没有选中任何一个
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

void ScreenShot::showAllDrawedShape(QPainter& pa)
{
    QPoint posText(0, 100);
    const int space = 15;
    QRect m_rtCalcu_selRect(m_rtCalcu.getSelRect());

    int i = 0;
    pa.drawText(posText + QPoint(0, space * i), QString("m_vDrawed:%0").arg(m_vDrawed.count()));

    //for (const auto& it : m_vDrawed){
    //    pa.drawText(posText + QPoint(0, space * i), QString("m_vDrawed[%0]:[%1]")
    //        .arg(i++).arg(it.shape));
    //}





    //pa.drawText(posText + QPoint(0, space * 1), QString("pos1: (%1, %2)  pos2: (%3, %4)")
    //    .arg(m_rtCalcu.pos1.x()).arg(m_rtCalcu.pos1.y()).arg(m_rtCalcu.pos2.x()).arg(m_rtCalcu.pos2.y()));
    //pa.drawText(posText + QPoint(0, space * 2), QString("pos2 - pos1:(%1, %2)")
    //    .arg(m_rtCalcu.pos2.x() - m_rtCalcu.pos1.x()).arg(m_rtCalcu.pos2.y() - m_rtCalcu.pos1.y()));
    //pa.drawText(posText + QPoint(0, space * 3), QString("m_rtCalcu.getSelRect(): (%1, %2, %3 * %4)")
    //    .arg(m_rtCalcu_selRect.x()).arg(m_rtCalcu_selRect.y()).arg(m_rtCalcu_selRect.width()).arg(m_rtCalcu_selRect.height()));
    //pa.drawText(posText + QPoint(0, space * 4), QString("rtSel: (%1, %2, %3 * %4)")
    //    .arg(rtSel.x()).arg(rtSel.y()).arg(rtSel.width()).arg(rtSel.height()));
    //pa.drawText(posText + QPoint(0, space * 5), QString("pos(): (%1, %2)")
    //    .arg(pos().x()).arg(pos().y()));
    //pa.drawText(posText + QPoint(0, space * 6), QString("m_rtAtuoMonitor: (%1, %2), rtAtuoMonitor: (%3, %4)")
    //    .arg(m_rtAtuoMonitor.x()).arg(m_rtAtuoMonitor.y()).arg(rtAtuoMonitor.x()).arg(rtAtuoMonitor.y()));
    //pa.drawText(posText + QPoint(0, space * 7), QString("m_rtCalcu.bSmartMonitor: %1")
    //    .arg(m_rtCalcu.bSmartMonitor));
}

// 效果：绘画的顺序重要
void ScreenShot::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

    // 原始图案
    QPainter pa(this);
    pa.translate(-1 * m_rtVirDesktop.topLeft()); // *** 偏移为显示器坐标下绘画 ***
    pa.setBrush(Qt::NoBrush);
    pa.setPen(Qt::NoPen);
    pa.drawPixmap(m_rtVirDesktop, *m_currPixmap);

    // 选中矩形图片
    QRect rtSel(m_rtCalcu.getSelRect());   // 移动选中矩形
    if (m_rtCalcu.bSmartMonitor)
        rtSel = m_rtAtuoMonitor;

    m_rtCalcu.limitBound(rtSel, m_rtVirDesktop);           // 修复边界时图片被拉伸
    if (rtSel.width() > 0 && rtSel.height() > 0) {
        m_savePixmap = m_currPixmap->copy(QRect(mapFromGlobal(rtSel.topLeft()) * getDevicePixelRatio(), rtSel.size() * getDevicePixelRatio()));  // 注意独立屏幕缩放比（eg: macox = 2）
        pa.drawPixmap(rtSel, m_savePixmap);

        // m_savePixmap 和 m_currPixmap 的地址没有改变，但前者的 cacheKey 总在变化???
        //qInfo() << "ScreenShot::paintEvent()";
        //qInfo() << "---------##-> m_savePixmap:[" << m_savePixmap << "],   &m_savePixmap:[" << &m_savePixmap << "]";
        //qInfo() << "---------##->*m_currPixmap:[" << *m_currPixmap << "],   m_currPixmap:[" << m_currPixmap << "]";

        //qInfo() << "m_currPixmap:" << m_currPixmap << "    &m_savePixmap:" << &m_savePixmap<< "    m_savePixmap:" << m_savePixmap;
        //qInfo() << "--------------->rtSel:" << rtSel << "  m_rtCalcu.getSelRect:" << m_rtCalcu.getSelRect();
    }

    // 画家准备
    pa.setRenderHint(QPainter::Antialiasing, true);
    const int penWidth = HAIF_INTERVAL;  // 画笔宽度
    QPen pen(QColor("#01bdff"));
    pen.setWidth(penWidth);
    pa.setPen(pen);
    pa.setOpacity(1);
    pa.setBrush(Qt::transparent);

	// 绘画图案
    for (XDrawStep& it : m_vDrawed)
        drawStep(pa, it);

    // 绘画当前步
	pen.setWidth(penWidth / 2);
	pen.setColor(Qt::green);
	pa.setPen(pen);
	drawStep(pa, m_step, false);

    // flameshot 选中图形的效果
    QRect rtCurMove;
    if (m_pCurrShape && (m_rtCalcu.scrnType == ScrnType::Move || m_rtCalcu.scrnType == ScrnType::Wait)){
        pa.save();
        rtCurMove = m_pCurrShape->rt.translated(m_rtCalcu.pos2 - m_rtCalcu.pos1);

        pa.setRenderHint(QPainter::Antialiasing, true);
        pa.setBrush(Qt::NoBrush);
        QPen penWhite(QColor(255, 255, 255, 1 * 255), 1);
        penWhite.setStyle(Qt::CustomDashLine);
        penWhite.setDashOffset(0);
        penWhite.setDashPattern(QVector<qreal>() << 4 * ScreenShot::getScale() << 4 * ScreenShot::getScale());
        penWhite.setCapStyle(Qt::FlatCap);
        pa.setPen(penWhite);

        const int r = 3;
        pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));
        if (m_rtCalcu.scrnType == ScrnType::Wait)
            pa.drawRect(m_pCurrShape->rt.adjusted(-r, -r, r, r));

        QPen penBlack(penWhite);
        penBlack.setColor(QColor(0, 0, 0, 1 * 255));
        penBlack.setDashOffset(4);
        pa.setPen(penBlack);

        pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));
        if (m_rtCalcu.scrnType == ScrnType::Wait)
            pa.drawRect(m_pCurrShape->rt.adjusted(-r, -r, r, r));

        pa.restore();
        pa.drawRect(rtCurMove);
    }

    // 屏幕遮罩
    QPainterPath path;
    path.addRect(m_rtVirDesktop);
    path.addRect(rtSel);
    path.setFillRule(Qt::OddEvenFill);
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(0, 0, 0, 0.5 * 255));
    pa.drawPath(path);

    // 边框
    if (rtSel.width() > 0 && rtSel.height() > 0){
        pen.setColor(QColor("#01bdff"));
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(penWidth);
        pa.setPen(pen);
        pa.setBrush(Qt::NoBrush);

        QString str = QString("rtSel(%1, %2, %3 * %4)  m_savePixmap.rect:%5 * %6").arg(rtSel.left()).arg(rtSel.top()).arg(rtSel.width()).arg(rtSel.height())
                .arg(m_savePixmap.width()).arg(m_savePixmap.height());
        pa.drawText(rtSel.topLeft() + QPoint(0, -10), str);

        #if 1
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
        topLeft.setY(rtSel.bottomRight().y() + penWidth + space);
        m_tbDrawBar->move(mapFromGlobal(topLeft));
    }


    //#ifdef _DEBUG  调试信息
    // 构造函数有偏移 geom.topLeft(); 绘画时候要偏移回来
    // TODO 2022.01.28: 要屏蔽部分窗口；尤其那个 "设置窗口名称的"，还要做一下区分
    const QRect geom = QApplication::desktop()->geometry();
    int offsetX = geom.x();
    int offsetY = geom.y();

    pen.setColor(Qt::red);
    pa.setPen(pen);
    pa.setBrush(Qt::NoBrush);

#ifdef Q_OS_WIN
    if (m_vec.size() > 0) {
        for (auto it = m_vec.cbegin(); it != m_vec.cend(); ++it) {
            QRect rt(it->rect.left, it->rect.top, it->rect.right - it->rect.left, it->rect.bottom - it->rect.top);

            QRect rtDraw(rt.left() - offsetX, rt.top() - offsetY, rt.width(), rt.height());
            pa.drawRect(rtDraw);
            qInfo() << "--------------->@geom:" << geom << "  m_vec 中 it 矩形:" << rt << "  it 偏移之后的绘画矩形 rtDraw:" << rtDraw;
            CString path = it->procPath;
            pa.drawText(rtDraw.topLeft() + QPoint(0, 50), QString("%1, %2, %3, %4, %5")
                        .arg(path.GetBuffer(path.GetLength())).arg(rt.left() - geom.left()).arg(rt.top() - geom.top()).arg(rt.width()).arg(rt.height()));

        }
    }
#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
#endif

    //pen.setColor(Qt::yellow);
    //pa.setPen(pen);
    //QRect rtAtuoMonitor(m_rtAtuoMonitor);  // 特地使用副本，保留原来数值避免被修改，也是按下和松开判断时候数据保持一致
    //if (m_rtCalcu.bSmartMonitor) {
    //    
    //    for (const auto & it : m_vWholeScrn) {
    //        if (it == rtAtuoMonitor)
    //            rtAtuoMonitor.adjust(10, 10, -10, -10);
    //    }

    //    pa.drawRect(rtAtuoMonitor);
    //}

    pen.setColor(Qt::white);
    pa.setPen(pen);

    // 调试的实时数据
    QFont font;//(font());
    font.setPointSize(12);  // 默认大小为 9
    pa.setFont(font);
    const int space = font.pointSize() * 2.5;

    QPoint tTopLeft;
    tTopLeft.setX(m_primaryScreen->geometry().x());
    tTopLeft.setY(m_primaryScreen->size().height());

    QPoint tPosText(tTopLeft.x(), tTopLeft.y() - 5 * space);

    QRect m_rtCalcu_selRect(m_rtCalcu.getSelRect());
    pa.drawText(tPosText - QPoint(0, space * 0), QString("m_rtCalcu.scrnType: %1")
                .arg(int(m_rtCalcu.scrnType)));
    pa.drawText(tPosText - QPoint(0, space * 1), QString("pos1: (%1, %2)  pos2: (%3, %4)")
                .arg(m_rtCalcu.pos1.x()).arg(m_rtCalcu.pos1.y()).arg(m_rtCalcu.pos2.x()).arg(m_rtCalcu.pos2.y()));
    pa.drawText(tPosText - QPoint(0, space * 2), QString("pos2 - pos1:(%1, %2)")
                .arg(m_rtCalcu.pos2.x() - m_rtCalcu.pos1.x()).arg(m_rtCalcu.pos2.y() - m_rtCalcu.pos1.y()));
    pa.drawText(tPosText - QPoint(0, space * 3), QString("m_rtCalcu.getSelRect(): (%1, %2, %3 * %4)")
                .arg(m_rtCalcu_selRect.x()).arg(m_rtCalcu_selRect.y()).arg(m_rtCalcu_selRect.width()).arg(m_rtCalcu_selRect.height()));
    pa.drawText(tPosText - QPoint(0, space * 4), QString("rtSel: (%1, %2, %3 * %4)")
                .arg(rtSel.x()).arg(rtSel.y()).arg(rtSel.width()).arg(rtSel.height()));
    pa.drawText(tPosText - QPoint(0, space * 5), QString("pos(): (%1, %2)")
                .arg(pos().x()).arg(pos().y()));
    pa.drawText(tPosText - QPoint(0, space * 6), QString("m_rtAtuoMonitor: (%1, %2)")
        .arg(m_rtAtuoMonitor.x()).arg(m_rtAtuoMonitor.y()));
    pa.drawText(tPosText - QPoint(0, space * 7), QString("m_rtCalcu.bSmartMonitor: %1")
        .arg(m_rtCalcu.bSmartMonitor));
    pa.drawText(tPosText - QPoint(0, space * 8), QString("m_vDrawed:%1").arg(m_vDrawed.count()));
    pa.drawText(tPosText - QPoint(0, space * 9), QString("rtMoveTest(%1, %2, %3 * %4)").arg(rtCurMove.x()).arg(rtCurMove.y())
        .arg(rtCurMove.width()).arg(rtCurMove.height()));
    pa.drawText(tPosText - QPoint(0, space * 10), QString("m_step=>pos1(%1, %2)  pos2(%3 * %4) editPos(%5, %6)  rt(%7, %8, %9 * %10)  text:%11")
        .arg(m_step.pos1.x()).arg(m_step.pos1.y()) .arg(m_step.pos2.x()).arg(m_step.pos2.y())
        .arg(m_step.editPos.x()).arg(m_step.editPos.y())
        .arg(m_step.rt.x()).arg(m_step.rt.y()).arg(m_step.rt.width()).arg(m_step.rt.height())
        .arg(m_step.text));

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

	/*qDebug() << "【paintEvent】  :" << m_rtCalcu.scrnType << m_rtCalcu.getSelRect() << rtSel << m_rtCalcu.getSelRect() << "   " << m_rtCalcu.selEndPos << "  " << m_basePixmap << "  " << QRect();*/
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
    XLOG_DEBUG("BEGIN m_rtCalcu.scrnType[{}], event->pos({}, {})", int(m_rtCalcu.scrnType), event->globalPos().x(), event->globalPos().y());
	if (event->button() != Qt::LeftButton)
		return;

	setMouseTracking(false);
	if (m_rtCalcu.getSelRect().isEmpty() && m_rtCalcu.scrnType == ScrnType::Wait) {
		//m_rtCalcu.clear();
        
		m_rtCalcu.scrnType = ScrnType::Select;
        m_rtCalcu.pos1 = event->globalPos();
        m_rtCalcu.pos2 = event->globalPos();
	} else if (m_rtCalcu.scrnType == ScrnType::Draw) { 
        m_step.pos1 = event->globalPos();
        m_step.pos2 = event->globalPos(); 
       
        if (m_step.shape == DrawShape::Text) {
            QPoint perviousPos = m_step.editPos;   // 修改之前的显示坐标

            m_step.editPos = event->globalPos();
            m_step.rt = m_textEdit->rect();
            m_step.rt.setTopLeft(m_step.editPos);

            if (m_step.bDisplay) {  // 输入框显示中
                //m_step.editPos = event->globalPos(); // pos1、pos2 松开鼠标时候会重置，而editPos不会
                //m_step.rt = m_textEdit->rect();
                //m_step.rt.setTopLeft(m_step.editPos);
                //m_textEdit->move(m_step.editPos);

                if (!m_step.rt.contains(event->globalPos(), true)) {  // 编辑完成
                    m_step.bTextComplete = true;
                    m_step.bDisplay = false;

                    m_step.text = m_textEdit->toPlainText();
                    m_textEdit->clear();
                    
                    m_step.editPos = perviousPos;
                    m_vDrawed.push_back(m_step);  // 绘画文字为单独处理【暂时特例】
                    m_step.index = m_step.g_index++;
                } else {   // 编辑ing
                    m_step.bTextComplete = false;
                }
            } else {  // 输入框未显示
                m_step.bDisplay = true;
                m_textEdit->clear();
            }

            m_textEdit->move(mapFromGlobal(m_step.editPos));
            m_textEdit->setVisible(m_step.bDisplay);

            XLOG_DEBUG("m_textEdit是否显示[{}]  event->pos({}, {})  m_step.editPos({}, {})  perviousPos({}, {}) m_textEdit->rect({}, {}, {} * {}) m_textEdit->toPlainText[{}]  m_step.text[{}]"
                , m_textEdit->isVisible(), event->globalPos().x(), event->globalPos().y(), m_step.editPos.x(), m_step.editPos.y(), perviousPos.x(), perviousPos.y()
                , m_textEdit->rect().left(), m_textEdit->rect().top(), m_textEdit->rect().width(), m_textEdit->rect().height()
                , m_textEdit->toPlainText().toUtf8().data()
                , m_step.text.toUtf8().data());
        }



	} else {  // 则可能为移动、拉伸、等待状态
		m_rtCalcu.scrnType = updateScrnType(event->globalPos());
    }

	if (m_rtCalcu.scrnType == ScrnType::Move) {
		m_rtCalcu.pos1 = event->globalPos();
		m_rtCalcu.pos2 = event->globalPos();

        whichShape(); // 判定移动选中的已绘图形
	} else if (m_rtCalcu.scrnType == ScrnType::Stretch) {
		m_rtCalcu.pos1 = event->globalPos();
		m_rtCalcu.pos2 = event->globalPos();
	} 

    updateCursorShape(event->globalPos());
	update();

    XLOG_DEBUG("END m_rtCalcu.scrnType[{}], event->pos({}, {})", int(m_rtCalcu.scrnType), event->globalPos().x(), event->globalPos().y());
}

void ScreenShot::mouseMoveEvent(QMouseEvent *event)
{
    XLOG_DEBUG("BEGIN m_rtCalcu.scrnType[{}], event->pos({}, {})", int(m_rtCalcu.scrnType), event->globalPos().x(), event->globalPos().y());
//    if (event->button() != Qt::LeftButton)
//        return;

	// 此时为 Qt::NoButton
	if (m_rtCalcu.scrnType == ScrnType::Wait) {
        if (m_rtCalcu.bSmartMonitor)
            updateGetWindowsInfo();

	} else if (m_rtCalcu.scrnType == ScrnType::Select) {
        m_rtCalcu.pos2 = event->globalPos();

        m_rtCalcu.bSmartMonitor = false;
        //if (m_rtCalcu.pos1 != m_rtCalcu.pos2)
        //  不显示 TODO: 2022.02.10 再添加一个变量即可
            
        
	} else if (m_rtCalcu.scrnType == ScrnType::Move) {
		m_rtCalcu.pos2 = event->globalPos();
	} else if (m_rtCalcu.scrnType == ScrnType::Draw) {
        m_step.pos2 = event->globalPos();
        m_step.editPos = event->globalPos();

        m_step.custPath.append(event->globalPos());
        m_step.rt = RectCalcu::getRect(m_step.pos1, m_step.pos2);

	} else if (m_rtCalcu.scrnType == ScrnType::Stretch) {
		m_rtCalcu.pos2 = event->globalPos();
	}

    updateCursorShape(event->globalPos());
	update();

    XLOG_DEBUG("END m_rtCalcu.scrnType[{}], event->pos({}, {})", int(m_rtCalcu.scrnType), event->globalPos().x(), event->globalPos().y());
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *event)
{
    XLOG_DEBUG("BEGIN m_rtCalcu.scrnType[{}], event->pos({}, {})", int(m_rtCalcu.scrnType), event->globalPos().x(), event->globalPos().y());
    if (event->button() != Qt::LeftButton)
        return;

	if (m_rtCalcu.scrnType == ScrnType::Wait) {
	} else if (m_rtCalcu.scrnType == ScrnType::Select) {
		m_rtCalcu.pos2 = event->globalPos();

        if (m_rtCalcu.pos1 == m_rtCalcu.pos2) {  // 点击到一个点，视作智能检测窗口； 否则就是手动选择走下面逻辑
            m_rtCalcu.setRtSel(m_rtAtuoMonitor);
        }

        m_rtCalcu.bSmartMonitor = false; // 自动选择也结束

	} else if (m_rtCalcu.scrnType == ScrnType::Move) {
		m_rtCalcu.pos2 = event->globalPos();

        // 移动选中的图形
        if (m_pCurrShape) {
            m_pCurrShape->rt.translate(m_rtCalcu.pos2 - m_rtCalcu.pos1);
        }

	} else if (m_rtCalcu.scrnType == ScrnType::Draw) {
        m_step.pos2 = event->globalPos();
        m_step.custPath.append(event->globalPos());
        m_step.rt = RectCalcu::getRect(m_step.pos1, m_step.pos2);

        // DrawShape::Text  在按下时候单独处理 m_vDrawed.push_back
        if (m_step.shape != DrawShape::Text && m_step.shape != DrawShape::NoDraw) {
            if (isDrawShape(m_step)) {
                m_vDrawed.push_back(m_step); // TODO 2022.01.16 优化:  不必每次(无效得)点击，也都记录一次
                m_step.index = m_step.g_index++;
                m_step.clear();
            }
        }
        
	} else if (m_rtCalcu.scrnType == ScrnType::Stretch) {
		m_rtCalcu.pos2 = event->globalPos();
	}

	m_rtCalcu.calcurRsultOnce();
    //m_pCurrShape = nullptr; // 此计算依次结果之后

	if (m_rtCalcu.scrnType != ScrnType::Draw) {
		m_rtCalcu.scrnType = ScrnType::Wait;
		setMouseTracking(true);
	}

	update();

    XLOG_DEBUG("END m_rtCalcu.scrnType[{}], event->pos({}, {})", int(m_rtCalcu.scrnType), event->globalPos().x(), event->globalPos().y());
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
	
	//WinInfoWin::instance().getAllWinInfoRealTime();

    if (m_primaryScreen) {
        m_rtVirDesktop = m_primaryScreen->virtualGeometry();
    } else {
        m_rtVirDesktop = QApplication::desktop()->rect();

        for (const auto& it : m_screens) {
            if (m_rtVirDesktop.x() > it->geometry().x())
                m_rtVirDesktop.setX(it->geometry().x());
            if (m_rtVirDesktop.y() > it->geometry().y())
                m_rtVirDesktop.setY(it->geometry().y());
        }
    }

    m_vWholeScrn.clear();
    
    XLOG_INFO("---------------m_screens[] Info BEGIN----------------");
    for (const auto& scrn : m_screens) {
        XLOG_DEBUG("屏幕详细信息：index[{}]", m_screens.indexOf(scrn));
        XLOG_DEBUG("size({}, {})", scrn->size().width(), scrn->size().height());
        XLOG_DEBUG("geometry({}, {}, {} * {})", scrn->geometry().left(), scrn->geometry().top(), scrn->geometry().width(), scrn->geometry().height());
        XLOG_DEBUG("virtualGeometry({}, {}, {} * {})", scrn->virtualGeometry().left(), scrn->virtualGeometry().top(), scrn->virtualGeometry().width(), scrn->virtualGeometry().height());
        XLOG_DEBUG("m_rtDesktop({}, {}, {} * {})\n", m_rtVirDesktop.left(), m_rtVirDesktop.top(), m_rtVirDesktop.width(), m_rtVirDesktop.height());

        m_vWholeScrn.push_back(scrn->geometry());
        m_vWholeScrn.push_back(scrn->virtualGeometry());
    }
    XLOG_INFO("---------------m_screens[] Info END----------------");

    m_vWholeScrn.push_back(m_rtVirDesktop);

#ifdef Q_OS_WIN
    m_vec.clear();

    //WinInfoWin::instance().getAllWinInfoCache();

    //if (m_rtCalcu.bSmartMonitor)  // 存储所需要全部窗口信息
        //m_vec = WinInfoWin::instance().m_vWinInfo;

#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
#endif

    this->getScrnInfo();
    // 因 QWidget 启动后 事件执行顺序，sizeHint() -> showEvent() -> paintEvent()；故全屏 show() 之前先获取桌面截图
    getVirtualScreen();

    this->show();
}

// 屏幕详细参数
void ScreenShot::getScrnInfo()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect rtAvailableGeometry = desktopWidget->availableGeometry();   //获取可用桌面大小
	QRect rtScreen = desktopWidget->screenGeometry();  //获取设备屏幕大小

    XLOG_INFO("---------------QApplication::desktop() Info BEGIN----------------");
    XLOG_INFO("可用区域 availableGeometry({}, {}, {} * {})", rtAvailableGeometry.left(), rtAvailableGeometry.top(), rtAvailableGeometry.width(), rtAvailableGeometry.height());
    XLOG_INFO("屏幕区域（默认主屏） screenGeometry({}, {}, {} * {})", rtScreen.left(), rtScreen.top(), rtScreen.width(), rtScreen.height());
    XLOG_INFO("是否开启虚拟桌面 isVirtualDesktop: {}", desktopWidget->isVirtualDesktop());
    XLOG_INFO("---------------QApplication::desktop() Info END----------------");

    XLOG_INFO("---------------Screen Infomation BEGIN----------------");
    for (QScreen* it : m_screens) {
        XLOG_INFO("设备像素比 devicePixelRatio[{}]  制造商 manufacturer[{}]  名称 name[{}]", it->devicePixelRatio(), it->manufacturer().toUtf8().data(), it->name().toUtf8().data());
        XLOG_INFO("序号 serialNumber[{}]  刷新率 refreshRate[{}]  模式 model[{}]", it->serialNumber().toUtf8().data(), it->refreshRate(), it->model().toUtf8().data());

        QRect rtT = it->virtualGeometry();
        const auto rtPhysicalSize = it->physicalSize();
        const auto rtSize = it->physicalSize();
        XLOG_INFO("虚拟几何 virtualGeometry:({}, {}, {} * {})  缩放比 getScale[{}]", rtT.left(), rtT.top(), rtT.width(), rtT.height(), getScale(it));
        XLOG_INFO("物理几何 physicalSize:({} * {})  大小 size({} * {})", rtPhysicalSize.width(), rtPhysicalSize.height(), rtSize.width(), rtSize.height());
        XLOG_INFO("物理 DPI physicalDotsPerInch: {}  DPIX: {}  DPIY: {} ", it->physicalDotsPerInch(), it->physicalDotsPerInchX(), it->physicalDotsPerInchY());
        XLOG_INFO("逻辑 DPI logicalDotsPerInch: {}  DPIX: {}  DPIY: {}\n", it->logicalDotsPerInch(), it->logicalDotsPerInchX(), it->logicalDotsPerInchX());
    }
    XLOG_INFO("---------------Screen Infomation End----------------");
}

double ScreenShot::getDevicePixelRatio()
{
#ifdef Q_OS_MAC
    return 2;  // TODO 2022.03.11 无奈之举；使用 CMake MACOSX_BUNDLE 则返回的缩放比不正常
#else
    return m_primaryScreen->devicePixelRatio();
#endif

}

double ScreenShot::getDevicePixelRatio(QScreen * screen)
{
	if (!screen)
		return 0.0;
	else
        return screen->devicePixelRatio();
}

// 随着光标移动，更新获取桌面所有窗口信息
void ScreenShot::updateGetWindowsInfo()
{
#ifdef Q_OS_WIN
    Util::getRectFromCurrentPoint((HWND)winId(), m_rtAtuoMonitor);
#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
#endif
}

double ScreenShot::getScale(QScreen * screen)
{
// or defined(Q_WS_WIN) || defined(Q_WS_X11)
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)
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
    return scale;
#else
    return screen->logicalDotsPerInch() / 96.0;  //
#endif
}

bool ScreenShot::isSelBorder()
{
    if (m_pCurrShape)
        return false;
    else
        return true;
}

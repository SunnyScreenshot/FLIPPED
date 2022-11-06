﻿/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.09.29
 * Description:
 ******************************************************************/
#include "screenshot.h"
#include "../core/xlog.h"
#include "../platform/wininfo.h"
#include "../tool/pin/pinwidget.h"
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
#include <QLine>
#include <QToolButton>
#include <QPainterPath>
#include <QGuiApplication>
#include <QDir>
#include <QMouseEvent>
#include <QTimer>
#include <QWindow>
#include <QStandardPaths>
#include <QStringList>
#include <QDebug>

namespace Util {
    bool getRectFromCurrentPoint(WinID winId, QRect &outRect)
    {
        const QRect rt = WinInfo::instance().targWinRect(winId, false);

        outRect.setLeft(rt.left());
        outRect.setTop(rt.top());
        outRect.setRight(rt.right());
        outRect.setBottom(rt.bottom());
        return true;
    }
}

ScreenShot::ScreenShot(QWidget *parent)
	: QWidget(parent)
    , m_scal(XHelper::instance().getScale())
    , m_scrns(qGuiApp->screens())
    , m_priScrn(qGuiApp->primaryScreen())
    , m_curScrn(nullptr)
    , m_virGeom(m_priScrn->virtualGeometry())
	, m_currPixmap(nullptr)
	, m_rtCalcu(this)
    , m_bSmartWin(XHelper::instance().smartWindow())
    , m_bFirstSel(false)
    , m_bFirstPress(false)
    , m_pCurrShape(nullptr)
	, m_edit(new XTextWidget(this))
    , m_rtSmartWindow(0, 0, 0, 0)
    , m_barOrien(Qt::Horizontal)  // Horizontal | Vertical
    , m_selSizeTip(new SelectSize("", this))
    , m_lineWidthTip(new SelectSize("", this))
    , m_selBar(new SelectBar(m_barOrien, this))
    , m_paraBar(new ParameterBar(m_barOrien, this))
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

    setAttribute(Qt::WA_DeleteOnClose, true);
    //setAttribute(Qt::WA_QuitOnClose, false);

    m_edit->setTextColor(Qt::red);
    m_edit->setFont(m_step.font);
    m_edit->setVisible(false);

    // 注意显示器摆放的位置不相同~；最大化的可能异常修复
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags());  // | Qt::WindowStaysOnTopHint
    #ifdef _MYDEBUG
        setWindowFlag(Qt::WindowStaysOnTopHint, false); // 删除置顶
        m_virGeom = currentScreen(QCursor::pos())->geometry();
        if (m_scrns.size() == 1)
            m_virGeom.setWidth(m_virGeom.width() / 2);
    #endif

    setFixedSize(m_virGeom.size());
    move(m_virGeom.topLeft());
#else // Q_OS_MAC
//    setWindowFlags(Qt::Window);  // 不设置则 mac 下 devicePixelRatio: 1
    #ifdef _MYDEBUG
        m_virGeom = currentScreen(QCursor::pos())->geometry();
        if (m_scrns.size() == 1)
            m_virGeom.setWidth(m_virGeom.width() / 2);
        setFixedSize(m_virGeom.size());
    #else
//        showFullScreen();  // 并不是当前的屏幕大小
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);   // 窗口置顶 + 隐藏标题栏
        m_curScrn = qApp->screenAt(QCursor::pos());
        const QRect geom = m_curScrn->geometry();
        setFixedSize(geom.size());   // resize() cannot get the desired effect
    #endif

    move(geom.topLeft());
#endif

    setMouseTracking(true);
    m_rtCalcu.scrnType = ScrnType::Wait;

    // new refactor
    m_selSizeTip->setVisible(false);
    m_lineWidthTip->setVisible(false);
    m_lineWidthTip->setFixedSize(50, 50);
    m_lineWidthTip->move(15, 15);
    m_selBar->setVisible(false);
    m_paraBar->setVisible(false);
    connect(m_selBar, &SelectBar::sigEnableDraw, this, &ScreenShot::onEnableDraw);
    connect(m_selBar, &SelectBar::sigSelShape, this, &ScreenShot::onSelShape);
    connect(m_selBar, &SelectBar::sigRevocation, this, &ScreenShot::onRevocation);
    connect(m_selBar, &SelectBar::sigRenewal, this, &ScreenShot::onRenewal);
    connect(m_selBar, &SelectBar::sigPin, this, &ScreenShot::onPin);
    connect(m_selBar, &SelectBar::sigSave, this, &ScreenShot::onSave);
    connect(m_selBar, &SelectBar::sigCancel, this, &ScreenShot::onCancel);
    connect(m_selBar, &SelectBar::sigFinish, this, &ScreenShot::onFinish);
    connect(m_selBar, &SelectBar::sigInterruptEdit, this, &ScreenShot::onInterruptEdit);

    connect(m_paraBar, &ParameterBar::sigParaBtnId, this, &ScreenShot::onParaBtnId);
    connect(m_paraBar, &ParameterBar::sigSelColor, this, &ScreenShot::onSelColor);

    connect(m_selBar, &SelectBar::sigEnableDraw, m_paraBar, &ParameterBar::onEnableDraw);
    connect(m_selBar, &SelectBar::sigSelShape, m_paraBar, &ParameterBar::onSelShape);
    connect(this, &ScreenShot::sigClearScreen, this, &ScreenShot::onClearScreen);
    connect(this, &ScreenShot::sigLineWidthChange, this, &ScreenShot::onLineWidthChange);
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
        return ScrnType::Wait;
    }
}

void ScreenShot::updateCursorShape(const QPoint pos)
{
    if (m_rtCalcu.scrnType == ScrnType::Draw)
        return;

    CursorArea cursArea = m_rtCalcu.getCursorArea(pos, true);
    if (m_rtCalcu.scrnType == ScrnType::Move) {
    } else if (m_rtCalcu.scrnType == ScrnType::Select) {
        setCursor(Qt::CrossCursor);
    } else if (m_rtCalcu.scrnType == ScrnType::Stretch) {
    } else if (m_rtCalcu.scrnType == ScrnType::Wait) {
        if (!XHelper::instance().smartWindow())
            return;

        if (cursArea == CursorArea::External) {

            (m_rtSmartWindow.contains(pos, false) && !m_bFirstSel) || m_rtCalcu.getSelRect().contains(pos, true) ?
                setCursor(Qt::ArrowCursor) : setCursor(Qt::ForbiddenCursor);
        } else if (cursArea == CursorArea::Internal){
            setCursor(Qt::SizeAllCursor);
            // 已修复：光标移动到绘画工具栏时，光标显示异常，在里面的事件过滤器中修复此出逻辑纰漏。光标进入 tbBar 后，
            // 光标坐标也【不会开始改变】，至少在 ScreenShot。
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
#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
#endif

    m_bFirstPress = false;
    m_specifyRts.clear();

	//m_screens、m_primaryScreen 还保留
	delete m_currPixmap;
	m_currPixmap = nullptr;
    m_pCurrShape = nullptr;

	m_vDrawed.clear();
	m_vDrawUndo.clear();
	m_rtCalcu.clear();

    m_bFirstSel = false;
    m_selSizeTip->setVisible(false);
    m_lineWidthTip->setVisible(false);

    m_selBar->setVisible(false);
    m_paraBar->setVisible(false);
    m_step.clear();

    m_edit->deleteLater();

    m_rtSmartWindow = QRect();
    if (m_rtCalcu.scrnType == ScrnType::Wait) // 状态重置
        setMouseTracking(true);
};

void ScreenShot::onLineEndsChange(LineEnds ends)
{
	m_step.lineEnds = ends;
}

void ScreenShot::onLineDasheChange(Qt::PenStyle dashes)
{
    m_step.pen.setStyle(dashes);
}

void ScreenShot::onEnableDraw(bool enable)
{
    if (enable) {
        m_rtCalcu.scrnType = ScrnType::Draw;
        setMouseTracking(false);  // Fix: 鼠标移动中会被自动绘画矩形，副作用绘画状态的光标不完美了(选中框内外的光标被固定了)，严格不算 bug，一种外观特效
    //    qInfo()<<"--------------onDrawStart"<<m_rtCalcu.scrnType;
    } else {
        m_rtCalcu.scrnType = ScrnType::Wait;
        setMouseTracking(true);  // 等待状态开启鼠标跟踪
    //    qInfo()<<"--------------onDrawEnd"<<m_rtCalcu.scrnType;
    }
}

void ScreenShot::onSelShape(DrawShape shape, bool checked)
{
    if (!checked)
        return;

    m_step.shape = shape;
    const bool bRect = shape == DrawShape::Rectangles;
    const bool bEllipses = shape == DrawShape::Ellipses;
    const bool bMosaics = shape == DrawShape::Mosaics;
    const bool bLine = shape == DrawShape::LineWidth;
    const bool bPen = shape == DrawShape::Pen;
    const bool bArrows = shape == DrawShape::Arrows;
    const bool bText = shape == DrawShape::Text;
    const bool bSeriNum = shape == DrawShape::SerialNumber;

    if (bRect || bEllipses || bMosaics) {
        m_step.bStyele = property(std::to_string((int)shape).c_str()).value<int>();
    } else if (bLine) {
        //int idx = property(std::to_string((int)shape).c_str()).value<int>();
        //int penWidth = 0;

        //if (idx == 0)
        //    penWidth = 1;
        //else if (idx == 1)
        //    penWidth = 2;
        //else if (idx == 2)
        //    penWidth = 4;
    } else if (bPen) {
    } else if (bArrows) {
        m_step.bStyele = property(std::to_string((int)shape).c_str()).value<int>();
    } else if (bText) {
    } else if (bSeriNum) {
    } else {
        //XLOG_INFO
    }
    //qDebug() << "--------@onDrawShape:" << int(m_step.shape);
}

void ScreenShot::onRevocation()
{
    if (m_vDrawed.count() <= 0)
        return;

    m_vDrawUndo.push_back(*(m_vDrawed.end() - 1));
    m_vDrawed.pop_back();
    qDebug() << "---->m_vDrawRevoke:" << m_vDrawUndo.count() << "    m_vDraw:" << m_vDrawed.count();
    update();
}

void ScreenShot::onRenewal()
{
    if (m_vDrawUndo.count() <= 0)
        return;

    m_vDrawed.push_back(*(m_vDrawUndo.end() - 1));
    m_vDrawUndo.pop_back();
    qDebug() << "---->m_vDrawRevoke:" << m_vDrawUndo.count() << "    m_vDraw:" << m_vDrawed.count();
    update();
}

void ScreenShot::onPin()
{
    if (m_savePixmap.isNull())
        return;

    auto pin = new PinWidget(m_savePixmap, m_savePixmap.rect(), nullptr);   // 使用 nullptr，不会泄露
    pin->move(mapToGlobal(m_rtCalcu.getSelRect().topLeft()));
    pin->show();

    clearnAndClose();
}

void ScreenShot::onSave()
{
    if (drawToCurrPixmap()) {
        if (XHelper::instance().autoCpoyClip())
            QApplication::clipboard()->setPixmap(m_savePixmap);

        const QString imageName = XHelper::instance().formatToName();
        QString fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files(*.*)"));
        QString fileNmae = QFileDialog::getSaveFileName(this, tr("Save Files"), imageName, fileter);
        if (fileNmae.isEmpty())
            return;

        QTime startTime = QTime::currentTime();
        m_savePixmap.save(fileNmae, nullptr, XHelper::instance().imgQuailty());  // 绘画在 m_savePixmap 中，若在 m_savePixmap 会有 selRect 的左上角的点的偏移
        QTime stopTime = QTime::currentTime();
        int elapsed = startTime.msecsTo(stopTime);

#ifdef QT_DEBUG
        qDebug() << "save m_savePixmap tim =" << elapsed << "ms" << m_savePixmap.size();
        //m_currPixmap->save("a2.png");

        // auto save pixmap
        const QString path = XHelper::instance().formatToName(XHelper::instance().path(toAutoSavePath).trimmed());
        if (!path.isEmpty()) {
            m_savePixmap.save(path + QDir::separator() + imageName);
        }
#else
#endif
    }

    clearnAndClose();
}

void ScreenShot::clearnAndClose()
{
    emit sigClearScreen();
    close();
}

void ScreenShot::onCancel()
{
    clearnAndClose();
}

void ScreenShot::onFinish()
{
    if (drawToCurrPixmap() && !m_savePixmap.isNull())
        QApplication::clipboard()->setPixmap(m_savePixmap);
    clearnAndClose();
}

void ScreenShot::onInterruptEdit(const QPoint& pos)
{
    QMouseEvent mousePressEvent(QEvent::MouseButtonPress, mapFromGlobal(pos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &mousePressEvent);
}

void ScreenShot::onParaBtnId(DrawShape shape, QToolButton* tb)
{
    if (!tb)
        return;

    const int idx = tb->objectName().right(1).toInt();
    const bool bRect = shape == DrawShape::Rectangles;
    const bool bEllipses = shape == DrawShape::Ellipses;
    const bool bMosaics = shape == DrawShape::Mosaics;
    const bool bLine = shape == DrawShape::LineWidth;
    const bool bPen = shape == DrawShape::Pen;
    const bool bArrows = shape == DrawShape::Arrows;
    const bool bText = shape == DrawShape::Text;
    const bool bSeriNum = shape == DrawShape::SerialNumber;

    if (bRect || bEllipses || bMosaics) {
        setProperty(std::to_string((int)shape).c_str(), idx);
        m_step.bStyele = idx;

    } else if (bLine) {
        int penWidth = 0;

        if (idx == 0)
            penWidth = 1;
        else if (idx == 1)
            penWidth = 4;
        else if (idx == 2)
            penWidth = 10;

        m_step.pen.setWidth(penWidth);
        emit sigLineWidthChange(penWidth);
    } else if (bPen) {
    } else if (bArrows) {
        setProperty(std::to_string((int)shape).c_str(), idx);
        m_step.bStyele = idx;
    } else if (bText) {
    } else if (bSeriNum) {
    } else {
    }
}

void ScreenShot::onLineWidthChange(int width)
{
    static QFont font(m_step.font);
    font.setPointSize(14);
    m_lineWidthTip->setText(QString::number(width));
    m_lineWidthTip->setAlignment(Qt::AlignCenter);
    m_lineWidthTip->setFont(font);
    m_lineWidthTip->move(mapFromGlobal(currentScreen()->geometry().topLeft()));
    m_lineWidthTip->raise();
    m_lineWidthTip->setVisible(true);

    QTimer::singleShot(4000, this, [&]() { m_lineWidthTip->setVisible(false); });
}

void ScreenShot::onSelColor(QColor col)
{
    m_step.pen.setColor(col);

    if (m_step.shape == DrawShape::Text) {
        m_edit->setTextColor(col);
    }
}

// 获取虚拟屏幕截图
QPixmap* ScreenShot::getVirScrnPixmap()
{
    if (!m_currPixmap) {
        const QScreen* curScrn = currentScreen(QCursor::pos());
        const QRect geom = curScrn->geometry();

#if defined(Q_OS_MAC)
        m_currPixmap = new QPixmap(m_priScrn->grabWindow(qApp->desktop()->winId(), geom.x(), geom.y(), geom.width(), geom.height()));
#else
    #ifdef _MYDEBUG
        m_currPixmap = new QPixmap(m_priScrn->grabWindow(qApp->desktop()->winId(), geom.x(), geom.y(), geom.width(), geom.height()));
    #else
        // 多屏的矩形取并集
        m_currPixmap = new QPixmap(m_priScrn->grabWindow(qApp->desktop()->winId(), m_virGeom.x(), m_virGeom.y(), m_virGeom.width(), m_virGeom.height()));
    #endif
#endif
    }

//    m_currPixmap->save(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first() + "/m_currPixmap_123456.png");
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
    const QRect rtAppVirDesktop(QPoint(0, 0), m_virGeom.size());
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

void ScreenShot::drawBorderPS(QPainter& pa, QRect rt, bool isRound)
{
    pa.save();
    pa.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setWidth(XHelper::instance().borderWidth() * m_scal + SELECT_ASSIST_RECT_PEN_WIDTH);
    pen.setColor(XHelper::instance().borderColor());  //
    pa.setPen(pen);
    pa.setBrush(Qt::NoBrush);

    int x1 = rt.left();
    int y1 = rt.top();
    int x2 = rt.right();
    int y2 = rt.bottom();

    const int penWidth = pen.width();
    const int penAssWidth = SELECT_ASSIST_RECT_WIDTH * m_scal;

    // hor 且补齐交叉角落的空缺的那一块
    QLine l1(QPoint(x1 - penWidth / 2, y1), QPoint(x1 + penAssWidth, y1));
    QLine l2(QPoint(x1 - penWidth / 2, y2), QPoint(x1 + penAssWidth, y2));
    QLine l3(QPoint(x2 + penWidth / 2, y1), QPoint(x2 - penAssWidth, y1));
    QLine l4(QPoint(x2 + penWidth / 2, y2), QPoint(x2 - penAssWidth, y2));

    // ver
    QLine l5(QPoint(x1, y1), QPoint(x1, y1 + penAssWidth));
    QLine l6(QPoint(x1, y2), QPoint(x1, y2 - penAssWidth));
    QLine l7(QPoint(x2, y1), QPoint(x2, y1 + penAssWidth));
    QLine l8(QPoint(x2, y2), QPoint(x2, y2 - penAssWidth));

    pa.drawLine(l1.translated(QPoint(0, -penWidth / 2)));
    pa.drawLine(l2.translated(QPoint(0, penWidth / 2)));
    pa.drawLine(l3.translated(QPoint(0, -penWidth / 2)));
    pa.drawLine(l4.translated(QPoint(0, penWidth / 2)));
    pa.drawLine(l5.translated(QPoint(-penWidth / 2, 0)));
    pa.drawLine(l6.translated(QPoint(-penWidth / 2, 0)));
    pa.drawLine(l7.translated(QPoint(penWidth / 2, 0)));
    pa.drawLine(l8.translated(QPoint(penWidth / 2, 0)));

    pen.setWidth(XHelper::instance().borderWidth() * m_scal);
    pa.setPen(pen);
    pa.drawRect(rt);
    pa.restore();
}

// 绘画当前类型的一个图案形状; isUseOwn 为 true 使用自带的画笔等；true 使用上一个环境的
void ScreenShot::drawStep(QPainter& pa, XDrawStep& step, bool isUseEnvContext)
{
    if (DrawShape::NoDraw == step.shape
        || (m_pCurrShape == &step && m_rtCalcu.scrnType == ScrnType::Move)) // 选中图形的处于移动状态，跳过这不绘画
        return;

    //pa.save();
    //pa.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //pa.setRenderHint(QPainter::HighQualityAntialiasing, true);
    pa.setRenderHint(QPainter::Antialiasing, true);

    if (!isUseEnvContext) {
        if (step.bStyele == 0){
            //step.brush.setColor(Qt::NoBrush);
            step.brush.setStyle(Qt::NoBrush);
        } else if (step.bStyele == 1) {
            step.brush.setColor(step.pen.color());
            step.brush.setStyle(Qt::SolidPattern);
        }

        pa.setPen(step.pen);
        pa.setBrush(step.brush);
        pa.setFont(step.font);
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

        if (step.bStyele == 0) {
            pa.drawLine(step.p1, step.p2);
        } else if (step.bStyele == 1 || step.bStyele == 2) {
            pa.drawLine(XHelper::instance().GetShorterLine(step.p1, step.p2));  // 0, 1, 2
            QPainterPath arrowPath = XHelper::instance().GetArrowHead(step.p1, step.p2);
            pa.fillPath(arrowPath, step.brush);
        }
        break;
    }
    case DrawShape::Pen: {
		pa.drawPolyline(step.custPath.data(), step.custPath.size());
		break;
	}
    case DrawShape::Text: {
        // Ref: http://qtdebug.com/qtbook-paint-text
        //      https://doc.qt.io/qt-5/qpainter.html#drawText-5
        if (!step.text.isEmpty() && m_edit) {
            const QFontMetrics fm(step.font);
            const int val = 5;
            int flags = Qt::TextWordWrap;
            QRect textBoundingRect = fm.boundingRect(QRect(0, 0, m_virGeom.width(), 0), flags, step.text);

            pa.setFont(step.font);
            pa.drawText(QRect(step.rt.topLeft() + QPoint(val, val), textBoundingRect.size()), flags, step.text);

            //const QMargins& margins = m_edit->contentsMargins();
            //QPoint pos(step.rt.topLeft() + QPoint(margins.left() + val, margins.top() + val + fm.ascent()));    // Offset to get the visual "right"
            //pa.drawText(pos, step.text);
        }
        break;
    }
    case DrawShape::Mosaics: {
		if (!m_currPixmap || step.rt.isEmpty())  // 优化，删除就很明显
			return;

        auto rt = QRect(step.rt.topLeft() * getDevicePixelRatio(), step.rt.size() * getDevicePixelRatio());
        QPixmap mosaicPixmap = m_currPixmap->copy(rt);
        mosaicPixmap.save(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first() + "/mosaicPixmap_123456.png");
        if (step.bStyele == 0) {
            const QPixmap* pix = XHelper::instance().SetMosaicSmooth(&mosaicPixmap, step.mscPx);
            pa.drawPixmap(step.rt, *pix);
		} else if (step.bStyele == 1) {
            const QImage img = XHelper::instance().SetMosaicPixlelated(&mosaicPixmap, step.mscPx);
            pa.drawImage(step.rt, img);
		}

        break;
    }
    default:
        break;
    }

    //pa.restore();
}

bool ScreenShot::isDrawShape(XDrawStep& step)
{
    if (step.p1 == step.p2)
        return false;

    //if (step.shape == DrawShape::Rectangles) {
    //} else if (step.shape == DrawShape::Ellipses) {

    //} else if (step.shape == DrawShape::Pen) {

    //} else if (step.shape == DrawShape::Arrows) {

    //} else if (step.shape == DrawShape::Mosaics) {

    //} else if (step.shape == DrawShape::Text || step.shape == DrawShape::NoDraw) {
    //    return false; // 特殊处理，或者不处理
    //} else {
    //    return true; // 避免警告
    //}

    return true;
}

const QVector<QPoint> ScreenShot::drawBarPosition(Qt::Orientation orien /*= Qt:: Horizonta*/, ToolBarOffset offset /*= ToolBarOffset::TBO_Middle*/)
{
    QVector<QPoint> vec;
    if (!m_selBar || !m_paraBar)
        return vec;

    const int space = B_SPACEING;
    int selBarHeight = m_selBar->height();
    int paraBarHeight = m_paraBar->height();

    if (orien == Qt::Vertical) {
        selBarHeight = m_selBar->width();
        paraBarHeight = m_paraBar->width();
    }

    const int sBarHeight = space + selBarHeight;
    const int pBarHeight = space + paraBarHeight;
    const QRect rtSel(m_rtCalcu.getSelRect());
    const int barMaxTop = rtSel.top() - sBarHeight;
    const int barMaxBottom = rtSel.bottom() + sBarHeight;
    const int barMaxLeft = rtSel.left() - sBarHeight;
    const int barMaxRight = rtSel.right() + sBarHeight;

    const QScreen* curScrn = currentScreen(rtSel.bottomRight());
    if (!curScrn)
        curScrn = qGuiApp->screenAt(rtSel.topRight());
    if (!curScrn)
        curScrn = qGuiApp->screenAt(QCursor::pos());

    QRect rtScrn = curScrn->geometry();
    QPoint p1;  // selBar
    QPoint p2;  // paraBar
    if (orien == Qt::Horizontal) {
        p1 = QPoint(rtSel.center().x() - m_selBar->width() / 2, rtSel.bottom() + space); // 默认底部中间
        p2 = QPoint(p1.x(), p1.y() + space + selBarHeight);

        if (offset == ToolBarOffset::TBO_Left)
            p1.setX(rtSel.left());
        else if (offset == ToolBarOffset::TBO_Right)
            p1.setX(rtSel.right() - m_selBar->width());

        int topLimit = qMax(m_virGeom.top(), rtScrn.top());
        int bottomLimit = qMin(m_virGeom.bottom(), rtScrn.bottom());

        if (barMaxTop > topLimit) { // 选中框，上未触顶
            if (barMaxBottom > bottomLimit) { // 底部触顶
                p1.setY(rtSel.top() - sBarHeight);
                p2.setY(p1.y() - pBarHeight);
            }

            if (m_paraBar->isVisible()) {
                if (barMaxBottom + pBarHeight > bottomLimit) {

                    if (barMaxTop - pBarHeight < topLimit) {
                        p1.setY(rtSel.top() + space);
                        p2.setY(rtSel.top() + sBarHeight + space);
                    } else {
                        p1.setY(rtSel.top() - sBarHeight);
                        p2.setY(p1.y() - pBarHeight);
                    }
                } else {
                    p1.setY(rtSel.bottom() + space);
                }
            }

        } else {
            if (barMaxBottom > bottomLimit
                || (m_paraBar->isVisible() && barMaxBottom + pBarHeight > bottomLimit)) { // 底部触顶
                p1.setY(rtSel.top() + space);
                p2.setY(rtSel.top() + sBarHeight + space);
            }
        }

    } else {

        p1 = QPoint(rtSel.left() - (selBarHeight + space), rtSel.center().y() - m_selBar->height() / 2);         // 默认左侧中间
        p2 = QPoint(p1.x() - space * 2 - selBarHeight, p1.y());
        if (offset == ToolBarOffset::TBO_Top)
            p1.setY(rtSel.top());
        else if (offset == ToolBarOffset::TBO_Bottom)
            p1.setY(rtSel.bottom() - m_selBar->height());

        int leftLimit = qMax(m_virGeom.left(), rtScrn.left());
        int rightLimit = qMin(m_virGeom.right(), rtScrn.right());

        if (barMaxLeft > leftLimit) { // 选中框左边未触顶
            //if (barMaxRight > rightLimit) // 右侧触顶
            //    basePos.setY(rtSel.bottom() - barHeight);
        } else {
            p1.setX(rtSel.right() + space);
            p2.setX(p1.x() + space + selBarHeight);

            if (barMaxRight > rightLimit) { // 右侧触顶
                p1.setX(rtSel.left() + space);
                p2.setX(p1.x() + space + selBarHeight);
            }
        }

        p2.setY(p1.y());
    }

    vec << p1 << p2;
    return vec;
}

const QPoint ScreenShot::drawSelSizeTip(const QRect& rt)
{
    const QPoint pos = mapToGlobal(rt.topLeft());
    QString str = QString("[%1, %2, %3 * %4]").arg(pos.x()).arg(pos.y()).arg(rt.width()).arg(rt.height());
    if (str.compare(m_selSizeTip->text()) != 0) {
        emit m_selSizeTip->sigTextChanged(str);
        m_selSizeTip->setText(str);
    }
    
    return QPoint(rt.left(), rt.top() - m_selSizeTip->height() - SS_SPACE_TO_SELECTRECT);
}

void ScreenShot::selectedShapeMove(QPainter& pa)
{
    QRect rtCurMove;
    if (m_pCurrShape && (m_rtCalcu.scrnType == ScrnType::Wait || m_rtCalcu.scrnType == ScrnType::Move)) {
        pa.save();

        rtCurMove = m_pCurrShape->rt.translated(m_rtCalcu.pos2 - m_rtCalcu.pos1);
        pa.setRenderHint(QPainter::Antialiasing, false);
        pa.setBrush(Qt::NoBrush);
        QPen penWhite(QColor(255, 255, 255, 1 * 255), 1);
        penWhite.setStyle(Qt::CustomDashLine);
        penWhite.setDashOffset(0);
        penWhite.setDashPattern(QVector<qreal>() << 4 * ScreenShot::getScale() << 4 * ScreenShot::getScale());
        penWhite.setCapStyle(Qt::FlatCap);
        pa.setPen(penWhite);

        const int r = 3;
        QPen penBlack(penWhite);
        penBlack.setColor(QColor(0, 0, 0, 1 * 255));
        penBlack.setDashOffset(4);

        if (m_pCurrShape->shape == DrawShape::Rectangles) {
            pa.setBrush(m_pCurrShape->brush);
            pa.setPen(m_pCurrShape->pen);
            pa.drawRect(rtCurMove);

            pa.setBrush(Qt::NoBrush);
            pa.setPen(penWhite);
            pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));
            pa.setPen(penBlack);
            pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));
        } else if (m_pCurrShape->shape == DrawShape::Ellipses) {
            pa.setBrush(m_pCurrShape->brush);
            pa.setPen(m_pCurrShape->pen);
            pa.drawEllipse(rtCurMove.center(), rtCurMove.width() / 2, rtCurMove.height() / 2);

            pa.setBrush(Qt::NoBrush);
            pa.setPen(penWhite);
            pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));
            pa.setPen(penBlack);
            pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));

            //pa.setBrush(Qt::NoBrush);
            //pa.setPen(penWhite);

            //// 生成可填充的轮廓
            //QPainterPathStroker stroker;
            //stroker.setCapStyle(Qt::RoundCap);      // 端点风格
            //stroker.setJoinStyle(Qt::RoundJoin);    // 连接样式
            //stroker.setDashPattern(Qt::DashLine);   // 虚线图案
            //stroker.setWidth(penWhite.width());     // 宽度

            //// 生成一个新路径（可填充区域），表示原始路径 path 的轮廓
            //QPainterPath outlinePath = stroker.createStroke(path);


            //pa.drawEllipse(rtCurMove.center(), rtCurMove.width() / 2, rtCurMove.height() / 2);
            //pa.setPen(penBlack);
            //pa.drawEllipse(rtCurMove.center(), rtCurMove.width() / 2, rtCurMove.height() / 2);
        }

        pa.restore();
    }
}
void ScreenShot::whichShape()
{
    bool bChecked = false; // 点击是否选中
    const int r = 4;
    QPoint pos(QCursor::pos());
    for (auto it = m_vDrawed.rbegin(); it != m_vDrawed.rend(); ++it) {
        const auto& rt = it->rt;
        const QRect& rtOut = rt.adjusted(-r, -r, r, r);
        const QRect& rtIn = rt.adjusted(r, r, -r, -r);

        QPainterPath path;
        path.setFillRule(Qt::OddEvenFill);
        if (it->shape == DrawShape::Rectangles) {
            path.addEllipse(rtOut);
            if (it->bStyele == 0)
                path.addEllipse(rtIn);
        } else if (it->shape == DrawShape::Ellipses) {

            path.addEllipse(rtOut.center(), rtOut.width() / 2, rtOut.height() / 2);
            if (it->bStyele == 0)
                path.addEllipse(rtIn.center(), rtIn.width() / 2, rtIn.height() / 2);
        } else if (it->shape == DrawShape::Arrows) {
            // TODO 2022.07.12: 倾斜的矩形 + 三角形； 旋转坐标轴后绘画？
        }

        if (path.contains(pos)) {
            m_pCurrShape = &(*it);
            bChecked = true;
            return;
        }
    }

    m_pCurrShape = nullptr;  // 没有选中任何一个
}

void ScreenShot::savePixmap(bool quickSave /*= true*/, bool autoSave /*= true*/)
{
    if (quickSave) {
        //auto path = XHelper::instance().path(toQuickSavePath);
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

void ScreenShot::showAllDrawedShape(QPainter& pa)
{
    QPoint posText(0, 100);
    const int space = 15;
//    QRect m_rtCalcu_selRect(m_rtCalcu.getSelRect());

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
    if (m_rtCalcu.scrnType == ScrnType::Draw)
        setCursor(Qt::CrossCursor);

    // 原始图案
    QPainter pa(this); // 改为始终从 (0, 0) 开始绘画
    pa.setBrush(Qt::NoBrush);
    pa.setPen(Qt::NoPen);
    if (m_currPixmap)
        pa.drawPixmap(QPoint(0, 0), *m_currPixmap);

    // 选中矩形图片
    QRect rtSel(m_rtCalcu.getSelRect());   // 移动选中矩形
    if (m_bSmartWin)
        rtSel = m_rtSmartWindow;
    QRect shotGeom(mapFromGlobal(m_virGeom.topLeft()), m_virGeom.size()); // 修复为相对窗口的
    #ifdef Q_OS_MAC
        shotGeom = QRect(mapFromGlobal(m_curScrn->geometry().topLeft()), m_curScrn->geometry().size());
    #endif
     m_rtCalcu.limitBound(rtSel, shotGeom); // 修复边界时图片被拉伸
    if (rtSel.width() > 0 && rtSel.height() > 0) {
        double devPixRatio = getDevicePixelRatio(m_curScrn);
        m_savePixmap = m_currPixmap->copy(QRect(rtSel.topLeft() * devPixRatio, rtSel.size() * devPixRatio));  // NOTE: devicePixelRatio（macox = 2）
        pa.drawPixmap(rtSel, m_savePixmap);

        // 放大镜实现
        //QSize tSize(100, 100);
        //auto& mousePos = QCursor::pos();
        ////QRect rtMagnifying(QPoint(mousePos.x() - tSize.width() / 2, mousePos.y() - tSize.height() / 2), tSize);
        //QRect rtPick(mousePos * getDevicePixelRatio(), tSize * getDevicePixelRatio());
        //pa.drawPixmap(mousePos + QPoint(100, 100), m_currPixmap->copy(rtPick).scaled(tSize * 4, Qt::KeepAspectRatio)); // 放大 4 倍

        // m_savePixmap 和 m_currPixmap 的地址没有改变，但前者的 cacheKey 总在变化???
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

    // drawWinInfo(pa);
    selectedShapeMove(pa);
    drawMaskLayer(shotGeom, rtSel, pa);
    drawBorder(rtSel, pa);
    drawCrosshair(pa);
    drawToolBar();
    pen.setColor(Qt::white);
    pa.setPen(pen);

    showDebugInfo(pa, rtSel);
}

void ScreenShot::showDebugInfo(QPainter& pa, QRect& rtSel)
{
#ifdef _MYDEBUG
    // 调试的实时数据
    QFont font;//(font());
    font.setPointSize(12);  // 默认大小为 9
    pa.setFont(font);
    const int space = font.pointSize() * 2.5;

    QPoint tTopLeft;
    tTopLeft.setX(m_priScrn->geometry().x());
    tTopLeft.setY(m_priScrn->size().height());

    QPoint tPosText(tTopLeft.x(), tTopLeft.y() - 5 * space);
    QRect m_rtCalcu_selRect(m_rtCalcu.getSelRect());

    pa.drawText(tPosText - QPoint(0, space * -1), QString("Wait(0) Select(1) Move(2) Draw(3) Stretch(4)"));
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
    pa.drawText(tPosText - QPoint(0, space * 6), QString("m_rtAtuoMonitor: (%1, %2, %3 * %4)")
        .arg(m_rtSmartWindow.x()).arg(m_rtSmartWindow.y()).arg(m_rtSmartWindow.width()).arg(m_rtSmartWindow.height()));
    pa.drawText(tPosText - QPoint(0, space * 7), QString("XHelper::instance().smartWindow(): %1")
        .arg(XHelper::instance().smartWindow()));
    pa.drawText(tPosText - QPoint(0, space * 8), QString("m_vDrawed:%1").arg(m_vDrawed.count()));
    if (m_pCurrShape) {
        QRect rtCurMove = m_pCurrShape->rt.translated(m_rtCalcu.pos2 - m_rtCalcu.pos1);
        pa.drawText(tPosText - QPoint(0, space * 9), QString("rtMoveTest(%1, %2, %3 * %4)").arg(rtCurMove.x()).arg(rtCurMove.y())
            .arg(rtCurMove.width()).arg(rtCurMove.height()));
    }
    pa.drawText(tPosText - QPoint(0, space * 10), QString("m_step=>pos1(%1, %2)  pos2(%3 * %4)  rt(%5, %6, %7 * %8)  text:%9")
        .arg(m_step.p1.x()).arg(m_step.p1.y()).arg(m_step.p2.x()).arg(m_step.p2.y())
        .arg(m_step.rt.x()).arg(m_step.rt.y()).arg(m_step.rt.width()).arg(m_step.rt.height())
        .arg(m_step.text));

    const int tSpace = 10;
    const int barHeight = m_selBar->height();
    //const QRect rtSel(m_rtCalcu.getSelRect());
    QPoint topLeft(rtSel.right() - m_selBar->width(), rtSel.bottom() + tSpace);
    const int barMaxTop = rtSel.top() - tSpace - barHeight;
    const int barMaxBottom = rtSel.bottom() + tSpace + barHeight;

    QDesktopWidget* desktop = QApplication::desktop();  // 获取桌面的窗体对象
    //QRect rtScrn = desktop->screen(desktop->screenNumber(rtSel.bottomRight()))->geometry();  // geometry 则左上角坐标非 0，0
    QRect rtScrn = m_scrns.at(desktop->screenNumber(rtSel.bottomRight()))->geometry();
    // QRect rtScrn = currentScreen(rtSel.bottomRight())->geometry(); // 使用此替换有个 bug， 不在时候返回空
    int topLimit = qMax(m_virGeom.top(), rtScrn.top());
    int bottomLimit = qMin(m_virGeom.bottom(), rtScrn.bottom());

    pa.drawText(tPosText - QPoint(0, space * 11), QString("barMaxTop:%1   barMaxBottom:%2  m_rtVirDesktop 左上右下(%3, %4, %5 * %6)")
        .arg(barMaxTop).arg(barMaxBottom).arg(m_virGeom.left()).arg(m_virGeom.top()).arg(m_virGeom.right()).arg(m_virGeom.bottom()));
    pa.drawText(tPosText - QPoint(0, space * 12), QString("rtScrn 左上右下(%1, %2, %3 * %4) topLimit:%5  bottomLimit:%6")
        .arg(rtScrn.left()).arg(rtScrn.top()).arg(rtScrn.right()).arg(rtScrn.bottom()).arg(topLimit).arg(bottomLimit));
#endif

#if 0
    QRect rtOuter = m_rtCalcu.getExteRect(rtSel);
    QRect rtInner = m_rtCalcu.getInteRect(rtSel);
    int interval = (rtOuter.height() - rtInner.height()) / 2;

    QRect rtLeft(rtOuter.left(), rtInner.top(), interval, rtInner.height());
    QRect rtTop(rtInner.left(), rtOuter.top(), rtInner.width(), interval);
    QRect rtRight(rtInner.right(), rtInner.top(), interval, rtInner.height());
    QRect rtBottom(rtInner.left(), rtInner.bottom(), rtInner.width(), interval);
    QRect rtTopLeft(rtOuter.left(), rtOuter.top(), interval, interval);
    QRect rtTopRight(rtInner.right(), rtOuter.top(), interval, interval);
    QRect rtBottomLeft(rtOuter.left(), rtInner.bottom(), interval, interval);
    QRect rtBottomRight(rtInner.right(), rtInner.bottom(), interval, interval);

    //pa.setBrush(Qt::blue);

    pa.setBrush(Qt::NoBrush);
    pa.setPen(Qt::blue);
    pa.drawRect(rtLeft);
    pa.drawRect(rtTop);
    pa.drawRect(rtRight);
    pa.drawRect(rtBottom);
    //pa.setBrush(Qt::yellow);
    pa.setPen(Qt::white);
    pa.drawRect(rtTopLeft);
    pa.drawRect(rtTopRight);
    pa.drawRect(rtBottomLeft);
    pa.drawRect(rtBottomRight);

    /*qDebug() << "【paintEvent】  :" << m_rtCalcu.scrnType << m_rtCalcu.getSelRect() << rtSel << m_rtCalcu.getSelRect() << "   " << m_rtCalcu.selEndPos << "  " << m_basePixmap << "  " << QRect();*/
    //<< "外部矩形：" << rtOuter << "内部矩形：" << rtInner;
#endif // 1
}

void ScreenShot::drawWinInfo(QPainter& pa)
{
    int i = 0;
    pa.save();
    pa.setPen(QPen(Qt::black, 12));
    pa.setBrush(Qt::NoBrush);
    for (const WinData& it : IWinInfo::m_vWinData) {
        if (!it.bFilter) {
            const QRect& rt = it.rect;
            pa.drawRect(rt);
            pa.drawText(QPoint(rt.topLeft()) + QPoint(0, 30), it.path);
            pa.drawText(QPoint(rt.topLeft()) + QPoint(0, 10), it.title + QString("[%1]").arg(i++));
        }
    }

    pa.restore();
}

void ScreenShot::drawMaskLayer(const QRect& virGeom, const QRect& rtSel, QPainter& pa)
{
    QPainterPath path;
    path.addRect(virGeom);
    path.addRect(rtSel);
    path.setFillRule(Qt::OddEvenFill);
    pa.save();
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(0, 0, 0, 0.5 * 255));
    pa.drawPath(path);
    pa.restore();
}

void ScreenShot::drawBorder(QRect& rtSel, QPainter& pa)
{
    if (rtSel.width() < 0 || rtSel.height() < 0)
        return;
    pa.save();
    const QPen pen(QPen(XHelper::instance().borderColor(), XHelper::instance().borderWidth()));
    pa.setPen(pen);
    pa.setBrush(Qt::NoBrush);
    m_selSizeTip->move(drawSelSizeTip(rtSel));

    QRect rt(rtSel);
    for (auto it = m_specifyRts.cbegin(); it != m_specifyRts.cend(); ++it) {
        if (*it == rtSel) {
            const int offset = pen.width();
            rt.adjust(offset, offset, -offset, -offset);
            break;
        }
    }

    // 绘画边框样式
    const QString style = XHelper::instance().boardStyle();
    if (style == "flipped") {
        drawBorderPS(pa, rt);
    } else if (style == "mac") {
        drawBorderMac(pa, rt);
    } else if (style == "deepin") {
        pa.drawRect(rt);
        drawBorderBlue(pa, rt);
    }

    pa.restore();
}

void ScreenShot::drawCrosshair(QPainter& pa)
{
    if (XHelper::instance().crosshair() && !m_bFirstPress) {
        pa.save();
        pa.setPen(QPen(XHelper::instance().crosshairColor(), XHelper::instance().crosshairWidth()));
        pa.setBrush(Qt::NoBrush);

        QPoint p(QCursor::pos());
        QLine l1(QPoint(m_virGeom.left(), p.y()), QPoint(m_virGeom.right(), p.y()));
        QLine l2(QPoint(p.x(), m_virGeom.top()), QPoint(p.x(), m_virGeom.bottom()));
        pa.drawLine(l1);
        pa.drawLine(l2);
        pa.restore();
    }
}

void ScreenShot::drawToolBar()
{
    if (isVisible() && m_selBar && m_bFirstSel) {
        const auto v = drawBarPosition(m_barOrien, ToolBarOffset::TBO_Middle);

        if (v.size() == 2) {
            m_selBar->move(v.at(0));
            m_paraBar->move(v.at(1));
        }

#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)
        // 添加磨砂透明效果
        const double blurRadius = 7;
        auto t1 = m_currPixmap->copy(QRect(v[0] * getDevicePixelRatio(), m_selBar->rect().size() * getDevicePixelRatio()));
        m_selBar->setBlurBackground(t1, blurRadius);
        auto t2 = m_currPixmap->copy(QRect(v[1] * getDevicePixelRatio(), m_paraBar->rect().size() * getDevicePixelRatio()));
        m_paraBar->setBlurBackground(t2, blurRadius);
#else
#endif

    }
}

void ScreenShot::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		qDebug() << "Key_Escape";
		emit sigClearScreen();
        // hide() 和 close() 区别: https://stackoverflow.com/questions/39407564
        //hide();
        close();   // // 销毁再不会有问题,由单例改写为 new 形式了。排查：1. tray 有关，改用 qpushbutton 和 close即可； 2.单例有关，该市建议修改为 new 指针的比较合适
        //deleteLater();
    }
}

// 注意: 1. 按下、松开时候会切换状态；点击绘画按钮也会切换状态
//      2. 开启鼠标跟踪时机；点击绘画按钮也会相应开启/关闭
//      3. mousePressEvent、mouseMoveEvent、mouseReleaseEvent 合成整体来看；以及不忘记绘画按钮的槽函数
void ScreenShot::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
		return;

    m_bFirstPress = true;
	setMouseTracking(false);
	if (m_rtCalcu.getSelRect().isEmpty() && m_rtCalcu.scrnType == ScrnType::Wait) {
		//m_rtCalcu.clear();

		m_rtCalcu.scrnType = ScrnType::Select;
        m_rtCalcu.pos1 = event->pos();
        m_rtCalcu.pos2 = m_rtCalcu.pos1;
	} else if (m_rtCalcu.scrnType == ScrnType::Draw) {

        if (m_step.shape == DrawShape::Text) {
            m_step.p2 = m_step.p1;   // p2 as perviousPos
            m_step.p1 = event->pos();
        } else {
            m_step.p2 = m_step.p1 = event->pos();
        }
        
        if (m_step.shape == DrawShape::Text) {

            m_edit->move(m_step.p1);
            const QRect rtEdit(mapFromGlobal(m_edit->mapToGlobal(QPoint(0, 0))), m_edit->size());
            if (!rtEdit.contains(m_step.p1, true)) {  // 编辑完成 || 初次编辑

                if (!m_edit->isVisible() && m_step.text.isEmpty()) {
                    m_edit->setVisible(true);
                    m_edit->setFocus();
                    
                } else {       // 显示中
                    m_step.text = m_edit->toPlainText();
                    m_step.idxLevel = m_step.totalIdx++;
                    m_step.rt.setTopLeft(m_step.p2);
                    m_vDrawed.push_back(m_step);  // 暂时特例：绘画文字为单独处理
                    m_edit->clear();
                    m_step.text.clear();
                    m_edit->setVisible(false);
                }
            }

            m_step.p2 = m_step.p1;
        }

	} else {  // 则可能为移动、拉伸、等待状态
		m_rtCalcu.scrnType = updateScrnType(event->pos());
    }

	if (m_rtCalcu.scrnType == ScrnType::Move) {
		m_rtCalcu.pos1 = event->pos();
        m_rtCalcu.pos2 = m_rtCalcu.pos1;

        whichShape(); // 判定移动选中的已绘图形
	} else if (m_rtCalcu.scrnType == ScrnType::Stretch) {
		m_rtCalcu.pos1 = event->pos();
        m_rtCalcu.pos2 = m_rtCalcu.pos1;
	}

    update();
}

void ScreenShot::mouseMoveEvent(QMouseEvent *event)
{
//    if (event->button() != Qt::LeftButton)
//        return;

	// 此时为 Qt::NoButton
	if (m_rtCalcu.scrnType == ScrnType::Wait) {
        if (m_bSmartWin)
            updateGetWindowsInfo();
	} else if (m_rtCalcu.scrnType == ScrnType::Select) {
        m_rtCalcu.pos2 = event->pos();
        m_bSmartWin = false;
        //if (m_rtCalcu.pos1 != m_rtCalcu.pos2)
        //  不显示 TODO: 2022.02.10 再添加一个变量即可
	} else if (m_rtCalcu.scrnType == ScrnType::Move) {
		m_rtCalcu.pos2 = event->pos();
	} else if (m_rtCalcu.scrnType == ScrnType::Draw) {
        m_step.p2 = event->pos();

        if (m_step.shape == DrawShape::Pen) {
            m_step.custPath.append(event->pos());
        } else if (m_step.shape == DrawShape::Text) {
            m_step.p1 = m_step.p2;
            m_edit->move(m_step.p1);
        }

        m_step.rt = RectCalcu::getRect(m_step.p1, m_step.p2);
	} else if (m_rtCalcu.scrnType == ScrnType::Stretch) {
		m_rtCalcu.pos2 = event->pos();
	}

    updateCursorShape(event->pos());
    update();
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (m_rtCalcu.scrnType == ScrnType::Wait) {
	} else if (m_rtCalcu.scrnType == ScrnType::Select) {
		m_rtCalcu.pos2 = event->pos();

        if (m_rtCalcu.pos1 == m_rtCalcu.pos2) {  // 点击到一个点，视作智能检测窗口； 否则就是手动选择走下面逻辑
            m_rtCalcu.setRtSel(m_rtSmartWindow);
        }

        m_bSmartWin = false; // 自动选择也结束

	} else if (m_rtCalcu.scrnType == ScrnType::Move) {
		m_rtCalcu.pos2 = event->pos();

        // 移动选中的图形
        if (m_pCurrShape) {
            m_pCurrShape->rt.translate(m_rtCalcu.pos2 - m_rtCalcu.pos1);
        }

	} else if (m_rtCalcu.scrnType == ScrnType::Draw) {
        
        m_step.p2 = event->pos();
        if (m_step.shape == DrawShape::Pen)
            m_step.custPath.append(event->pos());

        m_step.rt = RectCalcu::getRect(m_step.p1, m_step.p2);

        // DrawShape::Text  在按下时候单独处理 m_vDrawed.push_back
        if (m_step.shape != DrawShape::Text && m_step.shape != DrawShape::NoDraw) {
            if (isDrawShape(m_step)) {
                m_vDrawed.push_back(m_step); // TODO 2022.01.16 优化:  不必每次(无效得)点击，也都记录一次
                m_step.idxLevel = m_step.totalIdx++;
                m_step.clear();
            }
        }

	} else if (m_rtCalcu.scrnType == ScrnType::Stretch) {
		m_rtCalcu.pos2 = event->pos();
	}

    if (!m_rtCalcu.calcurRsultOnce().isEmpty()) {  // 计算一次结果
        m_bFirstSel = true;
        if (!m_selBar->isVisible())
            m_selBar->setVisible(true);

        if (!m_selSizeTip->isVisible())
            m_selSizeTip->setVisible(true);
    }

	if (m_rtCalcu.scrnType != ScrnType::Draw) {
		m_rtCalcu.scrnType = ScrnType::Wait;
		setMouseTracking(true);
	}

    update();
}

void ScreenShot::wheelEvent(QWheelEvent* event)
{
    // Note: On X11 this value is driver specific and unreliable, use angleDelta() instead
    // QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    QPoint numSteps = numDegrees / 15;

    if (numDegrees.isNull())
        return;

    if (m_step.shape == DrawShape::Text) {
        m_step.font.setPointSize(m_step.font.pointSize() + numSteps.y());
        m_edit->setFont(m_step.font);
    } else {
        m_step.pen.setWidth(m_step.pen.width() + numSteps.y());
        if (m_step.pen.width() <= 0)
            m_step.pen.setWidth(1);
        if (m_step.pen.width() >= 100)
            m_step.pen.setWidth(100);

        emit sigLineWidthChange(m_step.pen.width());
    }

    event->accept();
}

void ScreenShot::getScrnShots()
{
    qDebug() << "---------------@#1----------------";
    m_specifyRts.clear();
    m_specifyRts.insert(m_virGeom);
    for (const auto& it : m_scrns)
        m_specifyRts.insert(it->geometry());

    getScrnInfo();
    qDebug() << "---------------@#2----------------";
    getVirScrnPixmap();

#ifdef Q_OS_WIN
    show();
#else
    showFullScreen();         // Linux supports virtual multi-screen, Mac only one screen
#endif

    if (m_bSmartWin) 
        updateGetWindowsInfo();
    if (!isActiveWindow())
        activateWindow();     // fix: Initial use of global hotkeys to summon the screenshot window does not respond to Esc.  https://ifmet.cn/posts/44eabb4d
}

// 屏幕详细参数
void ScreenShot::getScrnInfo()
{
    XLOG_INFO("---------------QApplication::desktop() Info BEGIN----------------");
    XLOG_INFO("所有可用区域 m_virtualGeometry({}, {}, {} * {})", m_virGeom.left(), m_virGeom.top(), m_virGeom.width(), m_virGeom.height());
    XLOG_INFO("主屏可用区域 m_priScrn->geometry()({}, {}, {} * {})", m_priScrn->geometry().left(), m_priScrn->geometry().top(), m_priScrn->geometry().width(), m_priScrn->geometry().height());
    XLOG_INFO("是否开启虚拟桌面 isVirtualDesktop: {}",  m_priScrn->virtualSiblings().size() > 1 ? true : false);
    XLOG_INFO("---------------QApplication::desktop() Info END----------------");

    XLOG_INFO("---------------m_screens[] Info BEGIN----------------");
    for (const auto& it : m_scrns) {
        qDebug() << "序号 it:" << m_scrns.indexOf(it) << "  it:" << it
                 << "\n可用几何 availableGeometry:" << it->availableGeometry()
                 << "\n可用虚拟几何 availableVirtualSize:" << it->availableVirtualSize()
                 << "\n虚拟几何 virtualGeometry:" << it->virtualGeometry()
                 << "\n几何 geometry:" << it->geometry()
                 << "\n尺寸 size:" << it->size()
                 << "\n物理尺寸 physicalSize:" << it->physicalSize()
                 << "\n刷新率 refreshRate:" << it->refreshRate()
                 << "\n尺寸 size:" << it->size()
                 << "\n虚拟尺寸 virtualSize:" << it->virtualSize()
                 << "\n设备像素比 devicePixelRatio:" << it->devicePixelRatio()
                 << "\n逻辑DPI logicalDotsPerInch:" << int(it->logicalDotsPerInch()) << " DPIX:" << int(it->logicalDotsPerInchX()) << " DPIY:" << int(it->logicalDotsPerInchY())
                 << "\n物理DPI physicalDotsPerInch:" << int(it->physicalDotsPerInch()) << " DPIX:" << int(it->physicalDotsPerInchX()) << " DPIY:" << int(it->physicalDotsPerInchY())
                 << "\n手算缩放比 getScale:" << getScale(it)
                 << "\n虚拟尺寸 m_virGeom:" << m_virGeom << "\n\n";

        XLOG_DEBUG("屏幕详细信息：index[{}]", m_scrns.indexOf(it));
        XLOG_DEBUG("size({}, {})", it->size().width(), it->size().height());
        XLOG_DEBUG("geometry({}, {}, {} * {})", it->geometry().left(), it->geometry().top(), it->geometry().width(), it->geometry().height());
        XLOG_DEBUG("availableGeometry({}, {}, {} * {})", it->availableGeometry().left(), it->availableGeometry().top(), it->availableGeometry().width(), it->availableGeometry().height());
        XLOG_DEBUG("scrn->virtualGeometry({}, {}, {} * {})", it->virtualGeometry().left(), it->virtualGeometry().top(), it->virtualGeometry().width(), it->virtualGeometry().height());

        XLOG_INFO("设备像素比 devicePixelRatio[{}]  制造商 manufacturer[{}]  名称 name[{}]", it->devicePixelRatio(), it->manufacturer().toUtf8().data(), it->name().toUtf8().data());
        XLOG_INFO("序号 serialNumber[{}]  刷新率 refreshRate[{}]  模式 model[{}]", it->serialNumber().toUtf8().data(), it->refreshRate(), it->model().toUtf8().data());
        XLOG_INFO("虚拟几何 virtualGeometry:({}, {}, {} * {})  缩放比 getScale[{}]", it->availableGeometry().left(), it->availableGeometry().top(), it->availableGeometry().width(), it->availableGeometry().height(), getScale(it));
        XLOG_INFO("物理几何 physicalSize:({} * {})  大小 size({} * {})", it->physicalSize().width(), it->physicalSize().height(), it->physicalSize().width(), it->physicalSize().height());
        XLOG_INFO("物理 DPI physicalDotsPerInch: {}  DPIX: {}  DPIY: {} ", int(it->physicalDotsPerInch()), int(it->physicalDotsPerInchX()), int(it->physicalDotsPerInchY()));
        XLOG_INFO("逻辑 DPI logicalDotsPerInch: {}  DPIX: {}  DPIY: {}\n", int(it->logicalDotsPerInch()), int(it->logicalDotsPerInchX()), int(it->logicalDotsPerInchX()));
    }

    XLOG_INFO("m_virtualGeometry({}, {}, {} * {})\n", m_virGeom.left(), m_virGeom.top(), m_virGeom.width(), m_virGeom.height());
    XLOG_INFO("---------------m_screens[] Info END----------------");
}

double ScreenShot::getDevicePixelRatio(QScreen * screen)
{
#ifdef Q_OS_MAC
    return screen ? screen->devicePixelRatio() : 2;
#else
    return screen ? screen->devicePixelRatio() : 1;
#endif
}

// 随着光标移动，更新获取桌面所有窗口信息
void ScreenShot::updateGetWindowsInfo()
{
    WinID winId;

#ifdef Q_OS_WIN
    winId._hWnd = (void *)QWidget::winId();

#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
    winId._xWindow = (unsigned long)0;
#endif

    Util::getRectFromCurrentPoint(winId, m_rtSmartWindow);
    m_rtSmartWindow = QRect(mapFromGlobal(m_rtSmartWindow.topLeft()), m_rtSmartWindow.size());
}

double ScreenShot::getScale(QScreen * screen)
{
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)             // or defined(Q_WS_WIN) || defined(Q_WS_X11)
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
    return screen->logicalDotsPerInch() / 96.0;
#endif
}

bool ScreenShot::isSelBorder()
{
    return !m_pCurrShape;
}

const Qt::Orientation ScreenShot::getBarOrien() const
{
    return m_barOrien;
}

void ScreenShot::setBarOrien(Qt::Orientation val)
{
    m_barOrien = val;
}

const QScreen *ScreenShot::currentScreen(const QPoint& pos)
{
    const QScreen* curScrn = qGuiApp->screenAt(pos);

#if defined(Q_OS_MACOS)
    // On the MacOS if mouse position is at the edge of bottom or right sides
    // qGuiApp->screenAt will return nullptr, so we need to try to find current
    // screen by moving 1 pixel inside to the current desktop area
    if (!curScrn && (pos.x() > 0 || pos.y() > 0))
        curScrn = qGuiApp->screenAt(QPoint(pos.x() - 1, pos.y() - 1));
    if (!curScrn && (pos.x() >= m_virGeom.right() || pos.y() >= m_virGeom.bottom()))
        curScrn = qGuiApp->screenAt(m_virGeom.bottomRight() - QPoint(1, 1));
#endif

    //if (!curScrn)
    //    curScrn = qGuiApp->primaryScreen();

    if (!curScrn)
        qDebug() << "Gets that the current screen is empty";

    return curScrn;
}

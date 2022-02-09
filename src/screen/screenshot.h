//
// Created by xmuli on 2021/9/29.
//

#ifndef PICSHOT_WINFULLSCREEN_H
#define PICSHOT_WINFULLSCREEN_H

#include "rectcalcu.h"
#include "drawtoolbar.h"
#include "drawhelper.h"
#include "../widget/xtextwidget.h"
#include "../xglobal.h"
#include <QWidget>
#include <QList>
#include <QColor>
#include <vector>

#ifdef Q_OS_WIN
    #include "../wininfo/wininfo_win.h"
#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
#endif

QT_BEGIN_NAMESPACE
class QScreen;
class QPixmap;
QT_END_NAMESPACE


class ScreenShot : public  QWidget
{
    Q_OBJECT

public:
    static ScreenShot& instance();
    void getScrnShots();
    static double getScale(QScreen *screen = QApplication::primaryScreen());

private:
    void getScrnInfo();
	double getDevicePixelRatio();
	double getDevicePixelRatio(QScreen *screen);

	void updateGetWindowsInfo();


signals:
	void sigClearScreen();

public slots:
	void onClearScreen();
//    void onDrawContextChange(???);

    // toolBar 的槽函数
    void onDrawShape(DrawShape shape);
    void onUndo();
    void onRedo();
    void onDownload();
    void onCopy();

    void onDrawStart();
    void onDrawEnd();

	void onLineEndsChange(LineEnds ends);
	void onLineDasheChange(Qt::PenStyle dashes);

private:
    ScreenShot(QWidget *parent = nullptr);
    ~ScreenShot() override;

	ScrnType updateScrnType(const QPoint pos);
	void updateCursorShape(const QPoint pos);
    void updateBorderCursorShape(const CursorArea& cursArea);
    QPixmap* getVirtualScreen();

	// 绘画边框样式
    void drawBorderBlue(QPainter& pa, QRect rt, int num = 8, bool isRound = true);
	void drawBorderMac(QPainter& pa, QRect rt, int num = 8, bool isRound = true);

    // 辅助（绘画）函数， TODO 可以迁移到 XDrawHelper 中？
    void drawStep(QPainter& pa, XDrawStep &step, bool isUseOwn = true);

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
	QList<QScreen *> m_screens;  // 所有屏幕
	QScreen* m_primaryScreen;    // 主屏幕
	QPixmap* m_currPixmap;       // 当前屏幕截图
    QPixmap m_savePixmap;        // 当前屏幕截图 + 遮罩
	RectCalcu m_rtCalcu;         // 选中矩形区域
    

    DrawToolBar* m_tbDrawBar;    // 工具栏
    XDrawStep m_step;        // 当前绘画一步骤
//    XDraw* m_draw;             // 绘画 Helper

    QVector<XDrawStep> m_vDrawed;    // 已绘步骤
    QVector<XDrawStep> m_vDrawUndo;  // 撤销步骤

	// test
	XTextWidget* m_textEdit;
	QRect m_rtTest;
#ifdef Q_OS_WIN
    std::vector<WinInfo> m_vec;
#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
#endif

};

#endif //PICSHOT_WINFULLSCREEN_H

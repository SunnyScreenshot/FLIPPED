//
// Created by xmuli on 2021/9/29.
//

#ifndef PICSHOT_WINFULLSCREEN_H
#define PICSHOT_WINFULLSCREEN_H

#include <QWidget>
#include <QList>
#include <QColor>
#include "rectcalcu.h"
#include "drawtoolbar.h"
#include "subgraptoolbar.h"
#include "drawhelper.h"

#define HAIF_R_BORDER_MARK 4  // 边框上标记点的半径

QT_BEGIN_NAMESPACE
class QScreen;
class QPixmap;
QT_END_NAMESPACE

class WinFullScreen : public  QWidget
{
    Q_OBJECT

public:
    static WinFullScreen& instance();
    void getScrnShots();
    static double getScale(QScreen *screen = QApplication::primaryScreen());

private:
    void getScrnInfo();
	double getDevicePixelRatio();
	double getDevicePixelRatio(QScreen *screen);

signals:
	void sigClearScreen();

public slots:
	void onClearScreen();

    // toolBar 的槽函数
    void onDrawShape(XDrawShape shape);
    void onUndo();
    void onRedo();
    void onDownload();
    void onCopy();

    void onDrawStart();
    void onDrawEnd();

private:
    WinFullScreen(QWidget *parent = nullptr);
    ~WinFullScreen() override;

    QPixmap* getVirtualScreen();
	void modifyRectSize(QRect& rtSel);

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
	CursorArea m_cursorArea;     // 光标所在区域

    SubGrapToolBar* m_toolBar;       // 工具栏
    DrawToolBar* m_tbDrawBar;
    XDrawStep m_drawStep;        // 当前绘画一步骤
//    XDraw* m_draw;             // 绘画 Helper

    QVector<XDrawStep> m_vDrawed;    // 已绘步骤
    QVector<XDrawStep> m_vDrawUndo;  // 撤销步骤
};

#endif //PICSHOT_WINFULLSCREEN_H

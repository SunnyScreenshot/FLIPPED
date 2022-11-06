/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.09.29
 * Description:
 ******************************************************************/
#ifndef PICSHOT_WINFULLSCREEN_H
#define PICSHOT_WINFULLSCREEN_H

#include "rectcalcu.h"
#include "drawhelper.h"
#include "../widget/xtextwidget.h"
#include "../tool/selectbar.h"
#include "../tool/parameterbar.h"
#include "../tool/selectsize/selectsize.h"
#include <QWidget>
#include <QList>
#include <QColor>
#include <QVector>
#include <QPointer>
#include <QScreen>
#include <set>  // QSet 是哈希表, std::set 是红黑树变种

struct comp {
    bool operator()(const QRect& rt1, const QRect& rt2) const { return rt1.width() < rt2.width(); }
};

class QPixmap;
class ScreenShot : public  QWidget
{
    Q_OBJECT
public:
    ScreenShot(QWidget* parent = nullptr);
    ~ScreenShot() override;

    void getScrnShots();
    static double getScale(QScreen *screen = QApplication::primaryScreen());
    bool isSelBorder();

    const Qt::Orientation getBarOrien() const;
    void setBarOrien(Qt::Orientation val);

private:
    const QScreen *currentScreen(const QPoint &pos = QCursor::pos());
    void getScrnInfo();
    double getDevicePixelRatio(QScreen *screen = nullptr);
    void updateGetWindowsInfo();
    void whichShape();
    void savePixmap(bool quickSave = true, bool autoSave = true);

signals:
    void sigClearScreen();
    void sigLineWidthChange(int width);

public slots:
    // The slot function of the toolBar
    void onLineEndsChange(LineEnds ends);
    void onLineDasheChange(Qt::PenStyle dashes);

    // new refactor
    void onEnableDraw(bool enable);
    void onSelShape(DrawShape shape, bool checked);
    void onRevocation();
    void onRenewal();
    void onPin();
    void onSave();
    void onCancel();
    void onFinish();
    void onInterruptEdit(const QPoint& pos);
    void clearnAndClose();
    void onParaBtnId(DrawShape shape, QToolButton* tb);
    void onLineWidthChange(int width);
    void onSelColor(QColor col);
    void onClearScreen();

private:
    ScrnType updateScrnType(const QPoint pos);
    void updateCursorShape(const QPoint pos);
    void updateBorderCursorShape(const CursorArea& cursArea);
    QPixmap* getVirScrnPixmap();
    bool drawToCurrPixmap();
    bool getDrawedShapeRect();

    void drawStep(QPainter& pa, XDrawStep &step, bool isUseEnvContext = false);     // 辅助（绘画）函数， TODO 可以迁移到 XDrawHelper 中？
    bool isDrawShape(XDrawStep& step);

    const QVector<QPoint> drawBarPosition(Qt::Orientation orien = Qt::Horizontal, ToolBarOffset offset = ToolBarOffset::TBO_Middle);
    const QPoint drawSelSizeTip(const QRect& rt);

    // Test
    void showAllDrawedShape(QPainter& pa);

    // [paintEvent] refactor
    void drawBorderBlue(QPainter& pa, QRect rt, int num = 8, bool isRound = true);   // Blue 边框样式
    void drawBorderMac(QPainter& pa, QRect rt, int num = 8, bool isRound = true);    // Mac 边框样式
    void drawBorderPS(QPainter& pa, QRect rt, bool isRound = false);                 // Flipped 边框样式

    void drawWinInfo(QPainter& pa);                                                  // 绘画 窗口信息 path title 等
    void selectedShapeMove(QPainter& pa);                                            // flameshot 选中图形的效果
    void drawMaskLayer(const QRect& virGeom, const QRect& rtSel, QPainter& pa);      // 屏幕遮罩
    void drawBorder(QRect& rtSel, QPainter& pa);                                     // 绘画边框
    void drawCrosshair(QPainter& pa);                                                // 绘画十字线
    void drawToolBar();                                                              // 绘画工具栏

    void showDebugInfo(QPainter& pa, QRect& rtSel);                                  // 显示实时的预览调试信息

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    double m_scal;
    QList<QScreen *> m_scrns;              // 所有屏幕
    QScreen* m_priScrn;                    // 主屏幕
    QScreen* m_curScrn;                    // 截图时光标所在的屏幕
    QRect m_virGeom;                       // 截图时刻的虚拟桌面的大小
    QPixmap* m_currPixmap;                 // 当前屏幕截图
    QPixmap m_savePixmap;                  // 当前屏幕截图 + 遮罩   无构造初始化
    RectCalcu m_rtCalcu;                   // 选中矩形区域
    bool m_bSmartWin;                      // 是否开启智能窗口
    bool m_bFirstSel;                      // 初次选中 截图矩形 完成
    bool m_bFirstPress;                    // 初次 左键按下 完成(十字线)

    XDrawStep m_step;                      // 当前绘画一步骤

    QVector<XDrawStep> m_vDrawed;          // 已绘步骤
    QVector<XDrawStep> m_vDrawUndo;        // 撤销步骤
    std::set<QRect, comp> m_specifyRts;    // 特殊窗口的矩形，绘画时需略调整
    XDrawStep* m_pCurrShape;               // 移动状态下的选中矩形； nullptr 为 最外层框， 非 nullptr 为具体选中

    // new refactor
    QRect m_rtSmartWindow;                 // 自动检测窗口矩形大小；用以给其它赋值
    Qt::Orientation m_barOrien;
    QPointer<SelectSize> m_selSizeTip;     // 选中矩形的尺寸提示
    QPointer<SelectSize> m_lineWidthTip;    // 画笔宽度临时预览
    QPointer<SelectBar> m_selBar;
    QPointer<ParameterBar> m_paraBar;
    QPointer<XTextWidget> m_edit;
};

#endif //PICSHOT_WINFULLSCREEN_H

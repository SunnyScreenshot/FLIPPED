//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/06.
//
#ifndef WINTOOLBAR_H
#define WINTOOLBAR_H

#include "../widget/xroundwidget.h"
#include <QWidget>
#include <QVector>
#include "xdraw.h"

QT_BEGIN_NAMESPACE
class QToolButton;
class QStringList;
QT_END_NAMESPACE

class WinToolBar : public QWidget
{
    Q_OBJECT
public:
    WinToolBar(QWidget *parent = nullptr);
    virtual ~WinToolBar();

    bool isToolBtnChecked();

private:
    void init();

signals:
    void sigDrawStart();       // 处于绘画状态
    void sigDrawEnd();         // 处于绘画状态
    void sigDrawShape(XDrawShape shape);
    void sigUndo();
    void sigRedo();
    void sigDownload();
    void sigCopy();

public slots:
    void onToolBtn();

private:
    QStringList m_toolBtnName;
    QVector<QToolButton *> m_vecToolBar;
};

#endif // WINTOOLBAR_H

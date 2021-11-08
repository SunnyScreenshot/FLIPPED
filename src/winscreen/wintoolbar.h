//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/06.
//
#ifndef WINTOOLBAR_H
#define WINTOOLBAR_H

#include "../widget/xroundwidget.h"
#include <QWidget>
#include <QVector>

QT_BEGIN_NAMESPACE
class QToolButton;
class QStringList;
QT_END_NAMESPACE

class WinToolBar : public XRoundWidget
{
    Q_OBJECT
public:
    WinToolBar(QWidget *parent = nullptr);
    virtual ~WinToolBar();

    void init();

signals:
    void sigDownload();
    void sigCopy();

public slots:
    void onToolBtn();

private:
    QStringList m_toolBtnName;
    QVector<QToolButton *> m_vecToolBar;
};

#endif // WINTOOLBAR_H

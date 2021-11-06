#ifndef WINTOOLBAR_H
#define WINTOOLBAR_H

#include "../widget/xroundwidget.h"
#include <QWidget>

class WinToolBar : public XRoundWidget
{
    Q_OBJECT
public:
    WinToolBar(QWidget *parent = nullptr);
};

#endif // WINTOOLBAR_H

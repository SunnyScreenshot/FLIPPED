#ifndef SUBMOSAICTOOLBAR_H
#define SUBMOSAICTOOLBAR_H

#include "subabstoolbar.h"
#include <QObject>

class SubMosaicToolBar : public SubAbsToolBar
{
    Q_OBJECT
public:
    SubMosaicToolBar(QWidget *parent = nullptr);

    virtual void initUI() override;

};

#endif // SUBMOSAICTOOLBAR_H

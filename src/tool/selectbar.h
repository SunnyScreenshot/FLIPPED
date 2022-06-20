#ifndef SELECTBAR_H
#define SELECTBAR_H

#include "base/frametoolbar.h"
#include "../screen/drawhelper.h"
#include <QVector>

QT_BEGIN_NAMESPACE
class QToolButton;
class QStringList;
QT_END_NAMESPACE

// rectangle
// ellipse
// line
// arrow
// pen
// mosaic  、 smooth
// text
// serialnumber
// gif 暂不添加
// revocation
// renewal
// save
// cancel
// finish
class SelectBar : public FrameToolBar
{
    Q_OBJECT
public:
    SelectBar(Qt::Orientations orien, QWidget *parent = nullptr);
    virtual ~SelectBar();
    void init();

signals:
    void sigDrawStart();       // 处于绘画状态
    void sigDrawEnd();         // 处于绘画状态
    void sigSelShape(DrawShape shape, bool checked);
    void sigRevocation();
    void sigRenewal();
    void sigSave();
    void sigCancel();
    void sigFinish();

public slots:
    void onToolBtn();

private:
    QStringList m_tbName;
    QStringList m_tbOnlyClickName;
    QVector<QToolButton *> m_vItem;
};

#endif // SELECTBAR_H

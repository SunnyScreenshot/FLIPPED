#ifndef COLORPARABAR_H
#define COLORPARABAR_H

#include "frametoolbar.h"
#include <QWidget>
//#include "../screen/drawhelper.h"

class QGridLayout;
class ColorParaBar : public QWidget
{
    Q_OBJECT
public:
    ColorParaBar(Qt::Orientations orien = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~ColorParaBar();
    void init();

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    int m_scal;
    Qt::Orientations m_orien;
    QGridLayout* m_layout;


};

#endif // COLORPARABAR_H

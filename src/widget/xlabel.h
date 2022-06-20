#ifndef XLABEL_H
#define XLABEL_H

#include <QLabel>

class XLabel : public QLabel
{
    Q_OBJECT
public:
    explicit XLabel(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    explicit XLabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    virtual ~XLabel();



    void setInEllipseColor(QColor color, double alpha);
    void setOutEllipseColor(QColor color, double alpha);

private:
    void init();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QColor m_inEllipse;
    double m_inAlpha;
    QColor m_outEllipse;
    double m_outAlpha;
};

#endif // XLABEL_H

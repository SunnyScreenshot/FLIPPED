#ifndef XLABEL_H
#define XLABEL_H

#include <QLabel>
#include <QColor>

class QWidget;
class QString;
class QWidget;
class QPainter;
class QMouseEvent;
class XLabel : public QLabel
{
    Q_OBJECT
public:
    explicit XLabel(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    explicit XLabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    virtual ~XLabel();

    void setColor(QColor c, double alpha = 1);
    void setIsPickColor(bool enablem = false);
private:
    void init();
    void setConicalGradientColor(QPainter &pa);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_color;
    double m_alpha;
    bool m_bPickColor;    // 默认为非渐变色
};

#endif // XLABEL_H

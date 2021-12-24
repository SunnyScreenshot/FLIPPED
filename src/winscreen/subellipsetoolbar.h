//
// Created by xmuli on 2021/12/14.
//
#ifndef SUBELLIPSETOOLBAR_H
#define SUBELLIPSETOOLBAR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QToolButton;
class QSpinBox;
QT_END_NAMESPACE

class SubEllipseToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit SubEllipseToolBar(QWidget *parent = nullptr);

private:
    void initUI();

signals:
    void sigIsFill(bool bFill);

private slots:
    void onClicked(bool checked);

private:
    QToolButton* m_tbEllipse;
    QToolButton* m_tbFilledEllipse;

    QSpinBox* m_spBorder;
};

#endif // SUBELLIPSETOOLBAR_H

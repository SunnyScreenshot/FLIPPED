//
// Created by xmuli on 2021/12/04.
//
#ifndef WINDRAWRECT_H
#define WINDRAWRECT_H

#include <QComboBox>
#include <QSpinBox>
#include <QStringList>
#include <QToolButton>
#include <QVector>
#include <QWidget>

/*!
 * \brief The WinDrawRect class 工具栏绘画矩形详细选择
 */
class WinDrawRect : public QWidget
{
    Q_OBJECT
public:
    explicit WinDrawRect(QWidget *parent = nullptr);

private:
    void init();

private:
    QToolButton* m_tbRect;
    QToolButton* m_tbFilledRect;
//    QToolButton* m_tbColorBorder;
//    QSpinBox* m_spBorder;
//    QToolButton* m_tbRoundCorners;
//    QComboBox* m_cbDashes;
//    QSpinBox* m_spOutLine;
//    QToolButton* m_tbColorOutLine;
//    QSlider* m_sliderOpacity;
//    QToolButton* m_tbShadow;
//    QToolButton* m_tbShadowSet
};

#endif // WINDRAWRECT_H

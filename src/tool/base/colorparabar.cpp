/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.21
 * Description: 取色器面板
 ******************************************************************/
#include "colorparabar.h"
#include "../../xglobal.h"
#include "../../widget/xlabel.h"
#include "../../screen/drawhelper.h"
#include <QLabel>
#include <QBoxLayout>
#include <QGridLayout>
#include <QList>
#include <QVector>
#include <QMap>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QColorDialog>

//test
#include <QMessageBox>

ColorParaBar::ColorParaBar(Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(new QGridLayout())
    , m_curLab(nullptr)
    , m_curColor()
{
    m_labMap = { {"lab0_Red", "#DB000F"}
               , {"lab1_Yellow", "#FFCF53"}
               , {"lab2_Green", "#12F63B"}
               , {"lab3_Blue", "#0E70FF"}
               , {"lab4_Pink", "#FB4288"}
               , {"lab5_Black", "#323232"}
               , {"lab6_White", "#FBFBFB"}
               , {"lab7_Pick", ""}};  // 最后一个彩色

    int rowMax = 0;
    int colMax = 0;
    if(orien == Qt::Horizontal) {
        rowMax = 2;
        colMax = m_labMap.size() / rowMax;
    } else {
        colMax = 2;
        rowMax = m_labMap.size() / colMax;
    }

    auto it = m_labMap.begin();
    for (int i = 0; i < rowMax; ++i) {
        for (int j = 0; j < colMax; ++j) {

            XLabel* lab = new XLabel(this);
            lab->setObjectName(it.key());
            int width(COLOR_LABEL_WIDTH * m_scal);
            lab->setFixedSize(width, width);
            lab->setInEllipseR(width / 2.0);

            lab->installEventFilter(this);

            if ((it + 1) == m_labMap.end()) {  // 最后一个渐变色
                lab->setEnablemGradient(true);
            } else {
                lab->setInEllipseColor(it.value(), 1);
            }

            m_layout->addWidget(lab, i, j);
            it++;
        }
    }

    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(COLOR_PARA_HOR_SPACING * m_scal);
    m_layout->setVerticalSpacing(COLOR_PARA_VER_SPACING * m_scal);  // 检查比例一下
    setLayout(m_layout);

    connect(this, &ColorParaBar::sigPickColor, this, &ColorParaBar::onPickColor);
}

ColorParaBar::~ColorParaBar()
{

}

void ColorParaBar::init()
{
    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(COLOR_PARA_HOR_SPACING * m_scal);
    m_layout->setVerticalSpacing(COLOR_PARA_VER_SPACING * m_scal);  // 检查比例一下
}

void ColorParaBar::onPickColor(XLabel *lab, QColor col)
{
    Q_UNUSED(col);
    m_curLab = lab;
    m_curColor = col;   // TODO 2022.06.24: 后面可以用属性替换掉

    update();
}

// #see: 用法 https://blog.csdn.net/xiezhongyuan07/article/details/79992099
bool ColorParaBar::eventFilter(QObject *watched, QEvent *event)
{
    XLabel* lab = qobject_cast<XLabel *>(watched);

    if (!lab)
        return QWidget::eventFilter(watched, event);

    if (event->type() == QEvent::MouseButtonRelease) {
        if (lab->objectName().compare("lab7_Pick") == 0) {
            QColor color = QColorDialog::getColor(lab->palette().color(QPalette::Background), this, tr("选择文本颜色"));
            emit sigPickColor(lab, color);
//            QMessageBox::about(nullptr, lab->objectName(), color.name());
        } else {
            const auto& it = m_labMap.find(lab->objectName());
            emit sigPickColor(lab, it.value());
//            QMessageBox::about(nullptr, lab->objectName(), it.value());
        }
        return true;

    } else {
        return QWidget::eventFilter(watched, event);
    }

}

void ColorParaBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
//    Q_UNUSED(event)

    if (!m_curLab)
        return;

    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPen pen(m_curColor);
    pen.setWidth(5);  // 设计稿为 1， 最后看效果
    pa.setPen(pen);
    pa.setBrush(Qt::NoBrush);

    int margin = 4;
    QRect rt = m_curLab->rect().adjusted(-margin, -margin, margin, margin);
    auto t = mapToParent(rt.topLeft());
//    pa.drawEllipse(QRect(t, rt.size()));  // 替换正确那就好了， 使用 VS 进行调试

    pa.drawEllipse(QRect(QPoint(24, 24), rt.size()));
}

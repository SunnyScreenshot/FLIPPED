/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.25
 * Description: 测试
 ******************************************************************/
#include "exwidget.h"
#include "../xglobal.h"
#include "../widget/xlabel.h"
#include "../screen/drawhelper.h"
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

ExWidget::ExWidget(Qt::Orientations orien, QWidget* parent)
    : QWidget(parent)
    , m_scal(insXHelp->getScale())
    , m_orien(orien)
    , m_layout(new QGridLayout())
    , m_curLab(nullptr)
    , m_curColor()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    m_labMap = { {"lab0_Red", "#DB000F"}
               , {"lab1_Yellow", "#FFCF53"}
               , {"lab2_Green", "#12F63B"}
               , {"lab3_Blue", "#0E70FF"}
               , {"lab4_Pink", "#FB4288"}
               , {"lab5_Black", "#323232"}
               , {"lab6_White", "#FBFBFB"}
               , {"lab7_Pick", ""} };  // 最后一个彩色

    int rowMax = 0;
    int colMax = 0;
    if (orien == Qt::Horizontal) {
        rowMax = 2;
        colMax = m_labMap.size() / rowMax;
    }
    else {
        colMax = 2;
        rowMax = m_labMap.size() / colMax;
    }

    auto it = m_labMap.begin();
    for (int i = 0; i < rowMax; ++i) {
        for (int j = 0; j < colMax; ++j) {

            QPushButton* lab = new QPushButton(QString("%1").arg((i+1)* rowMax + (j+1)), this);
            lab->setObjectName(it.key());
            int width(CPB_PB_LABEL_WIDTH * m_scal);
            lab->setFixedSize(width, width);
            //lab->setInEllipseR(width / 2.0);

            //lab->installEventFilter(this);

            //if ((it + 1) == m_labMap.end()) {  // 最后一个渐变色
            //    lab->setEnablemGradient(true);
            //}
            //else {
            //    lab->setInEllipseColor(it.value(), 1);
            //}
            connect(lab, &QPushButton::pressed, this, &ExWidget::onBtnTest);
            m_layout->addWidget(lab, i, j);
            it++;
        }
    }

    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(CPB_PB_SPACING_HOR * m_scal);
    m_layout->setVerticalSpacing(CPB_PB_SPACING_VER * m_scal);  // 检查比例一下
    setLayout(m_layout);

    connect(this, &ExWidget::sigPickColor, this, &ExWidget::onPickColor);
}

ExWidget::~ExWidget()
{

}

void ExWidget::init()
{
    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(CPB_PB_SPACING_HOR * m_scal);
    m_layout->setVerticalSpacing(CPB_PB_SPACING_VER * m_scal);  // 检查比例一下
}

void ExWidget::onPickColor(XLabel* lab, QColor col)
{
    Q_UNUSED(col);
    m_curLab = lab;
    m_curColor = col;   // TODO 2022.06.24: 后面可以用属性替换掉

    update();
}

void ExWidget::onBtnTest()
{
    for (const auto& it : findChildren<QPushButton*>())
    {
        qDebug() << it << "  " << it->objectName() << it->isVisible() << "  " << it->rect();
    }

    auto pos1 = cursor().pos();
    XLabel* lab = static_cast<XLabel*>(childAt(mapFromGlobal(pos1)));

    if (!lab)
        return;

    auto rt1 = lab->rect();
    auto pos = rt1.topLeft();

    auto p0 = lab->mapToGlobal(QPoint(0, 0));
    auto p1 = mapFromGlobal(pos);
    auto p2 = mapToParent(pos);
    auto p3 = mapTo(this, pos);
    auto p4 = mapToGlobal(pos);
}


//
//
//// #see: 用法 https://blog.csdn.net/xiezhongyuan07/article/details/79992099
//bool eventFilter(QObject* watched, QEvent* event)
//{
//    XLabel* lab = qobject_cast<XLabel*>(watched);
//
//    if (!lab)
//        return QWidget::eventFilter(watched, event);
//
//    if (event->type() == QEvent::MouseButtonRelease) {
//        if (lab->objectName().compare("lab7_Pick") == 0) {
//            QColor color = QColorDialog::getColor(lab->palette().color(QPalette::Background), this, tr("选择文本颜色"));
//            emit sigPickColor(lab, color);
//
//            event->ignore();
//            //            QMessageBox::about(nullptr, lab->objectName(), color.name());
//        }
//        else {
//            const auto& it = m_labMap.find(lab->objectName());
//            emit sigPickColor(lab, it.value());
//            event->ignore();
//            //            QMessageBox::about(nullptr, lab->objectName(), it.value());
//        }
//        return false;
//
//    }
//    else {
//        return QWidget::eventFilter(watched, event);
//    }
//
//}

void ExWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    //    Q_UNUSED(event)

    //if (!m_curLab)
    //    return;

    //QPainter pa(this);
    //pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //QPen pen(m_curColor);

    ////auto p1 = m_curLab->parent();
    ////auto p2 = m_curLab->parentWidget();

    //for (const auto& it : findChildren<XLabel*>())
    //{
    //    qDebug() << it << "  " << it->objectName() << it->isVisible() << "  " << it->rect();
    //}

    //auto pos1 = cursor().pos();
    //XLabel* lab = static_cast<XLabel*>(childAt(mapFromGlobal(pos1)));

    //QRect p3;
    //if (lab)
    //    p3 = lab->rect();

    //pen.setWidth(5);  // 设计稿为 1， 最后看效果
    //pa.setPen(pen);
    //pa.setBrush(Qt::NoBrush);

    //int margin = 4;
    //QRect rt = lab->rect();// .adjusted(-margin, -margin, margin, margin);
    //auto t = mapTo(this, rt.topLeft());
    //auto t2 = mapToParent(rt.topLeft());

    //pa.drawEllipse(QRect(t, rt.size()));  // 替换正确那就好了， 使用 VS 进行调试

    ////pa.drawEllipse(QRect(QPoint(24, 24), rt.size()));
}

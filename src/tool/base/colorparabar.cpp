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
#include "../../core/xlog.h"
#include "../../widget/xlabel.h"
#include "../../screen/drawhelper.h"
#include <QLabel>
#include <QBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout> 
#include <QList>
#include <QVector>
#include <QMap>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QColorDialog>

//test
#include <QMessageBox>
#include <QDebug>

ColorParaBar::ColorParaBar(ColorParaBarMode mode, Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(nullptr)
{
    m_labMap = { {"lab0_Red", "#DB000F"}
   , {"lab1_Yellow", "#FFCF53"}
   , {"lab2_Green", "#12F63B"}
   , {"lab3_Blue", "#0E70FF"}
   , {"lab4_Pink", "#FB4288"}
   , {"lab5_Black", "#323232"}
   , {"lab6_White", "#FBFBFB"}
   , {"lab7_Pick", ""} };  // 最后一个彩色

    if (mode == ColorParaBarMode::CPB_ParaBar) {
        m_layout = new QGridLayout(this);
        QGridLayout* layout = static_cast<QGridLayout *>(m_layout);

        int rowMax = 0;
        int colMax = 0;
        if (m_orien == Qt::Horizontal) {
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

                if (i == 0 && j == 0) { // 初始化为默认的第一个 XLabel 颜色 
                    if (!setProperty("curPick", QVariant::fromValue((void*)lab)))
                        XLOG_INFO("Property [curPick] initialization faile.");

                    if (!setProperty("curColor", QColor(it.value())))
                        XLOG_INFO("Property [curColor] initialization faile.");
                }

                lab->setObjectName(it.key());
                int width(CPB_PB_LABEL_WIDTH * m_scal);
                lab->setFixedSize(width, width);
                lab->installEventFilter(this);

                if ((it + 1) == m_labMap.end()) {  // 最后一个渐变色
                    lab->setIsPickColor(true);
                } else {
                    lab->setColor(it.value(), 1);
                }

                layout->addWidget(lab, i, j);
                it++;
            }
        }

        //this->installEventFilter(this);
        layout->setContentsMargins(CPB_PB_MARGIN_HOR, CPB_PB_MARGIN_VER, CPB_PB_MARGIN_HOR, CPB_PB_MARGIN_VER);
        layout->setHorizontalSpacing(CPB_PB_SPACING_HOR * m_scal);
        layout->setVerticalSpacing(CPB_PB_SPACING_VER * m_scal);  // 检查比例一下
    } else if (mode == ColorParaBarMode::CPB_HighLight) {
        m_layout = new QHBoxLayout(this);
        QHBoxLayout* layout = static_cast<QHBoxLayout*>(m_layout);

        auto it = m_labMap.begin();
        for (int i = 0; i < m_labMap.size(); ++i) {

                XLabel* lab = new XLabel(this);
                if (i == 0) { // 初始化为默认的第一个 XLabel 颜色 
                    if (!setProperty("curPick", QVariant::fromValue((void*)lab)))
                        XLOG_INFO("Property [curPick] initialization faile.");

                    if (!setProperty("curColor", QColor(it.value())))
                        XLOG_INFO("Property [curColor] initialization faile.");
                }

                lab->setObjectName(it.key());
                int width(CPB_HL_LABEL_WIDTH * m_scal);
                lab->setFixedSize(width, width);
                lab->installEventFilter(this);

                if ((it + 1) == m_labMap.end()) {  // 最后一个渐变色
                    lab->setIsPickColor(true);
                } else {
                    lab->setColor(it.value(), 1);
                }

                layout->addWidget(lab);
                it++;
        }

        //this->installEventFilter(this);
        layout->setContentsMargins(CPB_HL_MARGIN_HOR, CPB_HL_MARGIN_VER, CPB_HL_MARGIN_HOR, CPB_HL_MARGIN_VER);
        layout->setSpacing(CPB_HL_SPACING_HOR * m_scal);
    }
    
    setLayout(m_layout);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}

ColorParaBar::~ColorParaBar()
{
}

void ColorParaBar::setOrientations(Qt::Orientations orien)
{
    m_orien = orien;
}

// #see: 用法 https://blog.csdn.net/xiezhongyuan07/article/details/79992099
// return 为 false，表示其余事件交还给目标对象处理； true 表示该事件不再进一步处理
bool ColorParaBar::eventFilter(QObject *watched, QEvent *event)
{
    XLabel* lab = qobject_cast<XLabel *>(watched);
    if (!lab)
        return false;

    if (event->type() == QEvent::MouseButtonRelease) {
        QColor color = property("curColor").value<QColor>();
        if (lab->objectName().compare("lab7_Pick") == 0) {
            color = QColorDialog::getColor(lab->palette().color(QPalette::Background), this, tr("选择文本颜色"));
        } else {
            const auto& it = m_labMap.find(lab->objectName());
            color = it.value();
        }

        emit sigColorChange(color);
        //QMessageBox::about(nullptr, lab->objectName(), it.value());
        //for (const auto& it : findChildren<XLabel*>()) {
        //    qDebug() << it << " --> " << it->objectName() << it->isVisible() << "  " << it->rect();
        //}

        if (!setProperty("curPick", QVariant::fromValue(static_cast<void*>(lab))))
            XLOG_INFO("Property [curPick] pick-up new ptr faile, ptr:{}.", static_cast<void*>(lab));

        if (color.isValid()) {
            if (!setProperty("curColor", color))
                XLOG_INFO("Property [curColor] pick-uo new color faile, color:{}.", color.name().toUtf8().data());
        }
        
        update();
        return true;

    } else if (event->type() == QEvent::MouseButtonPress) {
        return true;
    } else {
        return false;
    }
}

void ColorParaBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    XLabel* lab = static_cast<XLabel*>(property("curPick").value<void*>());
    //XLabel* lab = static_cast<XLabel *>(childAt(mapFromGlobal(cursor().pos()))); // 通过此刻的光标的位置，选中对应 XLabel*
    //for (const auto& it : findChildren<XLabel*>()) {
    //    qDebug() << it << "  " << it->objectName() << it->isVisible() << "  " << it->rect();
    //}

    if (!lab)
        return;

    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPen pen(property("curColor").value<QColor>());
    pen.setWidth(CPB_WIDTH_SELECTED * m_scal);
    pa.setPen(pen);
    pa.setBrush(Qt::NoBrush);

    int margin = CPB_MARGIN_SELECTED * m_scal;
    auto topLeft = lab->mapToGlobal(QPoint(0, 0)); // 子控件的窗口的（左上角的）绝对坐标; QPoint(0, 0) 为子控件的左上角坐标，子窗口的总是(0, 0)
    topLeft = mapFromGlobal(topLeft);              // 切换为相对父窗口的绝对坐标
    const QRect rt = QRect(topLeft, lab->size()).adjusted(-margin, -margin, margin, margin);
    pa.drawEllipse(rt.center(), rt.width() / 2, rt.height() / 2);
}

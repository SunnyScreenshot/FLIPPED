/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.06.22
 * Description: 工具栏框架类，负责选择画各种图形的 ToolBar
 ******************************************************************/
#include "selectbar.h"
#include "../xglobal.h"
#include "../widget/xframewidget.h"
#include "../widget/xhorizontalline.h"
#include "../widget/xverticalline.h"
#include <QToolButton>
#include <QStringList>
#include <QBoxLayout>
#include <QIcon>
#include <QToolButton>

SelectBar::SelectBar(Qt::Orientations orien, QWidget *parent)
    : XFrameWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(nullptr)
    , m_tbName()
    , m_vItem()
{
    initUI();


    m_scal = 1;

    m_tbName << "rectangle"
             << "ellipse"
             << "line"
             << "arrow"
             << "pen"
             << "mosaic"
             << "text"
             << "serialnumber"
             << "revocation"
             << "renewal"
             << "save"
             << "cancel"
             << "finish";

    m_tbOnlyClickName << "revocation"
                    << "renewal"
                    << "save"
                    << "cancel"
                    << "finish";

    QStringList barTip;
    barTip << tr("rectangle")
           << tr("ellipse")
           << tr("line")
           << tr("arrow")
           << tr("pen")
           << tr("mosaic")
           << tr("text")
           << tr("serialnumber")
           << tr("revocation")
           << tr("renewal")
           << tr("save")
           << tr("cancel")
           << tr("finish");

    m_vItem.fill(nullptr, m_tbName.count());

    for (int i = 0; i < m_tbName.count(); ++i) {
        if (!m_vItem[i])
            m_vItem[i] = new QToolButton();

        m_vItem[i]->setObjectName(m_tbName[i]);
        m_vItem[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_vItem[i]->setAutoRaise(true);   // 自动浮动模式
        m_vItem[i]->setIcon(QIcon(":/resources/tool/" + m_tbName[i] + ".svg"));
        m_vItem[i]->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * m_scal);
        m_vItem[i]->setContentsMargins(0, 0, 0, 0);
        m_vItem[i]->setFixedSize(QSize(ICON_WIDTH, ICON_HEIGHT) * m_scal);
        m_vItem[i]->setToolTip(barTip[i]);
        m_vItem[i]->setChecked(false);

        for (auto& it : m_tbOnlyClickName) {
            if (m_vItem[i]->objectName() == it)
                m_vItem[i]->setCheckable(false);
            else
                m_vItem[i]->setCheckable(true);
        }

        addWidget(m_vItem[i]);

        QString tbName(m_tbName[i]);
        if (tbName == "serialnumber"
                || tbName == "renewal"
                || tbName == "serialnumber")
            addSpacer();

        connect(m_vItem[i], &QToolButton::released, this, &SelectBar::onToolBtn);
    }
}

SelectBar::~SelectBar()
{
}

void SelectBar::initUI()
{
    if (!m_layout) {
        if (m_orien == Qt::Horizontal)
            m_layout = new QHBoxLayout(this);
        else
            m_layout = new QVBoxLayout(this);
    }

    setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
    m_layout->setContentsMargins(BAR_MARGIN_HOR, BAR_MARGIN_VER, BAR_MARGIN_HOR, BAR_MARGIN_VER);
    m_layout->setSpacing(CW_ITEM_SPACE);
}

void SelectBar::addWidget(QWidget *w)
{
    if (w && m_layout)
        m_layout->addWidget(w);
}

void SelectBar::addSpacer()
{
    if (!m_layout)
        return;

    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine(SPACER_LINE_HEIGHT * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine(SPACER_LINE_HEIGHT * m_scal, this));
}

void SelectBar::onToolBtn()
{
//    QToolButton* tb = nullptr;
//    QObject* obj = sender();
//    if (obj)
//        tb = qobject_cast<QToolButton *>(obj);
//    else
//        return;

//    // 仅单选
//    bool bDrawing = false;  // true 此 btn 被按下，处于绘画状态
//    QList<QToolButton *> listBtn = findChildren<QToolButton *>();
//    for (QToolButton* it : listBtn) {
//        QString path = ":/resources/tool/" + it->objectName() + ".svg";
//        it->setIconSize(QSize(ICON_WIDTH, ICON_WIDTH) * XHelp::getScale());

//        if (it == tb) {
//            if (it->isCheckable()) {
//                if (it->isChecked()) {
//                    bDrawing = true;
//                    it->setIcon(XHelp::changeSVGColor(path, QColor("#1F7AFF"), QSize(ICON_WIDTH, ICON_WIDTH) * XHelp::getScale()));
//                } else {
//                    it->setIcon(QIcon(path));
//                }
//            }
//        } else {
//            if (it->isCheckable()) {
//                it->setChecked(false);
//                it->setIcon(QIcon(path));  // 频繁切换 icon 应该不会有泄露，后面确认下。
//            }
                
//            continue;
//        }

//    }

//    if (bDrawing)
//        emit sigDrawStart();
//    else
//        emit sigDrawEnd();

//    // rectangle
//    // ellipse
//    // line
//    // arrow
//    // pen
//    // mosaic  、 smooth
//    // text
//    // serialnumber
//    // gif 暂不添加
//    // revocation
//    // renewal
//    // save
//    // cancel
//    // finish
//    // 发射信号
//    bool isChecked = tb->isChecked();
//    if (tb->objectName() == "rectangle") {
//        emit sigSelShape(XC::DrawShape::Rectangles, isChecked);
//    } else if (tb->objectName() == "ellipse") {
//        emit sigSelShape(XC::DrawShape::Ellipses, isChecked);
//    } else if (tb->objectName() == "line") {
//        emit sigSelShape(XC::DrawShape::Line, isChecked);
//    } else if (tb->objectName() == "arrow") {
//        emit sigSelShape(XC::DrawShape::Arrows, isChecked);
//    } else if (tb->objectName() == "pen") {
//        emit sigSelShape(XC::DrawShape::Pen, isChecked);
//    } else if (tb->objectName() == "mosaic") {
//        emit sigSelShape(XC::DrawShape::Mosaics, isChecked);
//    } else if (tb->objectName() == "text") {
//        emit sigSelShape(XC::DrawShape::Text, isChecked);
//    } else if (tb->objectName() == "serialnumber") {
//        emit sigSelShape(XC::DrawShape::SerialNumber, isChecked);
//    } else if (tb->objectName() == "revocation") {
//        emit sigRevocation();
//    } else if (tb->objectName() == "renewal") {
//        emit sigRenewal();
//    } else if (tb->objectName() == "save") {
//        emit sigSave();
//    } else if (tb->objectName() == "cancel") {
//        emit sigCancel();
//    } else if (tb->objectName() == "finish") {
//        emit sigFinish();
//    } else{
//    }
}

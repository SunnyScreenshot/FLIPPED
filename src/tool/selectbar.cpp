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
#include "../widget/xhorizontalline.h"
#include "../widget/xverticalline.h"
#include <QToolButton>
#include <QStringList>
#include <QBoxLayout>
#include <QIcon>
#include <QToolButton>

SelectBar::SelectBar(Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(XHelp::getScale())
    , m_blur(new BlurWidget(this))
    , m_orien(orien)
    , m_layout(nullptr)
    , m_tbName()
    , m_tbOnlyClickName()
    , m_vItem()
{
    initUI();

    m_tbName << "rectangle"
             << "ellipse"
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

        if (m_tbOnlyClickName.contains(m_vItem[i]->objectName())) {
            m_vItem[i]->setCheckable(false);
        } else {
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

void SelectBar::setBlurBackground(const QPixmap &pix, double blurRadius)
{
    if (!m_blur)
        return;

    m_blur->setPixmap(pix, blurRadius);
    m_blur->lower();
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
    m_layout->setContentsMargins(SB_MARGIN_HOR, SB_MARGIN_VER, SB_MARGIN_HOR, SB_MARGIN_VER);
    m_layout->setSpacing(SB_ITEM_SPACE);
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
        m_layout->addWidget(new XVerticalLine(B_SPACER_LINE_HEIGHT * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine(B_SPACER_LINE_HEIGHT * m_scal, this));
}

void SelectBar::onToolBtn()
{
    QToolButton* tb = nullptr;
    QObject* obj = sender();
    if (obj)
        tb = qobject_cast<QToolButton *>(obj);
    else
        return;

    // 仅单选
    bool enableDraw = false;  // true 此 btn 被按下，处于绘画状态
    QList<QToolButton *> listBtn = findChildren<QToolButton *>();
    for (QToolButton* it : listBtn) {
        QString path = ":/resources/tool/" + it->objectName() + ".svg";
        it->setIconSize(QSize(ICON_WIDTH, ICON_WIDTH) * XHelp::getScale());

        if (it == tb) {
            if (it->isCheckable()) {
                if (it->isChecked()) {
                    enableDraw = true;
                    it->setIcon(XHelp::changeSVGColor(path, "path", XHelp::highlightColor(), QSize(ICON_WIDTH, ICON_WIDTH) * XHelp::getScale()));
                } else {
                    it->setIcon(QIcon(path));
                }
            }
        } else {
            if (it->isCheckable()) {
                it->setChecked(false);
                it->setIcon(QIcon(path));  // 频繁切换 icon 应该不会有泄露，后面确认下。
            }
                
            continue;
        }
    }
    
    emit sigEnableDraw(enableDraw);

    //if (!enableDraw)
    //    return;

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
    // 发射信号
    bool isChecked = tb->isChecked();
    
    //if (!enableDraw) {
    //    emit sigSelShape(XC::DrawShape::NoDraw, isChecked);
    //} 
    
    if (tb->objectName() == "rectangle") {
        emit sigSelShape(XC::DrawShape::Rectangles, isChecked);
    } else if (tb->objectName() == "ellipse") {
        emit sigSelShape(XC::DrawShape::Ellipses, isChecked);
    } else if (tb->objectName() == "line") {
        emit sigSelShape(XC::DrawShape::LineWidth, isChecked);
    } else if (tb->objectName() == "arrow") {
        emit sigSelShape(XC::DrawShape::Arrows, isChecked);
    } else if (tb->objectName() == "pen") {
        emit sigSelShape(XC::DrawShape::Pen, isChecked);
    } else if (tb->objectName() == "mosaic") {
        emit sigSelShape(XC::DrawShape::Mosaics, isChecked);
    } else if (tb->objectName() == "text") {
        emit sigSelShape(XC::DrawShape::Text, isChecked);
    } else if (tb->objectName() == "serialnumber") {
        emit sigSelShape(XC::DrawShape::SerialNumber, isChecked);
    } else if (tb->objectName() == "revocation") {
        emit sigRevocation();
    } else if (tb->objectName() == "renewal") {
        emit sigRenewal();
    } else if (tb->objectName() == "save") {
        emit sigSave();
    } else if (tb->objectName() == "cancel") {
        emit sigCancel();
    } else if (tb->objectName() == "finish") {
        emit sigFinish();
    } else{
    }
}

void SelectBar::enterEvent(QEvent* event)
{
    setCursor(Qt::ArrowCursor);
}

void SelectBar::resizeEvent(QResizeEvent *event)
{
    m_blur->setGeometry(0, 0, width(), height());
    return QWidget::resizeEvent(event);
}

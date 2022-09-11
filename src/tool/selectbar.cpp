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
#include <QButtonGroup>
#include <QDebug>

SelectBar::SelectBar(Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(XHelper::instance().getScale())
    , m_blur(new BlurWidget(this))
    , m_orien(orien)
    , m_layout(nullptr)
    //, m_group(new QButtonGroup(this))
    , m_vItem()
{
    initUI();

    QStringList tbName1 = { "rectangle", "ellipse", "arrow", "pen", "mosaic", "text", "serialnumber"};
    QStringList tbName2 = { "pin", "revocation", "renewal", "save", "cancel", "finish" };
    QStringList tbName = tbName1 + tbName2;
    QStringList barTip = { tr("rectangle"), tr("ellipse"), tr("arrow"), tr("pen"), tr("mosaic"), tr("text"), tr("serialnumber")
                     , tr("pin"), tr("revocation"), tr("renewal"), tr("save"), tr("cancel"), tr("finish") };

    for (const auto& it : tbName1)
        m_vTbName.push_back(std::make_pair(it, true));
    for (const auto& it : tbName2)
        m_vTbName.push_back(std::make_pair(it, false));

    //m_group->setExclusive(true);
    m_vItem.fill(nullptr, tbName.count());
    int i = 0;
    for (const auto it : m_vTbName){
        auto tb = new QToolButton();
        tb->setObjectName(it.first);
        tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tb->setAutoRaise(true);
        tb->setIcon(QIcon(":/resources/tool/" + it.first + ".svg"));
        tb->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * m_scal);
        tb->setContentsMargins(0, 0, 0, 0);
        tb->setFixedSize(QSize(ICON_WIDTH, ICON_HEIGHT) * m_scal);
        tb->setToolTip(barTip[i++]);
        tb->setChecked(false);
        tb->setCheckable(it.second);
        m_vItem.push_back(tb);
        addWidget(tb);

        if (it.first == "pin" || it.first == "renewal")
            addSpacer();
        //if (it.second)
        //    m_group->addButton(tb);

        connect(tb, &QToolButton::released, this, &SelectBar::onToolBtn);
    }

    //void (QButtonGroup:: * sigFun)(QAbstractButton*) = &QButtonGroup::buttonClicked;
    //connect(m_group, sigFun, this, &SelectBar::onTBReleased);
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
    tb = qobject_cast<QToolButton*>(obj);
    if (!obj)
        return;

    bool enableDraw = false;  // true: btn is pressed, in drawing state
    QList<QToolButton *> listBtn = findChildren<QToolButton *>();

    //for (QToolButton* it : listBtn)
    //    qDebug() << "it:" << it << "   it->isCheckable():" << it->isCheckable() << "  it->isChecked():" << it->isChecked();
    //qDebug() << "\n";

    for (QToolButton* it : listBtn) {
        QString path = ":/resources/tool/" + it->objectName() + ".svg";
        it->setIconSize(QSize(ICON_WIDTH, ICON_WIDTH) * XHelper::instance().getScale());

        if (it == tb) {
            if (!it->isCheckable())
                continue;

            QIcon icon(path);
            if (it->isChecked()) {
                enableDraw = true;
                icon = XHelper::instance().ChangeSVGColor(path, "path", XHelper::instance().borderColor(), QSize(ICON_WIDTH, ICON_WIDTH) * XHelper::instance().getScale());
            }

            it->setIcon(icon);
        } else {
            if (it->objectName() == "text" && it->isChecked())  // fix: textedit edits half of the selected other drawing controls
                emit sigInterruptEdit(QCursor::pos());

            if (it->isCheckable()) {
                it->setChecked(false);
                it->setIcon(QIcon(path));  // 频繁切换 icon 应该不会有泄露，后面确认下
            }
        }
    }

    //for (QToolButton* it : listBtn)
    //    qDebug() << "it:" << it  << "   it->isCheckable():" << it->isCheckable() << "  it->isChecked():" << it->isChecked();
    //qDebug() << "---------------------------------------------\n";
    
    emit sigEnableDraw(enableDraw);
    bool isChecked = tb->isChecked();
    
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
    } else if (tb->objectName() == "pin") {
        emit sigPin();
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

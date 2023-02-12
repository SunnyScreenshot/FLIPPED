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
#include <vector>
#include <tuple>
#include <QString>
#include <QToolButton>
#include <QStringList>
#include <QBoxLayout>
#include <QIcon>
#include <QButtonGroup>
#include <QPainter>
#include <QDebug>
#include "../xglobal.h"
#include "../widget/xhorizontalline.h"
#include "../widget/xverticalline.h"

SelectBar::SelectBar(Qt::Orientations orien, QWidget *parent)
    : QWidget(parent)
    , m_scal(XHelper::instance().getScale())
    , m_orien(orien)
    , m_layout(nullptr)
    , m_blur(std::make_unique<BlurWidget>(this))
{
#if defined(Q_OS_MAC)
    m_blur.release();
#endif

    initUI();
    m_vBtns = {
        std::make_tuple("rectangle", tr("Rectangle"), true, false),
        std::make_tuple("ellipse", tr("Ellipse"), true, false),
        std::make_tuple("arrow", tr("Arrow"), true, false),
        std::make_tuple("custompath", tr("CustomPath"), true, false),
        std::make_tuple("mosaic", tr("Mosaic"), true, false),
        std::make_tuple("text", tr("Text"), true, false),
        std::make_tuple("serialnumber", tr("SerialNumber"), true, false),

        std::make_tuple("pin", tr("Pin"), false, true),
        std::make_tuple("revocation", tr("ReRevocationctangle"), false, false),
        std::make_tuple("renewal", tr("Renewal"), false, true),
        std::make_tuple("save", tr("Save"), false, false),
        std::make_tuple("cancel", tr("Cancel"), false, false),
        std::make_tuple("finish", tr("Finish"), false, false)
    };

    for (const auto& it : m_vBtns){
        const QString& btnName = std::get<0>(it);
        const QString& btnTip = std::get<1>(it);
        const bool& bCheckable = std::get<2>(it);
        const bool& bAddSpacer = std::get<3>(it);

        auto tb = new QToolButton();
        tb->setObjectName(btnName);
        tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tb->setAutoRaise(true);
        tb->setIcon(QIcon(":/resources/tool/" + btnName + ".svg"));
        tb->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * m_scal);
        tb->setContentsMargins(0, 0, 0, 0);
        tb->setFixedSize(QSize(ICON_WIDTH, ICON_HEIGHT) * m_scal);
        tb->setToolTip(btnTip);
        tb->setChecked(false);
        tb->setCheckable(bCheckable);
        addWidget(tb);

        if (bAddSpacer) addSpacer();
        connect(tb, &QToolButton::released, this, &SelectBar::onToolBtn);
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
//                icon = XHelper::instance().ChangeSVGColor(path, "path", XHelper::instance().borderColor(), QSize(ICON_WIDTH, ICON_WIDTH) * XHelper::instance().getScale());
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
    } else if (tb->objectName() == "custompath") {
        emit sigSelShape(XC::DrawShape::CustomPath, isChecked);
    } else if (tb->objectName() == "mosaic") {
        emit sigSelShape(XC::DrawShape::Mosaics, isChecked);
    } else if (tb->objectName() == "text") {
        emit sigSelShape(XC::DrawShape::Text, isChecked);
    } else if (tb->objectName() == "serialnumber") {
        emit sigSelShape(XC::DrawShape::SerialNumberShape, isChecked);
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
    if (!m_blur)
        return;

    m_blur->setGeometry(0, 0, width(), height());
    return QWidget::resizeEvent(event);
}

void SelectBar::paintEvent(QPaintEvent *event)
{
#if defined(Q_OS_MAC)
    Q_UNUSED(event)
//    updateToolBtnIcon();
    QPainter pa(this);
    pa.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(255, 255, 255, 0.7 * 255));

    const int round = 4;
    pa.drawRoundedRect(contentsRect().adjusted(1, 1, -1, -1), round, round);
#else
    QWidget::paintEvent(event);
#endif
}

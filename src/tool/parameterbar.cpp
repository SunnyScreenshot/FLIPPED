/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2022.06.22
 * Description: 绘画工具栏 下面那个具体参数 ToolBar
 ******************************************************************/
#include "parameterbar.h"
#include "../xglobal.h"
#include "../widget/xhorizontalline.h"
#include "../widget/xverticalline.h"
#include "../widget/xcombobox.h"

#include "../core/xlog.h"
#include <QColor>
#include <QSize>
#include <QPainter>
#include <QBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QButtonGroup>
#include <QAbstractButton>

//test
#include <QDebug>

ParameterBar::ParameterBar(Qt::Orientations orien, QWidget* parent)
    : XFrameWidget(parent)
    , m_scal(XHelp::getScale())
    , m_orien(orien)
    , m_layout(nullptr)
    //, m_widthBar(new WidthParaBar(orien))
    , m_colorBar(new ColorParaBar(orien))
    , m_serialBar(new XComboBox(this))
    , m_rectBar(nullptr)
    , m_ellipseBar(nullptr)
    , m_mosaicBar(nullptr)
    , m_arrowBar(nullptr)
    , m_lienWidthBar(nullptr)
{
    initUI();
}

void ParameterBar::addWidget(QWidget *w)
{
    if (m_layout && w)
        m_layout->addWidget(w);
}

void ParameterBar::addSpacer()
{
    if (!m_layout)
        return;

    if (m_orien == Qt::Horizontal)
        m_layout->addWidget(new XVerticalLine(SPACER_LINE_HEIGHT * m_scal, this));
    else
        m_layout->addWidget(new XHorizontalLine(SPACER_LINE_HEIGHT * m_scal, this));
}

void ParameterBar::creatorParaBar(QPointer<ManageBar>& manageBar, QString& path, QStringList items)
{
    if (!manageBar)
        manageBar = new ManageBar(Qt::Horizontal, this);

    QMap<QString, QString> map;
    for (int i = 0; i < items.size(); ++i) {
        map.insert(QString("bt%1").arg(i), path + items[i] + ".svg");
    }

    QButtonGroup* group = new QButtonGroup(this);
    group->setExclusive(true); // toolBtn 互斥

    auto it = map.begin();
    QToolButton* firstBtn = nullptr;
    for (int i = 0; i < items.size(); ++i) {
        QToolButton* tb = new QToolButton();
        tb->setIconSize(QSize(ICON_WIDTH * m_scal, ICON_WIDTH * m_scal));
        tb->setFixedSize(QSize(ICON_WIDTH * m_scal, ICON_WIDTH * m_scal));
        tb->setObjectName(it.key());
        tb->setIcon(QIcon(it.value()));
        tb->setAutoRaise(true);   // 自动浮动模式
        tb->setCheckable(true);
        tb->setChecked(false);
        if (i == 0) {  // 第一个为默认选中
            tb->setChecked(true);
            tb->setIcon(XHelp::changeSVGColor(it.value(), "rect", XHelp::highlightColor(), QSize(ICON_WIDTH, ICON_WIDTH) * XHelp::getScale()));
        }

        if (!tb->setProperty("path", it.value()))
            XLOG_INFO("ToolButton Property [path] initialization faile.");
        
        group->addButton(tb);
        manageBar->addWidget(tb);
        it++;
    }

    void (QButtonGroup::* sigFun)(QAbstractButton*) = &QButtonGroup::buttonReleased;
    connect(group, sigFun, this, &ParameterBar::onTBReleased);
}

void ParameterBar::initRectBar()
{
    QString path(":/resources/tool_para/rectangles/");
    QStringList list = { "rectangle",  "rectangle_fill" };

    creatorParaBar(m_rectBar, path, list);
}

void ParameterBar::initEllipseBar()
{
    QString path(":/resources/tool_para/ellipses/");
    QStringList list = { "ellipse",  "ellipse_fill" };
    creatorParaBar(m_ellipseBar, path, list);
}

void ParameterBar::initMosaicBar()
{
    QString path(":/resources/tool_para/mosaics/");
    QStringList list = { "pixelated",  "smooth" };
    creatorParaBar(m_mosaicBar, path, list);
}

void ParameterBar::initArrowBar()
{
    QString path(":/resources/tool_para/arrows/");
    QStringList list = { "arrow_1", "arrow_2", "arrow_3"};
    creatorParaBar(m_arrowBar, path, list);
}

void ParameterBar::initLineWidthBar()
{
    QString path(":/resources/tool_para/line_width/");
    QStringList list = { "line_width_1", "line_width_2", "line_width_3" };
    creatorParaBar(m_lienWidthBar, path, list);
}

void ParameterBar::removeAllBar()
{
    if (!m_layout)
        return;

    QWidgetList list;

    for (int i = 0; i < m_layout->count(); ++i) {
        QWidget* widget = m_layout->itemAt(i)->widget();

        if (widget)
            list << widget;
    }

    for (auto& it : list) {
        bool isXLine = false;
        if (m_orien == Qt::Horizontal) {
            auto t = qobject_cast<XVerticalLine*>(it);
            if (t)
                isXLine = true;

        } else {
            auto t = qobject_cast<XHorizontalLine*>(it);
            if (t)
                isXLine = true;
        }

        m_layout->removeWidget(it);
        it->setVisible(false);

        if (isXLine)  // 若是分割线直接销毁
            it->deleteLater();
    }
}

void ParameterBar::onEnableDraw(bool enable)
{
    setVisible(enable);
}

void ParameterBar::onSelShape(DrawShape shape, bool checked)
{
    //m_widthBar;
    //m_colorBar;
    //m_serialBar;

    //m_rectBar;
    //m_ellipseBar;
    //m_mosaicBar;
    //m_arrowBar;

    removeAllBar();

    int n = m_layout->count();
    if (shape == DrawShape::NoDraw) {
        XLOG_INFO("[shape] is DrawShape::NoDraw.");
    } else if (shape == DrawShape::Rectangles) {
        addWidget(m_rectBar);
        m_rectBar->setVisible(true);
        addSpacer();
        addWidget(m_lienWidthBar);
        m_lienWidthBar->setVisible(true);
        addWidget(m_colorBar);
        m_colorBar->setVisible(true);
    } else if (shape == DrawShape::Ellipses) {
        addWidget(m_ellipseBar);
        m_ellipseBar->setVisible(true);
        addSpacer();
        addWidget(m_lienWidthBar);
        m_lienWidthBar->setVisible(true);
        addSpacer();
        addWidget(m_colorBar);
        m_colorBar->setVisible(true);
    } else if (shape == DrawShape::Line) {
        addWidget(m_lienWidthBar);
        m_lienWidthBar->setVisible(true);
        addSpacer();
        addWidget(m_colorBar);
        m_colorBar->setVisible(true);
    } else if (shape == DrawShape::Arrows) {
        addWidget(m_arrowBar);
        m_arrowBar->setVisible(true);
        addSpacer();
        addWidget(m_lienWidthBar);
        m_lienWidthBar->setVisible(true);
        addSpacer();
        addWidget(m_colorBar);
        m_colorBar->setVisible(true);
    } else if (shape == DrawShape::Pen) {
        addWidget(m_lienWidthBar);
        m_lienWidthBar->setVisible(true);
        addSpacer();
        addWidget(m_colorBar);
        m_colorBar->setVisible(true);
    } else if (shape == DrawShape::Mosaics) {
        addWidget(m_mosaicBar);
        m_mosaicBar->setVisible(true);
        addSpacer();
        addWidget(m_lienWidthBar);
        m_lienWidthBar->setVisible(true);
    } else if (shape == DrawShape::Text) {
        addWidget(m_ellipseBar);        // TODO 2022.06.27： 需要替换为 font size 大小的 combobox
        m_ellipseBar->setVisible(true);   
        addSpacer();
        addWidget(m_colorBar);
        m_colorBar->setVisible(true);
    } else if (shape == DrawShape::SerialNumber) {
        addWidget(m_ellipseBar);        // TODO 2022.06.27： 还需要添加一个 font size 大小的 combobox
        m_ellipseBar->setVisible(true);
        addSpacer();
        addWidget(m_colorBar);
        m_colorBar->setVisible(true);
    } else {
        XLOG_ERROR("[shape] is not any enumeration.");
    }

    // TODO 2022.06.27: fix 此处添加一个更新设置边框 和 margin 的函数，根据放入的不同，来切换； 设置放在控件添加前面或者后面都尝试一下


    setVisible(m_layout->count());
    this->adjustSize();
    update();
}

void ParameterBar::onTBReleased(QAbstractButton* btn)
{
    const auto& parent = btn->parentWidget();
    if (!btn || !parent)
        return;

    for (auto& it : parent->findChildren<QToolButton *>()) {
        QString path = it->property("path").value<QString>();
        it->setIconSize(QSize(ICON_WIDTH, ICON_WIDTH) * XHelp::getScale());

        if (it == qobject_cast<QToolButton *>(btn)) {
            it->setIcon(XHelp::changeSVGColor(path, "rect", XHelp::highlightColor(), QSize(ICON_WIDTH, ICON_WIDTH) * XHelp::getScale()));
        } else {
            it->setIcon(QIcon(path));
        }
    }
}

void ParameterBar::initUI()
{
    if (m_orien == Qt::Horizontal)
        m_layout = new QHBoxLayout(this);
    else
        m_layout = new QVBoxLayout(this);

    initRectBar();
    initEllipseBar();
    initMosaicBar();
    initArrowBar();
    initLineWidthBar();

    if (m_colorBar)
        m_colorBar->setVisible(true);

    const int bbMarginHor = BAR_MARGIN_HOR;
    int bbMarginVer = BAR_MARGIN_VER;
    if (m_colorBar->isVisible()) {
        bbMarginVer = BAR_MARGIN_VER_HAS_COLOR_PARA;
        m_layout->setContentsMargins(bbMarginHor, 0, 0, 0);
    } else {
        m_layout->setContentsMargins(bbMarginHor, bbMarginVer, bbMarginHor, bbMarginVer);
    }

    setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(BAR_MARGIN_HOR);  // TODO 最后一个大概是两个这个间隔（间隔 + 取色盘自带的）
    
    m_serialBar->setFixedSize(COMBOBOX_WIDTH * m_scal, COMBOBOX_HEIGHT * m_scal);
    QStringList items;
    items << "①" << "a";
    m_serialBar->addItems(items);
    m_layout->addWidget(m_serialBar);

    m_layout->addWidget(m_rectBar);
    addSpacer();
    m_layout->addWidget(m_ellipseBar);
    addSpacer();
    m_layout->addWidget(m_mosaicBar);
    addSpacer();
    m_layout->addWidget(m_arrowBar);
    addSpacer();
    m_layout->addWidget(m_lienWidthBar);
    addSpacer();
    m_layout->addWidget(m_colorBar);


//    adjustSize();  // 布局后重新计算一下大小尺寸
//    resize(size().width() + bbMarginHor * 2, size().height() + bbMarginVer * 2);
}

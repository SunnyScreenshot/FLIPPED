//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/06.
//
#include "wintoolbar.h"
#include "winfullscreen.h"
#include <QToolButton>
#include <QStringList>
#include <QHBoxLayout>
#include <QDebug>
#include <QPainter>


WinToolBar::WinToolBar(QWidget *parent)
    : XRoundWidget(parent)
{
    init();
}

WinToolBar::~WinToolBar()
{

}

// 绘画栏是否处于点击状态;
bool WinToolBar::isToolBtnChecked()
{
    QList<QToolButton *> listBtn = findChildren<QToolButton *>();
    for (QToolButton* btn : listBtn) {
        if (btn && btn->isChecked())
            return true;
    }

    return false;
}

void WinToolBar::init()
{
    m_toolBtnName << "rectangle"
               << "ellipse"
               << "line"
               << "arrow"
               << "pen"
               << "text"
               << "mosaic"
               << "revoke"
               << "download"
               << "copy";

#if defined(Q_OS_MACOS)
    QStringList listToolTip = {tr("rectangle (⌘ + 1)")
                           , tr("ellipse (⌘ + 2)")
                           , tr("line (⌘ + 3)")
                           , tr("arrow (⌘ + 4)")
                           , tr("pen (⌘ + 5）")
                           , tr("text (⌘ + 6)")
                           , tr("mosaic (⌘ + 7)")
                           , tr("revoke (⌘ + z)")
                           , tr("download (⌘ + S)")
                           , tr("copy (⌘ + C)")};
#else
    QStringList listToolTip = {tr("rectangle (Ctrl + 1)")
                           , tr("ellipse (Ctrl + 2)")
                           , tr("line (Ctrl + 3)")
                           , tr("arrow (Ctrl + 4)")
                           , tr("pen (Ctrl + 5）")
                           , tr("text (Ctrl + 6)")
                           , tr("mosaic (Ctrl + 7)")
                           , tr("revoke (Ctrl + z)")
                           , tr("download (Ctrl + S)")
                           , tr("copy (Ctrl + C)")};
#endif

    m_vecToolBar.fill(nullptr, m_toolBtnName.count());
    setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout();
    const int margin = 8;
    hLayout->setContentsMargins(margin, margin, margin, margin);
    hLayout->setSpacing(0);
    setLayout(hLayout);

    for (int i = 0; i < m_toolBtnName.count(); ++i) {
        QString name = ":/resources/icons/normal/" + m_toolBtnName[i] + ".svg";

        m_vecToolBar[i] = new QToolButton();
        m_vecToolBar[i]->setObjectName(m_toolBtnName[i]);
        m_vecToolBar[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_vecToolBar[i]->setAutoRaise(true);   // 自动浮动模式
        m_vecToolBar[i]->setIcon(QIcon(name));
        m_vecToolBar[i]->setIconSize(QSize(16, 16) * WinFullScreen::getScale());
        m_vecToolBar[i]->setToolTip(listToolTip[i]);
        m_vecToolBar[i]->setChecked(false);

        // TODO 2021.11.17 后优化为 list 的最后三项
        if (m_vecToolBar[i]->objectName() == "revoke"
                || m_vecToolBar[i]->objectName() == "download"
                || m_vecToolBar[i]->objectName() == "copy")
            m_vecToolBar[i]->setCheckable(false);
        else
            m_vecToolBar[i]->setCheckable(true);

        qInfo()<<"===========>"<<name << m_vecToolBar[i]->iconSize()<<rect();
        hLayout->addWidget(m_vecToolBar[i]);
        connect(m_vecToolBar[i], &QToolButton::released, this, &WinToolBar::onToolBtn);
    }

}

void WinToolBar::onToolBtn()
{
    QObject* obj = sender();
    if (!obj)
        return;

    QToolButton* toolBtn = qobject_cast<QToolButton *>(obj);
    if (!toolBtn)
        return;

    // 仅单选
    bool bNoChecked = false;  // false 前面几个绘画按钮均为处于未点击状态,即非绘画 Drawing 状态
    QList<QToolButton *> listBtn = findChildren<QToolButton *>();
    for (QToolButton* btn : listBtn) {
        QString namePressed = ":/resources/icons/pressed/" + btn->objectName() + ".svg";
        QString nameNormal = ":/resources/icons/normal/" + btn->objectName() + ".svg";

        if (btn->isChecked() && btn == toolBtn) {
            if (btn->isChecked()) {
                btn->setIcon(QIcon(namePressed));
                bNoChecked = true;
            } else {
                btn->setIcon(QIcon(nameNormal));
            }

            continue;
        }

        btn->setChecked(false);
        if (btn->isChecked()) {
            btn->setIcon(QIcon(namePressed));
            bNoChecked = true;
        } else {
            btn->setIcon(QIcon(nameNormal));
        }
    }

    if (bNoChecked)
        emit sigDrawStart();
    else
        emit sigDrawEnd();

    m_toolBtnName << "rectangle"
               << "ellipse"
               << "line"
               << "arrow"
               << "pen"
               << "text"
               << "mosaic"
               << "revoke"
               << "download"
               << "copy";
    // 发射信号
    if (toolBtn->objectName() == "rectangle") {
        emit sigDrawShape(XDrawShape::Rectangles);
    } else if (toolBtn->objectName() == "ellipse") {
        emit sigDrawShape(XDrawShape::Ellipses);
    } else if (toolBtn->objectName() == "line") {
        emit sigDrawShape(XDrawShape::Lines);
    } else if (toolBtn->objectName() == "arrow") {
        emit sigDrawShape(XDrawShape::Arrows);
//    } else if (toolBtn->objectName() == "pen") {
//        emit sigDrawShape(XDrawShape::Pen);
    } else if (toolBtn->objectName() == "text") {
        emit sigDrawShape(XDrawShape::Texts);
    } else if (toolBtn->objectName() == "mosaic") {
        emit sigDrawShape(XDrawShape::Mosaics);
    } else if (toolBtn->objectName() == "revoke") {
        emit sigRevoke();
    } else if (toolBtn->objectName() == "download") {
        emit sigDownload();
    } else if (toolBtn->objectName() == "copy") {
        emit sigCopy();
    }

}


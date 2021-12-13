//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/06.
//
#include "subgraptoolbar.h"
#include "screenshot.h"
#include "../xglobal.h"
#include <QToolButton>
#include <QStringList>
#include <QHBoxLayout>
#include <QDebug>
#include <QPainter>


SubGrapToolBar::SubGrapToolBar(QWidget *parent)
    : QWidget(parent)
{
    init();
}

SubGrapToolBar::~SubGrapToolBar()
{

}

// 绘画栏是否处于点击状态;
bool SubGrapToolBar::isToolBtnChecked()
{
    QList<QToolButton *> listBtn = findChildren<QToolButton *>();
    for (QToolButton* btn : listBtn) {
        if (btn && btn->isChecked())
            return true;
    }

    return false;
}

QToolButton* SubGrapToolBar::getCheckedToolBtn(int ignoreLastBtnNum)
{
    QList<QToolButton *> listBtn = findChildren<QToolButton *>();
    for (auto it = listBtn.begin(); it != listBtn.end() - ignoreLastBtnNum ; ++it) {
        if ((*it)->isChecked())
            return *it;
    }

    return nullptr;
}

void SubGrapToolBar::init()
{
    m_toolBtnName << "rectangle"
               << "ellipse"
               << "arrow"
               << "pen"
               << "text"
               << "mosaic"
               << "undo"
               << "redo"
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
    QStringList listToolTip = {tr("rectangle")
                           , tr("ellipse")
                           , tr("arrow")
                           , tr("pen")
                           , tr("text")
                           , tr("mosaic")
                           , tr("undo")
                           , tr("redo")
                           , tr("download")
                           , tr("copy")};
#endif

    m_vecToolBar.fill(nullptr, m_toolBtnName.count());
    setContentsMargins(0, 0, 0, 0);

    const int margin = 0;
    QHBoxLayout* hLayout = new QHBoxLayout();
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
        m_vecToolBar[i]->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT) * ScreenShot::getScale());
        m_vecToolBar[i]->setToolTip(listToolTip[i]);
        m_vecToolBar[i]->setChecked(false);

        // TODO 2021.11.17 后优化为 list 的最后 4 项
        if (m_vecToolBar[i]->objectName() == "undo"
                || m_vecToolBar[i]->objectName() == "redo"
                || m_vecToolBar[i]->objectName() == "download"
                || m_vecToolBar[i]->objectName() == "copy")
            m_vecToolBar[i]->setCheckable(false);
        else
            m_vecToolBar[i]->setCheckable(true);

        qInfo()<<"===========>"<<name << m_vecToolBar[i]->iconSize()<<rect() << ScreenShot::getScale();
        hLayout->addWidget(m_vecToolBar[i]);
        connect(m_vecToolBar[i], &QToolButton::released, this, &SubGrapToolBar::onToolBtn);
    }
}

void SubGrapToolBar::onToolBtn()
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

//    m_toolBtnName << "rectangle"
//               << "ellipse"
//               << "arrow"
//               << "pen"
//               << "text"
//               << "mosaic"
//               << "undo"
//               << "redo"
//               << "download"
//               << "copy";
    // 发射信号
    bool isChecked = toolBtn->isChecked();
    if (toolBtn->objectName() == "rectangle") {
        emit sigDrawShape(XDrawShape::Rectangles, isChecked);
    } else if (toolBtn->objectName() == "ellipse") {
        emit sigDrawShape(XDrawShape::Ellipses, isChecked);
    } else if (toolBtn->objectName() == "arrow") {
        emit sigDrawShape(XDrawShape::Arrows, isChecked);
//    } else if (toolBtn->objectName() == "pen") {
//        emit sigDrawShape(XDrawShape::Pen);
    } else if (toolBtn->objectName() == "text") {
        emit sigDrawShape(XDrawShape::Texts, isChecked);
    } else if (toolBtn->objectName() == "mosaic") {
        emit sigDrawShape(XDrawShape::Mosaics, isChecked);
    } else if (toolBtn->objectName() == "undo") {
        emit sigUndo();
    } else if (toolBtn->objectName() == "redo") {
        emit sigRedo();
    } else if (toolBtn->objectName() == "download") {
        emit sigDownload();
    } else if (toolBtn->objectName() == "copy") {
        emit sigCopy();
    }

}


//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/06.
//
#include "wintoolbar.h"
#include "winfullscreen.h"
#include <QToolButton>
#include <QStringList>
#include <QHBoxLayout>
#include <QDebug>


WinToolBar::WinToolBar(QWidget *parent)
    : XRoundWidget(parent)
{
    init();
}

WinToolBar::~WinToolBar()
{

}

void WinToolBar::init()
{
    m_toolBtnName /*<< "rectangle"
               << "ellipse"
               << "line"
               << "arrow"
               << "pen"
               << "text"
               << "mosaic"
               << "revoke"
               << "update" */
               << "download"
               << "copy";

#if defined(Q_OS_MACOS)
    QStringList listToolTip = {/*tr("矩形（⌘ + 1）")
                           , tr("椭圆（⌘ + 2）")
                           , tr("直线（⌘ + 3）")
                           , tr("箭头（⌘ + 4）")
                           , tr("笔（⌘ + 5）")
                           , tr("文字（⌘ + 6）")
                           , tr("模糊（⌘ + 7）")
                           , tr("撤销（⌘ + z）")
                           , tr("上传（⌃ + ⇧ + 1）")
                           , */ tr("保存（⌘ + s）")
                           , tr("复制到剪切板 (return)") };
#else
    QStringList listToolTip = {/*tr("矩形（Ctrl + 1）")
                           , tr("椭圆（Ctrl + 2）")
                           , tr("直线（Ctrl + 3）")
                           , tr("箭头（Ctrl + 4）")
                           , tr("笔（Ctrl + 5）")
                           , tr("文字（Ctrl + 6）")
                           , tr("模糊（Ctrl + 7）")
                           , tr("撤销（Ctrl + z）")
                           , tr("上传（Ctrl + Shift + 1）")
                           , */ tr("保存（Ctrl + S）")
                           , tr("复制到剪切板 (Ctrl + C)")};
#endif

    m_vecToolBar.fill(nullptr, m_toolBtnName.count());
    setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(4, 4, 4, 4);
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
        m_vecToolBar[i]->setCheckable(false);
        m_vecToolBar[i]->setChecked(false);

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

    QString namePressed = ":/resources/icons/pressed/" + toolBtn->objectName() + ".svg";
    QString nameNormal = ":/resources/icons/normal/" + toolBtn->objectName() + ".svg";

    if (toolBtn->isChecked()) {
        toolBtn->setIcon(QIcon(namePressed));
    } else {
        toolBtn->setIcon(QIcon(nameNormal));
    }

    if (toolBtn->objectName() == "download") {
        emit sigDownload();
    } else if (toolBtn->objectName() == "copy") {
        emit sigCopy();
    }
}


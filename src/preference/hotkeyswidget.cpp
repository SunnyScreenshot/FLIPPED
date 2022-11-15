/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.17
 * Description: 偏好设置界面的 hotkeys UI；为了规避掉 编辑框 鼠标点击外面却仍有焦点
 ******************************************************************/
#include "hotkeyswidget.h"
#include "../xglobal.h"
#include "../screen/tray.h"
#include "../widget/xhorizontalline.h"
#include "../widget/xkeysequenceedit.h"
#include "../screen/drawhelper.h"
#include "appellation.h"
#include <QBoxLayout>
#include <QList>
#include <QMouseEvent>
#include <QPushButton>
#include <map>

HotkeysWidget::HotkeysWidget(QWidget *parent) : QWidget(parent)
{
    double m_scale = XHelper::instance().getScale();

    setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vLay = new QVBoxLayout(this);
    vLay->setContentsMargins(THV_MARGIN_HOR, THV_MARGIN_VER_TOP, THV_MARGIN_HOR, THV_MARGIN_VER_BOTTOM);

    int i = 0;
    int j = 0;
    QGridLayout* grid = new QGridLayout();
    grid->setMargin(0);
    grid->setVerticalSpacing(THG_SPACING_VER);
    grid->setHorizontalSpacing(THG_SPACING_HOR);
    grid->setColumnStretch(0, 8);
    grid->setColumnStretch(1, 9);

    QStringList list = { "Ctrl+Shift+Y", "Ctrl+Shift+W", "Ctrl+Shift+L", "Ctrl+Shift+S", "Ctrl+Shift+F", "Ctrl+Shift+T", "Ctrl+Shift+H", "Ctrl+Shift+X" };
    std::map<XKeySequenceEdit*, const QString> vHkEdit;
    int idx = 0;
    for (auto& it : Tray::instance().getVHotKeys()) {
        // TODO 2022.07.17: 虽然是值传递，但 std::get<0>(it) 为空，以后研究下
        
        QString& hotkey = std::get<1>(it);
        QString& describe = std::get<2>(it);
        XKeySequenceEdit* pEdit = new XKeySequenceEdit(QKeySequence(hotkey));
        vHkEdit.insert(std::make_pair(pEdit, list.at(idx++)));
        pEdit->setObjectName(describe);
        pEdit->setMinimumWidth(110 * m_scale);

        grid->addWidget(new QLabel(describe + ":"), i, j, 1, 1, Qt::AlignRight);
        grid->addWidget(pEdit, i++, j + 1, 1, 1, Qt::AlignLeft);
        connect(pEdit, &XKeySequenceEdit::sigKeySeqChanged, &(Tray::instance()), &Tray::onKeySequenceChanged);

        if (i == 5)
            grid->addWidget(new XHorizontalLine(contentsRect().width() * 3 / 4 - THV_MARGIN_HOR * m_scale * 2), i++, j, 1, grid->columnCount(), Qt::AlignCenter);
    }

    qDebug() << "tabHotkeys:grid->rowCount():" << grid->rowCount();
    vLay->addLayout(grid, grid->rowCount());
    vLay->addStretch(3);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->setContentsMargins(0, 0, 0, 0);
    hLay->addSpacing(0);
    hLay->addStretch(7);
    auto btn = new QPushButton(tr("Reset"), this);
    if (btn) {
        btn->setObjectName(thReset);
        hLay->addWidget(btn, 1, Qt::AlignRight);

        connect(btn, &QPushButton::released, this, [vHkEdit]() {
            for (auto& it : vHkEdit) {
                it.first->setKeySequence(it.second);
                emit it.first->sigKeySeqChanged(it.first->keySequence());
            }});
    }
    vLay->addLayout(hLay, 1);
}

void HotkeysWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    // 点击 快捷键编辑框 之外，焦点消失
    QList<XKeySequenceEdit*> list = findChildren<XKeySequenceEdit*>();
    for (const auto& it : list) {
        if (!it->geometry().contains(mapFromGlobal(QCursor::pos())))
            it->clearFocus();
    }
}

#include "drawtoolbar.h"
#include "../widget/xhorizontalline.h"
#include <QBoxLayout>

DrawToolBar::DrawToolBar(QWidget *parent)
    : XRoundWidget(parent)
    , m_subGrapTb(new SubGrapToolBar(this))
    , m_subRectTb(new SubRectToolBar(this))
    , m_subEllipseTb(new SubEllipseToolBar(this))
    , m_vLayout(new QVBoxLayout())
    , m_hLine(nullptr)
{
    initUI();
}

void DrawToolBar::initUI()
{
    const int margin = 4;
    setContentsMargins(0, 0, 0, 0);
    m_vLayout->setContentsMargins(margin, margin, margin, margin);
    m_vLayout->setSpacing(0);
    m_vLayout->addWidget(m_subGrapTb);
    m_subGrapTb->show();
    setLayout(m_vLayout);

    m_subRectTb->hide();
    m_subEllipseTb->hide();
    m_hLine = new XHorizontalLine(m_subGrapTb->width() - margin * 2, this);
    m_hLine->hide();

    connect(m_subGrapTb, &SubGrapToolBar::sigDrawShape, this, &DrawToolBar::onDrawShape);
}

void DrawToolBar::onDrawShape(XDrawShape shape, bool checked)
{
    switch (shape) {
    case XDrawShape::Rectangles: {
        if (checked) {
            m_vLayout->addWidget(m_subRectTb);
            m_vLayout->removeWidget(m_subEllipseTb);
            m_subRectTb->show();
            m_subEllipseTb->hide();
        } else {
            m_vLayout->removeWidget(m_subRectTb);
            m_subRectTb->hide();
        }

        break;
    }
    case XDrawShape::Ellipses: {
        if (checked) {
            m_vLayout->addWidget(m_subEllipseTb);
            m_vLayout->removeWidget(m_subRectTb);
            m_subEllipseTb->show();
            m_subRectTb->hide();
        } else {
            m_vLayout->removeWidget(m_subEllipseTb);
            m_subEllipseTb->hide();
        }

        break;
    }
    default:
        break;
    }

    // TODO 2021.12.14:可优化，暂固定死不同状态的高度
    setFixedHeight(50);

    if (checked) {
        m_vLayout->insertWidget(1, m_hLine);
        m_hLine->show();
        setFixedHeight(100);
    } else {
        m_vLayout->removeWidget(m_hLine);
        m_hLine->hide();
    }
}

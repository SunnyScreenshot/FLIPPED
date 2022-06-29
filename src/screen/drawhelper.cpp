/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2021.11.09
 * Description: 
 ******************************************************************/
#include "drawhelper.h"

#include <QScreen>
#include <QFile>
#include <QByteArray>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QSvgRenderer>
#include <QDomElement>
#include <QDomElement>
#include <QIcon>


int XDrawStep::g_index = 0;

ScrnHelper::ScrnHelper(QObject *parent)
    : QObject(parent)
{
}

ScrnHelper::~ScrnHelper()
{
}


// 辅助功能函数
namespace XHelp {

/*!
 * \brief changeSVGColor 修改 .svg 的填充色
 * \param path   .svg 文件路径
 * \param shape  欲修改的图形， 通常是 path、偶尔是 rect 等
 * \param color  填充的颜色
 * \param size   svg 伸缩后的尺寸
 * \return 一个 QIcon 对象
 */
QIcon changeSVGColor(QString path, QString shape, QColor color, QSize size)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();

    QDomDocument doc;
    doc.setContent(baData);
    QDomElement elem = doc.documentElement(); // const 和 值传递
    setAttrRecur(elem, shape, "fill", color.name());

    QSvgRenderer svgRenderer(doc.toByteArray());
    // create pixmap target (could be a QImage)
    QPixmap pix(svgRenderer.defaultSize().scaled(size.width(), size.height(), Qt::KeepAspectRatio));  // fix: 修改 svg 颜色后略有模糊
    pix.fill(Qt::transparent);
    // create painter to act over pixmap
    QPainter pixPainter(&pix);
    // use renderer to render over painter which paints on pixmap
    svgRenderer.render(&pixPainter);
    QIcon myicon(pix);
    return myicon;
}

void setAttrRecur(QDomElement &elem, QString strtagname, QString strattr, QString strattrval)
{
    // if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname) == 0)
        elem.setAttribute(strattr, strattrval);

    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++) {
        if (!elem.childNodes().at(i).isElement())
            continue;

        QDomElement t = elem.childNodes().at(i).toElement();
        setAttrRecur(t, strtagname, strattr, strattrval);
    }
}

double getScale(QScreen *screen)
{
// or defined(Q_WS_WIN) || defined(Q_WS_X11)
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)
    double scale = screen->logicalDotsPerInch() / 96.0;
    if (scale < 1.25)
        return 1;
    else if (1.25 <= scale && scale < 1.5)
        return 1.25;
    else if (1.5 <= scale && scale < 1.75)
        return 1.5;
    else if (1.75 <= scale && scale < 2)
        return 1.75;
    else if (2 <= scale && scale < 2.25)
        return 2;
    else if (2.25 <= scale && scale < 2.5)
        return 2.25;
    else if (2.5 <= scale && scale < 3)
        return 2.5;
    else if (3 <= scale && scale < 3.5)
        return 3;
    else if (3.5 <= scale && scale < 4)
        return 3.5;
    else
        return scale;
#elif  defined(Q_OS_MAC)
    double scale = screen->logicalDotsPerInch() / 72.0;
    return scale;
#else
    return screen->logicalDotsPerInch() / 96.0;  //
#endif
}

QColor highlightColor(QColor highlight)
{
    highlight.setAlphaF(1);
    return highlight;
}
}



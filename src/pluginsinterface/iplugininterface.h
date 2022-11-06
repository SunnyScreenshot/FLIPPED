/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.04.17
 * Description:
 ******************************************************************/
#ifndef IPLUGININTERFACE_H
#define IPLUGININTERFACE_H

#include <QWidget>
#include <QString>

class IPluginInterface
{
public:
    virtual ~IPluginInterface() {};

    virtual const QString plugName() const { return QString("test plugName");}
    virtual QWidget*itemWidget() { return nullptr;}
};


QT_BEGIN_NAMESPACE
#define IPluginInterface_iid "XMuli.Flipped.PluginsInterface"  // 唯一标识符
Q_DECLARE_INTERFACE(IPluginInterface, IPluginInterface_iid)
QT_END_NAMESPACE

#endif // IPLUGININTERFACE_H



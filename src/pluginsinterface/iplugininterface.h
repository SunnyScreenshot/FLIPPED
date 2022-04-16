#ifndef IPLUGININTERFACE_H
#define IPLUGININTERFACE_H

#include <QWidget>
#include <QString>

class IPluginInterface
{
public:
    virtual ~IPluginInterface() {};

    virtual const QString plugName() const { return QString("test plugName");}
    virtual QWidget*itemWidget(const QString &itemKey) { return nullptr;}
};


QT_BEGIN_NAMESPACE
#define IPluginInterface_iid "XMuli.PicShot.PluginsInterface"  // 唯一标识符
Q_DECLARE_INTERFACE(IPluginInterface, IPluginInterface_iid)
QT_END_NAMESPACE

#endif // IPLUGININTERFACE_H



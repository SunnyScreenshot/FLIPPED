#include "pluginwatemark.h"
#include <QLabel>

PluginWateMark::PluginWateMark(QObject *parent) : QObject(parent)
{

}

const QString PluginWateMark::plugName() const
{
    return "WateMark";
}

QWidget* PluginWateMark::itemWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey);
    return new QLabel("test label");
}

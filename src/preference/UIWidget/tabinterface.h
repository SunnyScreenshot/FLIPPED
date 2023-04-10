#ifndef TABINTERFACE_H
#define TABINTERFACE_H

#include <QWidget>

namespace Ui {
class TabInterface;
}

class TabInterface : public QWidget
{
    Q_OBJECT

public:
    explicit TabInterface(QWidget *parent = nullptr);
    ~TabInterface();

private:
    Ui::TabInterface *ui;
};

#endif // TABINTERFACE_H

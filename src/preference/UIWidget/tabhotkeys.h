#ifndef TABHOTKEYS_H
#define TABHOTKEYS_H

#include <QWidget>

namespace Ui {
class TabHotkeys;
}

class TabHotkeys : public QWidget
{
    Q_OBJECT

public:
    explicit TabHotkeys(QWidget *parent = nullptr);
    ~TabHotkeys();

private:
    Ui::TabHotkeys *ui;
};

#endif // TABHOTKEYS_H

#ifndef TABPIN_H
#define TABPIN_H

#include <QWidget>

namespace Ui {
class TabPin;
}

class TabPin : public QWidget
{
    Q_OBJECT

public:
    explicit TabPin(QWidget *parent = nullptr);
    ~TabPin();

private:
    Ui::TabPin *ui;
};

#endif // TABPIN_H

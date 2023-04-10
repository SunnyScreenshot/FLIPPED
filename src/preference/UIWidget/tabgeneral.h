#ifndef TABGENERAL_H
#define TABGENERAL_H

#include <QWidget>

namespace Ui {
class TabGeneral;
}

class TabGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit TabGeneral(QWidget *parent = nullptr);
    ~TabGeneral();

private:
    Ui::TabGeneral *ui;
};

#endif // TABGENERAL_H

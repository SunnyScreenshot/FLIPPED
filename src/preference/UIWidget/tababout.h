#ifndef TABABOUT_H
#define TABABOUT_H

#include <QWidget>

namespace Ui {
class TabAbout;
}

class TabAbout : public QWidget
{
    Q_OBJECT

public:
    explicit TabAbout(QWidget *parent = nullptr);
    ~TabAbout();

private:
    Ui::TabAbout *ui;
};

#endif // TABABOUT_H

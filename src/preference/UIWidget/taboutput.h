#ifndef TABOUTPUT_H
#define TABOUTPUT_H

#include <QWidget>

namespace Ui {
class TabOutput;
}

class TabOutput : public QWidget
{
    Q_OBJECT

public:
    explicit TabOutput(QWidget *parent = nullptr);
    ~TabOutput();

private:
    Ui::TabOutput *ui;
};

#endif // TABOUTPUT_H

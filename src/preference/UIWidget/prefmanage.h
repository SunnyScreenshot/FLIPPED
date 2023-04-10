#ifndef PREFMANAGE_H
#define PREFMANAGE_H

#include <QWidget>

namespace Ui {
class PrefManage;
}

class PrefManage : public QWidget
{
    Q_OBJECT

public:
    explicit PrefManage(QWidget *parent = nullptr);
    ~PrefManage();

private:
    Ui::PrefManage *ui;
};

#endif // PREFMANAGE_H

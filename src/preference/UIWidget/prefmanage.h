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

private slots:
    void on_generalReset_released();
    void on_interfaceReset_released();
    void on_outputReset_released();
    void on_pinReset_released();
    void on_hotkeysReset_released();

private:
    void initUIInterface();
    void initUIHotkeys();
    void initUIAbout();
    void initUI();

private:
    Ui::PrefManage *ui;
};

#endif // PREFMANAGE_H

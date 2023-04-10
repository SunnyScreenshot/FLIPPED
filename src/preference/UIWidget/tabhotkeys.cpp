#include "tabhotkeys.h"
#include "ui_tabhotkeys.h"

TabHotkeys::TabHotkeys(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabHotkeys)
{
    ui->setupUi(this);
}

TabHotkeys::~TabHotkeys()
{
    delete ui;
}

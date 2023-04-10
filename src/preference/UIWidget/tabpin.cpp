#include "tabpin.h"
#include "ui_tabpin.h"

TabPin::TabPin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabPin)
{
    ui->setupUi(this);
}

TabPin::~TabPin()
{
    delete ui;
}

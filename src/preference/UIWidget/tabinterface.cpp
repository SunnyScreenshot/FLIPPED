#include "tabinterface.h"
#include "ui_tabinterface.h"

TabInterface::TabInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabInterface)
{
    ui->setupUi(this);
}

TabInterface::~TabInterface()
{
    delete ui;
}

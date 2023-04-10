#include "tabgeneral.h"
#include "ui_tabgeneral.h"

TabGeneral::TabGeneral(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabGeneral)
{
    ui->setupUi(this);
}

TabGeneral::~TabGeneral()
{
    delete ui;
}

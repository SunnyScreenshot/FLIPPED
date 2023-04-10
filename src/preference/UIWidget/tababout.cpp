#include "tababout.h"
#include "ui_tababout.h"

TabAbout::TabAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabAbout)
{
    ui->setupUi(this);
}

TabAbout::~TabAbout()
{
    delete ui;
}

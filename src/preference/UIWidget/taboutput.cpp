#include "taboutput.h"
#include "ui_taboutput.h"

TabOutput::TabOutput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabOutput)
{
    ui->setupUi(this);
}

TabOutput::~TabOutput()
{
    delete ui;
}

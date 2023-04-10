#include "prefmanage.h"
#include "ui_prefmanage.h"

PrefManage::PrefManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrefManage)
{
    ui->setupUi(this);
}

PrefManage::~PrefManage()
{
    delete ui;
}

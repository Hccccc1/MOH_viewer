#include "rtvalues.h"
#include "ui_rtvalues.h"

RTValues::RTValues(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RTValues)
{
    ui->setupUi(this);
}

RTValues::~RTValues()
{
    delete ui;
}

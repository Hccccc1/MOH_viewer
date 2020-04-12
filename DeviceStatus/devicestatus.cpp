#include "devicestatus.h"
#include "ui_devicestatus.h"

DeviceStatus::DeviceStatus(QWidget *parent, uint8_t model) :
    QWidget(parent),
    ui(new Ui::DeviceStatus)
{
    ui->setupUi(this);

    current_model = model;

    ui->label->setText(QString::number(model));
}

DeviceStatus::~DeviceStatus()
{
    delete ui;
}

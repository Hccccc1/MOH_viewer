#include "devicelog.h"
#include "ui_devicelog.h"

DeviceLog::DeviceLog(QWidget *parent, uint8_t model) :
    QWidget(parent),
    ui(new Ui::DeviceLog)
{
    ui->setupUi(this);

    current_model = model;

    ui->label->setText(QString::number(model));
}

DeviceLog::~DeviceLog()
{
    delete ui;
}

#include "dataoverview.h"
#include "ui_dataoverview.h"
#include "DeviceStatus/DevStatus_regs.h"

DataOverview::DataOverview(QWidget *parent, ModbusSerial *serial) :
    QDialog(parent),
    ui(new Ui::DataOverview),
    current_serial(serial)
{
    ui->setupUi(this);
}

DataOverview::~DataOverview()
{
    delete ui;
}

void DataOverview::data_process(QModbusDataUnit unit)
{
    for (int i = 0, total = unit.valueCount(); i < total; i++)
    {

    }
}

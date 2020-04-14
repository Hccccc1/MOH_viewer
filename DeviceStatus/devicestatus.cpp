#include "devicestatus.h"
#include "ui_devicestatus.h"

#include "DeviceStatus/DevStatus_regs.h"

DeviceStatus::DeviceStatus(QWidget *parent, ModbusSerial *current_serial, uint8_t model) :
    QWidget(parent),
    ui(new Ui::DeviceStatus),
    serial(current_serial),
    current_model(model)
{
    ui->setupUi(this);

    ui->label->setText(QString::number(model));
}

DeviceStatus::~DeviceStatus()
{
    delete ui;
}

void DeviceStatus::onReadyRead()
{
    qDebug() << this->objectName() << __func__ << __LINE__;

    auto reply = qobject_cast<QModbusReply *>(sender());

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
        {
            unit.values();
        }
    }
}

//void DeviceStatus::on_readBtn_clicked()
//{
//    serial->read_from_modbus(QModbusDataUnit::Coils, TT_01, 77);
//}

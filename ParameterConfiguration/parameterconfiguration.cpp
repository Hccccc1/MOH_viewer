#include "parameterconfiguration.h"
#include "ui_parameterconfiguration.h"
#include "AllBitsAndRegs.h"

ParameterConfiguration::ParameterConfiguration(QWidget *parent, ModbusSerial *serial, uint8_t model) :
    QWidget(parent),
    ui(new Ui::ParameterConfiguration),
    current_serial(serial),
    current_model(model)
{
    ui->setupUi(this);

}

ParameterConfiguration::~ParameterConfiguration()
{
    delete ui;
}

void ParameterConfiguration::on_readData_clicked()
{
    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Manufacturer, 23);
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 1);
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL01, 30);
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_DataStorageCycle, 13);
    }
}

void ParameterConfiguration::onReadyRead()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        for (int i = 0, total = int(unit.valueCount()); i < total; i++)
        {
            int addr = unit.startAddress() + i;

            switch (addr)
            {
            case HoldingRegs_Manufacturer:
                if (unit.value(addr) == 1)
                {
                    m_parameters.manufactor = QString("摩氢科技");
                    ui->manufacturer->setText(m_parameters.manufactor);
                }
                break;
            case HoldingRegs_ProductName:
                break;
            case HoldingRegs_ProductType:
                break;
            case HoldingRegs_ProductSerialNum:
                m_parameters.product_serial_num = QString::number(unit.value(addr), 16).toUtf8();
                m_parameters.product_serial_num += QString::number(unit.value(addr+1), 16).toUtf8();
                m_parameters.product_serial_num += QString::number(unit.value(addr+2), 16).toUtf8();
                m_parameters.product_serial_num += QString::number(unit.value(addr+3), 16).toUtf8();

                ui->productSerialNum->setText(m_parameters.product_serial_num);
                break;
            case HoldingRegs_ProductDate:
                m_parameters.product_date = "%1\\%2\\%3";
                m_parameters.product_date = m_parameters.product_date.arg(unit.value(addr))
                                                                     .arg((unit.value(addr+1) & 0xff00) >> 8)
                                                                     .arg(unit.value(addr+1)& 0x00ff);

                ui->productionDate->setText(m_parameters.product_date);
                break;
            case HoldingRegs_FirmwareVersion:
                m_parameters.firmware_version = "%1.%2.%3";
                m_parameters.firmware_version = m_parameters.firmware_version.arg(unit.value(addr)/100)
                                                                             .arg((unit.value(addr)/100)/10)
                                                                             .arg(unit.value(addr)%10);
            }
        }
    }
}

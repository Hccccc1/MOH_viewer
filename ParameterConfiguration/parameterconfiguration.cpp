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
                    m_parameters.manufactor = 1;
                    ui->manufacturer->setText(tr("摩氢科技"));
                }
                break;
            case HoldingRegs_ProductName:
                break;
            case HoldingRegs_ProductType:
                break;
            case HoldingRegs_ProductSerialNum:
                m_parameters.product_serial_num[0] = unit.value(addr);
                m_parameters.product_serial_num[1] = unit.value(addr+1);
                m_parameters.product_serial_num[2] = unit.value(addr+2);
                m_parameters.product_serial_num[3] = unit.value(addr+3);

                ui->productSerialNum->setText(QString("%1%2%3%4").arg(QString(m_parameters.product_serial_num[0], 16))
                                                                 .arg(QString(m_parameters.product_serial_num[1], 16))
                                                                 .arg(QString(m_parameters.product_serial_num[2], 16))
                                                                 .arg(QString(m_parameters.product_serial_num[3], 16)));
                break;
            case HoldingRegs_ProductDate:
                m_parameters.product_date[0] = unit.value(addr);
                m_parameters.product_date[1] = unit.value(addr+1);

                ui->productionDate->setText(QString("%1\\%2\\%3").arg(QString::number(m_parameters.product_date[0]))
                                                                 .arg(QString::number((m_parameters.product_date[1]&0xff00)>>8))
                                                                 .arg(QString::number(m_parameters.product_date[1]&0x00ff)));

                break;
            case HoldingRegs_ReformingID:
                m_parameters.reforming_ID[0] = unit.value(addr);
                m_parameters.reforming_ID[1] = unit.value(addr+1);
                m_parameters.reforming_ID[2] = unit.value(addr+2);
                m_parameters.reforming_ID[3] = unit.value(addr+3);

                ui->reformingID->setText(QString("%1%2%3%4").arg(QString::number(m_parameters.reforming_ID[0], 16))
                                                            .arg(QString::number(m_parameters.reforming_ID[1], 16))
                                                            .arg(QString::number(m_parameters.reforming_ID[2], 16))
                                                            .arg(QString::number(m_parameters.reforming_ID[3], 16)));

                break;
            case HoldingRegs_StackID:
                m_parameters.stack_ID[0] = unit.value(addr);
                m_parameters.stack_ID[1] = unit.value(addr+1);
                m_parameters.stack_ID[2] = unit.value(addr+2);
                m_parameters.stack_ID[3] = unit.value(addr+3);

                ui->stackID->setText(QString("%1%2%3%4").arg(QString::number(m_parameters.stack_ID[0], 16))
                                                        .arg(QString::number(m_parameters.stack_ID[1], 16))
                                                        .arg(QString::number(m_parameters.stack_ID[2], 16))
                                                        .arg(QString::number(m_parameters.stack_ID[3], 16)));
                break;
            case HoldingRegs_DevSlaveAddr:
                m_parameters.dev_slave_addr = unit.value(addr);
                ui->devSlaveAddr->setValue(m_parameters.dev_slave_addr);
                break;
            case HoldingRegs_DevIPAddr:
                m_parameters.dev_IP_addr[0] = unit.value(addr);
                m_parameters.dev_IP_addr[1] = unit.value(addr+1);

                ui->devIPAddr->setText(QString("%1.%2.%3.%4").arg(QString::number((m_parameters.dev_IP_addr[0]&0xff00)>>8))
                                                             .arg(QString::number(m_parameters.dev_IP_addr[0]&0x00ff))
                                                             .arg(QString::number((m_parameters.dev_IP_addr[1]&0xff00)>>8))
                                                             .arg(QString::number((m_parameters.dev_IP_addr[1]&0x00ff))));
                break;
            case HoldingRegs_SerialPara:
                m_parameters.serial_paras = unit.value(addr);
                ui->serialPara->setValue(m_parameters.serial_paras);
                break;
            case HoldingRegs_PowerMode:
                m_parameters.power_mode = unit.value(addr);
                ui->sysGenerateMode->setValue(m_parameters.power_mode);
                break;
            case HoldingRegs_FCOutCurrent:
                m_parameters.fc_output_current = unit.value(addr);
                ui->FCOutputCurrent->setValue(double(m_parameters.fc_output_current)/10);
                break;
            case HoldingRegs_FCOutPower:
                m_parameters.fc_output_power = unit.value(addr);
                ui->FCOutputPower->setValue(m_parameters.fc_output_power);
                break;
            case HoldingRegs_BatChargeStartVoltage:
                m_parameters.bat_charge_start_voltage = unit.value(addr);
                ui->batChargeStartVoltage->setValue(m_parameters.bat_charge_start_voltage);
                break;
            case HoldingRegs_BatChargeStartDelay:
                m_parameters.charge_start_delay = unit.value(addr);
                ui->batChargeStartDelay->setValue(m_parameters.charge_start_delay);
                break;
            case HoldingRegs_BatChargeStopVoltage:
                m_parameters.bat_charge_stop_voltage = unit.value(addr);
                ui->batChargeStopVoltage->setValue(m_parameters.bat_charge_stop_voltage);
                break;
            case HoldingRegs_BatChargeStopDelay:
                m_parameters.charge_stop_delay= unit.value(addr);
                ui->batChargeStopVoltage->setValue(m_parameters.charge_stop_delay);
                break;

            case HoldingRegs_LowPressure_PT03:
                m_parameters.low_pressure_pt03 = unit.value(addr);
                ui->lowPressure_PT03->setValue(m_parameters.low_pressure_pt03);
                break;
            case HoldingRegs_HighPressure_PT03:
                m_parameters.high_pressure_pt03 = unit.value(addr);
                ui->highPressure_PT03->setValue(m_parameters.high_pressure_pt03);
                break;
            case HoldingRegs_HighPressure_PT04:
                m_parameters.high_pressure_pt04 = unit.value(addr);
                ui->highPressure_PT04->setValue(m_parameters.high_pressure_pt04);
                break;
            case HoldingRegs_HighTemperature_TT17:
                m_parameters.high_temperature_tt17 = unit.value(addr);
                ui->highTemperature_TT17->setValue(m_parameters.high_temperature_tt17);
                break;
            case HoldingRegs_HighTemperature_TT31:
                m_parameters.high_temperature_tt31 = unit.value(addr);
                ui->highTemperature_TT31->setValue(m_parameters.high_temperature_tt31);
                break;
            case HoldingRegs_HighConductivity_CS01:
                m_parameters.high_conductivity = unit.value(addr);
                ui->highConductivity_CS01->setValue(m_parameters.high_conductivity);
                break;
            case HoldingRegs_LowVoltage_BAT01:
                m_parameters.low_voltage_bat01 = unit.value(addr);
                ui->lowVoltage_BAT01->setValue(m_parameters.low_voltage_bat01);
                break;
            case HoldingRegs_LowLevel_LT01:
                m_parameters.low_level_lt1 = unit.value(addr);
                ui->lowLevel_LT01->setValue(m_parameters.low_level_lt1);
                break;
            case HoldingRegs_AutoLiquidLowLimit_LT01:
                m_parameters.auto_liquid_low_limit_lt1 = unit.value(addr);
                ui->autoLiquidLowLimit_LT01->setValue(m_parameters.auto_liquid_low_limit_lt1);
                break;
            case HoldingRegs_StopLiquidValue_LT01:
                m_parameters.stop_liquid_limit_lt1 = unit.value(addr);
                ui->stopLiquidValue_LT01->setValue(m_parameters.stop_liquid_limit_lt1);
                break;
            case HoldingRegs_LowLevel_LT02:
                m_parameters.low_level_lt2 = unit.value(addr);
                ui->lowLevel_LT02->setValue(m_parameters.low_level_lt2);
                break;
            default:
                break;
            }
        }
    }
}

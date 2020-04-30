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

//    refreshCurrentPage();

//    serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Manufacturer, 1);
}

ParameterConfiguration::~ParameterConfiguration()
{
    delete ui;
}

void ParameterConfiguration::on_readData_clicked()
{
    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    {
//        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 1);
//        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL01, 30);
//        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_DataStorageCycle, 13);
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
            case CoilsRegs_LT_01_AlarmCtrl:
                ui->autoControl_LT01->setStyleSheet(    \
                            (unit.value(i) == 1) ?    \
                            ("QLabel {font-size:14px;font-family:PingFang SC;font-weight:300;line-height:34px;color:rgba(255,255,255,1);background:rgba(81,223,0,1);}") :   \
                            ("QLabel {font-size:14px;font-family:PingFang SC;font-weight:300;line-height:34px;color:rgba(255,255,255,1);background:rgba(255,42,42,1);}")    \
                            );
                ui->autoControl_LT01->setText(  \
                            (unit.value(i) == 1) ? \
                            ("ON") : ("OFF")
                            );
                break;
            case CoilsRegs_LT_02_AlarmCtrl:
                ui->autoControl_LT02->setStyleSheet(    \
                            (unit.value(i) == 1) ?    \
                            ("QLabel {font-size:14px;font-family:PingFang SC;font-weight:300;line-height:34px;color:rgba(255,255,255,1);background:rgba(81,223,0,1);}") :   \
                            ("QLabel {font-size:14px;font-family:PingFang SC;font-weight:300;line-height:34px;color:rgba(255,255,255,1);background:rgba(255,42,42,1);}")    \
                            );
                ui->autoControl_LT02->setText(  \
                            (unit.value(i) == 1) ? \
                            ("ON") : ("OFF")
                            );
                break;

            case HoldingRegs_Manufacturer:
                if (unit.value(i))
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
                m_parameters.product_serial_num[0] = unit.value(i);
                m_parameters.product_serial_num[1] = unit.value(i+1);
                m_parameters.product_serial_num[2] = unit.value(i+2);
                m_parameters.product_serial_num[3] = unit.value(i+3);

                ui->productSerialNum->setText(QString("%1%2%3%4").arg(QString(m_parameters.product_serial_num[0], 16))
                                                                 .arg(QString(m_parameters.product_serial_num[1], 16))
                                                                 .arg(QString(m_parameters.product_serial_num[2], 16))
                                                                 .arg(QString(m_parameters.product_serial_num[3], 16)));
                break;
            case HoldingRegs_ProductDate:
                m_parameters.product_date[0] = unit.value(i);
                m_parameters.product_date[1] = unit.value(i+1);

                ui->productionDate->setText(QString("%1\\%2\\%3").arg(QString::number(m_parameters.product_date[0]))
                                                                 .arg(QString::number((m_parameters.product_date[1]&0xff00)>>8))
                                                                 .arg(QString::number(m_parameters.product_date[1]&0x00ff)));

                break;
            case HoldingRegs_ReformingID:
                m_parameters.reforming_ID[0] = unit.value(i);
                m_parameters.reforming_ID[1] = unit.value(i+1);
                m_parameters.reforming_ID[2] = unit.value(i+2);
                m_parameters.reforming_ID[3] = unit.value(i+3);

                ui->reformingID->setText(QString("%1%2%3%4").arg(QString::number(m_parameters.reforming_ID[0], 16))
                                                            .arg(QString::number(m_parameters.reforming_ID[1], 16))
                                                            .arg(QString::number(m_parameters.reforming_ID[2], 16))
                                                            .arg(QString::number(m_parameters.reforming_ID[3], 16)));

                break;
            case HoldingRegs_StackID:
                m_parameters.stack_ID[0] = unit.value(i);
                m_parameters.stack_ID[1] = unit.value(i+1);
                m_parameters.stack_ID[2] = unit.value(i+2);
                m_parameters.stack_ID[3] = unit.value(i+3);

                ui->stackID->setText(QString("%1%2%3%4").arg(QString::number(m_parameters.stack_ID[0], 16))
                                                        .arg(QString::number(m_parameters.stack_ID[1], 16))
                                                        .arg(QString::number(m_parameters.stack_ID[2], 16))
                                                        .arg(QString::number(m_parameters.stack_ID[3], 16)));
                break;
            case HoldingRegs_DevSlaveAddr:
                m_parameters.dev_slave_addr = unit.value(i);
                ui->devSlaveAddr->setValue(m_parameters.dev_slave_addr);
                break;
            case HoldingRegs_DevIPAddr:
                m_parameters.dev_IP_addr[0] = unit.value(i);
                m_parameters.dev_IP_addr[1] = unit.value(i+1);

                ui->devIPAddr->setText(QString("%1.%2.%3.%4").arg(QString::number((m_parameters.dev_IP_addr[0]&0xff00)>>8))
                                                             .arg(QString::number(m_parameters.dev_IP_addr[0]&0x00ff))
                                                             .arg(QString::number((m_parameters.dev_IP_addr[1]&0xff00)>>8))
                                                             .arg(QString::number((m_parameters.dev_IP_addr[1]&0x00ff))));
                break;
            case HoldingRegs_SerialPara:
                m_parameters.serial_paras = unit.value(i);
                ui->serialPara->setValue(m_parameters.serial_paras);
                break;
            case HoldingRegs_PowerMode:
                m_parameters.power_mode = unit.value(i);
                ui->sysGenerateMode->setValue(m_parameters.power_mode);
                break;
            case HoldingRegs_FCOutCurrent:
                m_parameters.fc_output_current = unit.value(i);
                ui->FCOutputCurrent->setValue(double(m_parameters.fc_output_current)/10);
                break;
            case HoldingRegs_FCOutPower:
                m_parameters.fc_output_power = unit.value(i);
                ui->FCOutputPower->setValue(m_parameters.fc_output_power);
                break;
            case HoldingRegs_BatChargeStartVoltage:
                m_parameters.bat_charge_start_voltage = unit.value(i);
                ui->batChargeStartVoltage->setValue(m_parameters.bat_charge_start_voltage);
                break;
            case HoldingRegs_BatChargeStartDelay:
                m_parameters.charge_start_delay = unit.value(i);
                ui->batChargeStartDelay->setValue(m_parameters.charge_start_delay);
                break;
            case HoldingRegs_BatChargeStopVoltage:
                m_parameters.bat_charge_stop_voltage = unit.value(i);
                ui->batChargeStopVoltage->setValue(m_parameters.bat_charge_stop_voltage);
                break;
            case HoldingRegs_BatChargeStopDelay:
                m_parameters.charge_stop_delay = unit.value(i);
                ui->batChargeStopVoltage->setValue(m_parameters.charge_stop_delay);
                break;
            case HoldingRegs_DataStorageCycle:
                m_parameters.sd_storage_delay = unit.value(i);
                ui->dataStorageCycle->setValue(m_parameters.sd_storage_delay);
                break;

            case HoldingRegs_Kp_BL01:
                running_para[0].kp = unit.value(i);
                ui->Kp_BL01->setValue(running_para[0].kp);
                break;
            case HoldingRegs_Ti_BL01:
                running_para[0].ti = unit.value(i);
                ui->Ti_BL01->setValue(running_para[0].ti);
                break;
            case HoldingRegs_Tsm_BL01:
                running_para[0].tsm = unit.value(i);
                ui->Tsm_BL01->setValue(running_para[0].tsm);
                break;
            case HoldingRegs_Kp_BL02:
                running_para[1].kp = unit.value(i);
                ui->Kp_BL02->setValue(running_para[1].kp);
                break;
            case HoldingRegs_Ti_BL02:
                running_para[1].ti = unit.value(i);
                ui->Ti_BL02->setValue(running_para[1].ti);
                break;
            case HoldingRegs_Tsm_BL02:
                running_para[1].tsm = unit.value(i);
                ui->Tsm_BL02->setValue(running_para[1].tsm);
                break;
            case HoldingRegs_Kp_BL03:
                running_para[2].kp = unit.value(i);
                ui->Kp_BL03->setValue(running_para[2].kp);
                break;
            case HoldingRegs_Ti_BL03:
                running_para[2].ti = unit.value(i);
                ui->Ti_BL03->setValue(running_para[2].ti);
                break;
            case HoldingRegs_Tsm_BL03:
                running_para[2].tsm = unit.value(i);
                ui->Tsm_BL03->setValue(running_para[2].tsm);
                break;
            case HoldingRegs_Kp_BL04:
                running_para[3].kp = unit.value(i);
                ui->Kp_BL04->setValue(running_para[3].kp);
                break;
            case HoldingRegs_Ti_BL04:
                running_para[3].ti = unit.value(i);
                ui->Ti_BL04->setValue(running_para[3].ti);
                break;
            case HoldingRegs_Tsm_BL04:
                running_para[3].tsm = unit.value(i);
                ui->Tsm_BL04->setValue(running_para[3].tsm);
                break;
            case HoldingRegs_Kp_PMP01:
                running_para[4].kp = unit.value(i);
                ui->Kp_PMP01->setValue(running_para[4].kp);
                break;
            case HoldingRegs_Ti_PMP01:
                running_para[4].ti = unit.value(i);
                ui->Ti_PMP01->setValue(running_para[4].ti);
                break;
            case HoldingRegs_Tsm_PMP01:
                running_para[4].tsm = unit.value(i);
                ui->Tsm_PMP01->setValue(running_para[4].tsm);
                break;
            case HoldingRegs_Kp_PMP02:
                running_para[5].kp = unit.value(i);
                ui->Kp_PMP02->setValue(running_para[5].kp);
                break;
            case HoldingRegs_Ti_PMP02:
                running_para[5].ti = unit.value(i);
                ui->Ti_PMP02->setValue(running_para[5].ti);
                break;
            case HoldingRegs_Tsm_PMP02:
                running_para[5].tsm = unit.value(i);
                ui->Tsm_PMP02->setValue(running_para[5].tsm);
                break;
            case HoldingRegs_Kp_PMP03:
                running_para[6].kp = unit.value(i);
                ui->Kp_PMP03->setValue(running_para[6].kp);
                break;
            case HoldingRegs_Ti_PMP03:
                running_para[6].ti = unit.value(i);
                ui->Ti_PMP03->setValue(running_para[6].ti);
                break;
            case HoldingRegs_Tsm_PMP03:
                running_para[6].tsm = unit.value(i);
                ui->Tsm_PMP03->setValue(running_para[6].tsm);
                break;
            case HoldingRegs_Kp_PMP04:
                running_para[7].kp = unit.value(i);
                ui->Kp_PMP04->setValue(running_para[7].kp);
                break;
            case HoldingRegs_Ti_PMP04:
                running_para[7].ti = unit.value(i);
                ui->Ti_PMP04->setValue(running_para[7].ti);
                break;
            case HoldingRegs_Tsm_PMP04:
                running_para[7].tsm = unit.value(i);
                ui->Tsm_PMP04->setValue(running_para[7].tsm);
                break;
            case HoldingRegs_Kp_PMP05:
                running_para[8].kp = unit.value(i);
                ui->Kp_PMP05->setValue(running_para[8].kp);
                break;
            case HoldingRegs_Ti_PMP05:
                running_para[8].ti = unit.value(i);
                ui->Ti_PMP05->setValue(running_para[8].ti);
                break;
            case HoldingRegs_Tsm_PMP05:
                running_para[8].tsm = unit.value(i);
                ui->Tsm_PMP05->setValue(running_para[8].tsm);
                break;
            case HoldingRegs_Kp_RAD01:
                running_para[9].kp = unit.value(i);
                ui->Kp_RAD01->setValue(running_para[9].kp);
                break;
            case HoldingRegs_Ti_RAD01:
                running_para[9].ti = unit.value(i);
                ui->Ti_RAD01->setValue(running_para[9].ti);
                break;
            case HoldingRegs_Tsm_RAD01:
                running_para[9].tsm = unit.value(i);
                ui->Tsm_RAD01->setValue(running_para[9].tsm);
                break;

            case HoldingRegs_LowPressure_PT03:
                m_parameters.low_pressure_pt03 = unit.value(i);
                ui->lowPressure_PT03->setValue(m_parameters.low_pressure_pt03);
                break;
            case HoldingRegs_HighPressure_PT03:
                m_parameters.high_pressure_pt03 = unit.value(i);
                ui->highPressure_PT03->setValue(m_parameters.high_pressure_pt03);
                break;
            case HoldingRegs_HighPressure_PT04:
                m_parameters.high_pressure_pt04 = unit.value(i);
                ui->highPressure_PT04->setValue(m_parameters.high_pressure_pt04);
                break;
            case HoldingRegs_HighTemperature_TT17:
                m_parameters.high_temperature_tt17 = unit.value(i);
                ui->highTemperature_TT17->setValue(m_parameters.high_temperature_tt17);
                break;
            case HoldingRegs_HighTemperature_TT31:
                m_parameters.high_temperature_tt31 = unit.value(i);
                ui->highTemperature_TT31->setValue(m_parameters.high_temperature_tt31);
                break;
            case HoldingRegs_HighConductivity_CS01:
                m_parameters.high_conductivity = unit.value(i);
                ui->highConductivity_CS01->setValue(m_parameters.high_conductivity);
                break;
            case HoldingRegs_LowVoltage_BAT01:
                m_parameters.low_voltage_bat01 = unit.value(i);
                ui->lowVoltage_BAT01->setValue(m_parameters.low_voltage_bat01);
                break;
            case HoldingRegs_LowLevel_LT01:
                m_parameters.low_level_lt1 = unit.value(i);
                ui->lowLevel_LT01->setValue(m_parameters.low_level_lt1);
                break;
            case HoldingRegs_AutoLiquidLowLimit_LT01:
                m_parameters.auto_liquid_low_limit_lt1 = unit.value(i);
                ui->autoLiquidLowLimit_LT01->setValue(m_parameters.auto_liquid_low_limit_lt1);
                break;
            case HoldingRegs_StopLiquidValue_LT01:
                m_parameters.stop_liquid_limit_lt1 = unit.value(i);
                ui->stopLiquidValue_LT01->setValue(m_parameters.stop_liquid_limit_lt1);
                break;
            case HoldingRegs_LowLevel_LT02:
                m_parameters.low_level_lt2 = unit.value(i);
                ui->lowLevel_LT02->setValue(m_parameters.low_level_lt2);
                break;
            default:
                break;
            }
        }
    }
}

void ParameterConfiguration::refreshCurrentPage()
{
    current_serial->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_LT_01_AlarmCtrl, 2);
    current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Manufacturer, 8);
    current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_ReformingID, 12);
    current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 1);
    current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL01, 48);
}

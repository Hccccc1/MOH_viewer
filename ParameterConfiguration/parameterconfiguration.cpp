#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include <QMessageBox>

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

//void ParameterConfiguration::on_readData_clicked()
//{
//    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
//    {
////        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 1);
////        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL01, 30);
////        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_DataStorageCycle, 13);
//    }
//}

void ParameterConfiguration::onReadyRead()
{
    QString tmp;

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
                                ("QPushButton {font-size:14px;font-family:PingFang SC;font-weight:300;line-height:34px;color:rgba(255,255,255,1);background:rgba(81,223,0,1);border:0px;}") :   \
                                ("QPushButton {font-size:14px;font-family:PingFang SC;font-weight:300;line-height:34px;color:rgba(255,255,255,1);background:rgba(255,42,42,1);border:0px;}")    \
                                );
                ui->autoControl_LT01->setText(  \
                            (unit.value(i) == 1) ? \
                                ("ON") : ("OFF")
                                );
                break;
            case CoilsRegs_LT_02_AlarmCtrl:
                ui->autoControl_LT02->setStyleSheet(    \
                            (unit.value(i) == 1) ?    \
                                ("QPushButton {font-size:14px;font-family:PingFang SC;font-weight:300;line-height:34px;color:rgba(255,255,255,1);background:rgba(81,223,0,1);border:0px;}") :   \
                                ("QPushButton {font-size:14px;font-family:PingFang SC;font-weight:300;line-height:34px;color:rgba(255,255,255,1);background:rgba(255,42,42,1);border:0px;}")    \
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
                m_parameters.product_name = unit.value(i);
                ui->productName->setText(QString::number(m_parameters.product_name));
                break;
            case HoldingRegs_ProductType:
                m_parameters.product_type = unit.value(i);
                ui->productType->setText(QString::number(m_parameters.product_type));
                break;
            case HoldingRegs_ProductSerialNum:
                m_parameters.product_serial_num[0] = unit.value(i);
                m_parameters.product_serial_num[1] = unit.value(i+1);
                m_parameters.product_serial_num[2] = unit.value(i+2);
                m_parameters.product_serial_num[3] = unit.value(i+3);

                tmp = QChar(m_parameters.product_serial_num[3]>>8).toLatin1();
                tmp += QChar(m_parameters.product_serial_num[3]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.product_serial_num[2]>>8).toLatin1();
                tmp += QChar(m_parameters.product_serial_num[2]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.product_serial_num[1]>>8).toLatin1();
                tmp += QChar(m_parameters.product_serial_num[1]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.product_serial_num[0]>>8).toLatin1();
                tmp += QChar(m_parameters.product_serial_num[0]&0x00ff).toLatin1();

                ui->productSerialNum->setText(tmp);

                //                ui->productSerialNum->setText(QString("%1%2%3%4").arg(QString(m_parameters.product_serial_num[0], 16))
                //                                                                 .arg(QString(m_parameters.product_serial_num[1], 16))
                //                                                                 .arg(QString(m_parameters.product_serial_num[2], 16))
                //                                                                 .arg(QString(m_parameters.product_serial_num[3], 16)));
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

                tmp = QChar(m_parameters.reforming_ID[3]>>8).toLatin1();
                tmp += QChar(m_parameters.reforming_ID[3]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.reforming_ID[2]>>8).toLatin1();
                tmp += QChar(m_parameters.reforming_ID[2]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.reforming_ID[1]>>8).toLatin1();
                tmp += QChar(m_parameters.reforming_ID[1]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.reforming_ID[0]>>8).toLatin1();
                tmp += QChar(m_parameters.reforming_ID[0]&0x00ff).toLatin1();

                ui->reformingID->setText(tmp);



                //                qDebug() << __LINE__ << QChar(m_parameters.reforming_ID[0]>>8).toLatin1();

                //                ui->reformingID->setText(QString("%1%2%3%4").arg(QString::number(m_parameters.reforming_ID[0], 16))
                //                                                            .arg(QString::number(m_parameters.reforming_ID[1], 16))
                //                                                            .arg(QString::number(m_parameters.reforming_ID[2], 16))
                //                                                            .arg(QString::number(m_parameters.reforming_ID[3], 16)));

                break;
            case HoldingRegs_StackID:
                m_parameters.stack_ID[0] = unit.value(i);
                m_parameters.stack_ID[1] = unit.value(i+1);
                m_parameters.stack_ID[2] = unit.value(i+2);
                m_parameters.stack_ID[3] = unit.value(i+3);

                tmp = QChar(m_parameters.stack_ID[3]>>8).toLatin1();
                tmp += QChar(m_parameters.stack_ID[3]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.stack_ID[2]>>8).toLatin1();
                tmp += QChar(m_parameters.stack_ID[2]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.stack_ID[1]>>8).toLatin1();
                tmp += QChar(m_parameters.stack_ID[1]&0x00ff).toLatin1();
                tmp += QChar(m_parameters.stack_ID[0]>>8).toLatin1();
                tmp += QChar(m_parameters.stack_ID[0]&0x00ff).toLatin1();

                ui->stackID->setText(tmp);

                //                ui->stackID->setText(QString("%1%2%3%4").arg(QString::number(m_parameters.stack_ID[0], 16))
                //                                                        .arg(QString::number(m_parameters.stack_ID[1], 16))
                //                                                        .arg(QString::number(m_parameters.stack_ID[2], 16))
                //                                                        .arg(QString::number(m_parameters.stack_ID[3], 16)));
                break;
            case HoldingRegs_DevSlaveAddr:
                m_parameters.dev_slave_addr = unit.value(i);
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
                break;
            case HoldingRegs_PowerMode:
                m_parameters.power_mode = unit.value(i);
                break;
            case HoldingRegs_FCOutCurrent:
                m_parameters.fc_output_current = unit.value(i);
                break;
            case HoldingRegs_FCOutPower:
                m_parameters.fc_output_power = unit.value(i);
                break;
            case HoldingRegs_BatChargeStartVoltage:
                m_parameters.bat_charge_start_voltage = unit.value(i);
                break;
            case HoldingRegs_BatChargeStartDelay:
                m_parameters.charge_start_delay = unit.value(i);
                break;
            case HoldingRegs_BatChargeStopVoltage:
                m_parameters.bat_charge_stop_voltage = unit.value(i);
                break;
            case HoldingRegs_BatChargeStopDelay:
                m_parameters.charge_stop_delay = unit.value(i);
                break;
            case HoldingRegs_DataStorageCycle:
                m_parameters.sd_storage_delay = unit.value(i);
                break;

            case HoldingRegs_Kp_BL01:
                running_para[0].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_BL01:
                running_para[0].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_BL01:
                running_para[0].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_BL02:
                running_para[1].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_BL02:
                running_para[1].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_BL02:
                running_para[1].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_BL03:
                running_para[2].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_BL03:
                running_para[2].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_BL03:
                running_para[2].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_BL04:
                running_para[3].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_BL04:
                running_para[3].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_BL04:
                running_para[3].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_PMP01:
                running_para[4].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_PMP01:
                running_para[4].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_PMP01:
                running_para[4].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_PMP02:
                running_para[5].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_PMP02:
                running_para[5].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_PMP02:
                running_para[5].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_PMP03:
                running_para[6].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_PMP03:
                running_para[6].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_PMP03:
                running_para[6].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_PMP04:
                running_para[7].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_PMP04:
                running_para[7].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_PMP04:
                running_para[7].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_PMP05:
                running_para[8].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_PMP05:
                running_para[8].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_PMP05:
                running_para[8].tsm = unit.value(i);
                break;
            case HoldingRegs_Kp_RAD01:
                running_para[9].kp = unit.value(i);
                break;
            case HoldingRegs_Ti_RAD01:
                running_para[9].ti = unit.value(i);
                break;
            case HoldingRegs_Tsm_RAD01:
                running_para[9].tsm = unit.value(i);
                break;

            case HoldingRegs_LowPressure_PT03:
                m_parameters.low_pressure_pt03 = unit.value(i);
                break;
            case HoldingRegs_HighPressure_PT03:
                m_parameters.high_pressure_pt03 = unit.value(i);
                break;
            case HoldingRegs_HighPressure_PT04:
                m_parameters.high_pressure_pt04 = unit.value(i);
                break;
            case HoldingRegs_HighTemperature_TT17:
                m_parameters.high_temperature_tt17 = unit.value(i);
                break;
            case HoldingRegs_HighTemperature_TT31:
                m_parameters.high_temperature_tt31 = unit.value(i);
                break;
            case HoldingRegs_HighConductivity_CS01:
                m_parameters.high_conductivity = unit.value(i);
                break;
            case HoldingRegs_LowVoltage_BAT01:
                m_parameters.low_voltage_bat01 = unit.value(i);
                break;
            case HoldingRegs_LowLevel_LT01:
                m_parameters.low_level_lt1 = unit.value(i);
                break;
            case HoldingRegs_AutoLiquidLowLimit_LT01:
                m_parameters.auto_liquid_low_limit_lt1 = unit.value(i);
                break;
            case HoldingRegs_StopLiquidValue_LT01:
                m_parameters.stop_liquid_limit_lt1 = unit.value(i);
                break;
            case HoldingRegs_LowLevel_LT02:
                m_parameters.low_level_lt2 = unit.value(i);
                break;
            default:
                break;
            }
        }

        displayData();
    }
    else
    {
        emit modbusErrorHappened(reply->error());
    }
}

void ParameterConfiguration::refreshCurrentPage()
{
    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        current_serial->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_LT_01_AlarmCtrl, 2);
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Manufacturer, 9);
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_ReformingID, 12);
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 1);
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL01, 49);
    }
}

void ParameterConfiguration::on_saveToFile_clicked()
{
    QFile cfg_file;
    QString cfgfile_fullpath = QFileDialog::getSaveFileName(this, "Choose cfg file", "", tr("Configuration (*.cfg)"));
    //    QString filename;

    QJsonArray json_array;
    QVector<QJsonObject> json_obj(7);

    if (cfgfile_fullpath.isEmpty())
        return;
    else
    {
        //        filename = cfgfile_fullpath.mid(cfgfile_fullpath.lastIndexOf('/')+1);

        cfg_file.setFileName(cfgfile_fullpath);
        if (!cfg_file.open(QIODevice::WriteOnly))
        {
            qDebug() << __FILE__ << __LINE__ << "Open file error..." << cfg_file.errorString();
        }
        else
        {
            //            json_obj[0]
            json_obj[0].insert("slave_addr", m_parameters.dev_slave_addr);
            json_obj[0].insert("serial_parameters", m_parameters.serial_paras);

            for (int i = 0; i < 4; i ++)
            {
                QString tmp_kp = "BL0%1_kp", tmp_ti = "BL0%1_ti", tmp_tsm = "BL0%1_tsm";
                json_obj[1].insert(tmp_kp.arg(i+1), running_para[i].kp);
                json_obj[1].insert(tmp_ti.arg(i+1), running_para[i].ti);
                json_obj[1].insert(tmp_tsm.arg(i+1), running_para[i].tsm);
            }

            for (int i = 4; i < 9; i++)
            {
                QString tmp_kp = "PMP0%1_kp", tmp_ti = "PMP0%1_ti", tmp_tsm = "PMP0%1_tsm";
                json_obj[1].insert(tmp_kp.arg(i), running_para[i].kp);
                json_obj[1].insert(tmp_ti.arg(i), running_para[i].ti);
                json_obj[1].insert(tmp_tsm.arg(i), running_para[i].tsm);
            }

            json_obj[1].insert("RAD01_kp", running_para[9].kp);
            json_obj[1].insert("RAD01_ti", running_para[9].ti);
            json_obj[1].insert("RAD01_tsm", running_para[9].tsm);

            json_obj[2].insert("PowerMode", m_parameters.power_mode);

            json_obj[3].insert("FCOutCurrent", m_parameters.fc_output_current);
            json_obj[3].insert("FCOutPower", m_parameters.fc_output_power);

            json_obj[4].insert("BatChargeStartVoltage", m_parameters.bat_charge_start_voltage);
            json_obj[4].insert("ChargeStartDelay", m_parameters.charge_start_delay);
            json_obj[4].insert("BatChargeStopVoltage", m_parameters.bat_charge_stop_voltage);
            json_obj[4].insert("ChargeStopDelay", m_parameters.charge_stop_delay);

            json_obj[5].insert("DataStorageCycle", m_parameters.sd_storage_delay);

            json_obj[6].insert("PT03_Low", m_parameters.low_pressure_pt03);
            json_obj[6].insert("PT03_High", m_parameters.high_pressure_pt03);
            json_obj[6].insert("PT04_High", m_parameters.high_pressure_pt04);
            json_obj[6].insert("TT17_High", m_parameters.high_temperature_tt17);
            json_obj[6].insert("TT31_High", m_parameters.high_temperature_tt31);
            json_obj[6].insert("CS01_High", m_parameters.high_conductivity);
            json_obj[6].insert("BAT01_Low", m_parameters.low_voltage_bat01);
            json_obj[6].insert("LT1_Low", m_parameters.low_level_lt1);
            json_obj[6].insert("LT1_AutoLiquidLow", m_parameters.auto_liquid_low_limit_lt1);
            json_obj[6].insert("LT1_StopLiquidLow", m_parameters.stop_liquid_limit_lt1);
            json_obj[6].insert("LT2_Low", m_parameters.low_level_lt2);

            foreach (const QJsonObject obj, json_obj)
                json_array.append(obj);

            QJsonDocument json_doc;
            json_doc.setArray(json_array);

            cfg_file.write(json_doc.toJson());

            cfg_file.close();
        }
    }
}

void ParameterConfiguration::on_loadFromFile_clicked()
{
    QFile cfgfile;
    //    QString filename;
    QString cfgfile_fullpath = QFileDialog::getOpenFileName(this, "Choose cfg file", "", tr("Configurations (*.cfg)"));

    QVector<QJsonObject> json_objs(7);

    if (cfgfile_fullpath.isEmpty())
        return;
    else
    {
        //        filename = cfgfile_fullpath.mid(cfgfile_fullpath.lastIndexOf('/')+1);

        cfgfile.setFileName(cfgfile_fullpath);
        if (!cfgfile.open(QIODevice::ReadOnly))
            qDebug() << __FILE__ << __LINE__ << cfgfile.errorString();
        else
        {
            QByteArray data = cfgfile.readAll();
            cfgfile.close();

            QJsonParseError json_error;
            QJsonDocument json_doc = QJsonDocument(QJsonDocument::fromJson(data, &json_error));
            if (json_doc.isEmpty())
                qDebug() << "doc empty";

            if (json_error.error != QJsonParseError::NoError)
            {
                qDebug() << __FILE__ << __LINE__ << json_error.errorString();
                QMessageBox::critical(this, "错误", "请确认是否为正确的配置文件！");
                return;
            }

            QJsonArray root_array = json_doc.array();

            for (int i = 0; i < root_array.size(); i++)
            {
                json_objs[i] = root_array[i].toObject();
            }

            for (QJsonObject obj : json_objs)
            {
                if (obj.contains("serial_parameters"))
                {
                    m_parameters.serial_paras = quint16(obj.value("serial_parameters").toInt());
                    m_parameters.dev_slave_addr = quint16(obj.value("slave_addr").toInt());
                }
                else if (obj.contains("BL01_kp"))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        QString tmp_kp = "BL0%1_kp", tmp_ti = "BL0%1_ti", tmp_tsm = "BL0%1_tsm";
                        running_para[i].kp = quint16(obj.value(tmp_kp.arg(1)).toInt());
                        running_para[i].ti = quint16(obj.value(tmp_ti.arg(1)).toInt());
                        running_para[i].tsm = quint16(obj.value(tmp_tsm.arg(1)).toInt());
                    }
                    for (int i = 4; i < 9; i++)
                    {
                        QString tmp_kp = "PMP0%1_kp", tmp_ti = "PMP0%1_ti", tmp_tsm = "PMP0%1_tsm";
                        running_para[i].kp = quint16(obj.value(tmp_kp.arg(1)).toInt());
                        running_para[i].ti = quint16(obj.value(tmp_ti.arg(1)).toInt());
                        running_para[i].tsm = quint16(obj.value(tmp_tsm.arg(1)).toInt());
                    }
                    running_para[9].kp = quint16(obj.value("RAD01_kp").toInt());
                    running_para[9].ti = quint16(obj.value("RAD01_ti").toInt());
                    running_para[9].tsm = quint16(obj.value("RAD01_tsm").toInt());
                }
                else if (obj.contains("PowerMode"))
                {
                    m_parameters.power_mode = quint16(obj.value("PowerMode").toInt());
                }
                else if (obj.contains("FCOutCurrent"))
                {
                    m_parameters.fc_output_current = quint16(obj.value("FCOutCurrent").toInt());
                    m_parameters.fc_output_power = quint16(obj.value("FCOutPower").toInt());
                }
                else if (obj.contains("BatChargeStartVoltage"))
                {
                    m_parameters.bat_charge_start_voltage = quint16(obj.value("BatChargeStartVoltage").toInt());
                    m_parameters.charge_start_delay = quint16(obj.value("ChargeStartDelay").toInt());
                    m_parameters.bat_charge_stop_voltage = quint16(obj.value("BatChargeStopVoltage").toInt());
                    m_parameters.charge_stop_delay = quint16(obj.value("ChargeStopDelay").toInt());
                }
                else if (obj.contains("DataStorageCycle"))
                {
                    m_parameters.sd_storage_delay = quint16(obj.value("DataStorageCycle").toInt());
                }
                else if (obj.contains("BAT01_Low"))
                {
                    m_parameters.low_pressure_pt03 = quint16(obj.value("PT03_Low").toInt());
                    m_parameters.high_pressure_pt03 = quint16(obj.value("PT03_High").toInt());
                    m_parameters.high_pressure_pt04 = quint16(obj.value("PT04_High").toInt());
                    m_parameters.high_temperature_tt17 = quint16(obj.value("TT17_High").toInt());
                    m_parameters.high_temperature_tt31 = quint16(obj.value("TT31_High").toInt());
                    m_parameters.high_conductivity = quint16(obj.value("CS01_High").toInt());
                    m_parameters.low_voltage_bat01 = quint16(obj.value("BAT01_Low").toInt());
                    m_parameters.low_level_lt1 = quint16(obj.value("LT1_Low").toInt());
                    m_parameters.auto_liquid_low_limit_lt1 = quint16(obj.value("LT1_AutoLiquidLow").toInt());
                    m_parameters.stop_liquid_limit_lt1 = quint16(obj.value("LT1_StopLiquidLow").toInt());
                    m_parameters.low_level_lt2 = quint16(obj.value("LT2_Low").toInt());
                }
                else
                {
                    return;
                }
            }
            displayData();
        }

        //        qDebug() << m_parameters.low_pressure_pt03 << m_parameters.high_pressure_pt03;

    }
}

void ParameterConfiguration::on_sendToLower_clicked()
{
    QVector<quint16> to_lower;

    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        to_lower.append(m_parameters.dev_slave_addr);
        to_lower.append(m_parameters.dev_IP_addr[0]);
        to_lower.append(m_parameters.dev_IP_addr[1]);
        current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_DevSlaveAddr, to_lower, 3);

        to_lower.clear();
        for (int i = 0; i < 10; i++)
        {
            to_lower.append(running_para[i].kp);
            to_lower.append(running_para[i].ti);
            to_lower.append(running_para[i].tsm);
        }
        current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL01, to_lower, 30);

        to_lower.clear();
        to_lower.append(m_parameters.fc_output_current);
        to_lower.append(m_parameters.fc_output_power);
        to_lower.append(m_parameters.bat_charge_start_voltage);
        to_lower.append(m_parameters.charge_start_delay);
        to_lower.append(m_parameters.bat_charge_stop_voltage);
        to_lower.append(m_parameters.charge_stop_delay);
        to_lower.append(m_parameters.sd_storage_delay);

        to_lower.append(m_parameters.low_pressure_pt03);
        to_lower.append(m_parameters.high_pressure_pt03);
        to_lower.append(m_parameters.high_pressure_pt04);
        to_lower.append(m_parameters.high_temperature_tt17);
        to_lower.append(m_parameters.high_temperature_tt31);
        to_lower.append(m_parameters.high_conductivity);
        to_lower.append(m_parameters.low_voltage_bat01);
        to_lower.append(m_parameters.low_level_lt1);
        to_lower.append(m_parameters.auto_liquid_low_limit_lt1);
        to_lower.append(m_parameters.stop_liquid_limit_lt1);
        to_lower.append(m_parameters.low_level_lt2);
        current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_FCOutPower, to_lower, 18);
    }
}

void ParameterConfiguration::displayData()
{
    ui->devSlaveAddr->setValue(m_parameters.dev_slave_addr);
    ui->serialPara->setValue(m_parameters.serial_paras);
    ui->sysGenerateMode->setValue(m_parameters.power_mode);
    ui->FCOutputCurrent->setValue(double(m_parameters.fc_output_current)/10);
    ui->FCOutputPower->setValue(m_parameters.fc_output_power);
    ui->batChargeStartVoltage->setValue(double(m_parameters.bat_charge_start_voltage)/10);
    ui->batChargeStartDelay->setValue(m_parameters.charge_start_delay);
    ui->batChargeStopVoltage->setValue(double(m_parameters.bat_charge_stop_voltage)/10);
    ui->batChargeStopVoltage->setValue(m_parameters.charge_stop_delay);
    ui->dataStorageCycle->setValue(m_parameters.sd_storage_delay);
    ui->Kp_BL01->setValue(double(running_para[0].kp)/10);
    ui->Ti_BL01->setValue(double(running_para[0].ti)/10);
    ui->Tsm_BL01->setValue(double(running_para[0].tsm)/10);
    ui->Kp_BL02->setValue(double(running_para[1].kp)/10);
    ui->Ti_BL02->setValue(double(running_para[1].ti)/10);
    ui->Tsm_BL02->setValue(double(running_para[1].tsm)/10);
    ui->Kp_BL03->setValue(double(running_para[2].kp)/10);
    ui->Ti_BL03->setValue(double(running_para[2].ti)/10);
    ui->Tsm_BL03->setValue(double(running_para[2].tsm)/10);
    ui->Kp_BL04->setValue(double(running_para[3].kp)/10);
    ui->Ti_BL04->setValue(double(running_para[3].ti)/10);
    ui->Tsm_BL04->setValue(double(running_para[3].tsm)/10);
    ui->Kp_PMP01->setValue(double(running_para[4].kp)/10);
    ui->Ti_PMP01->setValue(double(running_para[4].ti)/10);
    ui->Tsm_PMP01->setValue(double(running_para[4].tsm)/10);
    ui->Kp_PMP02->setValue(double(running_para[5].kp)/10);
    ui->Ti_PMP02->setValue(double(running_para[5].ti)/10);
    ui->Tsm_PMP02->setValue(double(running_para[5].tsm)/10);
    ui->Kp_PMP03->setValue(double(running_para[6].kp)/10);
    ui->Ti_PMP03->setValue(double(running_para[6].ti)/10);
    ui->Tsm_PMP03->setValue(double(running_para[6].tsm)/10);
    ui->Kp_PMP04->setValue(double(running_para[7].kp)/10);
    ui->Ti_PMP04->setValue(double(running_para[7].ti)/10);
    ui->Tsm_PMP04->setValue(double(running_para[7].tsm)/10);
    ui->Kp_PMP05->setValue(double(running_para[8].kp)/10);
    ui->Ti_PMP05->setValue(double(running_para[8].ti)/10);
    ui->Tsm_PMP05->setValue(double(running_para[8].tsm)/10);
    ui->Kp_RAD01->setValue(double(running_para[9].kp)/10);
    ui->Ti_RAD01->setValue(double(running_para[9].ti)/10);
    ui->Tsm_RAD01->setValue(double(running_para[9].tsm)/10);
    ui->lowPressure_PT03->setValue(double(m_parameters.low_pressure_pt03)/10);
    ui->highPressure_PT03->setValue(double(m_parameters.high_pressure_pt03)/10);
    ui->highPressure_PT04->setValue(double(m_parameters.high_pressure_pt04)/10);
    ui->highTemperature_TT17->setValue(double(m_parameters.high_temperature_tt17)/10);
    ui->highTemperature_TT31->setValue(double(m_parameters.high_temperature_tt31)/10);
    ui->highConductivity_CS01->setValue(double(m_parameters.high_conductivity)/10);
    ui->lowVoltage_BAT01->setValue(double(m_parameters.low_voltage_bat01)/10);
    ui->lowLevel_LT01->setValue(m_parameters.low_level_lt1);
    ui->autoLiquidLowLimit_LT01->setValue(m_parameters.auto_liquid_low_limit_lt1);
    ui->stopLiquidValue_LT01->setValue(m_parameters.stop_liquid_limit_lt1);
    ui->lowLevel_LT02->setValue(m_parameters.low_level_lt2);
}

void ParameterConfiguration::on_Kp_BL01_editingFinished()
{
    running_para[0].kp = quint16(ui->Kp_BL01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL01, running_para[0].kp);
}
void ParameterConfiguration::on_Ti_BL01_editingFinished()
{
    running_para[0].ti = quint16(ui->Ti_BL01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_BL01, running_para[0].ti);
}
void ParameterConfiguration::on_Tsm_BL01_editingFinished()
{
    running_para[0].tsm = quint16(ui->Tsm_BL01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_BL01, running_para[0].tsm);
}

void ParameterConfiguration::on_Kp_BL02_editingFinished()
{
    running_para[1].kp = quint16(ui->Kp_BL01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL02, running_para[1].kp);
}
void ParameterConfiguration::on_Ti_BL02_editingFinished()
{
    running_para[1].ti = quint16(ui->Ti_BL02->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_BL02, running_para[1].ti);
}
void ParameterConfiguration::on_Tsm_BL02_editingFinished()
{
    running_para[1].tsm = quint16(ui->Tsm_BL02->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_BL02, running_para[1].tsm);
}

void ParameterConfiguration::on_Kp_BL03_editingFinished()
{
    running_para[2].kp = quint16(ui->Kp_BL03->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL03, running_para[2].kp);
}
void ParameterConfiguration::on_Ti_BL03_editingFinished()
{
    running_para[2].ti = quint16(ui->Ti_BL03->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_BL03, running_para[2].ti);
}
void ParameterConfiguration::on_Tsm_BL03_editingFinished()
{
    running_para[2].tsm = quint16(ui->Tsm_BL03->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_BL03, running_para[2].tsm);
}

void ParameterConfiguration::on_Kp_BL04_editingFinished()
{
    running_para[3].kp = quint16(ui->Kp_BL04->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_BL04, running_para[3].kp);
}
void ParameterConfiguration::on_Ti_BL04_editingFinished()
{
    running_para[3].ti = quint16(ui->Ti_BL04->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_BL04, running_para[3].ti);
}
void ParameterConfiguration::on_Tsm_BL04_editingFinished()
{
    running_para[3].tsm = quint16(ui->Tsm_BL04->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_BL04, running_para[3].tsm);
}

void ParameterConfiguration::on_Kp_PMP01_editingFinished()
{
    running_para[4].kp = quint16(ui->Kp_PMP01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_PMP01, running_para[4].kp);
}
void ParameterConfiguration::on_Ti_PMP01_editingFinished()
{
    running_para[4].ti = quint16(ui->Ti_PMP01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_PMP01, running_para[4].ti);
}
void ParameterConfiguration::on_Tsm_PMP01_editingFinished()
{
    running_para[4].tsm = quint16(ui->Tsm_PMP01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_PMP01, running_para[4].tsm);
}

void ParameterConfiguration::on_Kp_PMP02_editingFinished()
{
    running_para[5].kp = quint16(ui->Kp_PMP02->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_PMP02, running_para[5].kp);
}
void ParameterConfiguration::on_Ti_PMP02_editingFinished()
{
    running_para[5].ti = quint16(ui->Ti_PMP02->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_PMP02, running_para[5].ti);
}
void ParameterConfiguration::on_Tsm_PMP02_editingFinished()
{
    running_para[5].tsm = quint16(ui->Tsm_PMP02->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_PMP02, running_para[5].tsm);
}

void ParameterConfiguration::on_Kp_PMP03_editingFinished()
{
    running_para[6].kp = quint16(ui->Kp_PMP03->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_PMP03, running_para[6].kp);
}
void ParameterConfiguration::on_Ti_PMP03_editingFinished()
{
    running_para[6].ti = quint16(ui->Ti_PMP03->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_PMP03, running_para[6].ti);
}
void ParameterConfiguration::on_Tsm_PMP03_editingFinished()
{
    running_para[6].tsm = quint16(ui->Tsm_PMP03->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_PMP03, running_para[6].tsm);
}

void ParameterConfiguration::on_Kp_PMP04_editingFinished()
{
    running_para[7].kp = quint16(ui->Kp_PMP04->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_PMP04, running_para[7].kp);
}
void ParameterConfiguration::on_Ti_PMP04_editingFinished()
{
    running_para[7].ti = quint16(ui->Ti_PMP04->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_PMP04, running_para[7].ti);
}
void ParameterConfiguration::on_Tsm_PMP04_editingFinished()
{
    running_para[7].tsm = quint16(ui->Tsm_PMP04->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_PMP04, running_para[7].tsm);
}

void ParameterConfiguration::on_Kp_PMP05_editingFinished()
{
    running_para[8].kp = quint16(ui->Kp_PMP05->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_PMP05, running_para[8].kp);
}
void ParameterConfiguration::on_Ti_PMP05_editingFinished()
{
    running_para[8].ti = quint16(ui->Ti_PMP05->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_PMP05, running_para[8].ti);
}
void ParameterConfiguration::on_Tsm_PMP05_editingFinished()
{
    running_para[8].tsm = quint16(ui->Tsm_PMP05->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_PMP05, running_para[8].tsm);
}

void ParameterConfiguration::on_Kp_RAD01_editingFinished()
{
    running_para[9].kp = quint16(ui->Kp_RAD01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Kp_RAD01, running_para[9].kp);
}
void ParameterConfiguration::on_Ti_RAD01_editingFinished()
{
    running_para[9].ti = quint16(ui->Ti_RAD01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Ti_RAD01, running_para[9].ti);
}
void ParameterConfiguration::on_Tsm_RAD01_editingFinished()
{
    running_para[9].tsm = quint16(ui->Tsm_RAD01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Tsm_RAD01, running_para[9].tsm);
}

void ParameterConfiguration::on_lowPressure_PT03_editingFinished()
{
    m_parameters.low_pressure_pt03 = quint16(ui->lowPressure_PT03->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_LowPressure_PT03, m_parameters.low_pressure_pt03);
}

void ParameterConfiguration::on_highPressure_PT03_editingFinished()
{
    m_parameters.high_pressure_pt03 = quint16(ui->highPressure_PT03->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_HighPressure_PT03, m_parameters.high_pressure_pt03);
}

void ParameterConfiguration::on_highPressure_PT04_editingFinished()
{
    m_parameters.high_pressure_pt04 = quint16(ui->highPressure_PT04->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_HighPressure_PT04, m_parameters.high_pressure_pt04);
}

void ParameterConfiguration::on_highTemperature_TT17_editingFinished()
{
    m_parameters.high_temperature_tt17 = quint16(ui->highTemperature_TT17->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_HighTemperature_TT17, m_parameters.high_temperature_tt17);
}

void ParameterConfiguration::on_highTemperature_TT31_editingFinished()
{
    m_parameters.high_temperature_tt31 = quint16(ui->highTemperature_TT31->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_HighTemperature_TT31, m_parameters.high_temperature_tt31);
}

void ParameterConfiguration::on_highConductivity_CS01_editingFinished()
{
    m_parameters.high_conductivity = quint16(ui->highConductivity_CS01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_HighConductivity_CS01, m_parameters.high_conductivity);
}

void ParameterConfiguration::on_lowVoltage_BAT01_editingFinished()
{
    m_parameters.low_voltage_bat01 = quint16(ui->lowVoltage_BAT01->value()*10);
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_LowVoltage_BAT01, m_parameters.low_voltage_bat01);
}

void ParameterConfiguration::on_lowLevel_LT01_editingFinished()
{
    m_parameters.low_level_lt1 = quint16(ui->lowLevel_LT01->value());
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_LowLevel_LT01, m_parameters.low_level_lt1);
}

void ParameterConfiguration::on_autoLiquidLowLimit_LT01_editingFinished()
{
    m_parameters.auto_liquid_low_limit_lt1 = quint16(ui->autoLiquidLowLimit_LT01->value());
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_AutoLiquidLowLimit_LT01, m_parameters.auto_liquid_low_limit_lt1);
}

void ParameterConfiguration::on_stopLiquidValue_LT01_editingFinished()
{
    m_parameters.stop_liquid_limit_lt1 = quint16(ui->stopLiquidValue_LT01->value());
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_StopLiquidValue_LT01, m_parameters.stop_liquid_limit_lt1);
}

void ParameterConfiguration::on_lowLevel_LT02_editingFinished()
{
    m_parameters.low_level_lt2 = quint16(ui->lowLevel_LT02->value());
    current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_LowLevel_LT02, m_parameters.low_level_lt2);
}


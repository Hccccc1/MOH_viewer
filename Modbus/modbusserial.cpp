#include "modbusserial.h"
#include "ui_modbusserial.h"

#include <MOH_viewer/moh_viewer.h>

ModbusSerial::ModbusSerial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModbusSerial)
{
    ui->setupUi(this);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);

        if (serial.open(QIODevice::ReadWrite))
        {
            ui->serial_portname->addItem(serial.portName());
            serial.close();
        }
    }

    ui->parityCombo->setCurrentIndex(1);
    ui->baudCombo->setCurrentText(QString::number(m_settings.baud));
    ui->dataBitsCombo->setCurrentText(QString::number(m_settings.databits));
    ui->stopBitsCombo->setCurrentText(QString::number(m_settings.stopbits));

    ui->timeoutSpinner->setValue(m_settings.response_time);
    ui->retriesSpinner->setValue(m_settings.number_of_retries);

    prepare_vector_regs();

}

ModbusSerial::~ModbusSerial()
{
    delete ui;
}

ModbusSerial::Settings ModbusSerial::settings() const
{
    return m_settings;
}

void ModbusSerial::on_confirm_btn_clicked()
{
    qDebug() << __func__ << __LINE__;

    if (qobject_cast<QPushButton *>(sender()) != nullptr)
    {
        hide();
        //        m_settings.portname = ui->serial_portname->currentText();
        m_settings.portname = tr("COM3");
        m_settings.parity = ui->parityCombo->currentIndex();
        if (m_settings.parity > 0)
            m_settings.parity++;
        m_settings.baud = ui->baudCombo->currentText().toInt();
        m_settings.databits = ui->dataBitsCombo->currentText().toInt();
        m_settings.stopbits = ui->stopBitsCombo->currentText().toInt();
        m_settings.response_time = ui->timeoutSpinner->value();
        m_settings.number_of_retries = ui->retriesSpinner->value();
        m_settings.slave_addr = ui->slaveAddr_spinner->value();
    }

    if (modbus_client->state() != QModbusDevice::ConnectedState)
    {
        modbus_client->setConnectionParameter(QModbusDevice::SerialPortNameParameter, m_settings.portname);
        modbus_client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, m_settings.baud);
        modbus_client->setConnectionParameter(QModbusDevice::SerialParityParameter, m_settings.parity);
        modbus_client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, m_settings.databits);
        modbus_client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, m_settings.stopbits);

        modbus_client->setTimeout(m_settings.response_time);
        modbus_client->setNumberOfRetries(m_settings.number_of_retries);
    }

    if (!modbus_client->connectDevice())
    {
        qDebug() << __func__ << __LINE__ << "Connected failed...";
    }

    emit serial_connected();

    qDebug() << modbus_client->connectionParameter(QModbusDevice::SerialPortNameParameter) <<
                modbus_client->connectionParameter(QModbusDevice::SerialBaudRateParameter) <<
                modbus_client->connectionParameter(QModbusDevice::SerialParityParameter)   <<
                modbus_client->connectionParameter(QModbusDevice::SerialDataBitsParameter) <<
                modbus_client->connectionParameter(QModbusDevice::SerialStopBitsParameter);
}

void ModbusSerial::change_portname(QString portname)
{
    m_settings.portname = portname;
}

//void ModbusSerial::onReadButtonClicked()
//{
//    if (!modbus_client)
//        return;

//    if (auto *reply = modbus_client->sendReadRequest(readRequest(), m_settings.slave_addr))
//    {
//        if (!reply->isFinished())
//            connect(reply, &QModbusReply::finished, this, &ModbusSerial::onReadyRead);
//        else
//            delete reply;
//    }
//    else
//        qDebug() << "Read error: " << modbus_client->errorString();
//}

void ModbusSerial::read_from_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &number_of_entries)
{
    MOH_viewer *mainwindow = qobject_cast<MOH_viewer *>(this->parent());

    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    if (auto *reply = modbus_client->sendReadRequest(readRequest(type, start_addr, number_of_entries), m_settings.slave_addr))
    {
        if (!reply->isFinished())
        {
            //不同的widget在各自的槽函数接收数据
//            switch (start_addr & 0xf000) {
//            case Coils:
//                connect(reply, &QModbusReply::finished, mainwindow->control_panel_widget, &ControlPanel::onReadyRead);
//                break;
//            case DiscreteInputs:
//                connect(reply, &QModbusReply::finished, mainwindow, &MOH_viewer::onReadyRead);
//                break;
//            case InputRegisters:
//                connect(reply, &QModbusReply::finished, mainwindow->device_status_widget, &DeviceStatus::onReadyRead);
//                break;
//            case HoldingRegisters:
//                connect(reply, &QModbusReply::finished, mainwindow->para_conf, &ParameterConfiguration::onReadyRead);
//                break;
//            }
            if (mohviewer_regs.contains(quint16(start_addr)))
                connect(reply, &QModbusReply::finished, mainwindow, &MOH_viewer::onReadyRead);
            if (control_panel_regs.contains(quint16(start_addr)))
                connect(reply, &QModbusReply::finished, mainwindow->control_panel_widget, &ControlPanel::onReadyRead);
            if (device_status_regs.contains(quint16(start_addr)))
                connect(reply, &QModbusReply::finished, mainwindow->device_status_widget, &DeviceStatus::onReadyRead);
            if (parameter_set_regs.contains(quint16(start_addr)))
                connect(reply, &QModbusReply::finished, mainwindow->para_conf, &ParameterConfiguration::onReadyRead);
        }
        else
            delete reply;
    }
    else
    {
        qDebug() << "Read error: " << modbus_client->errorString();
    }
}

void ModbusSerial::write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &data)
{
    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    Q_ASSERT(type == QModbusDataUnit::HoldingRegisters);

    QModbusDataUnit write_unit = writeRequest(type, start_addr, 1);

    if (type == QModbusDataUnit::HoldingRegisters)
        write_unit.setValue(0, data);

    if (auto *reply = modbus_client->sendWriteRequest(write_unit, 0x01))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [this, reply] ()
            {
                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    qDebug() << __FILE__ << __LINE__ << "Protocol error..." << reply->errorString() << this->objectName();
                }

                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
        }
    }
}

void ModbusSerial::write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const QVector<quint16> &data, const quint16 &number_of_entries)
{
    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    Q_ASSERT(type == QModbusDataUnit::HoldingRegisters);

    QModbusDataUnit write_unit = writeRequest(type, start_addr, number_of_entries);

    for (int i = 0, total = static_cast<int>(write_unit.valueCount()); i < total; ++i)
    {
        if (type == QModbusDataUnit::HoldingRegisters)
            write_unit.setValue(i, data[i]);
    }

    if (auto *reply = modbus_client->sendWriteRequest(write_unit, 0x01))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [this, reply] ()
            {
                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    qDebug() << __FILE__ << __LINE__ << "Protocol error...";
//                    this->
                }

                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
        }
    }
}

void ModbusSerial::write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &number_of_entries, const bool &enable)
{
    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    QModbusDataUnit write_unit = writeRequest(type, start_addr, number_of_entries);

    for (int i = 0, total = static_cast<int>(write_unit.valueCount()); i < total; ++i)
    {
        if (type == QModbusDataUnit::Coils)
        {
            if (enable)
                write_unit.setValue(i, 1);
            else
                write_unit.setValue(i, 0);
        }
    }

    if (auto *reply = modbus_client->sendWriteRequest(write_unit, 0x01))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [this, reply] ()
            {
                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    qDebug() << __FILE__ << __LINE__ << "Protocol error...";
                }

                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
        }
    }
}
/*
void ModbusSerial::write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &bit, const int &start_addr)
{
    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    Q_ASSERT(type == QModbusDataUnit::Coils);
    Q_ASSERT(bit < 16 && bit >= 0);

    QModbusDataUnit write_unit = writeRequest(type, start_addr, 1);
    write_unit.setValue(0, 1);

    if (auto *reply = modbus_client->sendWriteRequest(write_unit, 0x01))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [this, reply] ()
            {
                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    qDebug() << __FILE__ << __LINE__ << "Protocol error...";
                }

                reply->deleteLater();
            });
        }
        else
        {
            if (write_unit.registerType() == QModbusDataUnit::Coils)
            {
                m_coils.clearBit(bit);
            }
            else if (write_unit.registerType() == QModbusDataUnit::HoldingRegisters)
            {
                for (quint8 i = 0; i < write_unit.valueCount(); i++)
                    m_holdingRegisters[i] = 0x0;
            }

            reply->deleteLater();
        }
    }
}
*/
void ModbusSerial::onReadyRead()
{
    auto *reply = qobject_cast<QModbusReply *>(sender());

    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
    }
}

QModbusDataUnit ModbusSerial::readRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const
{
    return QModbusDataUnit(type, start_addr, number_of_entries);
}

QModbusDataUnit ModbusSerial::writeRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const
{
    return QModbusDataUnit(type, start_addr, number_of_entries);
}

void ModbusSerial::prepare_vector_regs()
{
    mohviewer_regs = {
        HoldingRegs_DevSlaveAddr,
        HoldingRegs_FirmwareVersion,
        HoldingRegs_HardwareVersion,
        HoldingRegs_DevIPAddr,
        HoldingRegs_SerialPara,
        InputRegs_SysStatus,
        DiscreteInputs_SelfCheck_TT03,
        DiscreteInputs_SelfCheck_TT05,
        DiscreteInputs_SelfCheck_TT15,
        DiscreteInputs_SelfCheck_TT16,
        DiscreteInputs_SelfCheck_TT17,
        DiscreteInputs_SelfCheck_TT19,
        DiscreteInputs_SelfCheck_TT23,
        DiscreteInputs_SelfCheck_TT24,
        DiscreteInputs_SelfCheck_TT25,
        DiscreteInputs_SelfCheck_TT27,
        DiscreteInputs_SelfCheck_TT29,
        DiscreteInputs_SelfCheck_TT31,
        DiscreteInputs_SelfCheck_TT33,
        DiscreteInputs_SelfCheck_TT34,
        DiscreteInputs_SelfCheck_TT37,
        DiscreteInputs_SelfCheck_PT01,
        DiscreteInputs_SelfCheck_PT02,
        DiscreteInputs_SelfCheck_PT03,
        DiscreteInputs_SelfCheck_PT04,
        DiscreteInputs_SelfCheck_PT05,
        DiscreteInputs_SelfCheck_PT06,
        DiscreteInputs_SelfCheck_AFM01,
        DiscreteInputs_SelfCheck_AFM02,
        DiscreteInputs_SelfCheck_AFM03,
        DiscreteInputs_SelfCheck_AFM04,
        DiscreteInputs_SelfCheck_MFM04,
        DiscreteInputs_SelfCheck_CM01,
        DiscreteInputs_SelfCheck_VT01,
        DiscreteInputs_SelfCheck_VT02,
        DiscreteInputs_SelfCheck_IT01,
        DiscreteInputs_SelfCheck_IT02,
        DiscreteInputs_SelfCheck_LT01,
        DiscreteInputs_SelfCheck_LT02,
        DiscreteInputs_LowPressure_PT03,
        DiscreteInputs_HighPressure_PT03,
        DiscreteInputs_HighPressure_PT05,
        DiscreteInputs_HighTemperature_TT17,
        DiscreteInputs_HighTemperature_TT18,
        DiscreteInputs_ConductivityAbnormal_CS01,
        DiscreteInputs_LowVoltage_BAT01,
        DiscreteInputs_LowLevel_LT1,
        DiscreteInputs_LowLevel_LT2,
        DiscreteInputs_LowLoading,
        HoldingRegs_SysTotalTime,
        CoilsRegs_SysCtrlSelfCheck,
        CoilsRegs_SysCtrlStart,
        CoilsRegs_SysCtrlRun,
        CoilsRegs_SysCtrlShutDown,
        CoilsRegs_SysCtrlEmergencyShutDown,
        CoilsRegs_SysCtrlReset,
        CoilsRegs_AutoCtrl,
        CoilsRegs_AutoCharge,
        HoldingRegs_PowerMode,
        DiscreteInputs_Status_Can,
        DiscreteInputs_Status_RS485_1,
        DiscreteInputs_Status_RS485_2,
        DiscreteInputs_Status_RS485_3,
        DiscreteInputs_Status_4G,
        DiscreteInputs_Status_Enternet
    };

    control_panel_regs = {
        CoilsRegs_SV_01_CtrlEnable,
        CoilsRegs_SV_02_CtrlEnable,
        CoilsRegs_SV_03_CtrlEnable,
        CoilsRegs_SV_04_CtrlEnable,
        CoilsRegs_SV_05_CtrlEnable,
        CoilsRegs_SV_06_CtrlEnable,
        CoilsRegs_SV_07_CtrlEnable,
        CoilsRegs_SV_08_CtrlEnable,
        CoilsRegs_SV_09_CtrlEnable,
        CoilsRegs_SV_10_CtrlEnable,
        CoilsRegs_SV_11_CtrlEnable,
        CoilsRegs_SV_12_CtrlEnable,
        CoilsRegs_SV_13_CtrlEnable,
        CoilsRegs_SV_14_CtrlEnable,
        CoilsRegs_BL_01_CtrlEnable,
        CoilsRegs_BL_02_CtrlEnable,
        CoilsRegs_BL_03_CtrlEnable,
        CoilsRegs_BL_04_CtrlEnable,
        CoilsRegs_PMP_01_CtrlEnable,
        CoilsRegs_PMP_02_CtrlEnable,
        CoilsRegs_PMP_03_CtrlEnable,
        CoilsRegs_PMP_04_CtrlEnable,
        CoilsRegs_PMP_05_CtrlEnable,
        CoilsRegs_RAD_01_CtrlEnable,
        CoilsRegs_KM_01_CtrlEnable,
        DiscreteInputs_OutputFeedback_SV01,
        DiscreteInputs_OutputFeedback_SV02,
        DiscreteInputs_OutputFeedback_SV03,
        DiscreteInputs_OutputFeedback_SV04,
        DiscreteInputs_OutputFeedback_SV05,
        DiscreteInputs_OutputFeedback_SV06,
        DiscreteInputs_OutputFeedback_SV07,
        DiscreteInputs_OutputFeedback_SV08,
        DiscreteInputs_OutputFeedback_SV09,
        DiscreteInputs_OutputFeedback_SV10,
        DiscreteInputs_OutputFeedback_SV11,
        DiscreteInputs_OutputFeedback_SV12,
        DiscreteInputs_OutputFeedback_SV13,
        DiscreteInputs_OutputFeedback_SV14,
        DiscreteInputs_OutputFeedback_BL01,
        DiscreteInputs_OutputFeedback_BL02,
        DiscreteInputs_OutputFeedback_BL03,
        DiscreteInputs_OutputFeedback_BL04,
        DiscreteInputs_OutputFeedback_PMP01,
        DiscreteInputs_OutputFeedback_PMP02,
        DiscreteInputs_OutputFeedback_PMP03,
        DiscreteInputs_OutputFeedback_PMP04,
        DiscreteInputs_OutputFeedback_PMP05,
        DiscreteInputs_OutputFeedback_RAD01,
        DiscreteInputs_OutputFeedback_KM01,
        InputRegs_BL_01,
        InputRegs_BL_02,
        InputRegs_BL_03,
        InputRegs_BL_04,
        InputRegs_PMP_01,
        InputRegs_PMP_02,
        InputRegs_PMP_03,
        InputRegs_PMP_04,
        InputRegs_PMP_05,
        InputRegs_RAD_01,
        CoilsRegs_BL_01_AutoCtrl,
        CoilsRegs_BL_02_AutoCtrl,
        CoilsRegs_BL_03_AutoCtrl,
        CoilsRegs_BL_04_AutoCtrl,
        CoilsRegs_PMP_01_AutoCtrl,
        CoilsRegs_PMP_02_AutoCtrl,
        CoilsRegs_PMP_03_AutoCtrl,
        CoilsRegs_PMP_04_AutoCtrl,
        CoilsRegs_PMP_05_AutoCtrl,
        CoilsRegs_RAD_01_AutoCtrl,
    };

    device_status_regs = {
        InputRegs_SysStatus, //1
        InputRegs_OutVoltage, //1
        InputRegs_OutCurrent, //1
        InputRegs_OutPower, //1
        InputRegs_SinglePowerProduced, //1
        InputRegs_TotalPowerProduced, //1
        InputRegs_VT_02, //1
        InputRegs_IT_02, //1
        InputRegs_VT_01, //1
        InputRegs_IT_01, //1
        InputRegs_FcPower, //1
        HoldingRegs_SysTime, //1
        HoldingRegs_SysSingleTime, //1
        HoldingRegs_SysTotalTime, //1
        InputRegs_TotalBootTimes, //1
        InputRegs_LT_01, //1
        InputRegs_LT_02, //1
        InputRegs_FuelConsumption, //1
        InputRegs_TotalFuelConsumption, //1
        InputRegs_TT_01,
        InputRegs_TT_02,
        InputRegs_TT_03,
        InputRegs_TT_04,
        InputRegs_TT_05,
        InputRegs_TT_06,
        InputRegs_TT_07,
        InputRegs_TT_08,
        InputRegs_TT_09,
        InputRegs_TT_10,
        InputRegs_TT_11,
        InputRegs_TT_12,
        InputRegs_TT_13,
        InputRegs_TT_14,
        InputRegs_TT_15,
        InputRegs_TT_16,
        InputRegs_TT_17,
        InputRegs_TT_18,
        InputRegs_TT_19,
        InputRegs_TT_20,
        InputRegs_TT_21,
        InputRegs_TT_22,
        InputRegs_TT_23,
        InputRegs_TT_24,
        InputRegs_TT_25,
        InputRegs_TT_26,
        InputRegs_TT_27,
        InputRegs_TT_28,
        InputRegs_TT_29,
        InputRegs_TT_30,
        InputRegs_TT_31,
        InputRegs_TT_32,
        InputRegs_TT_33,
        InputRegs_TT_34,
        InputRegs_TT_35,
        InputRegs_TT_36,
        InputRegs_PT_01,
        InputRegs_PT_02,
        InputRegs_PT_03,
        InputRegs_PT_04,
        InputRegs_PT_05,
        InputRegs_PT_06,
        InputRegs_AFM_01,
        InputRegs_AFM_02,
        InputRegs_AFM_03,
        InputRegs_AFM_04,
        InputRegs_MFM_01,
        InputRegs_BL_01,
        InputRegs_BL_02,
        InputRegs_BL_03,
        InputRegs_BL_04,
        InputRegs_PMP_01,
        InputRegs_PMP_02,
        InputRegs_PMP_03,
        InputRegs_PMP_04,
        InputRegs_PMP_05,
        InputRegs_RAD_01,
        InputRegs_CM_01,
        InputRegs_LT_01,
        InputRegs_LT_02,
        InputRegs_VT_01,
        InputRegs_IT_01,
        InputRegs_VT_02,
        InputRegs_IT_02
    };

    parameter_set_regs = {
        HoldingRegs_Manufacturer,
        HoldingRegs_ProductName,
        HoldingRegs_ProductType,
        HoldingRegs_ProductSerialNum,
        HoldingRegs_ProductDate,
        HoldingRegs_ReformingID,
        HoldingRegs_StackID,
        HoldingRegs_DevSlaveAddr,
        HoldingRegs_DevIPAddr,
        HoldingRegs_SerialPara,
        HoldingRegs_PowerMode,
        HoldingRegs_FCOutCurrent,
        HoldingRegs_FCOutPower,
        HoldingRegs_BatChargeStartVoltage,
        HoldingRegs_BatChargeStartDelay,
        HoldingRegs_BatChargeStopVoltage,
        HoldingRegs_BatChargeStopDelay,
        HoldingRegs_DataStorageCycle,
        HoldingRegs_Kp_BL01,
        HoldingRegs_Ti_BL01,
        HoldingRegs_Tsm_BL01,
        HoldingRegs_Kp_BL02,
        HoldingRegs_Ti_BL02,
        HoldingRegs_Tsm_BL02,
        HoldingRegs_Kp_BL03,
        HoldingRegs_Ti_BL03,
        HoldingRegs_Tsm_BL03,
        HoldingRegs_Kp_BL04,
        HoldingRegs_Ti_BL04,
        HoldingRegs_Tsm_BL04,
        HoldingRegs_Kp_PMP01,
        HoldingRegs_Ti_PMP01,
        HoldingRegs_Tsm_PMP01,
        HoldingRegs_Kp_PMP02,
        HoldingRegs_Ti_PMP02,
        HoldingRegs_Tsm_PMP02,
        HoldingRegs_Kp_PMP03,
        HoldingRegs_Ti_PMP03,
        HoldingRegs_Tsm_PMP03,
        HoldingRegs_Kp_PMP04,
        HoldingRegs_Ti_PMP04,
        HoldingRegs_Tsm_PMP04,
        HoldingRegs_Kp_PMP05,
        HoldingRegs_Ti_PMP05,
        HoldingRegs_Tsm_PMP05,
        HoldingRegs_Kp_RAD01,
        HoldingRegs_Ti_RAD01,
        HoldingRegs_Tsm_RAD01,
        HoldingRegs_LowPressure_PT03,
        HoldingRegs_HighPressure_PT03,
        HoldingRegs_HighPressure_PT04,
        HoldingRegs_HighTemperature_TT17,
        HoldingRegs_HighTemperature_TT31,
        HoldingRegs_HighConductivity_CS01,
        HoldingRegs_LowVoltage_BAT01,
        HoldingRegs_LowLevel_LT01,
        HoldingRegs_AutoLiquidLowLimit_LT01,
        HoldingRegs_StopLiquidValue_LT01,
        HoldingRegs_LowLevel_LT02,
        CoilsRegs_LT_01_AlarmCtrl,
        CoilsRegs_LT_02_AlarmCtrl,
    };
}

#include "modbusserial.h"
#include "ui_modbusserial.h"

#include <MOH_viewer/moh_viewer.h>

ModbusSerial::ModbusSerial(QWidget *parent, DeviceLog *log_handler) :
    QDialog(parent),
    ui(new Ui::ModbusSerial),
    current_log_handler(log_handler)
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

    ui->disconnectBtn->setDisabled(true);

    prepare_vector_regs();

    connect(this, &ModbusSerial::communicationRecord,
            current_log_handler->communicationLogs, &CommunicationLogs::addCommunicationRecord);

    //    rw_thread->start(QThread::NormalPriority);

}

ModbusSerial::~ModbusSerial()
{
    delete ui;

    //    rw_thread->deleteLater();
}

ModbusSerial::Settings ModbusSerial::settings() const
{
    return m_settings;
}

void ModbusSerial::on_confirm_btn_clicked()
{
    //    qDebug() << __func__ << __LINE__;

    if (qobject_cast<QPushButton *>(sender()) != nullptr)
    {
        hide();
        m_settings.portname = ui->serial_portname->currentText();
        //        m_settings.portname = tr("COM3");
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

    if (modbus_client->state() == QModbusDevice::ConnectedState)
    {
        modbus_client->disconnectDevice();
    }

    modbus_client->setConnectionParameter(QModbusDevice::SerialPortNameParameter, m_settings.portname);
    modbus_client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, m_settings.baud);
    modbus_client->setConnectionParameter(QModbusDevice::SerialParityParameter, m_settings.parity);
    modbus_client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, m_settings.databits);
    modbus_client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, m_settings.stopbits);

    modbus_client->setTimeout(m_settings.response_time);
    modbus_client->setNumberOfRetries(m_settings.number_of_retries);

    if (!modbus_client->connectDevice())
    {
        qDebug() << __func__ << __LINE__ << "Connected failed...";
        return;
    }
    else
    {
        open_port();
    }

    emit serial_connected();

    //    qDebug() << modbus_client->connectionParameter(QModbusDevice::SerialPortNameParameter) <<
    //                modbus_client->connectionParameter(QModbusDevice::SerialBaudRateParameter) <<
    //                modbus_client->connectionParameter(QModbusDevice::SerialParityParameter)   <<
    //                modbus_client->connectionParameter(QModbusDevice::SerialDataBitsParameter) <<
    //                modbus_client->connectionParameter(QModbusDevice::SerialStopBitsParameter);
}

void ModbusSerial::on_disconnectBtn_clicked()
{
    close_port();

    emit serial_disconnected();
}

void ModbusSerial::on_errorHappened(QModbusDevice::Error error)
{
    qDebug() << sender()->objectName() << error;

    if (error != QModbusDevice::NoError && modbus_client->state() == QModbusDevice::ConnectedState)
        QMessageBox::warning(this, tr("通讯异常"), QString(tr("串口读写失败：%1。即将断开串口！")).arg(error));

    if (modbus_client->state() == QModbusDevice::ConnectedState)
    {
        modbus_client->disconnectDevice();

        close_port();

        emit serial_disconnected();
    }
}

void ModbusSerial::open_port()
{
    ui->serial_portname->setDisabled(true);
    ui->parityCombo->setDisabled(true);
    ui->baudCombo->setDisabled(true);
    ui->dataBitsCombo->setDisabled(true);
    ui->stopBitsCombo->setDisabled(true);
    ui->slaveAddr_spinner->setDisabled(true);
    ui->timeoutSpinner->setDisabled(true);
    ui->retriesSpinner->setDisabled(true);
    ui->confirm_btn->setDisabled(true);

    ui->disconnectBtn->setEnabled(true);
}

void ModbusSerial::close_port()
{
    if (modbus_client->state() == QModbusDevice::ConnectedState)
        modbus_client->disconnectDevice();

    ui->serial_portname->setEnabled(true);
    ui->parityCombo->setEnabled(true);
    ui->baudCombo->setEnabled(true);
    ui->dataBitsCombo->setEnabled(true);
    ui->stopBitsCombo->setEnabled(true);
    ui->slaveAddr_spinner->setEnabled(true);
    ui->timeoutSpinner->setEnabled(true);
    ui->retriesSpinner->setEnabled(true);
    ui->confirm_btn->setEnabled(true);
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

//void ModbusSerial::Crc16Out(QByteArray &QBABytes) {
//    int buffer_length= QBABytes.length();
//    unsigned char *buffer= (unsigned char*)QBABytes.data();
//    unsigned char crc_hi = 0xFF; /* high CRC byte initialized */
//    unsigned char crc_lo = 0xFF; /* low CRC byte initialized */
//    int i; /* will index into CRC lookup */
//    while (buffer_length--) {
//        i = crc_hi ^ *buffer++; /* calculate the CRC  */
//        crc_hi = crc_lo ^ table_crc_hi[i];
//        crc_lo = table_crc_lo[i];
//    }
//    QBABytes.append(crc_hi);
//    QBABytes.append(crc_lo);
//}

WORD CRC16 (const BYTE *nData, WORD wLength)
{
    static const WORD wCRCTable[] = {
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

    BYTE nTemp;
    WORD wCRCWord = 0xFFFF;

    while (wLength--)
    {
        nTemp = *nData++ ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord  ^= wCRCTable[nTemp];
    }
    return wCRCWord;
}

QByteArray ModbusSerial::makeRTUFrame(int slave, int function, const QByteArray & data) {
    Q_ASSERT(data.size() <= 252);
    QByteArray frame;
    QDataStream ds(&frame, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds << quint8(slave) << quint8(function);
    ds.writeRawData(data.constData(), data.size());
    int const crc = CRC16((BYTE*)frame.constData(), frame.size());
    ds << quint16(crc);
    return frame;
}

void ModbusSerial::read_from_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &number_of_entries)
{

    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    QModbusDataUnit read_request = readRequest(type, start_addr, number_of_entries);

    rw_thread->insert_read_unit(read_request);

    //    if (rw_mutex->tryLock(100))
    //    {
//    if (auto *reply = modbus_client->sendReadRequest(readRequest(type, start_addr, number_of_entries), m_settings.slave_addr))
//    {
//        if (!reply->isFinished())
//        {
            //                rw_mutex->unlock();

//            if (mohviewer_regs.contains(quint16(start_addr)))
//                connect(reply, &QModbusReply::finished, mainwindow, &MOH_viewer::onReadyRead);
//            if (control_panel_regs.contains(quint16(start_addr)))
//                connect(reply, &QModbusReply::finished, mainwindow->control_panel_widget, &ControlPanel::onReadyRead);
//            if (device_status_regs.contains(quint16(start_addr)))
//                connect(reply, &QModbusReply::finished, mainwindow->device_status_widget, &DeviceStatus::onReadyRead);
//            if (parameter_set_regs.contains(quint16(start_addr)))
//                connect(reply, &QModbusReply::finished, mainwindow->para_conf, &ParameterConfiguration::onReadyRead);

//        }
//        else
//            delete reply;
//    }
//    else
//    {
//        qDebug() << "Read error: " << modbus_client->errorString();
//    }
    //    }
}

void ModbusSerial::on_modbus_reply_finished(const QModbusDataUnit ori_request, QModbusReply *reply)
{
    MOH_viewer *mainwindow = qobject_cast<MOH_viewer *>(this->parent());

    if (mohviewer_regs.contains(quint16(ori_request.startAddress())))
        connect(reply, &QModbusReply::finished, mainwindow, &MOH_viewer::onReadyRead);
    if (control_panel_regs.contains(quint16(ori_request.startAddress())))
        connect(reply, &QModbusReply::finished, mainwindow->control_panel_widget, &ControlPanel::onReadyRead);
    if (device_status_regs.contains(quint16(ori_request.startAddress())))
        connect(reply, &QModbusReply::finished, mainwindow->device_status_widget, &DeviceStatus::onReadyRead);
    if (parameter_set_regs.contains(quint16(ori_request.startAddress())))
        connect(reply, &QModbusReply::finished, mainwindow->para_conf, &ParameterConfiguration::onReadyRead);
}

void ModbusSerial::write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &data)
{
#if 0
    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    Q_ASSERT(type == QModbusDataUnit::HoldingRegisters);

    QModbusDataUnit write_unit = writeRequest(type, start_addr, 1);

    if (type == QModbusDataUnit::HoldingRegisters)
        write_unit.setValue(0, data);

    if (write_unit.isValid() && write_unit.valueCount() != 0)
    {
        QString result_str = makeRTUFrame(1, createWriteRequest(write_unit).functionCode(), createWriteRequest(write_unit).data()).toHex();
        //        emit communicationRecord("TX", result_str.to);
    }

    //    if (rw_mutex->tryLock(100))
    //    {
    if (auto *reply = modbus_client->sendWriteRequest(write_unit, 0x01))
    {
        //            rw_mutex->unlock();

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
    //    }
#endif
}

void ModbusSerial::write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const QVector<quint16> &data, const quint16 &number_of_entries)
{
#if 0
    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    Q_ASSERT(type == QModbusDataUnit::HoldingRegisters);

    QModbusDataUnit write_unit = writeRequest(type, start_addr, number_of_entries);

    for (int i = 0, total = static_cast<int>(write_unit.valueCount()); i < total; ++i)
    {
        if (type == QModbusDataUnit::HoldingRegisters)
            write_unit.setValue(i, data[i]);
    }

    if (write_unit.isValid())
    {
        QString result_str = makeRTUFrame(1, createWriteRequest(write_unit).functionCode(), createWriteRequest(write_unit).data()).toHex();
        emit communicationRecord("TX", result_str);
    }

    //    if (rw_mutex->tryLock(100))
    //    {
    if (auto *reply = modbus_client->sendWriteRequest(write_unit, 0x01))
    {
        if (!reply->isFinished())
        {
            //                rw_mutex->unlock();

            connect(reply, &QModbusReply::finished, this, [reply] ()
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
    //    }
#endif
}

void ModbusSerial::write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &number_of_entries, const bool &enable)
{
#if 0
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

    if (write_unit.isValid())
    {
        QString result_str = makeRTUFrame(1, createWriteRequest(write_unit).functionCode(), createWriteRequest(write_unit).data()).toHex();
        emit communicationRecord("TX", result_str);
    }

    //    if (rw_mutex->tryLock(100))
    //    {
    if (auto *reply = modbus_client->sendWriteRequest(write_unit, 0x01))
    {
        if (!reply->isFinished())
        {
            //                rw_mutex->unlock();

            connect(reply, &QModbusReply::finished, this, [reply] ()
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
    //    }
#endif
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
//void ModbusSerial::onReadyRead()
//{
//    auto *reply = qobject_cast<QModbusReply *>(sender());

//    if (!reply)
//        return;

//    if (reply->error() == QModbusDevice::NoError)
//    {
//        const QModbusDataUnit unit = reply->result();
//    }
//}

QModbusResponse ModbusSerial::createReadRequest(const QModbusDataUnit &data)
{
    if (!data.isValid())
        return QModbusRequest();

    switch (data.registerType()) {
    case QModbusDataUnit::Coils:
        return QModbusRequest(QModbusRequest::ReadCoils, quint16(data.startAddress()),
                              quint16(data.valueCount()));
    case QModbusDataUnit::DiscreteInputs:
        return QModbusRequest(QModbusRequest::ReadDiscreteInputs, quint16(data.startAddress()),
                              quint16(data.valueCount()));
    case QModbusDataUnit::InputRegisters:
        return QModbusRequest(QModbusRequest::ReadInputRegisters, quint16(data.startAddress()),
                              quint16(data.valueCount()));
    case QModbusDataUnit::HoldingRegisters:
        return QModbusRequest(QModbusRequest::ReadHoldingRegisters, quint16(data.startAddress()),
                              quint16(data.valueCount()));
    default:
        break;
    }

    return QModbusRequest();
}

QModbusRequest ModbusSerial::createWriteRequest(const QModbusDataUnit &data)
{
    switch (data.registerType()) {
    case QModbusDataUnit::Coils: {
        if (data.valueCount() == 1) {
            return QModbusRequest(QModbusRequest::WriteSingleCoil, quint16(data.startAddress()),
                                  quint16((data.value(0) == 0u) ? Coil::Off : Coil::On));
        }

        quint8 byteCount = data.valueCount() / 8;
        if ((data.valueCount() % 8) != 0)
            byteCount += 1;

        quint8 address = 0;
        QVector<quint8> bytes;
        for (quint8 i = 0; i < byteCount; ++i) {
            quint8 byte = 0;
            for (int currentBit = 0; currentBit < 8; ++currentBit)
                if (data.value(address++))
                    byte |= (1U << currentBit);
            bytes.append(byte);
        }

        return QModbusRequest(QModbusRequest::WriteMultipleCoils, quint16(data.startAddress()),
                              quint16(data.valueCount()), byteCount, bytes);
    }   break;

    case QModbusDataUnit::HoldingRegisters: {
        if (data.valueCount() == 1) {
            return QModbusRequest(QModbusRequest::WriteSingleRegister, quint16(data.startAddress()),
                                  data.value(0));
        }

        const quint8 byteCount = data.valueCount() * 2;
        return QModbusRequest(QModbusRequest::WriteMultipleRegisters, quint16(data.startAddress()),
                              quint16(data.valueCount()), byteCount, data.values());
    }   break;

    case QModbusDataUnit::DiscreteInputs:
    case QModbusDataUnit::InputRegisters:
    default:    // fall through on purpose
        break;
    }
    return QModbusRequest();
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
        CoilsRegs_SysCtrlSelfCheck,
        CoilsRegs_SysCtrlStart,
        CoilsRegs_SysCtrlRun,
        CoilsRegs_SysCtrlShutDown,
        CoilsRegs_SysCtrlEmergencyShutDown,
        CoilsRegs_SysCtrlReset,

        CoilsRegs_AutoCtrl,
        CoilsRegs_AutoCharge,

        DiscreteInputs_IOInput00,
        DiscreteInputs_IOInput01,
        DiscreteInputs_IOInput02,
        DiscreteInputs_IOInput03,
        DiscreteInputs_IOInput04,

        DiscreteInputs_Status_Can,
        DiscreteInputs_Status_RS485_1,
        DiscreteInputs_Status_RS485_2,
        DiscreteInputs_Status_RS485_3,
        DiscreteInputs_Status_4G,
        DiscreteInputs_Status_Enternet,

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
        DiscreteInputs_HighTemperature_TT31,
        DiscreteInputs_ConductivityAbnormal_CS01,
        DiscreteInputs_LowVoltage_BAT01,
        DiscreteInputs_LowLevel_LT1,
        DiscreteInputs_LowLevel_LT2,
        DiscreteInputs_LowLoading,

        InputRegs_SysStatus,

        InputRegs_VT_01,
        InputRegs_IT_01,
        InputRegs_FcPower,

        HoldingRegs_FirmwareVersion,
        HoldingRegs_HardwareVersion,

        HoldingRegs_DevSlaveAddr,
        HoldingRegs_DevIPAddr,      //2byte
        HoldingRegs_SerialPara,
        HoldingRegs_SysTotalTime,   //3byte

        HoldingRegs_PowerMode,
    };

    control_panel_regs = {
        CoilsRegs_SV_01,
        CoilsRegs_SV_02,
        CoilsRegs_SV_03,
        CoilsRegs_SV_04,
        CoilsRegs_SV_05,
        CoilsRegs_SV_06,
        CoilsRegs_SV_07,
        CoilsRegs_SV_08,
        CoilsRegs_SV_09,
        CoilsRegs_SV_10,
        CoilsRegs_SV_11,
        CoilsRegs_SV_12,
        CoilsRegs_SV_13,
        CoilsRegs_SV_14,
        CoilsRegs_BL_01,
        CoilsRegs_BL_02,
        CoilsRegs_BL_03,
        CoilsRegs_BL_04,
        CoilsRegs_PMP_01,
        CoilsRegs_PMP_02,
        CoilsRegs_PMP_03,
        CoilsRegs_PMP_04,
        CoilsRegs_PMP_05,
        CoilsRegs_RAD_01,
        CoilsRegs_KM_01,
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

        HoldingRegs_SpeedCtrl_BL01,
        HoldingRegs_SpeedCtrl_BL02,
        HoldingRegs_SpeedCtrl_BL03,
        HoldingRegs_SpeedCtrl_BL04,
        HoldingRegs_SpeedCtrl_PMP01,
        HoldingRegs_SpeedCtrl_PMP02,
        HoldingRegs_SpeedCtrl_PMP03,
        HoldingRegs_SpeedCtrl_PMP04,
        HoldingRegs_SpeedCtrl_PMP05,
        HoldingRegs_SpeedCtrl_RAD01,
    };

    device_status_regs = {
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
        InputRegs_SysStatus, //1
        InputRegs_CM_01,
        InputRegs_LT_01,    //1 2
        InputRegs_LT_02,    //1 2
        InputRegs_VT_01,    //1 2
        InputRegs_IT_01,    //1 2
        InputRegs_FcPower,  //1
        InputRegs_OutVoltage, //1
        InputRegs_OutCurrent, //1
        InputRegs_OutPower, //1
        InputRegs_VT_02, //1
        InputRegs_IT_02, //1
        InputRegs_SinglePowerProduced, //1  //2byte
        InputRegs_TotalPowerProduced, //1   //2byte
        InputRegs_TotalBootTimes, //1
        InputRegs_FuelConsumption, //1
        InputRegs_TotalFuelConsumption, //1 //2byte

        HoldingRegs_SysTime, //1        //4byte
        HoldingRegs_SysSingleTime, //1  //2byte
        HoldingRegs_SysTotalTime, //1   //3byte

        HoldingRegs_SpeedCtrl_BL01,
        HoldingRegs_SpeedCtrl_BL02,
        HoldingRegs_SpeedCtrl_BL03,
        HoldingRegs_SpeedCtrl_BL04,
        HoldingRegs_SpeedCtrl_PMP01,
        HoldingRegs_SpeedCtrl_PMP02,
        HoldingRegs_SpeedCtrl_PMP03,
        HoldingRegs_SpeedCtrl_PMP04,
        HoldingRegs_SpeedCtrl_PMP05,
        HoldingRegs_SpeedCtrl_RAD01,
    };

    parameter_set_regs = {
        CoilsRegs_LT_01_AlarmCtrl,
        CoilsRegs_LT_02_AlarmCtrl,

        HoldingRegs_Manufacturer,
        HoldingRegs_ProductName,
        HoldingRegs_ProductType,
        HoldingRegs_ProductSerialNum,   //4byte
        HoldingRegs_ProductDate,

        HoldingRegs_ReformingID,    //4byte
        HoldingRegs_StackID,        //4byte
        HoldingRegs_DevSlaveAddr,
        HoldingRegs_DevIPAddr,      //2byte
        HoldingRegs_SerialPara,

        HoldingRegs_PowerMode,

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
        HoldingRegs_FCOutCurrent,
        HoldingRegs_FCOutPower,
        HoldingRegs_BatChargeStartVoltage,
        HoldingRegs_BatChargeStartDelay,
        HoldingRegs_BatChargeStopVoltage,
        HoldingRegs_BatChargeStopDelay,
        HoldingRegs_DataStorageCycle,
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
    };
}

void ModbusSerial::on_languageChangeBtn_clicked()
{
    if (trans != nullptr)
    {
        qApp->removeTranslator(trans);
        delete trans;
    }

    trans = new QTranslator();

    if (ui->languageChangeBtn->text() == "中文")
    {
        //        ui->languageChangeBtn->setText("中文");
        if (!trans->load(":/english.qm"))
        {
            qDebug() << __FILE__ << __LINE__ << "failed to load qm file";
        }
    }
    else
    {
        //        ui->languageChangeBtn->setText("English");
        if (!trans->load(":/chinese.qm"))
        {
            qDebug() << __FILE__ << __LINE__ << "failed to load qm file";
        }
    }

    qApp->installTranslator(trans);
}

void ModbusSerial::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

bool ModbusSerial::is_serial_ready() const
{
    return serial_ready;
}

void ModbusSerial::set_serial_state(const bool ready)
{
    serial_ready = ready;
}

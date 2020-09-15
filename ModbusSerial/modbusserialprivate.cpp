#include "modbusserialprivate.h"
#include "MOH_Viewer/moh_viewer.h"
#include <QDebug>

ModbusSerialPrivate::ModbusSerialPrivate(QWidget *parent) :
    QWidget(parent)
{
    //    this->setConnectionParameter()
    Q_UNUSED(parent);

    //    read_reply->setParent(client);
    //    write_reply->setParent(client);
}

void ModbusSerialPrivate::insert_slave_addr(const int& slave_addr)
{
    slave_addrs.insert(slave_addr, true);
}

void ModbusSerialPrivate::connect_serial(const ModbusSerial::Settings &setting)
{
    client->setConnectionParameter(QModbusDevice::SerialPortNameParameter, setting.portname);
    client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, setting.baud);
    client->setConnectionParameter(QModbusDevice::SerialParityParameter, setting.parity);
    client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, setting.databits);
    client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, setting.stopbits);
    client->setTimeout(setting.response_time);
    client->setNumberOfRetries(setting.number_of_retries);

    if (!client->connectDevice())
    {
        qDebug() << __func__ << __LINE__ << "Connected failed...";
        return;
    }
    else
    {
        emit set_serial_connec_state(true);
        emit serial_connected();
    }
}

void ModbusSerialPrivate::disconnect_serial()
{
    if (client->state() == QModbusDevice::ConnectedState)
    {
        client->disconnectDevice();
        emit serial_disconnected();

        qDebug() << __func__ << __LINE__ << client->state();
    }
}

void ModbusSerialPrivate::do_the_actual_read(const int &reg_type, const int &start_addr, const quint32 &num_of_entries, const int slave_addr)
{
    //    const ModbusSerial *serial = qobject_cast<ModbusSerial*>(sender());
    //    qDebug() << this->sender()->parent() << slave_addr;
    MOH_Viewer *moh = qobject_cast<MOH_Viewer *>(this->sender()->parent());
    ModbusSerial *serial = qobject_cast<ModbusSerial*>(this->sender());

    QModbusDataUnit read_req = QModbusDataUnit(QModbusDataUnit::RegisterType(reg_type), start_addr, quint16(num_of_entries));


    const QModbusReply *read_reply = client->sendReadRequest(read_req, slave_addr);

    if (read_reply)
    {
        if (!read_reply->isFinished() && client->state() == QModbusDevice::ConnectedState)
        {
            //            connect(reply, &QModbusReply::finished, serial, &ModbusSerial::request_finished);

            //            qDebug() << __func__ << __LINE__ << moh << serial;
            //            if (read_reply->serverAddress())

            if (slave_addrs.contains(slave_addr) && slave_addrs.find(slave_addr).key())
            {

                slave_addrs.remove(slave_addr);

                connect(read_reply, &QModbusReply::finished, moh, &MOH_Viewer::onReadyRead);
                connect(read_reply, &QModbusReply::finished, moh->control_panel_widget, &ControlPanel::onReadyRead);
                if (serial->device_status_regs.contains(start_addr))
                    connect(read_reply, &QModbusReply::finished, moh->device_status_widget, &DeviceStatus::onReadyRead);
                if (serial->parameter_set_regs.contains(start_addr))
                    connect(read_reply, &QModbusReply::finished, moh->para_conf, &ParameterConfiguration::onReadyRead);

                connect(read_reply, &QModbusReply::finished, this, [=] {
                    auto reply = qobject_cast<QModbusReply *>(sender());
                    if (!reply)
                        return;

                    slave_addrs.insert(slave_addr, true);

                    if (read_req.isValid() && read_req.valueCount() != 0)
                    {
                        QString read_request_str = ModbusSerial::makeRTUFrame(slave_addr, ModbusSerial::createReadRequest(read_req).functionCode(), ModbusSerial::createReadRequest(read_req).data()).toHex();
                        emit communicationRecord("TX", read_request_str);
                    }

                    if (reply->error() == QModbusDevice::NoError)
                    {
                        const QModbusDataUnit unit = reply->result();

                        if (unit.isValid() && unit.valueCount() != 0)
                        {
                            QString result_str = ModbusSerial::makeRTUFrame(slave_addr, ModbusSerial::createReadRequest(unit).functionCode(), reply->rawResult().data()).toHex();
                            emit communicationRecord("RX", result_str);
                        }
                    }
                });
            }
        }
        else
            delete read_reply;
    }

}

void ModbusSerialPrivate::do_the_actual_write(const int &reg_type, const int &start_addr, const QVector<quint16> values, const int slave_addr)
{
    ModbusSerial *serial = qobject_cast<ModbusSerial*>(sender());
    QModbusDataUnit write_req = QModbusDataUnit(QModbusDataUnit::RegisterType(reg_type), start_addr, values);
    const QModbusReply *write_reply = client->sendWriteRequest(write_req, slave_addr);

    if (write_reply)
    {
        if (!write_reply->isFinished())
        {
            connect(write_reply, &QModbusReply::finished, this, [this, write_reply, write_req, slave_addr, serial] () {

                if (write_reply->error() == QModbusDevice::NoError)
                {
                    emit set_write_state(true);
                    emit set_serial_state(true);

                    if (write_req.isValid() && write_req.valueCount() != 0)
                    {
                        QString result_str = ModbusSerial::makeRTUFrame(slave_addr, ModbusSerial::createWriteRequest(write_req).functionCode(), ModbusSerial::createWriteRequest(write_req).data()).toHex();
                        emit communicationRecord("TX", result_str);
                    }

                    const QModbusDataUnit unit = write_reply->result();

                    if (unit.isValid() && unit.valueCount() != 0)
                    {
                        QString result_str = ModbusSerial::makeRTUFrame(slave_addr, ModbusSerial::createWriteRequest(unit).functionCode(), write_reply->rawResult().data()).toHex();
                        emit communicationRecord("RX", result_str);
                    }

                    emit serial->start_timer();
                }
                else if (write_reply->error() == QModbusDevice::ProtocolError)
                    qDebug() << __FILE__ << __LINE__ << "Protocol error" << write_reply->errorString();

                delete write_reply;
            });
        }
        else
            delete write_reply;
    }
}


//bool ModbusSerialPrivate::is_write_process_done() const
//{
//    return write_process_done;
//}

//bool ModbusSerialPrivate::is_serial_ready() const
//{
//    return serial_ready;
//}

//bool ModbusSerialPrivate::open()
//{
//    if (state() == QModbusDevice::ConnectedState)
//        return true;

//    Q_D(QModbusRtuSerialMaster);
//    d->set
//}

//void ModbusSerialPrivate::close()
//{

//}

//void ModbusSerialPrivate::set_serial_state(const bool ready)
//{
//    serial_ready = ready;
//}


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
        m_settings.portname = tr("COM1");
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
    MOH_viewer *mainwindow = qobject_cast<MOH_viewer *>(parent());

    if (modbus_client->state() != QModbusDevice::ConnectedState)
        return;

    if (auto *reply = modbus_client->sendReadRequest(readRequest(type, start_addr, number_of_entries), m_settings.slave_addr))
    {
        if (!reply->isFinished())
        {
            //不同的widget在各自的槽函数接收数据
            switch (start_addr & 0xf000) {
            case Coils:
                connect(reply, &QModbusReply::finished, mainwindow->control_panel_widget, &ControlPanel::onReadyRead);
                break;
            case DiscreteInputs:
                connect(reply, &QModbusReply::finished, mainwindow, &MOH_viewer::onReadyRead);
                break;
            case InputRegisters:
                connect(reply, &QModbusReply::finished, mainwindow->device_status_widget, &DeviceStatus::onReadyRead);
                break;
            case HoldingRegisters:
                connect(reply, &QModbusReply::finished, mainwindow->para_conf, &ParameterConfiguration::onReadyRead);
                break;
            }
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

void ModbusSerial::write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const QVector<quint16> &data, const int &number_of_entries)
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

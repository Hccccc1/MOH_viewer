#include <QDir>
#include <QDebug>
#include <QFileDialog>

#include "systemsetting.h"
#include "ui_systemsetting.h"

SystemSetting::SystemSetting(QWidget *parent, uint8_t model, ModbusSerial *serial, QTranslator *trans) :
    QWidget(parent),
    ui(new Ui::SystemSetting),
    current_trans(trans),
    current_serial(serial)
{
    ui->setupUi(this);

    current_model = model;

    //    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    //    {
    //        QSerialPort serial;
    //        serial.setPort(info);

    //        if (serial.open(QIODevice::ReadWrite))
    //        {
    //            ui->serial_portname->addItem(serial.portName());
    //            serial.close();
    //        }
    //    }

    ui->parityCombo->setCurrentIndex(1);
    ui->baudCombo->setCurrentText(QString::number(current_serial->settings().baud));
    ui->dataBitsCombo->setCurrentText(QString::number(current_serial->settings().databits));
    ui->stopBitsCombo->setCurrentText(QString::number(current_serial->settings().stopbits));

    ui->timeoutSpinner->setValue(current_serial->settings().response_time);
    ui->retriesSpinner->setValue(current_serial->settings().number_of_retries);

    ui->refresh_interval_spinner->setValue(current_serial->settings().refresh_interval);

    ui->disconnectBtn->setDisabled(true);

    connect(this, &SystemSetting::send_ymodem, this, &SystemSetting::do_ymodemUpgrade);

    //    ui->label->setText(QString::number(model));
}

SystemSetting::~SystemSetting()
{
    delete ui;
}

void SystemSetting::refresh_port()
{
    ui->serial_portname->clear();

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
}

void SystemSetting::on_confirm_btn_clicked()
{
    if (qobject_cast<QPushButton*>(sender()) != nullptr)
    {
        hide();
        current_serial->m_settings.portname = ui->serial_portname->currentText();
        current_serial->m_settings.parity = ui->parityCombo->currentIndex();
        if (current_serial->m_settings.parity > 0)
            current_serial->m_settings.parity++;
        current_serial->m_settings.baud = ui->baudCombo->currentText().toInt();
        current_serial->m_settings.databits = ui->dataBitsCombo->currentText().toInt();
        current_serial->m_settings.stopbits = ui->stopBitsCombo->currentText().toInt();
        current_serial->m_settings.response_time = ui->timeoutSpinner->value();
        current_serial->m_settings.number_of_retries = ui->retriesSpinner->value();
        current_serial->m_settings.slave_addr = ui->slaveAddr_spinner->value();
        current_serial->m_settings.refresh_interval = ui->refresh_interval_spinner->value();
    }

    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
        current_serial->modbus_client->disconnectDevice();

    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialPortNameParameter, current_serial->settings().portname);
    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, current_serial->settings().baud);
    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialParityParameter, current_serial->settings().parity);
    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, current_serial->settings().databits);
    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, current_serial->settings().stopbits);

    current_serial->modbus_client->setTimeout(current_serial->m_settings.response_time);
    current_serial->modbus_client->setNumberOfRetries(current_serial->m_settings.number_of_retries);

    //    qDebug() << current_serial->modbus_client->connectionParameter(QModbusDevice::SerialPortNameParameter)
    //             << current_serial->modbus_client->connectionParameter(QModbusDevice::SerialBaudRateParameter)
    //             << current_serial->modbus_client->connectionParameter(QModbusDevice::SerialParityParameter)
    //             << current_serial->modbus_client->connectionParameter(QModbusDevice::SerialDataBitsParameter)
    //             << current_serial->modbus_client->connectionParameter(QModbusDevice::SerialStopBitsParameter);

    if (!current_serial->modbus_client->connectDevice())
    {
        qDebug() << __func__ << __LINE__ << "Connected failed...";
        return;
    }
    else
    {
        open_port();
    }

    emit serial_connected();

}

void SystemSetting::on_disconnectBtn_clicked()
{
    close_port();

    emit serial_disconnected();
}

void SystemSetting::open_port()
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

void SystemSetting::close_port()
{
    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
        current_serial->modbus_client->disconnectDevice();

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

void SystemSetting::on_errorHappened(QModbusDevice::Error error)
{
    qDebug() << sender()->objectName() << error;

    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        current_serial->modbus_client->disconnectDevice();

        if (error != QModbusDevice::NoError)
            QMessageBox::warning(this, tr("通讯异常"), QString(tr("串口读写失败：%1。即将断开串口！")).arg(error));

        close_port();

        emit serial_disconnected();
    }
}

void SystemSetting::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void SystemSetting::save_language_settings_to_file()
{
    QFile language_file(QString(QDir::currentPath()+"/language.ini"));

    if (!language_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << __FILE__ << __LINE__ << "language init file open failed";
    }
    else
    {
        QByteArray tmp = language_file.readAll();

        if (!tmp.isEmpty())
        {
            language_file.close();

            if (!language_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
            {
                qDebug() << __FILE__ << __LINE__ << "language init file open failed";
            }
            else
            {
                if (tmp == "language=chinese")
                    language_file.write("language=english");
                else if (tmp == "language=english")
                    language_file.write("language=chinese");
            }
        }

        language_file.close();
    }
}

void SystemSetting::on_languageChangeBtn_clicked()
{
    if (current_trans != nullptr)
    {
        qApp->removeTranslator(current_trans);
        delete current_trans;
    }

    current_trans = new QTranslator();

    save_language_settings_to_file();

    if (ui->languageChangeBtn->text() == "中文")
    {
        if (!current_trans->load(":/english.qm"))
        {
            qDebug() << __FILE__ << __LINE__ << "failed to load qm file";
        }
    }
    else
    {
        if (!current_trans->load(":/chinese.qm"))
        {
            qDebug() << __FILE__ << __LINE__ << "failed to load qm file";
        }
    }

    qApp->installTranslator(current_trans);
}

void SystemSetting::on_upgradeBtn_clicked()
{
    upgrade_file = ui->filenameEdit->text();

    if (!upgrade_file.isEmpty())
    {
        emit stop_timer();

        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_ReadyForBoot, 1);
    }
}

void SystemSetting::do_upgrade(bool ready)
{
    if (ready)
    {
        if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
            current_serial->modbus_client->disconnectDevice();

        if (upgrade_serial == nullptr)
            upgrade_serial = new QSerialPort();

        upgrade_serial->setPortName(current_serial->settings().portname);
        upgrade_serial->setBaudRate(current_serial->settings().baud);

        if (upgrade_serial->open(QIODevice::ReadWrite))
        {
            quint8 enter_boot[] = { 0x01, 0x06, 0x30, 0x62, 0x00, 0x01, 0xe6, 0xd4 };

            upgrade_serial->write((char *)enter_boot, sizeof(enter_boot)/sizeof(quint8));

            connect(upgrade_serial, &QSerialPort::readyRead, this, [this] {
                static QByteArray tmp_recved;

                tmp_recved += upgrade_serial->readAll();

                qDebug() << tmp_recved.size() << tmp_recved;

                if (tmp_recved.size() >= 892)
                {
                    quint8 tmp[] = { 0x31 };

                    if (upgrade_serial->isOpen())
                    {
                        QThread::msleep(10);

                        if (upgrade_serial->write((char *)tmp, sizeof(quint8)))
                        {
                            tmp_recved.clear();

                            connect(upgrade_serial, &QIODevice::bytesWritten, this, [=] (qint64 bytes) {
                                if (bytes)
                                    upgrade_serial->close();

                                //                                qDebug() << this->objectName();
                                emit this->send_ymodem();
                            });
                        }
                    }
                }
            });
        }
        else
        {
            qDebug() << "Unable to open serial port";
        }
    }
    else
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_ReadyForBoot, 1);
}

//void SystemSetting::send_ymodem()
//{

//}

void SystemSetting::do_ymodemUpgrade()
{
    if (upgrade_serial && upgrade_serial->isOpen())
    {
        upgrade_serial->close();
    }

    ymodem_transmit = new YmodemFileTransmit;
    ymodem_transmit->setFileName(ui->filenameEdit->text());
    ymodem_transmit->setPortName(current_serial->settings().portname);
    ymodem_transmit->setPortBaudRate(current_serial->settings().baud);

    if (ymodem_transmit->startTransmit() == true)
    {
        connect(ymodem_transmit, &YmodemFileTransmit::transmitProgress, this, &SystemSetting::transmitProgress);
        connect(ymodem_transmit, &YmodemFileTransmit::transmitStatus, this, &SystemSetting::transmitStatus);

        ui->fileBrowseBtn->setDisabled(true);
        ui->filenameEdit->setDisabled(true);
        ui->upgradeBtn->setDisabled(true);
    }
}

void SystemSetting::transmitProgress(int progress)
{
    qDebug() << progress;

    ui->transmitProgress->setValue(progress);
}

void SystemSetting::transmitStatus(YmodemFileTransmit::Status status)
{
    switch (status) {
    case YmodemFileTransmit::StatusEstablish:
    case YmodemFileTransmit::StatusTransmit:
        break;

    case YmodemFileTransmit::StatusFinish:
    {
        ui->fileBrowseBtn->setEnabled(true);
        ui->filenameEdit->setEnabled(true);
        ui->upgradeBtn->setEnabled(true);

//        on_confirm_btn_clicked();

        QMessageBox::information(this, tr("提示"), tr("升级成功，请重启控制板与上位机软件!"));

        break;
    }

    case YmodemFileTransmit::StatusAbort:
    case YmodemFileTransmit::StatusTimeout:
    default:
    {
        ui->fileBrowseBtn->setEnabled(true);
        ui->filenameEdit->setEnabled(true);
        ui->upgradeBtn->setEnabled(true);

        QMessageBox::warning(this, tr("提示"), tr("升级失败!"));

        break;
    }
    }
}

void SystemSetting::on_fileBrowseBtn_clicked()
{
    current_serial->operation_mutex->lock();

    QString tmp_filename = QFileDialog::getOpenFileName(this, "Upgrade file", QDir::currentPath(), tr("bin (*.bin)"));

    current_serial->operation_mutex->unlock();

    if (!tmp_filename.isEmpty())
        ui->filenameEdit->setText(tmp_filename);
}

#include <QDir>
#include <QDebug>

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

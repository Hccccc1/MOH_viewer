#include <QDir>
#include <QDebug>
#include <QFileDialog>

#include "systemsetting.h"
#include "ui_systemsetting.h"

SystemSetting::SystemSetting(QWidget *parent, uint8_t model, ModbusSerial *serial, QTranslator *trans) :
    QWidget(parent),
    ui(new Ui::SystemSetting),
    current_model(model),
    current_trans(trans),
    current_serial(serial)
{
    ui->setupUi(this);

//    if (!current_serial->is_serial_connected())
//    {
//        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//        {
//            QSerialPort serial;
//            serial.setPort(info);

//            if (serial.open(QIODevice::ReadWrite))
//            {
//                ui->serial_portname->addItem(serial.portName());
//                serial.close();
//            }
//        }
//    }

    ui->parityCombo->setCurrentIndex(0);
    ui->baudCombo->setCurrentText(QString::number(current_serial->settings().baud));
    ui->dataBitsCombo->setCurrentText(QString::number(current_serial->settings().databits));
    ui->stopBitsCombo->setCurrentText(QString::number(current_serial->settings().stopbits));

    ui->timeoutSpinner->setValue(current_serial->settings().response_time);
    ui->retriesSpinner->setValue(current_serial->settings().number_of_retries);

    ui->refresh_interval_spinner->setValue(current_serial->settings().refresh_interval);

    ui->disconnectBtn->setDisabled(true);

    ui->groupBox_4->hide();

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
        current_serial->setSerialParameters(QModbusDevice::SerialPortNameParameter, ui->serial_portname->currentText());
        current_serial->setSerialParameters(QModbusDevice::SerialParityParameter, ui->parityCombo->currentIndex());
//        if (current_serial->settings().parity > 0)
//        {
//            int parity = current_serial->settings().parity;
//            current_serial->setSerialParameters(QModbusDevice::SerialParityParameter, parity+1);
//        }
        current_serial->setSerialParameters(QModbusDevice::SerialBaudRateParameter, ui->baudCombo->currentText().toInt());
        current_serial->setSerialParameters(QModbusDevice::SerialDataBitsParameter, ui->dataBitsCombo->currentText().toInt());
        current_serial->setSerialParameters(QModbusDevice::SerialStopBitsParameter, ui->stopBitsCombo->currentText().toInt());
        current_serial->setTimeout(ui->timeoutSpinner->value());
        current_serial->setNumberOfRetries(ui->retriesSpinner->value());
        current_serial->setRefreshInterval(ui->refresh_interval_spinner->value());
        current_serial->setSaveInterval(ui->save_interval_spinner->value());
        current_serial->setSlaveAddr(ui->slaveAddr_spinner->value());
    }

    if (current_serial->is_serial_connected())
    {
        emit current_serial->close_serial();

        current_serial->read_mutex->lock();
        current_serial->read_queue.clear();
        current_serial->read_mutex->unlock();

        current_serial->write_mutex->lock();
        current_serial->write_queue.clear();
        current_serial->write_mutex->unlock();
    }

    //    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialPortNameParameter, current_serial->settings().portname);
    //    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, current_serial->settings().baud);
    //    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialParityParameter, current_serial->settings().parity);
    //    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, current_serial->settings().databits);
    //    current_serial->modbus_client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, current_serial->settings().stopbits);

    //    current_serial->modbus_client->setTimeout(current_serial->m_settings.response_time);
    //    current_serial->modbus_client->setNumberOfRetries(current_serial->m_settings.number_of_retries);

    //    emit change_slave_addr(current_serial->settings().slave_addr);

//        qDebug() << current_serial->connectionParameter(QModbusDevice::SerialPortNameParameter)
//                 << current_serial->connectionParameter(QModbusDevice::SerialBaudRateParameter)
//                 << current_serial->connectionParameter(QModbusDevice::SerialParityParameter)
//                 << current_serial->connectionParameter(QModbusDevice::SerialDataBitsParameter)
//                 << current_serial->connectionParameter(QModbusDevice::SerialStopBitsParameter);

    //    if (!current_serial->modbus_client->connectDevice())
    //    {
    //        qDebug() << __func__ << __LINE__ << "Connected failed...";
    //        return;
    //    }
    //    else
    //    {
    //        open_port();
    //    }

    open_port();

    emit connect_to_serial(current_serial->settings());

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
    ui->save_interval_spinner->setDisabled(true);
    ui->refresh_interval_spinner->setDisabled(true);

    ui->disconnectBtn->setEnabled(true);
}

void SystemSetting::close_port()
{
    if (current_serial->is_serial_connected())
    {
        emit current_serial->close_serial();

        current_serial->read_mutex->lock();
        current_serial->read_queue.clear();
        current_serial->read_mutex->unlock();

        current_serial->write_mutex->lock();
        current_serial->write_queue.clear();
        current_serial->write_mutex->unlock();
    }

    ui->serial_portname->setEnabled(true);
    ui->parityCombo->setEnabled(true);
    ui->baudCombo->setEnabled(true);
    ui->dataBitsCombo->setEnabled(true);
    ui->stopBitsCombo->setEnabled(true);
    ui->slaveAddr_spinner->setEnabled(true);
    ui->timeoutSpinner->setEnabled(true);
    ui->retriesSpinner->setEnabled(true);
    ui->confirm_btn->setEnabled(true);
    ui->save_interval_spinner->setEnabled(true);
    ui->refresh_interval_spinner->setEnabled(true);
}

void SystemSetting::on_refresh_timeout_counter()
{
    timeout_counter = 0;
}

void SystemSetting::on_errorHappened(QModbusDevice::Error error)
{

    qDebug() << "Slave addr: " << current_serial->settings().slave_addr << sender()->objectName() << error;

    pri_error = error;

    if (current_serial->is_serial_connected() && !current_serial->multiple_moh && pri_error == QModbusDevice::TimeoutError)
    {
        //        current_serial->modbus_client->disconnectDevice();
        //        current_serial->set_serial_state(false);
//        emit current_serial->close_serial();
        if ( ++timeout_counter == 5 )
        {
            current_serial->set_serial_state(false);
            current_serial->set_serial_connec_state(false);

//            current_serial->close_serial();

            emit current_serial->close_serial();

            timeout_counter = 0;
        }

        current_serial->read_mutex->lock();
        current_serial->read_queue.clear();
        current_serial->read_mutex->unlock();

        current_serial->write_mutex->lock();
        current_serial->write_queue.clear();
        current_serial->write_mutex->unlock();

        //        current_serial->modbus_client->

        dete_serial = new QSerialPort(this);
        dete_serial->setPortName(current_serial->settings().portname);
        dete_serial->setBaudRate(current_serial->settings().baud);

        if (dete_serial->open(QIODevice::ReadWrite))
        {
            //            quint8 det_char = 'a';

            //            for (int i = 0 ; i < 3; i++)
            //            {
            //                QThread::msleep(20);
            //                dete_serial->write((char *)&det_char, sizeof(quint8));
            //            }

            qDebug() << __func__ << __LINE__;

            timer_id = startTimer(1000);

            connect(dete_serial, &QSerialPort::readyRead, this, [=] {
                static QByteArray tmp;
                tmp += dete_serial->readAll();

                if (tmp.contains(0x43))
                {
                    tmp.clear();

                    dete_serial->close();

                    killTimer(timer_id);

                    is_in_boot = true;

                    if (QMessageBox::question(this, tr("提示"), tr("检测到Boot运行，是否立即升级？")) == QMessageBox::Yes)
                    {
                        emit upgrade_now(dete_serial->portName(), dete_serial->baudRate());

                        dete_serial->deleteLater();
                    }
                }
            });
        }
    }
}

void SystemSetting::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void SystemSetting::timerEvent(QTimerEvent *)
{
    quint8 tmp_str = 'a';
    static quint8 boot_counter = 0;

    boot_counter++;

    if (boot_counter)
    {
        if (dete_serial->isOpen())
            dete_serial->write((char* )&tmp_str, sizeof(quint8));
    }

    qDebug() << __func__ << __LINE__ << boot_counter;

    if (!is_in_boot && boot_counter == 5)
    {
        boot_counter = 0;

        if (pri_error != QModbusDevice::NoError/* && current_serial->modbus_client->state() == QModbusDevice::ConnectedState*/)
            QMessageBox::warning(this, tr("通讯异常"), QString(tr("串口读写失败：%1。即将断开串口！")).arg(pri_error));

        close_port();

        emit serial_disconnected();
        emit stop_timer();

        killTimer(timer_id);
    }

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
    operation_mutex->lock();

    if (current_trans != nullptr)
    {
        qApp->removeTranslator(current_trans);
//        delete current_trans;
//        current_trans = Q_NULLPTR;
    }

//    current_trans = new QTranslator();

    save_language_settings_to_file();

    if (ui->languageChangeBtn->text() == "中文")
    {
        if (!current_trans->load(":/english.qm"))
        {
            qDebug() << __FILE__ << __LINE__ << "failed to load qm file";
        }
        else
            current_trans->setObjectName("English");
    }
    else
    {
        if (!current_trans->load(":/chinese.qm"))
        {
            qDebug() << __FILE__ << __LINE__ << "failed to load qm file";
        }
        else
            current_trans->setObjectName("Chinese");
    }

    qApp->installTranslator(current_trans);

    operation_mutex->unlock();
}

void SystemSetting::on_multipleWidget_clicked()
{
    emit new_widget_needed(ui->slaveAddrSpin->value());
}

void SystemSetting::on_upgradeNow_clicked()
{
    //    current_serial->stop_timer();

    emit switch_to_upgrade(ui->serial_portname->currentText(), ui->baudCombo->currentText().toInt());
}

void SystemSetting::on_helpBtn_clicked()
{
    emit helpBtn_clicked();
}

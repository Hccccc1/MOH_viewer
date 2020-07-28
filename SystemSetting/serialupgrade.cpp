#include "serialupgrade.h"
#include "ui_serialupgrade.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

SerialUpgrade::SerialUpgrade(QWidget *parent, QString portname, int baudrate) :
    QWidget(parent),
    ui(new Ui::SerialUpgrade),
    m_portname(portname),
    m_baudrate(baudrate)
{
    ui->setupUi(this);

    connect(this, &SerialUpgrade::boot_ready, this, &SerialUpgrade::do_upgrade);
    connect(this, &SerialUpgrade::send_ymodem, this, &SerialUpgrade::do_ymodemUpgrade);

    upgrade_serial = new QSerialPort();

    ui->portnameLabel->setText(m_portname);
    ui->baudrateLabel->setText(QString::number(m_baudrate));

    //    if (upgrade_serial->open(QIODevice::ReadWrite))
    {
        ui->tipsLabel->setText(tr("串口已连接，请在选择文件后点击升级按键开始升级！"));
    }
    //    else
    //    {
    //        ui->tipsLabel->setText(tr("串口打开失败，请检查串口连接后重试！"));
    //    }
}

SerialUpgrade::~SerialUpgrade()
{
    delete ui;
}

void SerialUpgrade::do_ymodemUpgrade()
{
    if (upgrade_serial && upgrade_serial->isOpen())
    {
        upgrade_serial->close();
    }

    ymodem_transmit = new YmodemFileTransmit;
    ymodem_transmit->setFileName(ui->filenameEdit->text());
    ymodem_transmit->setPortName(m_portname);
    ymodem_transmit->setPortBaudRate(m_baudrate);

    if (ymodem_transmit->startTransmit() == true)
    {
        connect(ymodem_transmit, &YmodemFileTransmit::transmitProgress, this, &SerialUpgrade::transmitProgress);
        connect(ymodem_transmit, &YmodemFileTransmit::transmitStatus, this, &SerialUpgrade::transmitStatus);
    }
}

void SerialUpgrade::transmitProgress(int progress)
{
    ui->transmitProgress->setValue(progress);
}

void SerialUpgrade::on_cancelUpgradeBtn_clicked()
{
    if (ymodem_transmit->getTransmitStatus() == YmodemFileTransmit::StatusTransmit)
        ymodem_transmit->stopTransmit();
}

void SerialUpgrade::on_fileBrowseBtn_clicked()
{
    //    current_serial->operation_mutex->lock();

    QString tmp_filename = QFileDialog::getOpenFileName(this, "Upgrade file", QDir::currentPath(), tr("bin (*.bin)"));

    //    current_serial->operation_mutex->unlock();

    if (!tmp_filename.isEmpty())
        ui->filenameEdit->setText(tmp_filename);
}

void SerialUpgrade::transmitStatus(YmodemFileTransmit::Status status)
{
    switch (status) {
    case YmodemFileTransmit::StatusEstablish:
    case YmodemFileTransmit::StatusTransmit:
    {
        ui->tipsLabel->setText(tr("固件升级中，请勿关闭！"));
        break;
    }

    case YmodemFileTransmit::StatusFinish:
    {
        ui->cancelUpgradeBtn->setDisabled(true);
        ui->fileBrowseBtn->setEnabled(true);
        ui->filenameEdit->setEnabled(true);
        ui->upgradeBtn->setEnabled(true);

        //        on_confirm_btn_clicked();

        ui->tipsLabel->setText(tr("固件升级成功！"));
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
        ui->cancelUpgradeBtn->setDisabled(true);

        ui->tipsLabel->setText(tr("固件升级失败！"));
        QMessageBox::warning(this, tr("提示"), tr("升级失败!"));

        break;
    }
    }
}

void SerialUpgrade::read_from_serial()
{
    tmp_recved += upgrade_serial->readAll();

    if (tmp_recved.contains(0x43))
    {
        emit send_ymodem();
    }
    else
    {
        if (tmp_recved.size() == 7)
        {
            tmp_recved.clear();
            emit boot_ready(true);
        }
        else if (tmp_recved.size() == 8)
        {
            tmp_recved.clear();
            emit send_ymodem();
        }
    }
}

void SerialUpgrade::on_upgradeBtn_clicked()
{
    upgrade_file = ui->filenameEdit->text();

    if (!upgrade_file.isEmpty())
    {
        //        emit stop_timer();

        //        if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
        //        {
        //            current_serial->modbus_client->disconnectDevice();
        //            current_serial->modbus_client->connectDevice();
        //        }

        //        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_ReadyForBoot, 1);
        if (upgrade_serial->isOpen())
            upgrade_serial->close();

        upgrade_serial->setPortName(m_portname);
        upgrade_serial->setBaudRate(m_baudrate);

        if (upgrade_serial->open(QIODevice::ReadWrite))
        {
            connect(upgrade_serial, &QSerialPort::readyRead, this, &SerialUpgrade::read_from_serial);

            ui->tipsLabel->setText(tr("等待控制板开始信号。"));
            ui->cancelUpgradeBtn->setEnabled(true);
            ui->fileBrowseBtn->setDisabled(true);
            ui->filenameEdit->setDisabled(true);
            ui->upgradeBtn->setDisabled(true);

            quint8 enter_boot[] = { 0x01, 0x03, 0x30, 0x61, 0x00, 0x01, 0xda, 0xd4 };

            upgrade_serial->write((char *)enter_boot, sizeof(enter_boot)/sizeof(quint8));
        }
    }
}

void SerialUpgrade::do_upgrade(bool ready)
{
    if (ready)
    {
        //        if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
        //            current_serial->modbus_client->disconnectDevice();

        //        qDebug() << upgrade_serial->portName() << upgrade_serial->baudRate();

        if (upgrade_serial->isOpen())
        {
            quint8 enter_boot[] = { 0x01, 0x06, 0x30, 0x62, 0x00, 0x01, 0xe6, 0xd4 };

            upgrade_serial->write((char *)enter_boot, sizeof(enter_boot)/sizeof(quint8));

            //            if (upgrade_serial->isOpen())
            //            {
            //                qDebug() << "Signal emitted";
            //                emit ();
            //            }

            //            connect(upgrade_serial, &QSerialPort::readyRead, this, [this] {

            //                tmp_recved += upgrade_serial->readAll();

            //                qDebug() << __LINE__ << tmp_recved.size() << tmp_recved;

            //                if (tmp_recved.size() >= 8)
            //                {
            //                    //                    quint8 tmp[] = { 0x31 };

            //                    if (upgrade_serial->isOpen())
            //                    {
            //                        QThread::msleep(10);

            //                        emit this->send_ymodem();
            //                    }
            //                }
            //            });
        }
        else
        {
            qDebug() << "Unable to open serial port";
        }
    }
    else
    {
        quint8 enter_boot[] = { 0x01, 0x03, 0x30, 0x61, 0x00, 0x01, 0xda, 0xd4 };

        upgrade_serial->write((char *)enter_boot, sizeof(enter_boot)/sizeof(quint8));

        //        connect(upgrade_serial, &QSerialPort::readyRead, this, [this] {

        //            tmp_recved += upgrade_serial->readAll();

        //            qDebug() << tmp_recved;

        //            if (tmp_recved.size() >= 8)
        //            {
        //                emit this->boot_ready(true);
        //            }
        //        });
    }
}

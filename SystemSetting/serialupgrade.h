#ifndef SERIALUPGRADE_H
#define SERIALUPGRADE_H

#include <QWidget>

#include "3rdparty/YModem/Ymodem.h"
#include "3rdparty/YModem/YmodemFileTransmit.h"

namespace Ui {
class SerialUpgrade;
}

class SerialUpgrade : public QWidget
{
    Q_OBJECT

public:
    explicit SerialUpgrade(QWidget *parent = nullptr, QString portname = "", int baudrate = QSerialPort::Baud9600);
    ~SerialUpgrade();

private:
    Ui::SerialUpgrade *ui;

    QString m_portname;
    int m_baudrate;

    QByteArray tmp_recved;

    QString upgrade_file;
    QSerialPort *upgrade_serial = nullptr;
    YmodemFileTransmit *ymodem_transmit = nullptr;

private slots:

    void do_upgrade(bool);

    //选择升级文件
    void on_fileBrowseBtn_clicked();
    //升级按键
    void on_upgradeBtn_clicked();
    //开始发送YModem数据包
    void do_ymodemUpgrade();
    //升级进度
    void transmitProgress(int);
    void transmitStatus(YmodemFileTransmit::Status status);

    void read_from_serial();

    void on_cancelUpgradeBtn_clicked();

Q_SIGNALS:
    //发送Ymodem数据包
    void send_ymodem();
    //开始升级
    void boot_ready(bool);
};

#endif // SERIALUPGRADE_H

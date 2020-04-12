#ifndef MODBUSSERIAL_H
#define MODBUSSERIAL_H

#include <QDialog>
#include <QDebug>
//#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QModbusClient>
#include <QModbusRtuSerialMaster>
//#include <QModbusDataUnit>

namespace Ui {
class ModbusSerial;
}

class ModbusSerial : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
       QString portname;
       int baud = QSerialPort::Baud19200;
       int parity = QSerialPort::EvenParity;
       int databits = QSerialPort::Data8;
       int stopbits = QSerialPort::OneStop;

       int response_time = 1000;
       int number_of_retries = 3;
    };

    explicit ModbusSerial(QWidget *parent = nullptr);
    ~ModbusSerial();

    QModbusClient *modbus_client = new QModbusRtuSerialMaster(this);

    Settings settings() const;

    void change_portname(QString portname);

public slots:
    void on_confirm_btn_clicked();
//    void on_confirm_btn_clicked(QString portname);

private:
    Settings m_settings;
    Ui::ModbusSerial *ui;

signals:
    void serial_connected();
};

#endif // MODBUSSERIAL_H

#ifndef MODBUSSERIAL_H
#define MODBUSSERIAL_H

#include <QDialog>
#include <QDebug>
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
    enum FunctionCode_Addresses
    {
        Coils = 0x0000,
        DiscreteInputs = 0x1000,
        InputRegisters = 0x2000,
        HoldingRegisters = 0x3000,
    };

    struct Settings {
       QString portname;
       int baud = QSerialPort::Baud19200;
       int parity = QSerialPort::EvenParity;
       int databits = QSerialPort::Data8;
       int stopbits = QSerialPort::OneStop;

       int response_time = 1000;
       int number_of_retries = 3;

       int slave_addr = 0x01;
    };

    explicit ModbusSerial(QWidget *parent = nullptr);
    ~ModbusSerial();

    QModbusClient *modbus_client = new QModbusRtuSerialMaster(this);

    Settings settings() const;

    void change_portname(QString portname);

    void read_from_modbus(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries);

public slots:
    void on_confirm_btn_clicked();

private:
    Settings m_settings;
    Ui::ModbusSerial *ui;

    QModbusDataUnit readRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const;
    QModbusDataUnit writeRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const;

private slots:
    void onReadyRead();

signals:
    void serial_connected();
};

#endif // MODBUSSERIAL_H

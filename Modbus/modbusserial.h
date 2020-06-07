#ifndef MODBUSSERIAL_H
#define MODBUSSERIAL_H

#include <QDialog>
#include <QDebug>
#include <QMutex>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QModbusClient>
#include <QModbusRtuSerialMaster>
#include <QBitArray>
#include <QTranslator>

#include "DeviceLog/devicelog.h"

//#include <QModbusDataUnit>

namespace Ui {
class ModbusSerial;
}

const int number_of_bits = 16;
//const

class ModbusSerial : public QDialog
{
    Q_OBJECT

public:
    enum Coil {
        On = 0xff00,
        Off = 0x0000
    };

    enum FunctionCode_Addresses
    {
        Coils = 0x0000,
        DiscreteInputs = 0x1000,
        InputRegisters = 0x2000,
        HoldingRegisters = 0x3000,
    };

    struct Settings {
       QString portname;
       int baud = QSerialPort::Baud9600;
       int parity = QSerialPort::NoParity;
       int databits = QSerialPort::Data8;
       int stopbits = QSerialPort::OneStop;

       int response_time = 1000;
       int number_of_retries = 3;

       int slave_addr = 0x01;
    };

    int m_number = 0;
    int m_address = 0;
//    QBitArray m_coils = QBitArray(number_of_bits, false);
//    QVector<quint16> m_holdingRegisters = QVector<quint16>(number_of_bits, 0u);

    QMutex *rw_mutex = new QMutex(QMutex::NonRecursive);

    explicit ModbusSerial(QWidget *parent = nullptr, DeviceLog *log_handler = nullptr);
    ~ModbusSerial();

    QModbusClient *modbus_client = new QModbusRtuSerialMaster(this);

    Settings settings() const;

    void change_portname(QString portname);

    static QByteArray makeRTUFrame(int slave, int function, const QByteArray & data);
    static QModbusResponse createReadRequest(const QModbusDataUnit &data);
    static QModbusRequest createWriteRequest(const QModbusDataUnit &data);

    void read_from_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &number_of_entries);
    //写单个寄存器
    void write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &data);
    //写多个寄存器
    void write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const QVector<quint16> &data, const quint16 &number_of_entries);
    //写线圈
    void write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &number_of_entries, const bool &enable);

    bool is_serial_ready();
    void set_serial_state(bool ready);

public slots:
    void on_confirm_btn_clicked();
    void on_errorHappened(QModbusDevice::Error);

private:
    Settings m_settings;
    Ui::ModbusSerial *ui;

    QTranslator *trans = new QTranslator(this->parent());

    DeviceLog *current_log_handler;

    QVector<quint16> mohviewer_regs;
    QVector<quint16> control_panel_regs;
    QVector<quint16> device_status_regs;
    QVector<quint16> parameter_set_regs;

    bool serial_ready = true;\

//    QQueue<QModbusDataUnit> request_queue;

    void prepare_vector_regs();

    QModbusDataUnit readRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const;
    QModbusDataUnit writeRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const;

    void open_port();
    void close_port();

private slots:
//    void onReadyRead();
    void on_disconnectBtn_clicked();
    void on_languageChangeBtn_clicked();

protected:
    virtual void changeEvent(QEvent *);

Q_SIGNALS:
    void serial_connected();
    void serial_disconnected();

    void communicationRecord(QString, QString);
};

#endif // MODBUSSERIAL_H

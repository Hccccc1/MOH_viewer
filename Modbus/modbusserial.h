#ifndef MODBUSSERIAL_H
#define MODBUSSERIAL_H

#include <QDialog>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QModbusClient>
#include <QModbusRtuSerialMaster>
#include <QTranslator>

#include <QThread>
#include <QMutex>
#include <QQueue>

#include "DeviceLog/devicelog.h"

namespace Ui {
class ModbusSerial;
}

const int number_of_bits = 16;

class ModbusSerial : public QThread
{
    Q_OBJECT

public:
    enum Coil {
        On = 0xff00,
        Off = 0x0000
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


    explicit ModbusSerial(QWidget *parent = nullptr, DeviceLog *log_handler = nullptr);
    ~ModbusSerial() override;

    Settings m_settings;
    QModbusClient *modbus_client = new QModbusRtuSerialMaster(this);

    Settings settings() const;

    static QByteArray makeRTUFrame(int slave, int function, const QByteArray & data);
    static QModbusResponse createReadRequest(const QModbusDataUnit &data);
    static QModbusRequest createWriteRequest(const QModbusDataUnit &data);

    void insert_read_unit(const QModbusDataUnit &unit);
    void insert_write_unit(const QModbusDataUnit &unit);

    void read_from_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &number_of_entries);
    //写单个寄存器
    void write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &data);
    //写多个寄存器
    void write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const QVector<quint16> &data, const quint16 &number_of_entries);
    //写线圈
    void write_to_modbus(const QModbusDataUnit::RegisterType &type, const int &start_addr, const quint16 &number_of_entries, const bool &enable);

    bool is_serial_ready() const;
    void set_serial_state(const bool ready);

private:
    DeviceLog *current_log_handler;

    QVector<quint16> mohviewer_regs;
    QVector<quint16> control_panel_regs;
    QVector<quint16> device_status_regs;
    QVector<quint16> parameter_set_regs;

    bool serial_ready = true;

    QQueue<QModbusDataUnit> read_queue;
    QQueue<QModbusDataUnit> write_queue;
    QMutex *read_mutex = new QMutex(QMutex::NonRecursive);
    QMutex *write_mutex = new QMutex(QMutex::NonRecursive);

    void prepare_vector_regs();

    QModbusDataUnit readRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const;
    QModbusDataUnit writeRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const;

//    void open_port();
//    void close_port();

    void modbus_reply_finished(const QModbusDataUnit, QModbusReply *);

private slots:
    void do_the_actual_read(const int &reg_type, const int &start_addr, const quint32 &num_of_entries);
    void do_the_actual_write(const int &reg_type, const int &start_addr, const QVector<quint16> values);

protected:
    void run() override;

Q_SIGNALS:

    void communicationRecord(QString, QString);

    void actual_read_req(const int &reg_type, const int &start_addr, const quint32 &num_of_entries);
    void actual_write_req(const int &reg_type, const int &start_addr, const QVector<quint16> values);
//    void insert_write_request(const QModbusDataUnit &);
//    void insert_read_request(const QModbusDataUnit &);
};

#endif // MODBUSSERIAL_H

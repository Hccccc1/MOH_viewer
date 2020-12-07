#ifndef MODBUSSERIAL_H
#define MODBUSSERIAL_H

#include <QThread>
#include <QModbusClient>
#include <QSerialPort>
#include <QVariant>
#include <QMutex>
#include <QQueue>

//#include "ModbusSerial/modbusserialprivate.h"
//#include "MOH_Viewer/moh_viewer.h"

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

       int response_time = 500;
       int number_of_retries = 2;

       int slave_addr = 0x01;

       int refresh_interval = 1000;
    };

    ModbusSerial(QObject *parent = nullptr);
    ~ModbusSerial();

    bool multiple_moh = false;

    QMutex *operation_mutex = new QMutex(QMutex::NonRecursive);

    QQueue<QModbusDataUnit> read_queue;
    QQueue<QModbusDataUnit> write_queue;

    QMutex *read_mutex = new QMutex(QMutex::NonRecursive);
    QMutex *write_mutex = new QMutex(QMutex::NonRecursive);

    QVector<quint16> mohviewer_regs;
    QVector<quint16> control_panel_regs;
    QVector<quint16> device_status_regs;
    QVector<quint16> parameter_set_regs;

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

    QModbusDataUnit readRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const;
    QModbusDataUnit writeRequest(QModbusDataUnit::RegisterType type, int start_addr, quint16 number_of_entries) const;

    bool is_serial_ready() const;
    bool is_write_process_done() const;

    bool is_serial_connected() const;

    void setSerialParameters(QModbusDevice::ConnectionParameter, const QVariant &);
    const Settings settings() const;
    void setTimeout(const int& timeout);
    void setNumberOfRetries(const int &time);
    void setSlaveAddr(const int &slave_addr);
    void setRefreshInterval(const int &time);

public slots:
    void set_serial_connec_state(const bool& connected);

    void set_serial_state(const bool &ready);
    void set_write_state(const bool &ready);

//    void request_finished();

private:
    Settings m_settings;

    bool serial_connected = false;
    bool serial_ready = true;       //串口准备好标志，保证在处理完数据后才有下一个读写请求入队

    bool write_process_done = true;

    void prepare_vector_regs();

protected:
    void run() override;

Q_SIGNALS:
    void close_serial();

    void modbusErrorHappened(QModbusDevice::Error);

    void actual_read_req(const int &reg_type, const int &start_addr, const quint32 &num_of_entries, const int &slave_addr);
    void actual_write_req(const int &reg_type, const int &start_addr, const QVector<quint16> values, const int &slave_addr);

    void start_timer(); //写操作完成后，打开定时器
    void stop_timer();  //有写操作时，暂停读定时器
};

#endif // MODBUSSERIAL_H

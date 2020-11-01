#ifndef MODBUSSERIALPRIVATE_H
#define MODBUSSERIALPRIVATE_H

#include <QWidget>
#include <QModbusRtuSerialMaster>

#include "ModbusSerial/modbusserial.h"

class ModbusSerial;

class ModbusSerialPrivate : public QWidget
{
    Q_OBJECT
//    Q_DECLARE_PRIVATE(QModbusRtuSerialMaster)

public:
    QModbusClient* client = new QModbusRtuSerialMaster();

    QMap<quint8, QTimer *> timeout_timers;

    ModbusSerialPrivate(QWidget *parent = nullptr);

    void insert_slave_addr(const int& slave_addr);

    void start_timeout_counter(quint8 slave_addr);
    void refresh_timeout_counter(quint8 slave_addr);
    void stop_timeout_counter(quint8 slave_addr);

public slots:

    void connect_serial(const ModbusSerial::Settings &);
    void disconnect_serial();

    void do_the_actual_read(const int &reg_type, const int &start_addr, const quint32 &num_of_entries, const int slave_addr);
    void do_the_actual_write(const int &reg_type, const int &start_addr, const QVector<quint16> values, const int slave_addr);


private:
    QMap<quint8, bool> slave_addrs;

    bool write_process_done = true;
//    bool serial_ready = true;       //串口准备好标志，保证在处理完数据后才有下一个读写请求入队

    bool is_write_process_done() const;

//    bool is_serial_ready() const;
//    void set_serial_state(const bool ready);

protected:
//    bool open() override;
//    void close() override;

Q_SIGNALS:
    void serial_connected();
    void serial_disconnected();

    void set_serial_connec_state(const bool &connected);

    void set_serial_state(const bool &ready);
    void set_write_state(const bool &ready);

    void modbusErrorHappened(QModbusDevice::Error);

    void communicationRecord(QString, QString);

    void actual_read_req(const int &reg_type, const int &start_addr, const quint32 &num_of_entries);
    void actual_write_req(const int &reg_type, const int &start_addr, const QVector<quint16> values);

};

#endif // MODBUSSERIALPRIVATE_H

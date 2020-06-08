#ifndef READWRITETHREAD_H
#define READWRITETHREAD_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QModbusDataUnit>
#include <QModbusReply>

//#include "Modbus/modbusserial.h"

//class ModbusSerial;

class ReadWriteThread : public QThread
{
    Q_OBJECT
public:
    ReadWriteThread(QObject *parent = nullptr);
    ~ReadWriteThread();

    QMutex *read_mutex = new QMutex(QMutex::NonRecursive);
    QMutex *write_mutex = new QMutex(QMutex::NonRecursive);

public slots:
    void insert_read_unit(const QModbusDataUnit &unit);
    void insert_write_unit(const QModbusDataUnit &unit);

private:
    QQueue<QModbusDataUnit> read_queue;
    QQueue<QModbusDataUnit> write_queue;

//    ModbusSerial *_modbus = nullptr;

protected:
    void run() override;

Q_SIGNALS:
    void reply_finished(QModbusDataUnit, QModbusReply *);
};

#endif // READWRITETHREAD_H

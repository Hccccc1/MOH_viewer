#include <QDebug>
#include "readwritethread.h"
#include "Modbus/modbusserial.h"

ReadWriteThread::ReadWriteThread(QObject *parent) : QThread(parent)
{
    _modbus = qobject_cast<ModbusSerial *>(parent);
    connect(this, &ReadWriteThread::reply_finished, _modbus, &ModbusSerial::on_modbus_reply_finished);
}

ReadWriteThread::~ReadWriteThread()
{
    qDebug() << "Thread deleted";

    deleteLater();
}

void ReadWriteThread::run()
{
//    ModbusSerial* _modbus = qobject_cast<ModbusSerial *>(this->parent());

    while (1)
    {
        if (_modbus->is_serial_ready())
        {
            if (!write_queue.isEmpty())
            {
                //write request enqueue
            }
            else
            {
                //read request enqueue
                if (read_mutex->tryLock(100))
                {
                    QModbusDataUnit ori_request = read_queue.dequeue();
                    read_mutex->unlock();

                    _modbus->set_serial_state(false);

                    if (auto *reply = _modbus->modbus_client->sendReadRequest(ori_request, _modbus->settings().slave_addr))
                    {
                        if (!reply->isFinished())
                            emit reply_finished(ori_request, reply);
                        else
                            delete reply;
                    }
                }
            }
        }
    }
}

void ReadWriteThread::insert_read_unit(const QModbusDataUnit &unit)
{
    if (unit.isValid())
    {
        if (read_mutex->tryLock(100))
        {
            read_queue.enqueue(unit);
            read_mutex->unlock();
        }
    }
}

void ReadWriteThread::insert_write_unit(const QModbusDataUnit &unit)
{
    if (unit.isValid())
    {
        if (write_mutex->tryLock(100))
        {
            write_queue.enqueue(unit);
            write_mutex->unlock();
        }
    }
}

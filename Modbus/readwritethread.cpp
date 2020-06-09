#include "readwritethread.h"

#include <QDebug>

ReadWriteThread::ReadWriteThread(QObject *parent) : QThread(parent)
{
//    start();
}

void ReadWriteThread::run()
{
    while (1)
    {
        sleep(500);

        qDebug() << "Thread ID: " << currentThreadId();
    }
}

void ReadWriteThread::insert_rw_request(QModbusDataUnit unit)
{

}

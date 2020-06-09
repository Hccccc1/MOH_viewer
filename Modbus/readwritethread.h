#ifndef READWRITETHREAD_H
#define READWRITETHREAD_H

#include <QThread>
#include <QModbusDataUnit>

class ReadWriteThread : public QThread
{
    Q_OBJECT
public:
    ReadWriteThread(QObject *parent = nullptr);

public slots:
    void insert_rw_request(QModbusDataUnit);

protected:
    virtual void run();
};

#endif // READWRITETHREAD_H

#ifndef DEVICELOG_H
#define DEVICELOG_H

#include <QWidget>
#include "WarningLogs/warninglogs.h"
#include "OperationLogs/operationlogs.h"
#include "CommunicationLogs/communicationlogs.h"

namespace Ui {
class DeviceLog;
}

class DeviceLog : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceLog(QWidget *parent = nullptr, uint8_t model = 0, int slave_addr = 0x01);
    ~DeviceLog();

    int m_slave_addr;
    WarningLogs *warningLogs = nullptr;
    OperationLogs *operationLogs = nullptr;
    CommunicationLogs *communicationLogs = nullptr;

    int get_slave_addr();
    void change_slave_addr(int slave_addr);

private:
    Ui::DeviceLog *ui;

    uint8_t current_model;

protected:
    virtual void changeEvent(QEvent *);
};

#endif // DEVICELOG_H

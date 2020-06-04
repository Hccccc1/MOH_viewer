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
    explicit DeviceLog(QWidget *parent = nullptr, uint8_t model = 0);
    ~DeviceLog();

    WarningLogs *warningLogs = new WarningLogs();
    OperationLogs *operationLogs = new OperationLogs();
    CommunicationLogs *communicationLogs = new CommunicationLogs();

private:
    Ui::DeviceLog *ui;

    uint8_t current_model;

protected:
    virtual void changeEvent(QEvent *);
};

#endif // DEVICELOG_H

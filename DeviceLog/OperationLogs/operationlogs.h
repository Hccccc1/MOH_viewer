#ifndef OPERATIONLOGS_H
#define OPERATIONLOGS_H

#include <QDialog>
#include <QResizeEvent>
#include "DeviceLog/LogDatabase/logdatabase.h"

namespace Ui {
class OperationLogs;
}

class OperationLogs : public QDialog
{
    Q_OBJECT

public:
    explicit OperationLogs(QWidget *parent = nullptr);
    ~OperationLogs();

public slots:
    void addOperationRecord(QString first_column, QString second_column);

private:
    Ui::OperationLogs *ui;

    LogDatabase database;
    QString db_name = "operation_log.db";
    QString table_name = "operation_table";

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // OPERATIONLOGS_H

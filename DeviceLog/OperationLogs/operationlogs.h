#ifndef OPERATIONLOGS_H
#define OPERATIONLOGS_H

#include <QDialog>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QMessageBox>
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

    QString db_name = "operation_log.db";
    QString table_name = "operation_table";

    QStandardItemModel *model = new QStandardItemModel(this);
    LogDatabase operation_database = LogDatabase(db_name, table_name, OperationLog);

private slots:
    void on_getDataBtn_clicked();
    void on_quickSearch_currentIndexChanged(int index);

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // OPERATIONLOGS_H

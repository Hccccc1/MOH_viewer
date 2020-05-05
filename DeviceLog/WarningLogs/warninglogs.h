#ifndef WARNINGLOGS_H
#define WARNINGLOGS_H

#include <QDebug>
#include <QDialog>
#include <QStandardItemModel>
#include <QResizeEvent>

#include "DeviceLog/LogDatabase/logdatabase.h"

namespace Ui {
class WarningLogs;
}

class WarningLogs : public QDialog
{
    Q_OBJECT

public:
    explicit WarningLogs(QWidget *parent = nullptr);
    ~WarningLogs();

public slots:
    void addWarningRecord(QString first_column, QString second_column);

private:
    Ui::WarningLogs *ui;

    QString db_name = "warning_log.db";
    QString table_name = "warninglog_table";

    LogDatabase database;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // WARNINGLOGS_H

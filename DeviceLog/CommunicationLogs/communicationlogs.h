#ifndef COMMUNICATIONLOGS_H
#define COMMUNICATIONLOGS_H

#include <QDialog>
#include <QResizeEvent>
#include <QDebug>

#include "DeviceLog/LogDatabase/logdatabase.h"

namespace Ui {
class CommunicationLogs;
}

class CommunicationLogs : public QDialog
{
    Q_OBJECT

public:
    explicit CommunicationLogs(QWidget *parent = nullptr);
    ~CommunicationLogs();

//    LogDatabase::DatabaseHandler handler;

public slots:
    void addCommunicationRecord(QString first_column, QString second_column);

private:
    Ui::CommunicationLogs *ui;

    QString db_name = "communication_log.db";
    QString table_name = "communication_table";

    LogDatabase database;

protected:
    void resizeEvent(QResizeEvent *);

};

#endif // COMMUNICATIONLOGS_H

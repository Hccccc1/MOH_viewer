#ifndef LOGDATABASE_H
#define LOGDATABASE_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

enum DatabaseOwner
{
    WarningLog,
    OperationLog,
    CommunicaitionLog,
};

enum QuickSearchDate
{
    Today,
    Yesterday,
    LastSevenDays,
    CurrentMonth,
    LastMonth,
    CustomDates,
};

class LogDatabase
{
public:
//    struct DatabaseHandler
//    {
//        QString tableName;
//        QString firstColumn;
//        QString secondColumn;
//    };

    LogDatabase(QString databaseName, QString databaseTableName, DatabaseOwner ownerType);

//    void create_database_table(QString db_name, QString db_table_name, DatabaseOwner type);
    void insert_values_into_table(QString table_name,
                                  QString first_column,
                                  QString second_column);

//    qint64 time = QDateTime::currentSecsSinceEpoch();
    QVector<QVector<QString>> get_columns_by_time(const qint64 &start_time, const qint64 &end_time);

//    QString tmp = QDateTime::currentDateTime().toString("yyyy.dd.MM hh:mm:ss");

private:
    QString db_name;
    QString db_tablename;
    DatabaseOwner type;

    void create_database_table();
};

#endif // LOGDATABASE_H

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

class LogDatabase
{
public:
//    struct DatabaseHandler
//    {
//        QString tableName;
//        QString firstColumn;
//        QString secondColumn;
//    };

    LogDatabase();

    void create_database_table(QString db_name, QString db_table_name, DatabaseOwner type);
    void insert_values_into_table(QString table_name,
                                  QString first_column,
                                  QString second_column);

//    QString tmp = QDateTime::currentDateTime().toString("yyyy.dd.MM hh:mm:ss");
};

#endif // LOGDATABASE_H

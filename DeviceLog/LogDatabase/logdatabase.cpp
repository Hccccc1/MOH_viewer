#include "logdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

LogDatabase::LogDatabase()
{
//    qDebug() << tmp;
}

void LogDatabase::create_database_table(QString db_name, QString db_table_name, DatabaseOwner type)
{
    QString cmd = "create table %1 (datetime vchar(30) primary key, %2 vchar(30), %3 vchar(30))";
    QSqlQuery query;
    QSqlDatabase needed_db;

    if (needed_db.contains("qt_sql_default_connection"))
        needed_db = QSqlDatabase::database("qt_sql_default_connection");
    else
        needed_db = QSqlDatabase::addDatabase("QSQLITE");

    needed_db.setDatabaseName(db_name);

    QDir current_path = QDir::currentPath();

    if (!current_path.exists(db_name))
    {
        if (!needed_db.open())
            qDebug() << "Open " << db_name << "error" << needed_db.lastError();

        switch (type) {
        case WarningLog:
            cmd = cmd.arg(db_table_name).arg("contents").arg("level");
            break;
        case OperationLog:
            cmd = cmd.arg(db_table_name).arg("contents").arg("operator");
            break;
        case CommunicaitionLog:
            cmd = cmd.arg(db_table_name).arg("contents").arg("framedata");
            break;
        }

        query.prepare(cmd);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error:" <<query.lastError();

    }
    else
    {
        if (!needed_db.open())
            qDebug() << __FILE__ << __LINE__ << needed_db.lastError();
    }
}

void LogDatabase::insert_values_into_table(QString table_name,
                                           QString first_column,
                                           QString second_column)
{
    QSqlQuery query;
    QString insert_cmd = "insert into %1 values ('%2', '%3', '%4')";
    insert_cmd = insert_cmd.arg(table_name)
                           .arg(QDateTime::currentDateTime().toString("yyyy.dd.MM hh:mm:ss"))
                           .arg(first_column)
                           .arg(second_column);

    query.prepare(insert_cmd);

    if (!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }
}

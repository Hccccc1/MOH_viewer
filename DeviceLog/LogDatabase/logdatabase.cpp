#include "logdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

LogDatabase::LogDatabase(QString databaseName, QString databaseTableName, DatabaseOwner ownerType):
    db_name(databaseName),
    db_tablename(databaseTableName),
    type(ownerType)
{
//    qDebug() << tmp;
    create_database_table();
}

void LogDatabase::create_database_table()
{
    QString cmd = "create table %1 (datetime INTEGER primary key, %2 vchar(30), %3 vchar(30))";
    QSqlQuery query;
    QSqlDatabase needed_db;

//    needed_db = QSqlDatabase::addDatabase("QSQLITE");

    switch (type) {
    case WarningLog:
        if (QSqlDatabase::contains("WarningLog"))
            needed_db = QSqlDatabase::database("WarningLog");
        else
            needed_db = QSqlDatabase::addDatabase("QSQLITE", "WarningLog");
        query = QSqlQuery(QSqlDatabase::database("WarningLog", true));
        break;
    case OperationLog:
        if (QSqlDatabase::contains("OperationLog"))
            needed_db = QSqlDatabase::database("OperationLog");
        else
            needed_db = QSqlDatabase::addDatabase("QSQLITE", "OperationLog");
        query = QSqlQuery(QSqlDatabase::database("OperationLog", true));
        break;
    case CommunicaitionLog:
        if (QSqlDatabase::contains("CommunicationLog"))
            needed_db = QSqlDatabase::database("CommunicationLog");
        else
            needed_db = QSqlDatabase::addDatabase("QSQLITE", "CommunicationLog");
        query = QSqlQuery(QSqlDatabase::database("CommunicationLog", true));
        break;
    }

    needed_db.setDatabaseName(db_name);

    QDir current_path = QDir::currentPath();

    if (!current_path.exists(db_name))
    {
        if (!needed_db.open())
        {
            qDebug() << "Open " << db_name << "error" << needed_db.lastError();
            return;
        }

        switch (type) {
        case WarningLog:
            cmd = cmd.arg(db_tablename).arg("contents").arg("level");
            break;
        case OperationLog:
            cmd = cmd.arg(db_tablename).arg("contents").arg("operator");
            break;
        case CommunicaitionLog:
            cmd = cmd.arg(db_tablename).arg("contents").arg("framedata");
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
//                           .arg(QDateTime::currentDateTime().toString("yyyy.dd.MM hh:mm:ss"))
                           .arg(QString::number(QDateTime::currentMSecsSinceEpoch()))
                           .arg(first_column)
                           .arg(second_column);

//    qDebug() << QString::number(QDateTime::currentMSecsSinceEpoch());
//    qDebug() << __func__ << __LINE__ << insert_cmd;

    switch (type) {
    case WarningLog:
        query = QSqlQuery(QSqlDatabase::database("WarningLog", true));
        break;
    case OperationLog:
        query = QSqlQuery(QSqlDatabase::database("OperationLog", true));
        break;
    case CommunicaitionLog:
        query = QSqlQuery(QSqlDatabase::database("CommunicationLog", true));
        break;
    }

    query.prepare(insert_cmd);

    while (!query.exec())
    {
//        qDebug() << insert_cmd;
        qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }
}
QVector<QString> LogDatabase::get_newest_data()
{
    QVector<QString> res;

    QString cmd = "select contents from %1 where level = '1' order by datetime desc limit 0,20";
    cmd = cmd.arg(db_tablename);
    QSqlQuery query;

    query = QSqlQuery(QSqlDatabase::database("WarningLog", true));

    query.prepare(cmd);

    if (!query.exec())
        qDebug() << __FILE__ << __LINE__ << query.lastError();

    query.first();

    while (query.isValid())
    {
        res.append(query.value(0).toString());
        query.next();
    }

    return res;
}

QVector<QVector<QString>> LogDatabase::get_columns_by_time(const qint64 &start_time, const qint64 &end_time)
{
    QVector<QVector<QString>> result;
    QVector<QString> first_columns, second_columns, third_columns;

    QString cmd = "select %1 from %2 where datetime >= %3 AND datetime <= %4";
    QSqlQuery query;

    switch (type) {
    case WarningLog:
        query = QSqlQuery(QSqlDatabase::database("WarningLog", true));
        break;
    case OperationLog:
        query = QSqlQuery(QSqlDatabase::database("OperationLog", true));
        break;
    case CommunicaitionLog:
        query = QSqlQuery(QSqlDatabase::database("CommunicationLog", true));
        break;
    }

    //first
    QString select_first = cmd.arg("datetime").arg(db_tablename).arg(start_time).arg(end_time);
    query.prepare(select_first);
    if (!query.exec())
        qDebug() << __FILE__ << __LINE__ << query.lastError();

    query.first();

    while (query.isValid())
    {
        first_columns.append(query.value(0).toString());
        query.next();
    }


    //second
    QString select_second= cmd.arg("contents").arg(db_tablename).arg(start_time).arg(end_time);
    query.prepare(select_second);
    if (!query.exec())
        qDebug() << __FILE__ << __LINE__ << query.lastError();

    query.first();

    while (query.isValid())
    {
//        qDebug() << __FILE__ << __LINE__ << query.value(0).toString();
        second_columns.append(query.value(0).toString());
        query.next();
    }

    //third
    QString select_third;

    switch (type) {
    case WarningLog:
        select_third = cmd.arg("level").arg(db_tablename).arg(start_time).arg(end_time);
        break;
    case OperationLog:
        select_third = cmd.arg("operator").arg(db_tablename).arg(start_time).arg(end_time);
        break;
    case CommunicaitionLog:
        select_third = cmd.arg("framedata").arg(db_tablename).arg(start_time).arg(end_time);
        break;
    }

    query.prepare(select_third);
    if (!query.exec())
        qDebug() << __FILE__ << __LINE__ << query.lastError();

    query.first();

    while (query.isValid())
    {
//        qDebug() << __FILE__ << __LINE__ << query.value(0).toString();
        third_columns.append(query.value(0).toString());
        query.next();
    }


    result.append(first_columns);
    result.append(second_columns);
    result.append(third_columns);

    return result;
}

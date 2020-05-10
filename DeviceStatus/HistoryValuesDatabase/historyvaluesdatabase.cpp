#include "historyvaluesdatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

HistoryValuesDatabase::HistoryValuesDatabase(DisplayGroups group) : current_group(group)
{
    if (needed_db.contains("qt_sql_default_connection"))
        needed_db = QSqlDatabase::database("qt_sql_default_connection");
    else
        needed_db = QSqlDatabase::addDatabase("QSQLITE");

    needed_db.setDatabaseName(db_name);

    QDir current_path = QDir::currentPath();
    if (!current_path.exists(db_name))
    {
        if (!needed_db.isOpen())
        {
            if (!needed_db.open())
                qDebug() << "Open " << db_name << "error" << needed_db.lastError();
        }
    }
}

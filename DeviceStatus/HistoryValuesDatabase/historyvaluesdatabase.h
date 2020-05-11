#ifndef HISTORYVALUESDATABASE_H
#define HISTORYVALUESDATABASE_H

#include <QObject>
#include <QDateTime>
#include <QSqlDatabase>
#include <QDebug>
#include <DeviceStatus/DevStatus_regs.h>

class HistoryValuesDatabase
{
public:
    HistoryValuesDatabase();
    ~HistoryValuesDatabase();

//    QSqlDatabase current_database();
//    void open_current_databse();
//    void close_current_database();

    void insert_values_to_tables(QVector<QVector<quint16>> values);
    QVector<QVector<double>> search_values_from_tables(DisplayGroups group, qint64 start_time, qint64 end_time);

private:
    DisplayGroups current_group;

    QSqlDatabase needed_db;
    QString db_name = "history_values.db";
    QString connection_name = "history_values";

    void create_tables();
};

#endif // HISTORYVALUESDATABASE_H

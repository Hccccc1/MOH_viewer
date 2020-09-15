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
    HistoryValuesDatabase(int slave_addr = 0x01);
    ~HistoryValuesDatabase();

//    QSqlDatabase current_database();
//    void open_current_databse();
//    void close_current_database();

    void insert_values_to_tables(QVector<QVector<quint16>> values);
    QVector<QVector<double>> search_values_from_tables(DisplayGroups group, qint64 start_time, qint64 end_time);

//    int get_slave_addr();
//    void change_slave_addr(int slave_addr);

private:
    DisplayGroups current_group;

    QSqlDatabase needed_db;

    int m_slave_addr;

    QString db_name;
    QString tmp_db_name = "history_values_%1.db";
    QString connection_name = "history_values";

    void create_tables();

};

#endif // HISTORYVALUESDATABASE_H

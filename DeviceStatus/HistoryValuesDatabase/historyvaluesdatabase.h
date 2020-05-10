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
    HistoryValuesDatabase(DisplayGroups group = TT01_TT08);
    ~HistoryValuesDatabase();

    void create_table(DisplayGroups group);

private:
    DisplayGroups current_group;

    QSqlDatabase needed_db;
    QString db_name = "history_values.db";

};

#endif // HISTORYVALUESDATABASE_H

#include "historyvaluesdatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDir>

HistoryValuesDatabase::HistoryValuesDatabase(int slave_addr) : m_slave_addr(slave_addr)
{
    if (QSqlDatabase::contains(connection_name))
        needed_db = QSqlDatabase::database(connection_name);
    else
        needed_db = QSqlDatabase::addDatabase("QSQLITE", connection_name);

    db_name = tmp_db_name.arg(m_slave_addr);

    needed_db.setDatabaseName(db_name);

    QDir current_path = QDir::currentPath();
    if (!current_path.exists(db_name))
    {
        if (!needed_db.open())
            qDebug() << "Open " << db_name << "error" << needed_db.lastError();

        create_tables();
    }
    else
    {
        if (!needed_db.open())
            qDebug() << "Open " << db_name << "error" << needed_db.lastError();
    }
}

HistoryValuesDatabase::~HistoryValuesDatabase()
{
    //    close_current_database();

    if (needed_db.isOpen())
    {
        needed_db.close();
    }
}

//int HistoryValuesDatabase::get_slave_addr()
//{
//    return m_slave_addr;
//}

//void HistoryValuesDatabase::change_slave_addr(int slave_addr)
//{
//    m_slave_addr = slave_addr;

//    db_name = tmp_db_name.arg(m_slave_addr);
//}

void HistoryValuesDatabase::create_tables()
{
    QString cmd = "create table %1 (datetime INTEGER primary key, %2 INTEGER, %3 INTEGER, %4 INTEGER, %5 INTEGER, %6 INTEGER, %7 INTEGER, %8 INTEGER, %9 INTEGER)";
    QSqlQuery query = QSqlQuery(QSqlDatabase::database(connection_name));

    {
        QString create = cmd.arg("TT01_TT08").arg("TT01").arg("TT02").arg("TT03").arg("TT04").arg("TT05").arg("TT06").arg("TT07").arg("TT08");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        QString create = cmd.arg("TT09_TT16").arg("TT09").arg("TT10").arg("TT11").arg("TT12").arg("TT13").arg("TT14").arg("TT15").arg("TT16");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        QString create = cmd.arg("TT17_TT24").arg("TT17").arg("TT18").arg("TT19").arg("TT20").arg("TT21").arg("TT22").arg("TT23").arg("TT24");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        QString create = cmd.arg("TT25_TT32").arg("TT25").arg("TT26").arg("TT27").arg("TT28").arg("TT29").arg("TT30").arg("TT31").arg("TT32");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        cmd = "create table %1 (datetime INTEGER primary key, %2 INTEGER, %3 INTEGER, %4 INTEGER, %5 INTEGER)";
        QString create = cmd.arg("TT33_TT36").arg("TT33").arg("TT34").arg("TT35").arg("TT36");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        cmd = "create table %1 (datetime INTEGER primary key, %2 INTEGER, %3 INTEGER, %4 INTEGER, %5 INTEGER, %6 INTEGER, %7 INTEGER)";
        QString create = cmd.arg("PressureChart").arg("PT01").arg("PT02").arg("PT03").arg("PT04").arg("PT05").arg("PT06");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        cmd = "create table %1 (datetime INTEGER primary key, %2 INTEGER, %3 INTEGER, %4 INTEGER, %5 INTEGER, %6 INTEGER)";
        QString create = cmd.arg("FlowChart").arg("AFM01").arg("AFM02").arg("AFM03").arg("AFM04").arg("MFM01");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        cmd = "create table %1 (datetime INTEGER primary key, %2 INTEGER, %3 INTEGER, %4 INTEGER, %5 INTEGER)";
        QString create = cmd.arg("SpeedChart_1").arg("BL01").arg("BL02").arg("BL03").arg("BL04");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        cmd = "create table %1 (datetime INTEGER primary key, %2 INTEGER, %3 INTEGER, %4 INTEGER, %5 INTEGER, %6 INTEGER, %7 INTEGER)";
        QString create = cmd.arg("SpeedChart_2").arg("PMP01").arg("PMP02").arg("PMP03").arg("PMP04").arg("PMP05").arg("RAD01");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }

    {
        cmd = "create table %1 (datetime INTEGER primary key, %2 INTEGER, %3 INTEGER, %4 INTEGER, %5 INTEGER, %6 INTEGER, %7 INTEGER, %8 INTEGER, %9 INTEGER)";
//        QString create = cmd.arg("OthersChart").arg("CM01").arg("LT01").arg("LT02").arg("VT01").arg("IT01").arg("VT02").arg("IT02");
        QString create = cmd.arg("OthersChart").arg("LT01").arg("LT02").arg("VT01").arg("IT01").arg("FCPower").arg("OutPower").arg("VT02").arg("IT02");

        query.prepare(create);

        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Create error" << query.lastError();
    }
}

void HistoryValuesDatabase::insert_values_to_tables(QVector<QVector<quint16>> values)
{
    QSqlQuery query = QSqlQuery(QSqlDatabase::database(connection_name));
    qint64 current_dateime = QDateTime::currentMSecsSinceEpoch();

    if (!values[TT01_TT08].isEmpty() && values[TT01_TT08].size() == 8)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6, %7, %8, %9, %10)";
        QString insert = insert_cmd.arg("TT01_TT08")
                .arg(current_dateime)
                .arg(QString::number(values[TT01_TT08][0]))
                .arg(QString::number(values[TT01_TT08][1]))
                .arg(QString::number(values[TT01_TT08][2]))
                .arg(QString::number(values[TT01_TT08][3]))
                .arg(QString::number(values[TT01_TT08][4]))
                .arg(QString::number(values[TT01_TT08][5]))
                .arg(QString::number(values[TT01_TT08][6]))
                .arg(QString::number(values[TT01_TT08][7]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[TT09_TT16].isEmpty() && values[TT09_TT16].size() == 8)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6, %7, %8, %9, %10)";
        QString insert = insert_cmd.arg("TT09_TT16")
                .arg(current_dateime)
                .arg(QString::number(values[TT09_TT16][0]))
                .arg(QString::number(values[TT09_TT16][1]))
                .arg(QString::number(values[TT09_TT16][2]))
                .arg(QString::number(values[TT09_TT16][3]))
                .arg(QString::number(values[TT09_TT16][4]))
                .arg(QString::number(values[TT09_TT16][5]))
                .arg(QString::number(values[TT09_TT16][6]))
                .arg(QString::number(values[TT09_TT16][7]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[TT17_TT24].isEmpty() && values[TT17_TT24].size() == 8)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6, %7, %8, %9, %10)";
        QString insert = insert_cmd.arg("TT17_TT24")
                .arg(current_dateime)
                .arg(QString::number(values[TT17_TT24][0]))
                .arg(QString::number(values[TT17_TT24][1]))
                .arg(QString::number(values[TT17_TT24][2]))
                .arg(QString::number(values[TT17_TT24][3]))
                .arg(QString::number(values[TT17_TT24][4]))
                .arg(QString::number(values[TT17_TT24][5]))
                .arg(QString::number(values[TT17_TT24][6]))
                .arg(QString::number(values[TT17_TT24][7]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[TT25_TT32].isEmpty() && values[TT25_TT32].size() == 8)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6, %7, %8, %9, %10)";
        QString insert = insert_cmd.arg("TT25_TT32")
                .arg(current_dateime)
                .arg(QString::number(values[TT25_TT32][0]))
                .arg(QString::number(values[TT25_TT32][1]))
                .arg(QString::number(values[TT25_TT32][2]))
                .arg(QString::number(values[TT25_TT32][3]))
                .arg(QString::number(values[TT25_TT32][4]))
                .arg(QString::number(values[TT25_TT32][5]))
                .arg(QString::number(values[TT25_TT32][6]))
                .arg(QString::number(values[TT25_TT32][7]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[TT33_TT36].isEmpty() && values[TT33_TT36].size() == 4)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6)";
        QString insert = insert_cmd.arg("TT33_TT36")
                .arg(current_dateime)
                .arg(QString::number(values[TT33_TT36][0]))
                .arg(QString::number(values[TT33_TT36][1]))
                .arg(QString::number(values[TT33_TT36][2]))
                .arg(QString::number(values[TT33_TT36][3]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[PressureChart].isEmpty() && values[PressureChart].size() == 6)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6, %7, %8)";
        QString insert = insert_cmd.arg("PressureChart")
                .arg(current_dateime)
                .arg(QString::number(qint16(values[PressureChart][0])))
                .arg(QString::number(qint16(values[PressureChart][1])))
                .arg(QString::number(qint16(values[PressureChart][2])))
                .arg(QString::number(qint16(values[PressureChart][3])))
                .arg(QString::number(qint16(values[PressureChart][4])))
                .arg(QString::number(qint16(values[PressureChart][5])));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[FlowChart].isEmpty() && values[FlowChart].size() == 5)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6, %7)";
        QString insert = insert_cmd.arg("FlowChart")
                .arg(current_dateime)
                .arg(QString::number(values[FlowChart][0]))
                .arg(QString::number(values[FlowChart][1]))
                .arg(QString::number(values[FlowChart][2]))
                .arg(QString::number(values[FlowChart][3]))
                .arg(QString::number(values[FlowChart][4]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[SpeedChart_1].isEmpty() && values[SpeedChart_1].size() == 4)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6)";
        QString insert = insert_cmd.arg("SpeedChart_1")
                .arg(current_dateime)
                .arg(QString::number(values[SpeedChart_1][0]))
                .arg(QString::number(values[SpeedChart_1][1]))
                .arg(QString::number(values[SpeedChart_1][2]))
                .arg(QString::number(values[SpeedChart_1][3]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[SpeedChart_2].isEmpty() && values[SpeedChart_2].size() == 6)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6, %7, %8)";
        QString insert = insert_cmd.arg("SpeedChart_2")
                .arg(current_dateime)
                .arg(QString::number(values[SpeedChart_2][0]))
                .arg(QString::number(values[SpeedChart_2][1]))
                .arg(QString::number(values[SpeedChart_2][2]))
                .arg(QString::number(values[SpeedChart_2][3]))
                .arg(QString::number(values[SpeedChart_2][4]))
                .arg(QString::number(values[SpeedChart_2][5]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }

    if (!values[OthersChart].isEmpty() && values[OthersChart].size() == 8)
    {
        QString insert_cmd = "insert into %1 values (%2, %3, %4, %5, %6, %7, %8, %9, %10)";
        QString insert = insert_cmd.arg("OthersChart")
                .arg(current_dateime)
                .arg(QString::number(values[OthersChart][0]))
                .arg(QString::number(values[OthersChart][1]))
                .arg(QString::number(values[OthersChart][2]))
                .arg(QString::number(values[OthersChart][3]))
                .arg(QString::number(values[OthersChart][4]))
                .arg(QString::number(values[OthersChart][5]))
                .arg(QString::number(values[OthersChart][6]))
                .arg(QString::number(values[OthersChart][7]));

        query.prepare(insert);

        while (!query.exec())
            qDebug() << __FILE__ << __LINE__ << "Insert error: " << query.lastError();
    }
}

QVector<QVector<double>> HistoryValuesDatabase::search_values_from_tables(DisplayGroups group, qint64 start_time, qint64 end_time)
{
    QSqlRecord record;
    QVector<double> tmp;
    QVector<QVector<double>> result;

    if (needed_db.isOpen())
    {

        QSqlQuery query = QSqlQuery(QSqlDatabase::database(connection_name));
        QString search_cmd = "select * from %1 where datetime >= %2 AND datetime <= %3";
        QString search;

        switch (group)
        {
        case TT01_TT08:search = search_cmd.arg("TT01_TT08").arg(start_time).arg(end_time);break;
        case TT09_TT16:search = search_cmd.arg("TT09_TT16").arg(start_time).arg(end_time);break;
        case TT17_TT24:search = search_cmd.arg("TT17_TT24").arg(start_time).arg(end_time);break;
        case TT25_TT32:search = search_cmd.arg("TT25_TT32").arg(start_time).arg(end_time);break;
        case TT33_TT36:search = search_cmd.arg("TT33_TT36").arg(start_time).arg(end_time);break;

        case PressureChart:search = search_cmd.arg("PressureChart").arg(start_time).arg(end_time);break;
        case FlowChart:search = search_cmd.arg("FlowChart").arg(start_time).arg(end_time);break;
        case SpeedChart_1:search = search_cmd.arg("SpeedChart_1").arg(start_time).arg(end_time);break;
        case SpeedChart_2:search = search_cmd.arg("SpeedChart_2").arg(start_time).arg(end_time);break;
        case OthersChart:search = search_cmd.arg("OthersChart").arg(start_time).arg(end_time);break;
        }

        query.prepare(search);
        if (!query.exec())
            qDebug() << __FILE__ << __LINE__ << query.lastError();

        query.first();

        record = query.record();

        //        qDebug() << __FILE__ << __LINE__ << record.count();

        for (int i = 0; i < record.count(); i++)
        {
            //            if (group == OthersChart)
            //            {
            //                tmp.append(query.value(0).toDouble()/10);
            //                query.next();
            //            }

            while (query.next())
            {
                record = query.record();

//                qDebug() << record.value(i);

                if (i != 0)
                {

                    if (group == TT25_TT32 && i >= 5)
                    {
                        tmp.append(qint16(record.value(i).toInt()));
                    }
                    else if (group == TT33_TT36)
                    {
                        tmp.append(qint16(record.value(i).toInt()));
                    }
                    else if (group == PressureChart)
                    {
                        tmp.append(record.value(i).toDouble()/100);
                    }
                    else if (group == FlowChart)
                    {
                        tmp.append(record.value(i).toDouble()/10);
                    }
                    else if (group == OthersChart && i != 1 && i != 2)
                    {
                        tmp.append(record.value(i).toDouble()/10);
                    }
                    else
                        tmp.append(record.value(i).toDouble());
                }
                else
                    tmp.append(record.value(i).toDouble());
            }

            query.first();
            result.append(tmp);
            tmp.clear();
        }
    }

    return result;
}

#include "operationlogs.h"
#include "ui_operationlogs.h"

#include <QStandardItemModel>

OperationLogs::OperationLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OperationLogs)
{
    ui->setupUi(this);

    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("内容")));
    model->setItem(0, 2, new QStandardItem(tr("操作员")));

//    model->setItem(1, 0, new QStandardItem(tr("2020/4/19 4:17")));
//    model->setItem(1, 1, new QStandardItem(tr("1asdqweqasdfcvzxfasdf")));
//    model->setItem(1, 2, new QStandardItem(tr("SuperUser")));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();

    ui->startDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->startDateTimeEdit->setDisabled(true);
    ui->endDateTimeEdit->setDisabled(true);

//    database.create_database_table(db_name, table_name, OperationLog);
}

OperationLogs::~OperationLogs()
{
    delete ui;
}

void OperationLogs::resizeEvent(QResizeEvent *event)
{
    int column_time = static_cast<int>(event->size().width()*0.12);
    int column_content = static_cast<int>(event->size().width()*0.77);
    int column_level = event->size().width() - column_time - column_content-20;

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);
}

void OperationLogs::addOperationRecord(QString first_column, QString second_column)
{
    operation_database.insert_values_into_table(table_name, first_column, second_column);
}

void OperationLogs::on_getDataBtn_clicked()
{
    QDateTime startDateTime, endDateTime;
    qint64 start_time, end_time;
    QVector<QVector<QString>> tmp_result;

    switch (ui->quickSearch->currentIndex()) {
    case CustomDates:
        if (ui->endDateTimeEdit->dateTime().toMSecsSinceEpoch() < ui->startDateTimeEdit->dateTime().toMSecsSinceEpoch())
        {
            QMessageBox::critical(this, tr("错误"), tr("请选择正确的查询时间段"));
        }
        else
        {
            startDateTime = ui->startDateTimeEdit->dateTime();
            endDateTime = ui->endDateTimeEdit->dateTime();
        }
        break;
    case Today:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        endDateTime = QDateTime::currentDateTime();
        break;

    case Yesterday:
        endDateTime = QDateTime::currentDateTime();
        endDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime = endDateTime;
        startDateTime = endDateTime.addDays(-1);
        break;

    case LastSevenDays:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime = startDateTime.addDays(-7);
        endDateTime = QDateTime::currentDateTime();
        break;

    case CurrentMonth:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime.setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));
        endDateTime = QDateTime::currentDateTime();
        break;

    case LastMonth:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime.setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month()-1, 1));
        endDateTime.QDateTime::currentDateTime();
        endDateTime.setTime(QTime(0, 0, 0, 0));
        endDateTime.setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));
        break;
    }

    start_time = startDateTime.toMSecsSinceEpoch();
    end_time = endDateTime.toMSecsSinceEpoch();

    qDebug() << startDateTime << endDateTime;

    tmp_result = operation_database.get_columns_by_time(start_time, end_time);

    if (tmp_result[0].isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("没有数据！"));
    }
    else
    {
        for (int i = 0; i < tmp_result[0].size(); i++)
        {
            QString first_column = QDateTime::fromMSecsSinceEpoch(tmp_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem(i+1, 0, new QStandardItem(first_column));
            model->setItem(i+1, 1, new QStandardItem(tmp_result[1][i]));
            model->setItem(i+1, 2, new QStandardItem(tmp_result[2][i]));
        }
    }
}

void OperationLogs::on_quickSearch_currentIndexChanged(int index)
{
    if (index != CustomDates)
    {
        ui->startDateTimeEdit->setDisabled(true);
        ui->endDateTimeEdit->setDisabled(true);
    }
    else
    {
        ui->startDateTimeEdit->setEnabled(true);
        ui->endDateTimeEdit->setEnabled(true);
    }
}

void OperationLogs::changeEvent(QEvent *e)
{

}

#include "communicationlogs.h"
#include "ui_communicationlogs.h"

#include <QMessageBox>

CommunicationLogs::CommunicationLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicationLogs)
{
    ui->setupUi(this);

    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("类型")));
    model->setItem(0, 2, new QStandardItem(tr("帧数据")));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();

    ui->startDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    ui->startDateTimeEdit->setDisabled(true);
    ui->endDateTimeEdit->setDisabled(true);
}

CommunicationLogs::~CommunicationLogs()
{
    delete ui;
}

void CommunicationLogs::resizeEvent(QResizeEvent *event)
{
    int column_time = static_cast<int>(event->size().width()*0.20);
    int column_content = static_cast<int>(event->size().width()*0.12);
    int column_level = event->size().width() - column_time - column_content-20;

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);
}

void CommunicationLogs::addCommunicationRecord(QString first_column, QString second_column)
{
    warn_database.insert_values_into_table(table_name, first_column, second_column);
}

//void CommunicationLogs::on_pushButton_clicked()
//{
//    addCommunicationRecord("asdasd", "qweqrtr");
//}

void CommunicationLogs::on_getDataBtn_clicked()
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
//        endDateTime.setTime(QTime(0, 0, 0, 0));

        break;

    case CurrentMonth:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime.setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));
        endDateTime = QDateTime::currentDateTime();
//        endDateTime.setTime(QTime(0, 0, 0, 0));
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

    tmp_result = warn_database.get_columns_by_time(start_time, end_time);

    if (tmp_result[0].isEmpty())
    {
//        QString dbg_msg = "在"
        QMessageBox::critical(this, tr("错误"), tr("数据库中没有数据！"));
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

void CommunicationLogs::on_quickSearch_currentIndexChanged(int index)
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

void CommunicationLogs::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        model->item(0, 0)->setText(tr("时间"));
        model->item(0, 1)->setText(tr("类型"));
        model->item(0, 2)->setText(tr("帧数据"));

        ui->retranslateUi(this);
    }
}

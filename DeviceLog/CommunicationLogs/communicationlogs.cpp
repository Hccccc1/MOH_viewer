#include "communicationlogs.h"
#include "ui_communicationlogs.h"

#include <QStandardItem>

CommunicationLogs::CommunicationLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicationLogs)
{
    ui->setupUi(this);

//    handler.tableName = "warninglog_table";

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("类型")));
    model->setItem(0, 2, new QStandardItem(tr("帧数据")));

    model->setItem(1, 0, new QStandardItem(tr("2020/4/19 4:17")));
    model->setItem(1, 1, new QStandardItem(tr("1asdqweqasdfcvzxfasdf")));
    model->setItem(1, 2, new QStandardItem(tr("SuperUser")));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();

    database.create_database_table(db_name, table_name, WarningLog);
}

CommunicationLogs::~CommunicationLogs()
{
    delete ui;
}

void CommunicationLogs::resizeEvent(QResizeEvent *event)
{
//    qDebug() << __FILE__ << __LINE__ << event->size();

    int column_time = static_cast<int>(event->size().width()*0.12);
    int column_content = static_cast<int>(event->size().width()*0.12);
    int column_level = event->size().width() - column_time - column_content-20;

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);
}

void CommunicationLogs::addCommunicationRecord(QString first_column, QString second_column)
{
    database.insert_values_into_table(table_name, first_column, second_column);
}

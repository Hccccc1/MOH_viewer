#include "warninglogs.h"
#include "ui_warninglogs.h"

WarningLogs::WarningLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningLogs)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("内容")));
    model->setItem(0, 2, new QStandardItem(tr("等级")));

    model->setItem(1, 0, new QStandardItem(tr("2020/4/19 4:17")));
    model->setItem(1, 1, new QStandardItem(tr("1asdqweqasdfcvzxfasdf")));
    model->setItem(1, 2, new QStandardItem(tr("SuperUser")));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();
}

WarningLogs::~WarningLogs()
{
    delete ui;
}

void WarningLogs::resizeEvent(QResizeEvent *event)
{
    qDebug() << __FILE__ << __LINE__ << event->size();

    int column_time = static_cast<int>(event->size().width()*0.12);
    int column_content = static_cast<int>(event->size().width()*0.78);
    int column_level = event->size().width() - column_time - column_content;

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);
}

#include "devicelog.h"
#include "ui_devicelog.h"

DeviceLog::DeviceLog(QWidget *parent, uint8_t model) :
    QWidget(parent),
    ui(new Ui::DeviceLog)
{
    ui->setupUi(this);

    current_model = model;

    ui->tabWidget->clear();
    ui->tabWidget->addTab(warningLogs, tr("报警日志"));
    ui->tabWidget->addTab(operationLogs, tr("操作日志"));
    ui->tabWidget->addTab(communicationLogs, tr("通讯日志"));

    qDebug() << __FILE__ << __LINE__ << ui->tabWidget->size();

//    ui->label->setText(QString::number(model));
}

DeviceLog::~DeviceLog()
{
    delete ui;
}

#include "devicelog.h"
#include "ui_devicelog.h"

DeviceLog::DeviceLog(QWidget *parent, uint8_t model, int slave_addr) :
    QWidget(parent),
    m_slave_addr(slave_addr),
    ui(new Ui::DeviceLog),
    current_model(model)
{
    ui->setupUi(this);

    warningLogs = new WarningLogs(nullptr, m_slave_addr);
    operationLogs = new OperationLogs(nullptr, m_slave_addr);
    communicationLogs = new CommunicationLogs(nullptr, m_slave_addr);

    ui->tabWidget->clear();
    ui->tabWidget->addTab(warningLogs, tr("报警日志"));
    ui->tabWidget->addTab(operationLogs, tr("操作日志"));
    ui->tabWidget->addTab(communicationLogs, tr("通讯日志"));

//    ui->label->setText(QString::number(model));
}

DeviceLog::~DeviceLog()
{
    delete ui;
}

void DeviceLog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        ui->tabWidget->setTabText(0, tr("报警日志"));
        ui->tabWidget->setTabText(1, tr("操作日志"));
        ui->tabWidget->setTabText(2, tr("通讯日志"));

        ui->retranslateUi(this);
    }
}

int DeviceLog::get_slave_addr()
{
    return m_slave_addr;
}

void DeviceLog::change_slave_addr(int slave_addr)
{
    m_slave_addr = slave_addr;

    warningLogs->change_slave_addr(slave_addr);
    operationLogs->change_slave_addr(slave_addr);
    communicationLogs->change_slave_addr(slave_addr);
}

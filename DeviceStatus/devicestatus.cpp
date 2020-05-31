#include "devicestatus.h"
#include "ui_devicestatus.h"

#include "DeviceStatus/DevStatus_regs.h"

DeviceStatus::DeviceStatus(QWidget *parent, ModbusSerial *serial, uint8_t model, Accounts account) :
    QWidget(parent),
    ui(new Ui::DeviceStatus),
    current_serial(serial),
    current_model(model),
    current_account(account)
{
    ui->setupUi(this);

    dataOverview = new DataOverview(ui->tabWidget, current_serial, current_account);
    if (current_account == Customer)
    {
        customer_rtCurve = new CustomerRTCurve(ui->tabWidget, current_serial);
        customer_rtCurve->startTimer(1000);

        customer_hisCurve = new customer_HistoryCurve(ui->tabWidget);
    }
    else
    {
        rtCurve = new RTCurve(ui->tabWidget, current_serial, current_account);
        rtCurve->startTimer(1000);

        hisCurve = new HisCurve(ui->tabWidget);
    }

    realTimeValues = new RTValues(ui->tabWidget, current_serial, current_account);

    ui->tabWidget->clear();
    ui->tabWidget->addTab(dataOverview, tr("数据概况"));
    ui->tabWidget->addTab(realTimeValues, tr("实时数值"));

    if (current_account == Customer)
    {
        ui->tabWidget->addTab(customer_rtCurve, tr("实时曲线"));
        ui->tabWidget->addTab(customer_hisCurve, tr("历史曲线"));
    }
    else
    {
        ui->tabWidget->addTab(rtCurve, tr("实时曲线"));
        ui->tabWidget->addTab(hisCurve, tr("历史曲线"));
    }

//    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &DeviceStatus::index_changed);
}

DeviceStatus::~DeviceStatus()
{
    delete ui;
}

void DeviceStatus::onReadyRead()
{
//    qDebug() << this->objectName() << __func__ << __LINE__ << ui->tabWidget->currentIndex();

    auto reply = qobject_cast<QModbusReply *>(sender());

//    qDebug() << __FILE__ <<  __LINE__ << reply->error();

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        switch (ui->tabWidget->currentIndex()) {
        case 0:
            dataOverview->data_process(unit);
            break;
        case 1:
            realTimeValues->data_process(unit);
            break;
        case 2:
            if (current_account == Customer)
                customer_rtCurve->data_process(unit);
            else
                rtCurve->data_process(unit);
            break;
        }
    }
    else
    {
        emit modbusErrorHappened(reply->error());
    }
}

void DeviceStatus::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 0:
        dataOverview->refreshCurrentPage();
        break;
    case 1:
        realTimeValues->refreshCurrentPage();
        break;
    default:break;
    }
}

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

    dataOverview = new DataOverview(nullptr, current_serial);
    rtCurve = new RTCurve(nullptr, current_serial, current_account);
    hisCurve = new HisCurve();
    realTimeValues = new RTValues();

    ui->tabWidget->clear();
    ui->tabWidget->addTab(dataOverview, tr("数据概况"));
    ui->tabWidget->addTab(realTimeValues, tr("实时数值"));
    ui->tabWidget->addTab(rtCurve, tr("实时曲线"));
    ui->tabWidget->addTab(hisCurve, tr("历史曲线"));

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
            break;
        case 2:
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
//    qDebug() << index;
    switch (index) {
    case 0:
        dataOverview->refreshCurrentPage();
        break;
    default:break;
    }
}

//void DeviceStatus::on_readBtn_clicked()
//{
//    serial->read_from_modbus(QModbusDataUnit::Coils, TT_01, 77);
//}

//void DeviceStatus::index_changed()
//{
//    qDebug() << ui->tabWidget->currentIndex();
//}

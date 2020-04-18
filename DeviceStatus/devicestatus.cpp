#include "devicestatus.h"
#include "ui_devicestatus.h"

#include "DeviceStatus/DevStatus_regs.h"

DeviceStatus::DeviceStatus(QWidget *parent, ModbusSerial *current_serial, uint8_t model) :
    QWidget(parent),
    ui(new Ui::DeviceStatus),
    serial(current_serial),
    current_model(model)
{
    ui->setupUi(this);

    ui->tabWidget->clear();
    ui->tabWidget->addTab(dataOverview, tr("数据概况"));
    ui->tabWidget->addTab(rtCurve, tr("实时曲线"));
    ui->tabWidget->addTab(hisCurve, tr("历史曲线"));

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &DeviceStatus::index_changed);
}

DeviceStatus::~DeviceStatus()
{
    delete ui;
}

void DeviceStatus::onReadyRead()
{
    qDebug() << this->objectName() << __func__ << __LINE__;

    auto reply = qobject_cast<QModbusReply *>(sender());

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        switch (ui->tabWidget->currentIndex()) {
        case 0:
            dataOverview->data_process(unit);
            break;
        case 1:
            rtCurve->data_process(unit);
            break;
        }
    }
}

//void DeviceStatus::on_readBtn_clicked()
//{
//    serial->read_from_modbus(QModbusDataUnit::Coils, TT_01, 77);
//}

void DeviceStatus::index_changed()
{
    qDebug() << ui->tabWidget->currentIndex();
}

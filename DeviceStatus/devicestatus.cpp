#include "devicestatus.h"
#include "ui_devicestatus.h"

#include "DeviceStatus/DevStatus_regs.h"

DeviceStatus::DeviceStatus(QWidget *parent,
                           ModbusSerial *serial,
                           uint8_t model,
                           Accounts account,
                           DeviceLog *log_handler) :
    QWidget(parent),
    ui(new Ui::DeviceStatus),
    current_serial(serial),
    current_model(model),
    current_account(account),
    current_log_handler(log_handler)
{
    ui->setupUi(this);

    dataOverview = new DataOverview(ui->tabWidget, current_serial, current_account);
    if (current_account == Customer)
    {
        customer_rtCurve = new CustomerRTCurve(ui->tabWidget, current_serial);
        //        customer_rtCurve->startTimer(1000);

        customer_hisCurve = new customer_HistoryCurve(ui->tabWidget, current_serial->operation_mutex);
    }
    else
    {
        rtCurve = new RTCurve(ui->tabWidget, current_serial, current_account);
        //        rtCurve->startTimer(1000);

        hisCurve = new HisCurve(ui->tabWidget, current_serial->operation_mutex);
    }

    realTimeValues = new RTValues(ui->tabWidget, current_serial, current_account);

    ui->tabWidget->clear();
    ui->tabWidget->addTab(dataOverview, tr("数据概况"));

    if (current_account != Customer)
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

    connect(this, &DeviceStatus::communicationRecord,
            current_log_handler->communicationLogs, &CommunicationLogs::addCommunicationRecord);
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
    if (!reply)
        return;

    disconnect(reply, &QModbusReply::finished, this, &DeviceStatus::onReadyRead);

    if (current_serial->is_write_process_done())
    {
        if (reply->error() == QModbusDevice::NoError)
        {
            const QModbusDataUnit unit = reply->result();

//            if (unit.isValid() && unit.valueCount() != 0)
//            {
//                QString result_str = ModbusSerial::makeRTUFrame(current_serial->settings().slave_addr, ModbusSerial::createReadRequest(unit).functionCode(), reply->rawResult().data()).toHex();
//                emit communicationRecord("RX", result_str);
//            }

            switch (ui->tabWidget->currentIndex()) {
            case 0:
                dataOverview->data_process(unit);
                break;
            case 1:
                if (current_account != Customer)
                    realTimeValues->data_process(unit);
                else
                    customer_rtCurve->data_process(unit);
                break;
            case 2:
                if (current_account == Customer)
                    customer_rtCurve->data_process(unit);
                else
                    rtCurve->data_process(unit);
                break;
            }

            emit refresh_timeout_counter();

        }
        else
        {
            emit modbusErrorHappened(reply->error());
        }

        if (!current_serial->is_serial_ready())
            current_serial->set_serial_state(true);
    }

}

void DeviceStatus::on_tabWidget_currentChanged(int index)
{
    if (current_account != Customer)
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
    else
    {
        switch (index) {
        case 0:
            dataOverview->refreshCurrentPage();
            break;
        default:break;
        }
    }
}

void DeviceStatus::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        ui->tabWidget->setTabText(0, tr("数据概况"));
        if (current_account != Customer)
        {
            ui->tabWidget->setTabText(1, tr("实时数值"));
            ui->tabWidget->setTabText(2, tr("实时曲线"));
            ui->tabWidget->setTabText(3, tr("历史曲线"));
        }
        else
        {
            ui->tabWidget->setTabText(1, tr("实时曲线"));
            ui->tabWidget->setTabText(2, tr("历史曲线"));
        }

        ui->retranslateUi(this);
    }
}

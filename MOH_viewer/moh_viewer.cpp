#include "moh_viewer.h"
#include "ui_moh_viewer.h"

MOH_viewer::MOH_viewer(QWidget *parent, uint8_t model)
    : QMainWindow(parent)
    , ui(new Ui::MOH_viewer)
    , current_model(model)
{
    ui->setupUi(this);

    control_panel_widget    = new ControlPanel(this, model);
    device_log_widget       = new DeviceLog(this, model);
    device_status_widget    = new DeviceStatus(this, model);
    para_conf               = new ParameterConfiguration(this, model);
    sys_setting             = new SystemSetting(this, model);

    ui->tabWidget->clear();

    ui->tabWidget->addTab(device_status_widget, "设备状态");
    ui->tabWidget->addTab(control_panel_widget, "控制面板");
    ui->tabWidget->addTab(para_conf, "参数配置");
    ui->tabWidget->addTab(device_log_widget, "设备日志");
//    ui->tabWidget->addTab(sys_setting, "系统设置");

    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
    {
        QSerialPort detected;
        detected.setPort(info);
        if (detected.open(QIODevice::ReadWrite))
        {
            ui->serial_list->addItem(detected.portName());
            detected.close();
        }
    }

    ui->connect_btn->setEnabled(true);
    ui->close_connect_btn->setDisabled(true);
    ui->refresh_connect_btn->setEnabled(true);

    connect(_modbus, &ModbusSerial::serial_connected, this, &MOH_viewer::slot_serial_connected);
}

MOH_viewer::~MOH_viewer()
{
    delete ui;
}

void MOH_viewer::on_connect_btn_clicked()
{
    _modbus->change_portname(ui->serial_list->currentText());
    _modbus->on_confirm_btn_clicked();

    slot_serial_connected();
}

void MOH_viewer::on_refresh_connect_btn_clicked()
{
    ui->serial_list->clear();

    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
    {
        QSerialPort detected;
        detected.setPort(info);
        if (detected.open(QIODevice::ReadWrite))
        {
            ui->serial_list->addItem(detected.portName());
            detected.close();
        }
    }
}

void MOH_viewer::on_close_connect_btn_clicked()
{
    if (_modbus->modbus_client->state() == QModbusClient::ConnectedState)
    {
        _modbus->modbus_client->disconnectDevice();
    }

    ui->connect_btn->setEnabled(true);
    ui->refresh_connect_btn->setEnabled(true);
    ui->close_connect_btn->setDisabled(true);
    ui->more_para_btn->setEnabled(true);
}

void MOH_viewer::on_more_para_btn_clicked()
{
    _modbus->show();
}

void MOH_viewer::slot_serial_connected()
{
    ui->close_connect_btn->setEnabled(true);
    ui->connect_btn->setDisabled(true);
    ui->refresh_connect_btn->setDisabled(true);
    ui->more_para_btn->setDisabled(true);
}

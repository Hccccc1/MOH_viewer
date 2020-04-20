#include "moh_viewer.h"
#include "ui_moh_viewer.h"
#include <QTabBar>
#include <AllBitsAndRegs.h>

MOH_viewer::MOH_viewer(QWidget *parent, uint8_t model)
    : QMainWindow(parent)
    , ui(new Ui::MOH_viewer)
    , current_model(model)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/logo_2x.png"));

    control_panel_widget    = new ControlPanel(nullptr, model);
    device_log_widget       = new DeviceLog(nullptr, model);
    device_status_widget    = new DeviceStatus(nullptr, _modbus, model);
    para_conf               = new ParameterConfiguration(nullptr, model);
    sys_setting             = new SystemSetting(nullptr, model);

    ui->mainWidget->clear();

    ui->mainWidget->addTab(device_status_widget, QStringLiteral("设备状态"));
    ui->mainWidget->addTab(control_panel_widget, QStringLiteral("控制面板"));
    ui->mainWidget->addTab(para_conf, QStringLiteral("参数配置"));
    ui->mainWidget->addTab(device_log_widget, QStringLiteral("设备日志"));

    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
    {
        QSerialPort detected;
        detected.setPort(info);
        if (detected.open(QIODevice::ReadWrite))
        {
            detected.close();
        }
    }

    connect(ui->readData_btn, &QPushButton::clicked, _modbus, &ModbusSerial::on_confirm_btn_clicked);
    //    connect(ui->comtrolMode_combobox, &QComboBox::currentIndexChanged, this, &MOH_viewer::)
}

MOH_viewer::~MOH_viewer()
{
    delete ui;
}

void MOH_viewer::on_powerCtrl_btn_clicked()
{
    if (!running_status)
    {
        running_status = true;

        _modbus->write_to_modbus(QModbusDataUnit::Coils, Coils_SysCtrlStart, 1, true);

        ui->powerCtrl_btn->setStyleSheet(QString("QPushButton {width: 93px;height:43px;border:0px;image: url(:/switch_on.png);}"));
        ui->powerCtrl_label->setText(tr("关机"));
        ui->run_btn->setStyleSheet(QString("QPushButton {width: 44px;height:44px;border:0px;image: url(:/stop_button.png);\n}"));
        ui->emergency_stop->setStyleSheet(QString("QPushButton {\n	width: 44px;\n	height:44px;\n	border:0px;\n	image: url(:/emergency_stop_light.png);\n}"));
        ui->restore_btn->setStyleSheet(QString("QPushButton {\n	width: 52px;\n	height:52px;\n	border:0px;\n	image: url(:/restore_btn_light.png);\n}"));
    }
    else
    {
        running_status = false;

        _modbus->write_to_modbus(QModbusDataUnit::Coils, Coils_SysCtrlStart, 1, false);

        ui->powerCtrl_btn->setStyleSheet(QString("QPushButton {width: 93px;height:43px;border:0px;image: url(:/switch_off.png);}"));
        ui->powerCtrl_label->setText(tr("开机"));
        ui->run_btn->setStyleSheet(QString("QPushButton {width: 44px;height:44px;border:0px;image: url(:/run_btn.png);\n}"));
        ui->emergency_stop->setStyleSheet(QString("QPushButton {\n	width: 44px;\n	height:44px;\n	border:0px;\n	image: url(:/emergency_stop.png);\n}"));
        ui->restore_btn->setStyleSheet(QString("QPushButton {\n	width: 52px;\n	height:52px;\n	border:0px;\n	image: url(:/restore_btn.png);\n}"));
    }
}

void MOH_viewer::on_run_btn_clicked()
{
    if (running_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, Coils_SysCtrlSelfCheck, 1, true);

        ui->selfcheck_btn->setStyleSheet("QPushButton {width:83px;height:32px;border:0px;image: url(:/selfcheck.png);}");
    }
    else
        QMessageBox::critical(this, "错误", "请打开串口后尝试！");
}

void MOH_viewer::on_emergency_stop_clicked()
{
    if (running_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, Coils_SysCtrlEmergencyShutDown, 1, true);
    }
    else
        QMessageBox::critical(this, "错误", "请打开串口后尝试！");
}

void MOH_viewer::on_restore_btn_clicked()
{
    if (running_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, Coils_SysCtrlReset, 1, true);
    }
    else
        QMessageBox::critical(this, "错误", "请打开串口后尝试！");
}

void MOH_viewer::on_controlMode_combobox_currentIndexChanged()
{
    int index = ui->controlMode_combobox->currentIndex();

    if (running_status)
    {
        switch (index) {
        case 1:
            _modbus->write_to_modbus(QModbusDataUnit::Coils, Coils_AutoCtrl, 1, true);break;
        case 2:
            _modbus->write_to_modbus(QModbusDataUnit::Coils, Coils_AutoCtrl, 1, false);break;
        default:
            break;
        }
    }
    else
        QMessageBox::critical(this, "错误", "请打开串口后尝试！");
}

void MOH_viewer::on_generateMode_combobox_currentIndexChanged()
{
    int index = ui->generateMode_combobox->currentIndex();

    if (running_status)
    {
        switch (index) {
        case 0:
            _modbus->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 0x01);break;
        case 1:
            _modbus->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 0x02);break;
        case 2:
            _modbus->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 0x03);break;
        default:
            break;
        }
    }
    else
        QMessageBox::critical(this, "错误", "请打开串口后尝试！");
}

void MOH_viewer::resizeEvent(QResizeEvent *event)
{
    //    qDebug() << __FILE__ << __LINE__ << this->size();

    int width = event->size().width()- ui->groupBox_2->width();
    int tab_count = ui->mainWidget->count();
    int tab_width = width / tab_count;
    QString tmp_sheet = ui->mainWidget->styleSheet();

    tmp_sheet += QString(QString("QTabBar::tab {width:%1px;}").arg(tab_width));
    this->setStyleSheet(tmp_sheet);

}

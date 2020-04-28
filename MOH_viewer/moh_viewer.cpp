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

    control_panel_widget    = new ControlPanel(nullptr, _modbus, model);
    device_log_widget       = new DeviceLog(nullptr, model);
    device_status_widget    = new DeviceStatus(nullptr, _modbus, model);
    para_conf               = new ParameterConfiguration(nullptr, _modbus, model);
    sys_setting             = new SystemSetting(nullptr, model);

    ui->mainWidget->clear();

    ui->mainWidget->addTab(device_status_widget, QStringLiteral("设备状态"));
    ui->mainWidget->addTab(control_panel_widget, QStringLiteral("控制面板"));
    ui->mainWidget->addTab(para_conf, QStringLiteral("参数配置"));
    ui->mainWidget->addTab(device_log_widget, QStringLiteral("设备日志"));

    set_setylesheet_to_default();

    connect(device_status_widget->rtCurve, &RTCurve::dataChanged, this, &MOH_viewer::showRealTimeValue);

    connect(ui->globalSetting_btn, &QPushButton::clicked, _modbus, &ModbusSerial::on_confirm_btn_clicked);
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

        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlStart, 1, true);

        ui->powerCtrl_btn->setStyleSheet(QString("QPushButton {width: 93px;height:43px;border:0px;image: url(:/switch_on.png);}"));
        ui->powerCtrl_label->setText(tr("关机"));
        ui->run_btn->setStyleSheet(QString("QPushButton {width: 44px;height:44px;border:0px;image: url(:/stop_button.png);}"));
        ui->emergency_stop->setStyleSheet(QString("QPushButton {width: 44px;height:44px;border:0px;image: url(:/emergency_stop_light.png);}"));
        ui->restore_btn->setStyleSheet(QString("QPushButton {width: 52px;height:52px;border:0px;image: url(:/restore_btn_light.png);}"));
    }
    else
    {
        running_status = false;

        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlStart, 1, false);

        ui->powerCtrl_btn->setStyleSheet(QString("QPushButton {width: 93px;height:43px;border:0px;image: url(:/switch_off.png);}"));
        ui->powerCtrl_label->setText(tr("开机"));
        ui->run_btn->setStyleSheet(QString("QPushButton {width: 44px;height:44px;border:0px;image: url(:/run_btn.png);}"));
        ui->emergency_stop->setStyleSheet(QString("QPushButton {	width: 44px;	height:44px;	border:0px;	image: url(:/emergency_stop.png);}"));
        ui->restore_btn->setStyleSheet(QString("QPushButton {	width: 52px;	height:52px;	border:0px;	image: url(:/restore_btn.png);}"));
    }
}

void MOH_viewer::on_run_btn_clicked()
{
    if (running_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 1, true);

        ui->selfcheck_btn->setStyleSheet("QPushButton {width:83px;height:32px;border:0px;image: url(:/selfcheck.png);}");
    }
    else
        QMessageBox::critical(this, "错误", "设备未运行！");
}

void MOH_viewer::on_emergency_stop_clicked()
{
    if (running_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlEmergencyShutDown, 1, true);
    }
    else
        QMessageBox::critical(this, "错误", "设备未运行！");
}

void MOH_viewer::on_restore_btn_clicked()
{
    if (running_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlReset, 1, true);
    }
    else
        QMessageBox::critical(this, "错误", "设备未运行！");
}

void MOH_viewer::on_controlMode_combobox_currentIndexChanged()
{
    int index = ui->controlMode_combobox->currentIndex();

    if (running_status)
    {
        switch (index) {
        case 1:
            _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_AutoCtrl, 1, true);break;
        case 2:
            _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_AutoCtrl, 1, false);break;
        default:
            break;
        }
    }
    else
        QMessageBox::critical(this, "错误", "设备未运行！");
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
        QMessageBox::critical(this, "错误", "设备未运行！");
}

void MOH_viewer::on_selfcheck_btn_clicked()
{
    if (running_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 1, true);

        set_setylesheet_to_default();

        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_IT01, 15);
        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_PT01, 12);
        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_VT01, 6);
    }
    else
        QMessageBox::critical(this, "错误", "请打开串口后尝试！");
}

void MOH_viewer::set_setylesheet_to_default()
{
    ui->TT_03_label->setStyleSheet(selfcheck_default_status);
    ui->TT_05_label->setStyleSheet(selfcheck_default_status);
    ui->TT_15_label->setStyleSheet(selfcheck_default_status);
    ui->TT_16_label->setStyleSheet(selfcheck_default_status);
    ui->TT_17_label->setStyleSheet(selfcheck_default_status);
    ui->TT_19_label->setStyleSheet(selfcheck_default_status);
    ui->TT_23_label->setStyleSheet(selfcheck_default_status);
    ui->TT_24_label->setStyleSheet(selfcheck_default_status);
    ui->TT_25_label->setStyleSheet(selfcheck_default_status);
    ui->TT_27_label->setStyleSheet(selfcheck_default_status);
    ui->TT_29_label->setStyleSheet(selfcheck_default_status);
    ui->TT_31_label->setStyleSheet(selfcheck_default_status);
    ui->TT_33_label->setStyleSheet(selfcheck_default_status);
    ui->TT_34_label->setStyleSheet(selfcheck_default_status);
    ui->TT_37_label->setStyleSheet(selfcheck_default_status);
    ui->PT_01_label->setStyleSheet(selfcheck_default_status);
    ui->PT_02_label->setStyleSheet(selfcheck_default_status);
    ui->PT_03_label->setStyleSheet(selfcheck_default_status);
    ui->PT_04_label->setStyleSheet(selfcheck_default_status);
    ui->PT_05_label->setStyleSheet(selfcheck_default_status);
    ui->PT_06_label->setStyleSheet(selfcheck_default_status);
    ui->AFM_01_label->setStyleSheet(selfcheck_default_status);
    ui->AFM_02_label->setStyleSheet(selfcheck_default_status);
    ui->AFM_03_label->setStyleSheet(selfcheck_default_status);
    ui->AFM_04_label->setStyleSheet(selfcheck_default_status);
    ui->MFM_04_label->setStyleSheet(selfcheck_default_status);
    ui->CM_01_label->setStyleSheet(selfcheck_default_status);
    ui->VT_01_label->setStyleSheet(selfcheck_default_status);
    ui->VT_02_label->setStyleSheet(selfcheck_default_status);
    ui->IT_01_label->setStyleSheet(selfcheck_default_status);
    ui->IT_02_label->setStyleSheet(selfcheck_default_status);
    ui->LT_01_label->setStyleSheet(selfcheck_default_status);
    ui->LT_02_label->setStyleSheet(selfcheck_default_status);
}

void MOH_viewer::onReadyRead()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        for (int i = 0, total = int(unit.valueCount()); i < total; i++)
        {
            int addr = unit.startAddress() + i;

            switch (addr)
            {
            case DiscreteInputs_SelfCheck_TT03:
                if (unit.value(i) == 1)
                    ui->TT_03_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_03_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT05:
                if (unit.value(i) == 1)
                    ui->TT_05_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_05_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT15:
                if (unit.value(i) == 1)
                    ui->TT_15_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_15_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT16:
                if (unit.value(i) == 1)
                    ui->TT_16_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_16_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT17:
                if (unit.value(i) == 1)
                    ui->TT_17_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_17_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT19:
                if (unit.value(i) == 1)
                    ui->TT_19_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_19_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT23:
                if (unit.value(i) == 1)
                    ui->TT_23_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_23_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT24:
                if (unit.value(i) == 1)
                    ui->TT_24_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_24_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT25:
                if (unit.value(i) == 1)
                    ui->TT_25_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_25_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT27:
                if (unit.value(i) == 1)
                    ui->TT_27_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_27_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT29:
                if (unit.value(i) == 1)
                    ui->TT_29_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_29_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT31:
                if (unit.value(i) == 1)
                    ui->TT_31_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_31_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT33:
                if (unit.value(i) == 1)
                    ui->TT_33_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_33_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT34:
                if (unit.value(i) == 1)
                    ui->TT_34_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_34_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_TT37:
                if (unit.value(i) == 1)
                    ui->TT_37_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->TT_37_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_PT01:
                if (unit.value(i) == 1)
                    ui->PT_01_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->PT_01_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_PT02:
                if (unit.value(i) == 1)
                    ui->PT_02_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->PT_02_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_PT03:
                if (unit.value(i) == 1)
                    ui->PT_03_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->PT_03_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_PT04:
                if (unit.value(i) == 1)
                    ui->PT_04_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->PT_04_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_PT05:
                if (unit.value(i) == 1)
                    ui->PT_05_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->PT_05_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_PT06:
                if (unit.value(i) == 1)
                    ui->PT_06_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->PT_06_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_AFM01:
                if (unit.value(i) == 1)
                    ui->AFM_01_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->AFM_01_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_AFM02:
                if (unit.value(i) == 1)
                    ui->AFM_02_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->AFM_02_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_AFM03:
                if (unit.value(i) == 1)
                    ui->AFM_03_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->AFM_03_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_AFM04:
                if (unit.value(i) == 1)
                    ui->AFM_04_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->AFM_04_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_MFM04:
                if (unit.value(i) == 1)
                    ui->MFM_04_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->MFM_04_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_CM01:
                if (unit.value(i) == 1)
                    ui->CM_01_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->CM_01_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_VT01:
                if (unit.value(i) == 1)
                    ui->VT_01_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->VT_01_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_VT02:
                if (unit.value(i) == 1)
                    ui->VT_02_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->VT_02_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_IT01:
                if (unit.value(i) == 1)
                    ui->IT_01_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->IT_01_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_IT02:
                if (unit.value(i) == 1)
                    ui->IT_02_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->IT_02_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_LT01:
                if (unit.value(i) == 1)
                    ui->LT_01_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->LT_01_label->setStyleSheet(selfcheck_ok_status);
                break;
            case DiscreteInputs_SelfCheck_LT02:
                if (unit.value(i) == 1)
                    ui->LT_02_label->setStyleSheet(selfcheck_malfunction_status);
                else
                    ui->LT_02_label->setStyleSheet(selfcheck_ok_status);
                break;
            case HoldingRegs_DevSlaveAddr:
                ui->devSlaveAddr->setText(QString::number(unit.value(i)));
                break;
            case HoldingRegs_DevIPAddr:
                ui->devIPAddr->setText(QString("%1.%2.%3.%4").arg(QString::number((unit.value(i)&0xff00)>>8))
                                                             .arg(QString::number(unit.value(i)&0x00ff))
                                                             .arg(QString::number((unit.value(i+1)&0xff00)>>8))
                                                             .arg(QString::number(unit.value(i+1)&0x00ff)));
                break;
            case HoldingRegs_FirmwareVersion:
                ui->firmwareVersion->setText(QString("%1.%2.%3").arg(QString::number(unit.value(i)/100))
                                                                .arg(QString::number(unit.value(i)/10%100))
                                                                .arg(QString::number(unit.value(i)%10)));
                break;
            case HoldingRegs_HardwareVersion:
                ui->hardWareVersion->setText(QString("%1.%2.%3").arg(QString::number(unit.value(i)/100))
                                                                .arg(QString::number(unit.value(i)/10%100))
                                                                .arg(QString::number(unit.value(i)%10)));
                break;
            case HoldingRegs_SerialPara:
                if ((unit.value(i)&0xff00)>>8 == 0x01)
                {
                    ui->serialParity->setText("even");
                    ui->serialDatabits->setText(QString::number(8));
                    ui->serialStopbits->setText(QString::number(1));
                }
                else if ((unit.value(i)&0xff00)>>8 == 0x10)
                {
                    ui->serialParity->setText("odd");
                    ui->serialDatabits->setText(QString::number(8));
                    ui->serialStopbits->setText(QString::number(1));
                }
                else if ((unit.value(i)&0xff00)>>8 == 0x11)
                {
                    ui->serialParity->setText("none");
                    ui->serialDatabits->setText(QString::number(8));
                    ui->serialStopbits->setText(QString::number(2));
                }
                else
                {
                    ui->serialParity->setText("none");
                    ui->serialDatabits->setText(QString::number(8));
                    ui->serialStopbits->setText(QString::number(1));
                }

                switch (unit.value(i)&0x00ff) {
                case 0x00:
                    ui->serialBaudrate->setText(QString::number(QSerialPort::Baud1200));
                    break;
                case 0x01:
                    ui->serialBaudrate->setText(QString::number(QSerialPort::Baud4800));
                    break;
                case 0x02:
                    ui->serialBaudrate->setText(QString::number(QSerialPort::Baud9600));
                    break;
                case 0x03:
                    ui->serialBaudrate->setText(QString::number(QSerialPort::Baud19200));
                    break;
                case 0x04:
                    ui->serialBaudrate->setText(QString::number(QSerialPort::Baud38400));
                    break;
                case 0x05:
                    ui->serialBaudrate->setText(QString::number(QSerialPort::Baud115200));
                    break;
                }
                break;
            case InputRegs_SysStatus:
                switch(unit.value(i)) {
                case ST_00:
                    ui->sysRunnningStatus->setText("ST-00");
                    break;
                case ST_10:
                    ui->sysRunnningStatus->setText("ST-10");
                    break;
                case ST_11:
                    ui->sysRunnningStatus->setText("ST-11");
                    break;
                case ST_20:
                    ui->sysRunnningStatus->setText("ST-20");
                    break;
                case ST_30:
                    ui->sysRunnningStatus->setText("ST-30");
                    break;
                case ST_40:
                    ui->sysRunnningStatus->setText("ST-40");
                    break;
                case ST_50:
                    ui->sysRunnningStatus->setText("ST-50");
                    break;
                case ST_60:
                    ui->sysRunnningStatus->setText("ST-60");
                    break;
                case ST_70:
                    ui->sysRunnningStatus->setText("ST-70");
                    break;
                case ST_80:
                    ui->sysRunnningStatus->setText("ST-80");
                    break;
                case ST_90:
                    ui->sysRunnningStatus->setText("ST-90");
                    break;
                case ST_100:
                    ui->sysRunnningStatus->setText("ST-100");
                    break;
                case ST_110:
                    ui->sysRunnningStatus->setText("ST-110");
                    break;
                case ST_120:
                    ui->sysRunnningStatus->setText("ST-120");
                    break;
                }
                break;
            case DiscreteInputs_LowPressure_PT03:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("PT-04压力低"));
                break;
            case DiscreteInputs_HighPressure_PT03:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("PT-04压力高"));
                break;
            case DiscreteInputs_HighPressure_PT05:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("PT-05压力高"));
                break;
            case DiscreteInputs_HighTemperature_TT17:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("TT-17温度高"));
                break;
            case DiscreteInputs_HighTemperature_TT18:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("TT-18温度高"));
                break;
            case DiscreteInputs_ConductivityAbnormal_CS01:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("电导率异常"));
                break;
            case DiscreteInputs_LowVoltage_BAT01:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("BAT-01电池电压低"));
                break;
            case DiscreteInputs_LowLevel_LT1:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("LT1低液位"));
                break;
            case DiscreteInputs_LowLevel_LT2:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("LT2低液位"));
                break;
            case DiscreteInputs_LowLoading:
                if (unit.value(i))
                    ui->warningInfo->setText(QString("低负载"));
                break;
            case HoldingRegs_SysTotalTime:
                ui->totalBootTimes->setText(QString("设备已累计运行%1:%2:%3").arg((unit.value(i) << 16)|(unit.value(i+1)))
                                                                           .arg((unit.value(i+2)&0xff00)>>8)
                                                                           .arg(unit.value(i+2)&0x00ff));
                break;
            case CoilsRegs_AutoCtrl:
                if (unit.value(i))
                    ui->controlMode_combobox->setCurrentIndex(0);
                else
                    ui->controlMode_combobox->setCurrentIndex(1);
                break;
            case CoilsRegs_AutoCharge:
                if (unit.value(i))
                    ui->autoCharge_combobox->setCurrentIndex(0);
                else
                    ui->autoCharge_combobox->setCurrentIndex(1);
                break;
            case HoldingRegs_PowerMode:
                if (unit.value(i) == 1)
                    ui->generateMode_combobox->setCurrentIndex(0);
                else if (unit.value(i) == 2)
                    ui->generateMode_combobox->setCurrentIndex(1);
                else
                    ui->generateMode_combobox->setCurrentIndex(2);
                break;
            case DiscreteInputs_Status_Can:
                if (unit.value(i))
                    ui->statusCAN->setStyleSheet(status_off);
                else
                    ui->statusCAN->setStyleSheet(status_on);
                break;
            case DiscreteInputs_Status_RS485_1:
                if (unit.value(i))
                    ui->statusRS4851->setStyleSheet(status_off);
                else
                    ui->statusRS4851->setStyleSheet(status_on);
                break;
            case DiscreteInputs_Status_RS485_2:
                if (unit.value(i))
                    ui->statusRS4852->setStyleSheet(status_off);
                else
                    ui->statusRS4852->setStyleSheet(status_on);
                break;
            case DiscreteInputs_Status_RS485_3:
                if (unit.value(i))
                    ui->statusRS4853->setStyleSheet(status_off);
                else
                    ui->statusRS4853->setStyleSheet(status_on);
                break;
            case DiscreteInputs_Status_4G:
                if (unit.value(i))
                    ui->status4G->setStyleSheet(status_off);
                else
                    ui->status4G->setStyleSheet(status_on);
                break;
            case DiscreteInputs_Status_Enternet:
                if (unit.value(i))
                    ui->statusEnternet->setStyleSheet(status_off);
                else
                    ui->statusEnternet->setStyleSheet(status_on);
                break;
            default:
                break;
            }
        }
    }
}

void MOH_viewer::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width()- ui->groupBox_2->width();
    int tab_count = ui->mainWidget->count();
    int tab_width = width / tab_count;
    QString tmp_sheet = ui->mainWidget->styleSheet();

    tmp_sheet += QString("QTabBar::tab {width:%1px;}").arg(tab_width);
    this->setStyleSheet(tmp_sheet);

}

void MOH_viewer::on_globalSetting_btn_clicked()
{
//    sys_setting = new  SystemSetting(this);
//    sys_setting->show();
}

void MOH_viewer::showRealTimeValue(QString data)
{
    qDebug() << __FILE__ << __LINE__ << data;

    ui->statusbar->showMessage(data, 2500);
}

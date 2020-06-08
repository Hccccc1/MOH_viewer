#include "moh_viewer.h"
#include "ui_moh_viewer.h"
#include <QTabBar>
#include <AllBitsAndRegs.h>

//#include <QMediaPlayer>
#include <QSound>

MOH_viewer::MOH_viewer(QWidget *parent, uint8_t model, Accounts account)
    : QMainWindow(parent),
      ui(new Ui::MOH_viewer),
      current_model(model),
      current_account(account)
{
    ui->setupUi(this);

//    QTranslator *trans = new QTranslator();
//    if (!trans->load(":/english.qm"))
//    {
//        qDebug() << __FILE__ << __LINE__;
//    }
//    qApp->installTranslator(trans);

    this->setWindowIcon(QIcon(":/logo_2x.png"));
    device_log_widget       = new DeviceLog(nullptr, model);

    _modbus = new ModbusSerial(this, device_log_widget);

    control_panel_widget    = new ControlPanel(nullptr, _modbus, model, current_account, device_log_widget);
    device_status_widget    = new DeviceStatus(nullptr, _modbus, model, current_account, device_log_widget);
    para_conf               = new ParameterConfiguration(nullptr, _modbus, model, current_account, device_log_widget);
    sys_setting             = new SystemSetting(nullptr, model);

    ui->mainWidget->clear();

    ui->mainWidget->addTab(device_status_widget, tr("设备状态"));
    if (current_account != Customer)
        ui->mainWidget->addTab(control_panel_widget, tr("控制面板"));
    ui->mainWidget->addTab(para_conf, tr("参数配置"));
    ui->mainWidget->addTab(device_log_widget, tr("设备日志"));

    //    set_stylesheet_to_default();

    //    connect(device_status_widget->rtCurve, &RTCurve::dataChanged, this, &MOH_viewer::showRealTimeValue);
    //    connect(device_status_widget->hisCurve, &HisCurve::dataChanged, this, &MOH_viewer::showRealTimeValue);

    connect(_modbus, &ModbusSerial::serial_connected, this, &MOH_viewer::on_serialConnected);
    connect(_modbus, &ModbusSerial::serial_disconnected, this, &MOH_viewer::on_serialDisconnected);

    connect(this, &MOH_viewer::communicationRecord,
            device_log_widget->communicationLogs, &CommunicationLogs::addCommunicationRecord);
    connect(this, &MOH_viewer::warningRecord,
            device_log_widget->warningLogs, &WarningLogs::addWarningRecord);

    setWindowState(Qt::WindowMaximized);

    //    QMediaPlayer *player = new QMediaPlayer();
    //    player->setMedia(QUrl::fromLocalFile(":/Smoke_Alarm.wav"));
    //    player->setVolume(30);
    //    player->play();

    //    qDebug() << player->state() << player->isAudioAvailable();

    //    QSound::play(":/Smoke_Alarm.wav");

    connect(para_conf, &ParameterConfiguration::modbusErrorHappened, _modbus, &ModbusSerial::on_errorHappened);
    connect(device_status_widget, &DeviceStatus::modbusErrorHappened, _modbus, &ModbusSerial::on_errorHappened);
    connect(control_panel_widget, &ControlPanel::modbusErrorHappened, _modbus, &ModbusSerial::on_errorHappened);
    connect(this, &MOH_viewer::modbusErrorHappened, _modbus, &ModbusSerial::on_errorHappened);

    if (current_account == Customer)
    {
        ui->groupSelfcheck->hide();
        ui->selfcheckBtn->hide();
    }
}

MOH_viewer::~MOH_viewer()
{
    delete ui;
}

void MOH_viewer::on_mainWidget_currentChanged(int index)
{
    //    control_panel_widget->stop_refresh_timer();

    if (current_account != Customer)
    {
        switch (index) {
        case 0:
            this->refreshCurrentPage();
            break;
        case 1:
            control_panel_widget->refreshCurrentPage();
            control_panel_widget->start_refresh_timer(1);
            break;
        case 2:
            para_conf->refreshCurrentPage();
            break;
        default:
            break;
        }
    }
    else
    {
        switch (index) {
        case 0:
            this->refreshCurrentPage();
            break;
        case 1:
            para_conf->refreshCurrentPage();
            break;
        default:
            break;
        }
    }
}

//void MOH_viewer::set_stylesheets(bool status)
//{
//    ui->powerCtrl_btn->setStyleSheet(
//                status ? (powerCtrl_button_on) : (powerCtrl_button_off)
//                         );
////    ui->powerCtrl_label->setText(
////                status ? (tr("开机")) : (tr("关机"))
////                         );
//    ui->run_btn->setStyleSheet(
//                status ? (stop_button) : (run_button)
//                         );
//    ui->emergency_stop->setStyleSheet(emergency_stop_button);
//    ui->restore_btn->setStyleSheet(restore_button);
//}

/*
void MOH_viewer::on_powerCtrl_btn_clicked()
{
    if (!start_status)
    {
        start_status = true;

        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlStart, 1, true);

        set_stylesheets(start_status);
    }
    else
    {
        start_status = false;

        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlStart, 1, false);

        set_stylesheets(start_status);
    }
}

void MOH_viewer::on_run_btn_clicked()
{
    if (start_status)
    {
        if (running_status)
        {
            _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlRun, 1, false);
            ui->run_btn->setStyleSheet(run_button);
            running_status = false;
        }
        else
        {
            _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlRun, 1, true);
            ui->run_btn->setStyleSheet(stop_button);
            running_status = true;
        }

        //        ui->selfcheck_btn->setStyleSheet("QPushButton {width:83px;height:32px;border:0px;image: url(:/selfcheck.png);}");
    }
    else
        QMessageBox::critical(this, "错误", "设备未运行！");
}

void MOH_viewer::on_emergency_stop_clicked()
{
    if (start_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlEmergencyShutDown, 1, true);
    }
    else
        QMessageBox::critical(this, "错误", "设备未运行！");
}

void MOH_viewer::on_restore_btn_clicked()
{
    if (start_status)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlReset, 1, true);
    }
    else
        QMessageBox::critical(this, "错误", "设备未运行！");
}
*/

void MOH_viewer::on_bootBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlStart, 1, true);
}

void MOH_viewer::on_shutdownBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlShutDown, 1, true);
}

void MOH_viewer::on_runBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlRun, 1, true);
}

//void MOH_viewer::on_stopBtn_clicked()
//{
//    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
//        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlRun, 1, true);
//}

void MOH_viewer::on_emergencyStopBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlEmergencyShutDown, 1, true);
}

void MOH_viewer::on_restoreBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlReset, 1, true);
}

void MOH_viewer::on_selfcheckBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
    {
        _modbus->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 1, true);

        //        set_stylesheet_to_default();

        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_TT03, 15);
        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_PT01, 12);
        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_VT01, 6);
    }
    else
        QMessageBox::critical(this, tr("错误"), tr("请打开串口后尝试！"));
}

void MOH_viewer::on_controlMode_combobox_currentIndexChanged(int index)
{
    //    int index = ui->controlMode_combobox->currentIndex();

    //    qDebug() << sender()->objectName();

    if (_modbus->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        //        if (start_status)
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
        //        else
        //            QMessageBox::critical(this, "错误", "设备未运行！");
    }
}

void MOH_viewer::on_generateMode_combobox_currentIndexChanged(int index)
{
    //    int index = ui->generateMode_combobox->currentIndex();

    //    if (start_status)
    //    {
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
    //    }
    //    else
    //        QMessageBox::critical(this, "错误", "设备未运行！");
}

/*
void MOH_viewer::set_stylesheet_to_default()
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
*/

void MOH_viewer::onReadyRead()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        if (unit.isValid() && unit.valueCount() != 0)
        {
            QString result_str = ModbusSerial::makeRTUFrame(1, ModbusSerial::createReadRequest(unit).functionCode(), reply->rawResult().data()).toHex();
            emit communicationRecord("RX", result_str);
        }

        for (int i = 0, total = int(unit.valueCount()); i < total; i++)
        {
            int addr = unit.startAddress() + i;

            switch (addr)
            {
            case CoilsRegs_SysCtrlSelfCheck:break;
            case CoilsRegs_SysCtrlStart:
                //                start_status = true;
                //                set_stylesheets(start_status);
                break;
            case CoilsRegs_SysCtrlRun:
                //                unit.value(i) ? (running_status = true) : (running_status = false);

                //                ui->run_btn->setStyleSheet(
                //                            (unit.value(i)) ? (stop_button) : (run_button)
                //                                              );
                break;

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
                {
                    ui->warningInfo->setText(QString(tr("PT-04压力低")));
                    emit warningRecord(tr("PT-04压力低"), "1");
                }
                break;
            case DiscreteInputs_HighPressure_PT03:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("PT-04压力高")));
                    emit warningRecord(tr("PT-04压力高"), "1");
                }
                break;
            case DiscreteInputs_HighPressure_PT05:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("PT-05压力高")));
                    emit warningRecord(tr("PT-05压力高"), "1");
                }
                break;
            case DiscreteInputs_HighTemperature_TT17:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("TT-17温度高")));
                    emit warningRecord(tr("TT-17温度高"), "1");
                }
                break;
            case DiscreteInputs_HighTemperature_TT31:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("TT-18温度高")));
                    emit warningRecord(tr("TT-18温度高"), "1");
                }
                break;
            case DiscreteInputs_ConductivityAbnormal_CS01:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("电导率异常")));
                    emit warningRecord(tr("电导率异常"), "1");
                }
                break;
            case DiscreteInputs_LowVoltage_BAT01:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("BAT-01电池电压低")));
                    emit warningRecord(tr("BAT-01电池电压低"), "1");
                }
                break;
            case DiscreteInputs_LowLevel_LT1:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("LT1低液位")));
                    emit warningRecord(tr("LT1低液位"), "1");
                }
                break;
            case DiscreteInputs_LowLevel_LT2:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("LT2低液位")));
                    emit warningRecord(tr("LT2低液位"), "1");
                }
                break;
            case DiscreteInputs_LowLoading:
                if (unit.value(i))
                {
                    ui->warningInfo->setText(QString(tr("低负载")));
                    emit warningRecord(tr("低负载"), "1");
                }
                break;
            case HoldingRegs_SysTotalTime:
                ui->totalBootTimes->setText(QString(tr("设备已累计运行%1:%2:%3")).arg((unit.value(i) << 16)|(unit.value(i+1)))
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

            case DiscreteInputs_IOInput00:
                ui->ioInput_start->setStyleSheet(
                            (unit.value(i)) ? \
                                selfcheck_ok_status : selfcheck_malfunction_status
                                );
                break;
            case DiscreteInputs_IOInput01:
                ui->ioInput_shutdown->setStyleSheet(
                            (unit.value(i)) ? \
                                selfcheck_ok_status : selfcheck_malfunction_status
                                );
                break;
            case DiscreteInputs_IOInput02:
                ui->ioInput_restore->setStyleSheet(
                            (unit.value(i)) ? \
                                selfcheck_ok_status : selfcheck_malfunction_status
                                );
                break;
            case DiscreteInputs_IOInput03:
                ui->ioInput_emergencyStop->setStyleSheet(
                            (unit.value(i)) ? \
                                selfcheck_ok_status : selfcheck_malfunction_status
                                );
                break;
            case DiscreteInputs_IOInput04:
                ui->ioInput_gateSensor->setStyleSheet(
                            (unit.value(i)) ? \
                                selfcheck_ok_status : selfcheck_malfunction_status
                                );
                break;

            case InputRegs_VT_01:
                ui->VT_01->setText(QString::number(double(unit.value(i))/10));
                break;
            case InputRegs_IT_01:
                ui->IT_01->setText(QString::number(double(unit.value(i))/10));
                break;
            case InputRegs_FcPower:
                ui->FCPower->setText(QString::number(double(unit.value(i))/10));
                break;

            default:
                break;
            }
        }

        if (!_modbus->is_serial_ready())
            _modbus->set_serial_state(true);

    }
    else
    {
        emit modbusErrorHappened(reply->error());
    }
}

void MOH_viewer::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width()- ui->groupBox_2->width() - 50;
    int tab_count = ui->mainWidget->count();
    int tab_width = width / tab_count;

    QString tmp_sheet = ui->mainWidget->styleSheet();
    tmp_sheet += QString("QTabBar::tab {width:%1px;}").arg(tab_width);

    this->setStyleSheet(tmp_sheet);
}

void MOH_viewer::on_globalSetting_btn_clicked()
{
    _modbus->show();
}

void MOH_viewer::showRealTimeValue(QString data)
{
    //    qDebug() << __FILE__ << __LINE__ << data;

    ui->statusbar->showMessage(data, 2500);
}

void MOH_viewer::on_serialConnected()
{
    //Serial is connected, need to update values of main widget
    //    qDebug() << "Serial connected";

    refreshCurrentPage();

    ui->serialPortname->setText(_modbus->settings().portname);
    ui->communicationStatus->setStyleSheet(status_on);

    startTimer(1000);
}

void MOH_viewer::on_serialDisconnected()
{
    ui->communicationStatus->setStyleSheet(status_off);
}

void MOH_viewer::refreshWarningMsg()
{
    if (_modbus->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_LowPressure_PT03, 10);
    }
}

void MOH_viewer::timerEvent(QTimerEvent *)
{
    refreshCurrentPage();
}

void MOH_viewer::refreshCurrentPage()
{
    if (_modbus->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        //        _modbus->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 6);
#if 0
        _modbus->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_AutoCtrl, 2);
        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_IOInput00, 5);
        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_Status_Can, 6);
        refreshWarningMsg();
        _modbus->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_FirmwareVersion, 2);
        _modbus->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_DevSlaveAddr, 7);
        _modbus->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 1);

        if (current_account != Customer)
        {
            switch (ui->mainWidget->currentIndex()) {
            case 0:device_status_widget->dataOverview->refreshCurrentPage();break;
            case 1:control_panel_widget->refreshCurrentPage();break;
            case 2:para_conf->refreshCurrentPage();break;
            default:break;
            }
        }
        else
        {
            switch (ui->mainWidget->currentIndex()) {
            case 0:device_status_widget->dataOverview->refreshCurrentPage();break;
            case 1:para_conf->refreshCurrentPage();break;
            default:break;
            }
        }
#endif

        _modbus->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 96);
        _modbus->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_IOInput00, 128);
        _modbus->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_TT_01, 77);
        _modbus->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Manufacturer, 90);

    }
}

void MOH_viewer::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        ui->mainWidget->setTabText(0, tr("设备状态"));
        if (current_account != Customer)
        {
            ui->mainWidget->setTabText(1, tr("控制面板"));
            ui->mainWidget->setTabText(2, tr("参数配置"));
            ui->mainWidget->setTabText(3, tr("设备日志"));
        }
        else
        {
            ui->mainWidget->setTabText(1, tr("参数配置"));
            ui->mainWidget->setTabText(2, tr("设备日志"));
        }

        ui->retranslateUi(this);
    }
}

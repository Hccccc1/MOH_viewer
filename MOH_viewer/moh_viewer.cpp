#include "moh_viewer.h"
#include "ui_moh_viewer.h"

MOH_Viewer::MOH_Viewer(QWidget *parent, uint8_t model, Accounts account, QTranslator* trans, int slave_addr)
    : QMainWindow(parent)
    , ui(new Ui::MOH_viewer)
    , current_model(model)
    , current_account(account)
    , m_slave_addr(slave_addr)
    , current_trans(trans)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/logo_2x.png"));
    setWindowState(Qt::WindowMaximized);

    m_serial = new ModbusSerial(this);
    sys_setting = new SystemSetting(Q_NULLPTR, current_model, m_serial, current_trans);

    device_log_widget       = new DeviceLog(this, current_model, m_serial->settings().slave_addr);
    control_panel_widget    = new ControlPanel(nullptr, m_serial, model, current_account, device_log_widget);
    device_status_widget    = new DeviceStatus(nullptr, m_serial, model, current_account, device_log_widget);
    para_conf               = new ParameterConfiguration(nullptr, m_serial, model, current_account, device_log_widget);
    //    sys_setting             = new SystemSetting(nullptr, model, m_serial, current_trans);

    sys_setting->hide();

    ui->mainWidget->clear();

    ui->mainWidget->addTab(device_status_widget, tr("设备状态"));
    if (current_account != Customer)
        ui->mainWidget->addTab(control_panel_widget, tr("控制面板"));
    ui->mainWidget->addTab(para_conf, tr("参数配置"));
    ui->mainWidget->addTab(device_log_widget, tr("设备日志"));

//    connect(this, &MOH_Viewer::communicationRecord,
//            device_log_widget->communicationLogs, &CommunicationLogs::addCommunicationRecord);
//    connect(this, &MOH_Viewer::warningRecord,
//            device_log_widget->warningLogs, &WarningLogs::addWarningRecord);
//    connect(this, &MOH_Viewer::operationRecord,
//            device_log_widget->operationLogs, &OperationLogs::addOperationRecord);

//    connect(sound_thread, &WarningSound::warningRecord,
//            device_log_widget->warningLogs, &WarningLogs::addWarningRecord);

    if (current_account == Customer)
    {
        ui->groupSelfcheck->hide();
        ui->selfcheckBtn->hide();

        connect(device_status_widget->customer_hisCurve, &customer_HistoryCurve::operation_needs_lock, this, [=] {
            m_serial->operation_mutex->lock();
            emit stop_timer(m_serial->settings().slave_addr);
        });

        connect(device_status_widget->customer_hisCurve, &customer_HistoryCurve::operation_release_lock, this, [=] {
            m_serial->operation_mutex->unlock();
            emit resume_timer(m_serial->settings().slave_addr);
        });
    }
    else
    {
        connect(device_status_widget->hisCurve, &HisCurve::operation_needs_lock, this, [=] {
            m_serial->operation_mutex->lock();
            emit stop_timer(m_serial->settings().slave_addr);
        });

        connect(device_status_widget->hisCurve, &HisCurve::operation_release_lock, this, [=] {
            m_serial->operation_mutex->unlock();
            emit resume_timer(m_serial->settings().slave_addr);
        });
    }

    connect(refresh_timer, &QTimer::timeout, this, &MOH_Viewer::refreshRealTimeValues);
    connect(m_serial, &ModbusSerial::stop_timer, this, &MOH_Viewer::stop_refresh_timer);
    connect(m_serial, &ModbusSerial::start_timer, this, &MOH_Viewer::start_refresh_timer);

    connect(sound_thread, &WarningSound::change_color, this, &MOH_Viewer::changeBlinkState);
//    connect(sound_thread, &WarningSound::change_text, this, [=] {
//        qDebug() << sender() << sender()->parent();
//    });

    connect(sound_thread, &WarningSound::change_text, this, &MOH_Viewer::changeWarningText);
    connect(ui->warningInfo, &QPushButton::clicked, sound_thread, &WarningSound::clear_warning_msg);
    connect(this, &MOH_Viewer::warning_msg, sound_thread, &WarningSound::warning_msg_detected);
    connect(this, &MOH_Viewer::warning_dissmissed, sound_thread, &WarningSound::warning_msg_dissmissed);
    sound_thread->start();

    connect(sound_thread, &WarningSound::remove_msg, this, [=] {
        ui->warningInfo->setText("");
    });

    connect(sys_setting, &SystemSetting::start_timer, this, &MOH_Viewer::start_refresh_timer);
    connect(sys_setting, &SystemSetting::stop_timer, this, &MOH_Viewer::stop_refresh_timer);

    connect(sys_setting, &SystemSetting::upgrade_now, this, &MOH_Viewer::show_upgradeWidget);
    connect(sys_setting, &SystemSetting::switch_to_upgrade, this, &MOH_Viewer::show_upgradeWidget);

    connect(device_log_widget->warningLogs, &WarningLogs::operation_needs_lock, this, [=] {
        m_serial->operation_mutex->lock();
        emit stop_timer(m_serial->settings().slave_addr);
    });

    connect(device_log_widget->warningLogs, &WarningLogs::operation_release_lock, this, [=] {
        m_serial->operation_mutex->unlock();
        emit resume_timer(m_serial->settings().slave_addr);
    });

    connect(device_log_widget->operationLogs, &OperationLogs::operation_needs_lock, this, [=]{
        m_serial->operation_mutex->lock();
        emit stop_timer(m_serial->settings().slave_addr);
    });

    connect(device_log_widget->operationLogs, &OperationLogs::operation_release_lock, this, [=] {
        m_serial->operation_mutex->unlock();
        emit resume_timer(m_serial->settings().slave_addr);
    });

    connect(device_log_widget->communicationLogs, &CommunicationLogs::operation_needs_lock, this, [=]{
        m_serial->operation_mutex->lock();
        emit stop_timer(m_serial->settings().slave_addr);
    });

    connect(device_log_widget->communicationLogs, &CommunicationLogs::operation_release_lock, this, [=] {
        m_serial->operation_mutex->unlock();
        emit resume_timer(m_serial->settings().slave_addr);

    });

    connect(para_conf, &ParameterConfiguration::operation_needs_lock, this, [=]{
        m_serial->operation_mutex->lock();
        emit stop_timer(m_serial->settings().slave_addr);
    });

    connect(para_conf, &ParameterConfiguration::operation_release_lock, this, [=] {
        m_serial->operation_mutex->unlock();
        emit resume_timer(m_serial->settings().slave_addr);

    });

    connect(sys_setting, &SystemSetting::helpBtn_clicked, this, [=] {
        QProcess *help = new QProcess(this);
        QStringList manuals(".\\helper\\MOH_Helper.chm");
        help->start("hh.exe", manuals);
    });

//    msg_show.insert(LowPressure_PT03, tr("PT-04压力低"));
//    msg_show.insert(HighPressure_PT03, tr("PT-04压力高"));
//    msg_show.insert(HighPressure_PT05, tr("PT-05压力高"));
//    msg_show.insert(HighTemperature_TT17, tr("TT-17温度高"));
//    msg_show.insert(HighTemperature_TT31, tr("TT-18温度高"));
//    msg_show.insert(ConductivityAbnormal_CS01, tr("电导率异常"));
//    msg_show.insert(LowVoltage_BAT01, tr("BAT-01电池电压低"));
//    msg_show.insert(LowLevel_LT1, tr("LT1低液位"));
//    msg_show.insert(LowLevel_LT2, tr("LT2低液位"));
//    msg_show.insert(LowLoading, tr("低负载"));
}

MOH_Viewer::~MOH_Viewer()
{
    delete ui;
}

void MOH_Viewer::changeBlinkState(bool state)
{

//    qDebug() << __func__ << __LINE__ << sender()->parent();

    if (state)
        ui->warningBlink->setStyleSheet(warningRed);
    else
        ui->warningBlink->setStyleSheet(warningWhite);
}

void MOH_Viewer::changeWarningText()
{
    QList<quint8> msg_keys;

//    qDebug() << __func__ << __LINE__ << sender()->parent();

//    this->refreshWarningMsg();

    if (!msg_show.isEmpty())
    {
//        msg_show.append(msg);
        msg_keys = msg_show.keys();

        if (text_counter >= msg_show.size())
            text_counter = 0;

        //        qDebug() << tr(msg_show[text_counter].toLatin1());

        //        QString tmp = msg_show[text_counter];

        sys_setting->operation_mutex->lock();

        ui->warningInfo->setText(tr(msg_show.find(msg_keys[text_counter]).value().toStdString().c_str()));

        sys_setting->operation_mutex->unlock();

        text_counter++;
    }
    else
    {
        ui->warningInfo->setText("");
    }
}

void MOH_Viewer::show_upgradeWidget(const QString& portname, const int& baudrate)
{
    emit ui->warningInfo->clicked();

    emit m_serial->stop_timer();

    if (m_serial->isRunning())
    {
        m_serial->quit();
    }

    if (m_serial->is_serial_connected())
    {
        //        m_serial->modbus_client->disconnectDevice();
        m_serial->set_serial_state(false);
        emit m_serial->close_serial();
        m_serial->read_mutex->lock();
        m_serial->read_queue.clear();
        m_serial->read_mutex->unlock();

        m_serial->write_mutex->lock();
        m_serial->write_queue.clear();
        m_serial->write_mutex->unlock();
    }

//    on_warningInfo_clicked();

    SerialUpgrade* upgradeWidget = Q_NULLPTR;

//    qDebug() << __func__ << __LINE__ <<

    if (!multiple_moh && m_serial->is_serial_connected())
        upgradeWidget = new SerialUpgrade(Q_NULLPTR, m_serial->settings().portname, m_serial->settings().baud);
    else
        upgradeWidget = new SerialUpgrade(nullptr, portname, baudrate);

    upgradeWidget->show();

    //        m_serial->deleteLater();
    sys_setting->hide();
    this->hide();
}

//void MOH_Viewer::request_finished()
//{
//    auto reply = qobject_cast<QModbusReply *>(sender());
//    if (!reply)
//        return;

//    if (m_serial->is_write_process_done())
//    {
//        if (reply->error() == QModbusDevice::NoError)
//        {
//            qDebug() << "Request finished";

//            m_serial->set_serial_state(true);

//            const QModbusDataUnit unit = reply->result();

//            int start_addr = unit.startAddress();

//            onReadyRead(unit);
//            control_panel_widget->onReadyRead(unit);
//            if (m_serial->device_status_regs.contains(start_addr))
//                device_status_widget->onReadyRead(unit);
//            if (m_serial->parameter_set_regs.contains(start_addr))
//                para_conf->onReadyRead(unit);
//        }
//        else
//            modbusErrorHappened(reply->error());
//    }
//}

void MOH_Viewer::closeEvent(QCloseEvent *)
{
    //    this->deleteLater();

    sys_setting->deleteLater();

}

void MOH_Viewer::on_mainWidget_currentChanged(int index)
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
            //            control_panel_widget->start_refresh_timer(1);
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

void MOH_Viewer::on_bootBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
    {
        m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlStart, 1, true);
        emit operationRecord(tr("启动"), current_account);
    }
}

void MOH_Viewer::on_shutdownBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
    {
        m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlShutDown, 1, true);
        emit operationRecord(tr("关机"), current_account);
    }
}

void MOH_Viewer::on_runBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
    {
        m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlRun, 1, true);
        emit operationRecord(tr("运行"), current_account);
    }
}

void MOH_Viewer::on_emergencyStopBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
    {
        m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlEmergencyShutDown, 1, true);
        emit operationRecord(tr("紧急关机"), current_account);
    }
}

void MOH_Viewer::on_restoreBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
    {
        m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlReset, 1, true);
        emit operationRecord(tr("复位"), current_account);
    }
}

void MOH_Viewer::on_selfcheckBtn_clicked()
{
    if (QMessageBox::question(this, tr("提示"), tr("确定进行该操作吗？")) == QMessageBox::Yes)
    {
        m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 1, true);

        emit operationRecord(tr("自检"), current_account);

//        m_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_TT03, 15);
//        m_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_PT01, 12);
//        m_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_VT01, 6);
        m_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_SelfCheck_TT03, 38);
    }
}

void MOH_Viewer::on_controlMode_combobox_currentIndexChanged(int index)
{
    //    int index = ui->controlMode_combobox->currentIndex();

    //    qDebug() << sender()->objectName();

    if (m_serial->is_serial_connected())
    {
        //        if (start_status)
        {
            switch (index) {
            case 0:
                m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_AutoCtrl, 1, true);emit operationRecord(tr("开自动控制"), current_account);break;
            case 1:
                m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_AutoCtrl, 1, false);emit operationRecord(tr("关自动控制"), current_account);break;
            default:
                break;
            }
        }
        //        else
        //            QMessageBox::critical(this, "错误", "设备未运行！");
    }
}

void MOH_Viewer::on_autoCharge_combobox_currentIndexChanged(int index)
{
    qDebug() << index;

    if (m_serial->is_serial_connected())
    {
        switch (index) {
        case 0:
            m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_AutoCharge, 1, true);emit operationRecord(tr("开自动充电模式"), current_account);break;
        case 1:
            m_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_AutoCharge, 1, false);emit operationRecord(tr("关自动充电模式"), current_account);break;
        default:break;
        }
    }
}

void MOH_Viewer::on_generateMode_combobox_currentIndexChanged(int index)
{
    //    int index = ui->generateMode_combobox->currentIndex();

    //    if (start_status)
    //    {
    switch (index) {
    case 0:
        m_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 0x01);emit operationRecord(tr("CP模式"), current_account);break;
    case 1:
        m_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 0x02);emit operationRecord(tr("CV模式"), current_account);break;
    case 2:
        m_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_PowerMode, 0x03);emit operationRecord(tr("CC模式"), current_account);break;
    default:
        break;
    }
    //    }
    //    else
    //        QMessageBox::critical(this, "错误", "设备未运行！");
}

void MOH_Viewer::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width()- ui->groupBox_2->width() - 50;
    int tab_count = ui->mainWidget->count();
    int tab_width = width / tab_count;

    QString tmp_sheet = ui->mainWidget->styleSheet();
    tmp_sheet += QString("QTabBar::tab {width:%1px;}").arg(tab_width);

    this->setStyleSheet(tmp_sheet);
}

void MOH_Viewer::on_globalSetting_btn_clicked()
{
    //    _modbus->show();
    if (!m_serial->is_serial_connected())
        sys_setting->refresh_port();

    sys_setting->show();
}

void MOH_Viewer::set_setting_disabled()
{
    ui->globalSetting_btn->setDisabled(true);
}

void MOH_Viewer::on_serialConnected()
{
    //Serial is connected, need to update values of main widget
    qDebug() << "Serial connected";

    m_serial->start(QThread::NormalPriority);

    if (m_serial->settings().slave_addr != device_log_widget->get_slave_addr())
        device_log_widget->change_slave_addr(m_serial->settings().slave_addr);

    refreshCurrentPage();

    ui->serialPortname->setText(m_serial->settings().portname);
    ui->communicationStatus->setStyleSheet(status_on);

    m_serial->set_serial_state(true);

    start_refresh_timer();
}

void MOH_Viewer::on_serialDisconnected()
{
    ui->communicationStatus->setStyleSheet(status_off);

    sound_thread->quit();

    m_serial->quit();
    m_serial->set_serial_connec_state(false);
    m_serial->set_serial_state(false);

    m_serial->stop_timer();

    emit stop_timer(m_serial->settings().slave_addr);
}

void MOH_Viewer::refreshWarningMsg()
{
    if (m_serial->is_serial_connected())
    {
        m_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_LowPressure_PT03, 10);
    }
}

void MOH_Viewer::refreshRealTimeValues()
{
    static quint8 counter = 0;
    if (m_serial->is_serial_connected())
    {
        ui->serialPortname->setText(m_serial->settings().portname);

//        this->refreshWarningMsg();

//        m_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_IOInput00, 63);
        m_serial->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 96);
        m_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_IOInput00, 128);
        m_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_TT_01, 77);
        m_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_BL01, 10);
//        m_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SysTime, 19);

//        qDebug() << DataOverview
//        device_status_widget->dataOverview->sys_status.sys_year = 0;'

        if (++counter < 3)
            m_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SysTime, 19);
    }
}

void MOH_Viewer::refreshCurrentPage()
{
    if (m_serial->is_serial_connected())
    {
        //        _modbus->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 6);
        if ( m_slave_addr != m_serial->settings().slave_addr)
            m_slave_addr = m_serial->settings().slave_addr;

        {
            m_serial->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_SysCtrlSelfCheck, 96);
            m_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_IOInput00, 128);
            m_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_TT_01, 77);
            m_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_Manufacturer, 94);
        }
//        for (quint8 i = 0; i < 3; i++)
//            m_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SysTime, 19);
    }
}

void MOH_Viewer::changeEvent(QEvent *e)
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

        if (m_serial->is_serial_connected())
        {
            ui->serialPortname->setText(m_serial->settings().portname);
            refreshCurrentPage();
        }
    }
}

void MOH_Viewer::start_refresh_timer()
{
    refresh_timer->start(m_serial->settings().refresh_interval);

    if (!m_serial->isRunning())
        m_serial->start();
}

void MOH_Viewer::stop_refresh_timer()
{
    if (refresh_timer->isActive())
        refresh_timer->stop();

    if (m_serial->isRunning())
    {
//        m_serial->set_serial_state(false);
        m_serial->quit();
    }
}

void MOH_Viewer::on_warningInfo_clicked()
{
    ui->warningInfo->setText("");
}

//void MOH_Viewer::changeEvent(QEvent *e)
//{
//    if (e->type() == QEvent::LanguageChange)
//    {
//        ui->mainWidget->setTabText(0, tr("设备状态"));
//        if (current_account != Customer)
//        {
//            ui->mainWidget->setTabText(1, tr("控制面板"));
//            ui->mainWidget->setTabText(2, tr("参数配置"));
//            ui->mainWidget->setTabText(3, tr("设备日志"));
//        }
//        else
//        {
//            ui->mainWidget->setTabText(1, tr("参数配置"));
//            ui->mainWidget->setTabText(2, tr("设备日志"));
//        }

//        ui->retranslateUi(this);
//    }
//}

void MOH_Viewer::onReadyRead()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

//    qDebug() << __func__ << __LINE__ << this;

    disconnect(reply, &QModbusReply::finished, this, &MOH_Viewer::onReadyRead);

    if (m_serial->is_write_process_done())
    {
        if (reply->error() == QModbusDevice::NoError)
        {
            const QModbusDataUnit unit = reply->result();

            //            if (unit.isValid() && unit.valueCount() != 0)
            //            {
            //                QString result_str = ModbusSerial::makeRTUFrame(_modbus->settings().slave_addr, ModbusSerial::createReadRequest(unit).functionCode(), reply->rawResult().data()).toHex();
            //                emit communicationRecord("RX", result_str);
            //            }

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
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-03"));
                        ui->TT_03_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_03_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT05:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-05"));
                        ui->TT_05_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_05_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT15:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-15"));
                        ui->TT_15_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_15_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT16:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-16"));
                        ui->TT_16_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_16_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT17:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-17"));
                        ui->TT_17_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_17_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT19:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-19"));
                        ui->TT_19_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_19_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT23:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-23"));
                        ui->TT_23_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_23_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT24:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-24"));
                        ui->TT_24_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_24_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT25:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-25"));
                        ui->TT_25_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_25_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT27:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-27"));
                        ui->TT_27_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_27_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT29:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-29"));
                        ui->TT_29_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_29_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT31:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-31"));
                        ui->TT_31_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_31_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT33:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-33"));
                        ui->TT_33_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_33_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT34:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-34"));
                        ui->TT_34_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_34_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_TT37:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("TT-37"));
                        ui->TT_37_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->TT_37_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_PT01:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("PT-01"));
                        ui->PT_01_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->PT_01_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_PT02:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("PT-02"));
                        ui->PT_02_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->PT_02_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_PT03:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("PT-03"));
                        ui->PT_03_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->PT_03_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_PT04:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("PT-04"));
                        ui->PT_04_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->PT_04_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_PT05:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("PT-05"));
                        ui->PT_05_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->PT_05_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_PT06:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("PT-06"));
                        ui->PT_06_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->PT_06_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_AFM01:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("AFM-01"));
                        ui->AFM_01_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->AFM_01_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_AFM02:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("AFM-02"));
                        ui->AFM_02_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->AFM_02_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_AFM03:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("AFM-03"));
                        ui->AFM_03_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->AFM_03_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_AFM04:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("AFM-04"));
                        ui->AFM_04_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->AFM_04_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_MFM04:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("MFM-01"));
                        ui->MFM_04_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->MFM_04_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_CM01:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("CM-01"));
                        ui->CM_01_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->CM_01_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_VT01:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("VT-01"));
                        ui->VT_01_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->VT_01_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_VT02:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("VT-02"));
                        ui->VT_02_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->VT_02_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_IT01:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("IT-01"));
                        ui->IT_01_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->IT_01_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_IT02:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("IT-02"));
                        ui->IT_02_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->IT_02_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_LT01:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("LT-01"));
                        ui->LT_01_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->LT_01_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case DiscreteInputs_SelfCheck_LT02:
                    if (unit.value(i) == 1)
                    {
                        msg_selfdet.append(QString(tr("%1故障")).arg("LT-02"));
                        ui->LT_02_label->setStyleSheet(selfcheck_malfunction_status);
                    }
                    else
                        ui->LT_02_label->setStyleSheet(selfcheck_ok_status);
                    break;
                case HoldingRegs_DevSlaveAddr:
                    ui->devSlaveAddr->setText(QString::number(unit.value(i)));
                    break;
                case HoldingRegs_DevIPAddr:
                    ui->devIPAddr->setText(QString("%1.%2.%3.%4").arg(QString::number((unit.value(i+1)&0xff00)>>8))
                                           .arg(QString::number(unit.value(i+1)&0x00ff))
                                           .arg(QString::number((unit.value(i)&0xff00)>>8))
                                           .arg(QString::number(unit.value(i)&0x00ff)));
                    break;
                case HoldingRegs_FirmwareVersion:
                    ui->firmwareVersion->setText(QString("%1.%2.%3").arg(QString::number(unit.value(i)/1000))
                                                 .arg(QString::number(unit.value(i)/10%100))
                                                 .arg(QString::number(unit.value(i)%10)));
                    break;
                case HoldingRegs_HardwareVersion:
                    ui->hardWareVersion->setText(QString("%1.%2.%3").arg(QString::number(unit.value(i)/1000))
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
//                        qDebug() << __LINE__ << this;

                        if (!(warningMsg & LowPressure_PT03))
                        {
//                            ui->warningInfo->setText(QString(tr("PT-04压力低")));

                            warningMsg |= LowPressure_PT03;

//                            msg_show.append(tr("PT-04压力低"));
                            msg_show.insert(0, tr("PT-04压力低"));

                            emit warning_msg(LowPressure_PT03);
                            emit warningRecord(tr("PT-04压力低"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & LowPressure_PT03)
                        {
                            warningMsg &= ~(LowPressure_PT03);

                            msg_show.remove(0);

                            emit warning_dissmissed(LowPressure_PT03);
                            emit warningRecord(tr("PT-04压力低报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_HighPressure_PT03:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & HighPressure_PT03))
                        {
//                            ui->warningInfo->setText(QString(tr("PT-04压力高")));

                            //                        msg_show.append(tr("PT-04压力高"));

                            warningMsg |= HighPressure_PT03;

                            msg_show.insert(1, tr("PT-04压力高"));

                            emit warning_msg(HighPressure_PT03);
                            emit warningRecord(tr("PT-04压力高"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & HighPressure_PT03)
                        {
                            warningMsg &= ~(HighPressure_PT03);

                            msg_show.remove(1);

                            emit warning_dissmissed(HighPressure_PT03);
                            emit warningRecord(tr("PT-04压力高报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_HighPressure_PT05:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & HighPressure_PT05))
                        {
//                            ui->warningInfo->setText(QString(tr("PT-05压力高")));

                            warningMsg |= HighPressure_PT05;

                            msg_show.insert(2, tr("PT-05压力高"));

                            emit warning_msg(HighPressure_PT05);
                            emit warningRecord(tr("PT-05压力高"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & HighPressure_PT05)
                        {
                            warningMsg &= ~(HighPressure_PT05);

                            msg_show.remove(2);

                            emit warning_dissmissed(HighPressure_PT05);
                            emit warningRecord(tr("PT-05压力高报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_HighTemperature_TT17:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & HighTemperature_TT17))
                        {

//                            ui->warningInfo->setText(QString(tr("TT-17温度高")));

                            warningMsg |= HighTemperature_TT17;

                            msg_show.insert(3, tr("TT-17温度高"));

                            emit warning_msg(HighTemperature_TT17);
                            emit warningRecord(tr("TT-17温度高"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & HighTemperature_TT17)
                        {
                            warningMsg &= ~(HighTemperature_TT17);

                            msg_show.remove(3);

                            emit warning_dissmissed(HighTemperature_TT17);
                            emit warningRecord(tr("TT-17温度高报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_HighTemperature_TT31:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & HighTemperature_TT31))
                        {

//                            ui->warningInfo->setText(QString(tr("TT-18温度高")));

                            warningMsg |= HighTemperature_TT31;

                            msg_show.insert(4, tr("TT-18温度高"));

                            emit warning_msg(HighTemperature_TT31);
                            emit warningRecord(tr("TT-18温度高"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & HighTemperature_TT31)
                        {
                            warningMsg &= ~(HighTemperature_TT31);

                            msg_show.remove(4);

                            emit warning_dissmissed(HighTemperature_TT31);
                            emit warningRecord(tr("TT-18温度高报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_ConductivityAbnormal_CS01:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & ConductivityAbnormal_CS01))
                        {

//                            ui->warningInfo->setText(QString(tr("电导率异常")));

                            warningMsg |= ConductivityAbnormal_CS01;

                            msg_show.insert(5, tr("电导率异常"));

                            emit warning_msg(ConductivityAbnormal_CS01);
                            emit warningRecord(tr("电导率异常"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & ConductivityAbnormal_CS01)
                        {
                            warningMsg &= ~(ConductivityAbnormal_CS01);

                            msg_show.remove(5);

                            emit warning_dissmissed(ConductivityAbnormal_CS01);
                            emit warningRecord(tr("电导率异常报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_LowVoltage_BAT01:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & LowVoltage_BAT01))
                        {

//                            ui->warningInfo->setText(QString(tr("BAT-01电池电压低")));

                            warningMsg |= LowVoltage_BAT01;

                            msg_show.insert(6, tr("BAT-01电池电压低"));

                            emit warning_msg(LowVoltage_BAT01);
                            emit warningRecord(tr("BAT-01电池电压低"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & LowVoltage_BAT01)
                        {
                            warningMsg &= ~(LowVoltage_BAT01);

                            msg_show.remove(6);

                            emit warning_dissmissed(LowVoltage_BAT01);
                            emit warningRecord(tr("BAT-01电池电压低报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_LowLevel_LT1:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & LowLevel_LT1))
                        {

//                            ui->warningInfo->setText(QString(tr("LT1低液位")));

                            warningMsg |= LowLevel_LT1;

                            msg_show.insert(7, tr("LT1低液位"));

                            emit warning_msg(LowLevel_LT1);
                            emit warningRecord(tr("LT1低液位"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & LowLevel_LT1)
                        {
                            warningMsg &= ~(LowLevel_LT1);

                            msg_show.remove(7);

                            emit warning_dissmissed(LowLevel_LT1);
                            emit warningRecord(tr("LT1低液位报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_LowLevel_LT2:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & LowLevel_LT2))
                        {

//                            ui->warningInfo->setText(QString(tr("LT2低液位")));

                            warningMsg |= LowLevel_LT2;

                            msg_show.insert(8, tr("LT2低液位"));

                            emit warning_msg(LowLevel_LT2);
                            emit warningRecord(tr("LT2低液位"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & LowLevel_LT2)
                        {
                            warningMsg &= ~(LowLevel_LT2);

                            msg_show.remove(8);

                            emit warning_dissmissed(LowLevel_LT2);
                            emit warningRecord(tr("LT2低液位报警消除"), "1");
                        }
                    }
                    break;
                case DiscreteInputs_LowLoading:
                    if (unit.value(i))
                    {
                        if (!(warningMsg & LowLoading))
                        {

//                            ui->warningInfo->setText(QString(tr("低负载")));

                            warningMsg |= LowLoading;

                            msg_show.insert(9, tr("低负载"));

                            emit warning_msg(LowLoading);
                            emit warningRecord(tr("低负载"), "1");
                        }
                    }
                    else
                    {
                        if (warningMsg & LowLoading)
                        {
                            warningMsg &= ~(LowLoading);

                            msg_show.remove(9);

                            emit warning_dissmissed(LowLoading);
                            emit warningRecord(tr("低负载报警消除"), "1");
                        }
                    }
                    break;
                case HoldingRegs_SysTotalTime:

                    ui->totalBootTimes->setText(QString(tr("设备已累计运行%1:%2:%3")).arg(QString::number(unit.value(i)))
                                                .arg(QString::number((unit.value(i+1)&0xff00)>>8))
                                                .arg(QString::number(unit.value(i+1)&0x00ff)));
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

                    //                case HoldingRegs_ReadyForBoot:
                    //                    if (unit.value(i))
                    //                    {
                    ////                        _modbus->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_EnterBoot, 1);
                    //                        emit boot_ready(true);
                    //                    }
                    //                    else
                    //                        emit boot_ready(false);

                default:
                    break;
                }
            }

            emit refresh_timeout_counter();

        }
        else
        {
            emit modbusErrorHappened(reply->error());
        }

        if (!m_serial->is_serial_ready())
            m_serial->set_serial_state(true);
    }
}

void MOH_Viewer::keyPressEvent(QKeyEvent* event)
{
//    qDebug() << __func__ << event->key();
    if (event->key() == Qt::Key_F1)
    {
        QProcess *help = new QProcess(this);
        QStringList manuals(".\\helper\\MOH_Helper.chm");
        help->start("hh.exe", manuals);
    }
}

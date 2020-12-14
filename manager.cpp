#include "manager.h"

#include <QProcess>

Manager::Manager(QWidget *parent, Accounts account, uint8_t model, QTranslator* trans) : QWidget(parent)
  , current_model(model)
  , current_account(account)
  , current_trans(trans)
{
    //    m_serial.append(new ModbusSerial());

    moh_viewers.append(new MOH_Viewer(nullptr, current_model, current_account, current_trans));
    moh_viewers[moh_counter]->show();

    moh_counter++;

    //    m_serial[0]->setParent(moh_viewers[0]);

    //    connect(m_serial[0], &ModbusSerial::set_serial_state, )
    connect(m_serialPrivate, &ModbusSerialPrivate::serial_connected, moh_viewers[0], &MOH_Viewer::on_serialConnected);
    connect(m_serialPrivate, &ModbusSerialPrivate::serial_disconnected, moh_viewers[0], &MOH_Viewer::on_serialDisconnected);

    connect(m_serialPrivate, &ModbusSerialPrivate::set_serial_connec_state, moh_viewers[0]->m_serial, &ModbusSerial::set_serial_connec_state);

    connect(m_serialPrivate, &ModbusSerialPrivate::set_serial_state, moh_viewers[0]->m_serial, &ModbusSerial::set_serial_state);
    connect(m_serialPrivate, &ModbusSerialPrivate::set_write_state, moh_viewers[0]->m_serial, &ModbusSerial::set_write_state);

    connect(moh_viewers[0]->m_serial, &ModbusSerial::actual_read_req, m_serialPrivate, &ModbusSerialPrivate::do_the_actual_read);
    connect(moh_viewers[0]->m_serial, &ModbusSerial::actual_write_req, m_serialPrivate, &ModbusSerialPrivate::do_the_actual_write);

//    connect(m_serialPrivate->read_reply, &QModbusReply::finished, moh_viewers[0], &MOH_Viewer::request_finished);

    connect(moh_viewers[0]->m_serial, &ModbusSerial::close_serial, m_serialPrivate, &ModbusSerialPrivate::disconnect_serial);
    connect(moh_viewers[0]->sys_setting, &SystemSetting::connect_to_serial, m_serialPrivate, &ModbusSerialPrivate::connect_serial);

    connect(m_serialPrivate, &ModbusSerialPrivate::communicationRecord, moh_viewers[0]->device_log_widget->communicationLogs, &CommunicationLogs::addCommunicationRecord);

    connect(moh_viewers[0], &MOH_Viewer::warningRecord,
            moh_viewers[0]->device_log_widget->warningLogs, &WarningLogs::addWarningRecord);
    connect(moh_viewers[0], &MOH_Viewer::operationRecord,
            moh_viewers[0]->device_log_widget->operationLogs, &OperationLogs::addOperationRecord);

//    connect(moh_viewers[0]->m_serial, &ModbusSerial::, moh_viewers[0]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[0]->para_conf, &ParameterConfiguration::refresh_timeout_counter, moh_viewers[0]->sys_setting, &SystemSetting::on_refresh_timeout_counter);
    connect(moh_viewers[0]->device_status_widget, &DeviceStatus::refresh_timeout_counter, moh_viewers[0]->sys_setting, &SystemSetting::on_refresh_timeout_counter);
    connect(moh_viewers[0]->control_panel_widget, &ControlPanel::refresh_timeout_counter, moh_viewers[0]->sys_setting, &SystemSetting::on_refresh_timeout_counter);
    connect(moh_viewers[0], &MOH_Viewer::refresh_timeout_counter, moh_viewers[0]->sys_setting, &SystemSetting::on_refresh_timeout_counter);

    connect(moh_viewers[0]->m_serial, &ModbusSerial::modbusErrorHappened, moh_viewers[0]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[0]->para_conf, &ParameterConfiguration::modbusErrorHappened, moh_viewers[0]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[0]->device_status_widget, &DeviceStatus::modbusErrorHappened, moh_viewers[0]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[0]->control_panel_widget, &ControlPanel::modbusErrorHappened, moh_viewers[0]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[0], &MOH_Viewer::modbusErrorHappened, moh_viewers[0]->sys_setting, &SystemSetting::on_errorHappened);

    connect(moh_viewers[0]->sys_setting, &SystemSetting::new_widget_needed, this, &Manager::creat_new_moh);

    connect(moh_viewers[0]->sys_setting, &SystemSetting::connect_to_serial, this, [=](const ModbusSerial::Settings settings) {
        m_serialPrivate->insert_slave_addr(settings.slave_addr);

        m_serialPrivate->start_timeout_counter(settings.slave_addr);
        connect(m_serialPrivate->timeout_timers[settings.slave_addr], &QTimer::timeout, moh_viewers[0], &MOH_Viewer::on_serialDisconnected);
    });

    connect(moh_viewers[0], &MOH_Viewer::resume_timer, m_serialPrivate, &ModbusSerialPrivate::on_resume_timer);
    connect(moh_viewers[0], &MOH_Viewer::stop_timer, m_serialPrivate, &ModbusSerialPrivate::on_stop_timer);

//    m_serialPrivate->insert_slave_addr(moh_viewers[0]->m_serial->settings().slave_addr);
}

Manager::~Manager()
{
    moh_viewers.clear();
    //    m_serial.clear();

    if (m_serialPrivate)
        delete m_serialPrivate;
}

void Manager::creat_new_moh(int slave_addr)
{

    emit m_serialPrivate->set_serial_connec_state(false);

    moh_viewers.append(new MOH_Viewer(nullptr, current_model, current_account, current_trans, slave_addr));
    moh_viewers[moh_counter]->show();

//    for (auto const moh : moh_viewers)
//        qDebug() << __func__ << moh;

    moh_viewers[moh_counter]->m_serial->setSlaveAddr(slave_addr);
//    moh_viewers[moh_counter]->device_log_widget->change_slave_addr(slave_addr);

//    moh_viewers[moh_counter]->on_serialConnected();

//    if (m_serialPrivate->client->state() == QModbusDevice::ConnectedState)
//        moh_viewers[moh_counter]->m_serial->set_serial_connec_state(true);

    connect(m_serialPrivate, &ModbusSerialPrivate::serial_connected, moh_viewers[moh_counter], &MOH_Viewer::on_serialConnected);
    connect(m_serialPrivate, &ModbusSerialPrivate::serial_disconnected, moh_viewers[moh_counter], &MOH_Viewer::on_serialDisconnected);

    connect(m_serialPrivate, &ModbusSerialPrivate::set_serial_state, moh_viewers[moh_counter]->m_serial, &ModbusSerial::set_serial_state);
    connect(m_serialPrivate, &ModbusSerialPrivate::set_write_state, moh_viewers[moh_counter]->m_serial, &ModbusSerial::set_write_state);

    connect(m_serialPrivate, &ModbusSerialPrivate::set_serial_connec_state, moh_viewers[moh_counter]->m_serial, &ModbusSerial::set_serial_connec_state);

    connect(moh_viewers[moh_counter]->m_serial, &ModbusSerial::actual_read_req, m_serialPrivate, &ModbusSerialPrivate::do_the_actual_read);
    connect(moh_viewers[moh_counter]->m_serial, &ModbusSerial::actual_write_req, m_serialPrivate, &ModbusSerialPrivate::do_the_actual_write);

    connect(m_serialPrivate, &ModbusSerialPrivate::communicationRecord, moh_viewers[moh_counter]->device_log_widget->communicationLogs, &CommunicationLogs::addCommunicationRecord);

    connect(moh_viewers[moh_counter], &MOH_Viewer::warningRecord,
            moh_viewers[moh_counter]->device_log_widget->warningLogs, &WarningLogs::addWarningRecord);
    connect(moh_viewers[moh_counter], &MOH_Viewer::operationRecord,
            moh_viewers[moh_counter]->device_log_widget->operationLogs, &OperationLogs::addOperationRecord);

    connect(moh_viewers[moh_counter]->para_conf, &ParameterConfiguration::refresh_timeout_counter, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_refresh_timeout_counter);
    connect(moh_viewers[moh_counter]->device_status_widget, &DeviceStatus::refresh_timeout_counter, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_refresh_timeout_counter);
    connect(moh_viewers[moh_counter]->control_panel_widget, &ControlPanel::refresh_timeout_counter, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_refresh_timeout_counter);
    connect(moh_viewers[moh_counter], &MOH_Viewer::refresh_timeout_counter, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_refresh_timeout_counter);

    connect(moh_viewers[moh_counter]->m_serial, &ModbusSerial::modbusErrorHappened, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[moh_counter]->para_conf, &ParameterConfiguration::modbusErrorHappened, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[moh_counter]->device_status_widget, &DeviceStatus::modbusErrorHappened, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[moh_counter]->control_panel_widget, &ControlPanel::modbusErrorHappened, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_errorHappened);
    connect(moh_viewers[moh_counter], &MOH_Viewer::modbusErrorHappened, moh_viewers[moh_counter]->sys_setting, &SystemSetting::on_errorHappened);

    emit m_serialPrivate->set_serial_connec_state(true);
    emit m_serialPrivate->serial_connected();
//    moh_viewers[moh_counter]->m_serial->

    moh_viewers[moh_counter]->m_serial->setSerialParameters(QModbusDevice::SerialPortNameParameter, moh_viewers[0]->m_serial->settings().portname);
    moh_viewers[moh_counter]->set_setting_disabled();

//    connect(moh_viewers[0]->sys_setting, &SystemSetting::connect_to_serial, this, [=](const ModbusSerial::Settings settings) {
//        m_serialPrivate->insert_slave_addr(settings.slave_addr);
//    });

    m_serialPrivate->insert_slave_addr(moh_viewers[moh_counter]->m_serial->settings().slave_addr);

    m_serialPrivate->start_timeout_counter(slave_addr);
    connect(m_serialPrivate->timeout_timers[slave_addr], &QTimer::timeout, moh_viewers[moh_counter], &MOH_Viewer::on_serialDisconnected);

    connect(moh_viewers[moh_counter], &MOH_Viewer::resume_timer, m_serialPrivate, &ModbusSerialPrivate::on_resume_timer);
    connect(moh_viewers[moh_counter], &MOH_Viewer::stop_timer, m_serialPrivate, &ModbusSerialPrivate::on_stop_timer);

    for (auto moh : moh_viewers)
    {
        moh->multiple_moh = true;
        moh->m_serial->multiple_moh = true;
    }

    moh_counter++;
}

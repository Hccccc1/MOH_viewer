#ifndef MOH_VIEWER_H
#define MOH_VIEWER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include "ControlPanel/controlpanel.h"
#include "DeviceLog/devicelog.h"
#include "DeviceStatus/devicestatus.h"
#include "LoginInterface/logininterface.h"
#include "Modbus/modbusserial.h"
#include "ParameterConfiguration/parameterconfiguration.h"
#include "SystemSetting/systemsetting.h"
#include "MOH_viewer/warningsound.h"
#include "AllBitsAndRegs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MOH_viewer; }
QT_END_NAMESPACE

enum PERMISSION_LEVEL
{
    Lowest = 0,
    Mid,
    Highest,
};

class MOH_viewer : public QMainWindow
{
    Q_OBJECT

public:
//    struct DevParas
//    {
//        quint16 firmwareVersion;
//    };

    MOH_viewer(QWidget *parent = nullptr, uint8_t model = 0, Accounts account = Customer, QTranslator *trans = nullptr);
    ~MOH_viewer();

    ControlPanel *control_panel_widget = nullptr;
    DeviceLog *device_log_widget = nullptr;
    DeviceStatus *device_status_widget = nullptr;
    ParameterConfiguration *para_conf = nullptr;
    SystemSetting *sys_setting = nullptr;

public slots:
    void onReadyRead();
//    void showRealTimeValue(QString);

private:
    Ui::MOH_viewer *ui;

    uint8_t current_model;
    Accounts current_account;

    QString selfcheck_default_status = "QLabel{min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(255,255,255,1);border:1px solid rgba(112,112,112,1);}";
    QString selfcheck_ok_status = "QLabel {min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(81,223,0,1);}";
    QString selfcheck_malfunction_status = "QLabel {min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(255,42,42,1);}";

    QString status_on = "QLabel {min-width:14px;min-height:14px;max-width:14px;max-height:14px;border-radius:7px;background:rgba(81,223,0,1);}";
    QString status_off = "QLabel {min-width:14px;min-height:14px;max-width:14px;max-height:14px;border-radius:7px;background:rgba(255,42,42,1);}";

    ModbusSerial *_modbus = nullptr;
    WarningSound *sound_thread = new WarningSound();
    QTranslator *current_trans = nullptr;

//    QSound *sound_warning = new QSound(":/Smoke_Alarm.wav");

    QTimer *refresh_timer = new QTimer(this);

private slots:

    void on_bootBtn_clicked();
    void on_shutdownBtn_clicked();
    void on_runBtn_clicked();
//    void on_stopBtn_clicked();
    void on_emergencyStopBtn_clicked();
    void on_restoreBtn_clicked();
    void on_selfcheckBtn_clicked();

//    void on_selfcheck_btn_clicked();

    void on_controlMode_combobox_currentIndexChanged(int index);
    void on_generateMode_combobox_currentIndexChanged(int index);

    void on_globalSetting_btn_clicked();

    void on_serialConnected();
    void on_serialDisconnected();

    void refreshWarningMsg();
    void refreshRealTimeValues();
    void refreshCurrentPage();

    void on_mainWidget_currentChanged(int index);

    void start_refresh_timer();
    void stop_refresh_timer();

    void show_upgradeWidget();

//    void on_warningInfo_clicked();

protected:
//    void showEvent(QShowEvent *event);
    void changeEvent(QEvent *);
//    virtual void timerEvent(QTimerEvent *);
    virtual void resizeEvent(QResizeEvent *event);

    virtual void closeEvent(QCloseEvent *);

Q_SIGNALS:
    void communicationRecord(QString, QString);
    void warningRecord(QString, QString);
    void operationRecord(QString, Accounts);
    void modbusErrorHappened(QModbusDevice::Error);

    void warning_msg(WarningType);

    //控制板可以进入boot信号
//    void boot_ready(bool);
};
#endif // MOH_VIEWER_H

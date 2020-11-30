#ifndef MOH_VIEWER_H
#define MOH_VIEWER_H

#include <QTranslator>
#include <QMainWindow>

#include "AllBitsAndRegs.h"
#include "DeviceStatus/devicestatus.h"
#include "SystemSetting/systemsetting.h"
#include "ControlPanel/controlpanel.h"
#include "ParameterConfiguration/parameterconfiguration.h"
#include "MOH_Viewer/warningsound.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ModbusSerial; class MOH_viewer; }
QT_END_NAMESPACE

class MOH_Viewer : public QMainWindow
{
    Q_OBJECT

public:
    bool multiple_moh = false;

    MOH_Viewer(QWidget *parent = nullptr, uint8_t model = 0, Accounts account = Customer, QTranslator* trans = nullptr, int slave_addr = 0x01);
    ~MOH_Viewer();

    ControlPanel *control_panel_widget = nullptr;
    DeviceStatus *device_status_widget = nullptr;
    ParameterConfiguration *para_conf = nullptr;
    DeviceLog *device_log_widget = nullptr;

    ModbusSerial *m_serial = nullptr;
    SystemSetting *sys_setting = nullptr;

    void set_setting_disabled();

public slots:
    void on_serialConnected();
    void on_serialDisconnected();

//    void request_finished();

//    void onReadyRead(const QModbusDataUnit&);
    void onReadyRead();

private:
    Ui::MOH_viewer *ui;

    uint8_t current_model;
    Accounts current_account;

    int m_slave_addr = 0x01;

    QString selfcheck_default_status = "QLabel{min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(255,255,255,1);border:1px solid rgba(112,112,112,1);}";
    QString selfcheck_ok_status = "QLabel {min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(81,223,0,1);}";
    QString selfcheck_malfunction_status = "QLabel {min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(255,42,42,1);}";

    QString status_on = "QLabel {min-width:14px;min-height:14px;max-width:14px;max-height:14px;border-radius:7px;background:rgba(81,223,0,1);}";
    QString status_off = "QLabel {min-width:14px;min-height:14px;max-width:14px;max-height:14px;border-radius:7px;background:rgba(255,42,42,1);}";

    QString warningRed = "QLabel {min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:transparent;background:rgba(255,255,255,1);background:rgba(255,42,42,1);}";
    QString warningWhite = "QLabel {min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:transparent;background:rgba(255,255,255,1);background:rgba(255,255,255,1);}";

    QTranslator *current_trans = nullptr;
    WarningSound *sound_thread = new WarningSound(this);

    quint16 warningMsg = 0x0;

    QTimer *refresh_timer = new QTimer(this);

    quint8 text_counter;
    QMap<quint8, QString> msg_show;
    QVector<QString> msg_selfdet;

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
    void on_autoCharge_combobox_currentIndexChanged(int index);
    void on_generateMode_combobox_currentIndexChanged(int index);

    void on_globalSetting_btn_clicked();

    void refreshWarningMsg();
    void refreshRealTimeValues();
    void refreshCurrentPage();

    void on_mainWidget_currentChanged(int index);

    void start_refresh_timer();
    void stop_refresh_timer();

    void show_upgradeWidget(const QString& portname, const int& baudrate);

    void changeBlinkState(bool);
    void changeWarningText();

    void on_warningInfo_clicked();

protected:
    virtual void changeEvent(QEvent *);
    //    virtual void timerEvent(QTimerEvent *);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *);

Q_SIGNALS:
    void communicationRecord(QString, QString);
    void warningRecord(QString, QString);
    void operationRecord(QString, Accounts);
    void modbusErrorHappened(QModbusDevice::Error);

    void warning_msg(WarningType);
    void warning_dissmissed(WarningType);

    void stop_timer(int slave_addr);
    void resume_timer(int slave_addr);

    void refresh_timeout_counter();
};
#endif // MOH_VIEWER_H

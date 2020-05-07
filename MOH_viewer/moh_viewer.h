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
    struct DevParas
    {
        quint16 firmwareVersion;
    };

    MOH_viewer(QWidget *parent = nullptr, uint8_t model = 0);
    ~MOH_viewer();

    ControlPanel *control_panel_widget = nullptr;
    DeviceLog *device_log_widget = nullptr;
    DeviceStatus *device_status_widget = nullptr;
    ParameterConfiguration *para_conf = nullptr;
    SystemSetting *sys_setting = nullptr;

public slots:
    void onReadyRead();
    void showRealTimeValue(QString);

private:
    Ui::MOH_viewer *ui;

    QString powerCtrl_button_on = "QPushButton {width: 93px;height:43px;border:0px;image: url(:/switch_on.png);}";
    QString powerCtrl_button_off = "QPushButton {width: 93px;height:43px;border:0px;image: url(:/switch_off.png);}";

    QString run_button = "QPushButton {width: 44px;height:44px;border:0px;image: url(:/run_btn.png);}";
    QString stop_button = "QPushButton {width: 44px;height:44px;border:0px;image: url(:/stop_button.png);}";

    QString emergency_stop_button = "QPushButton {width: 44px;height:44px;border:0px;image: url(:/emergency_stop_light.png);}";

    QString restore_button = "QPushButton{width:52px;height:52px;border:0px;image:url(:/restore_btn_light.png);}";

    QString selfcheck_default_status = "QLabel{min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(255,255,255,1);border:1px solid rgba(112,112,112,1);}";
    QString selfcheck_ok_status = "QLabel {min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(81,223,0,1);}";
    QString selfcheck_malfunction_status = "QLabel {min-width:18px;min-height:18px;max-width:18px;max-height:18px;border-radius:9px;background:rgba(255,42,42,1);}";

    QString status_on = "QLabel {min-width:14px;min-height:14px;max-width:14px;max-height:14px;border-radius:7px;background:rgba(81,223,0,1);}";
    QString status_off = "QLabel {min-width:14px;min-height:14px;max-width:14px;max-height:14px;border-radius:7px;background:rgba(255,42,42,1);}";

    bool start_status = false;            //indicates running or not
    bool running_status = false;           //indicates running or not

    uint8_t current_model;

    ModbusSerial *_modbus = new ModbusSerial(this);

    void set_stylesheet_to_default();
    void set_stylesheets(bool);

private slots:
    void on_powerCtrl_btn_clicked();
    void on_run_btn_clicked();
    void on_emergency_stop_clicked();
    void on_restore_btn_clicked();

    void on_controlMode_combobox_currentIndexChanged(int index);
    void on_generateMode_combobox_currentIndexChanged(int index);

    void on_selfcheck_btn_clicked();

    void on_globalSetting_btn_clicked();

    void on_serialConnected();

    void refreshCurrentPage();

    void on_mainWidget_currentChanged(int index);

protected:
//    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
    void warningRecord(QString, QString);

};
#endif // MOH_VIEWER_H

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
    MOH_viewer(QWidget *parent = nullptr, uint8_t model = 0);
    ~MOH_viewer();

    ControlPanel *control_panel_widget = nullptr;
    DeviceLog *device_log_widget = nullptr;
    DeviceStatus *device_status_widget = nullptr;
    ParameterConfiguration *para_conf = nullptr;
    SystemSetting *sys_setting = nullptr;

private:
    Ui::MOH_viewer *ui;

    bool running_status = false;            //indicate running or not

    uint8_t current_model;

    ModbusSerial *_modbus = new ModbusSerial();

private slots:
    void on_powerCtrl_btn_clicked();
    void on_run_btn_clicked();
    void on_emergency_stop_clicked();
    void on_restore_btn_clicked();

    void on_controlMode_combobox_currentIndexChanged();
    void on_generateMode_combobox_currentIndexChanged();

protected:
//    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

};
#endif // MOH_VIEWER_H

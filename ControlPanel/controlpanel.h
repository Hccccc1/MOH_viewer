#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QDebug>
#include <QWidget>
#include <QTimer>
#include <Modbus/modbusserial.h>
#include <QMessageBox>
#include "3rdparty/QRoundProgressBar/QRoundProgressBar.h"

#include "AllBitsAndRegs.h"

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, uint8_t model = 0, Accounts account = Customer);
    ~ControlPanel();

    void start_refresh_timer(int sec);
    void stop_refresh_timer();
    void refreshCurrentPage();

public slots:
    void onReadyRead();

private:
    struct IOControls
    {
        quint16 io_ctrl_enable = 0;
        quint16 io_ctrl = 0;
        quint16 output_status_feedback = 0;
    }Q_PACKED;

    struct SpeedControls
    {
        bool auto_control = false;
        quint16 speed_percentage = 0;
        quint16 speed_rpm = 0;
    }Q_PACKED;

    Ui::ControlPanel *ui;

    QString square_off_label = "QPushButton{width: 90px;height:28px;border:0px;background-image: url(:/square_off.png);}";
    QString square_on_label = "QPushButton{width: 90px;height:28px;border:0px;background-image: url(:/square_on.png);}";
    QString red_round_circle = "QLabel{min-width: 28px;min-height:28px;max-width:28px;max-height:28px;border-radius:14px;background:rgba(255,42,42,1);border:1px solid rgba(112,112,112,1);}";
    QString green_round_circle = "QLabel{min-width: 28px;min-height:28px;max-width:28px;max-height:28px;border-radius:14px;background:rgba(81,223,0,1);border:1px solid rgba(112,112,112,1);}";
    QString switch_on_label = "QPushButton{border:0px;image:url(:/switch_on.png);}";
    QString switch_off_label = "QPushButton{border:0px;image:url(:/switch_off.png);}";

    ModbusSerial *current_serial;
    uint8_t current_model;
    Accounts current_account;

    //0~13: SV01-SV14 14~17:BL01-BL04 18~22:PMP01-PMP05 23:RAD01 24:KM01
    IOControls io_controls[25];
    SpeedControls speed_controls[10];

    QTimer *refresh_timer = nullptr;

//    QVector<QPushButton *> button;

private slots:

    void onValueChanged(double value);

    void on_autoControl_1_clicked();
    void on_autoControl_2_clicked();
    void on_autoControl_3_clicked();
    void on_autoControl_4_clicked();
    void on_autoControl_5_clicked();
    void on_autoControl_6_clicked();
    void on_autoControl_7_clicked();
    void on_autoControl_8_clicked();
    void on_autoControl_9_clicked();
    void on_autoControl_10_clicked();

    void on_IOCtrl_SV_1_clicked();
    void on_IOCtrl_SV_2_clicked();
    void on_IOCtrl_SV_3_clicked();
    void on_IOCtrl_SV_4_clicked();
    void on_IOCtrl_SV_5_clicked();
    void on_IOCtrl_SV_6_clicked();
    void on_IOCtrl_SV_7_clicked();
    void on_IOCtrl_SV_8_clicked();
    void on_IOCtrl_SV_9_clicked();
    void on_IOCtrl_SV_10_clicked();
    void on_IOCtrl_SV_11_clicked();
    void on_IOCtrl_SV_12_clicked();
    void on_IOCtrl_SV_13_clicked();
    void on_IOCtrl_SV_14_clicked();

    void on_IOCtrl_BL_1_clicked();
    void on_IOCtrl_BL_2_clicked();
    void on_IOCtrl_BL_3_clicked();
    void on_IOCtrl_BL_4_clicked();

    void on_IOCtrl_PMP_1_clicked();
    void on_IOCtrl_PMP_2_clicked();
    void on_IOCtrl_PMP_3_clicked();
    void on_IOCtrl_PMP_4_clicked();
    void on_IOCtrl_PMP_5_clicked();

    void on_IOCtrl_RAD_1_clicked();
    void on_IOCtrl_KM_1_clicked();

    void on_IOCtrlEnable_SV_1_clicked(bool state);
    void on_IOCtrlEnable_SV_2_clicked(bool state);
    void on_IOCtrlEnable_SV_3_clicked(bool state);
    void on_IOCtrlEnable_SV_4_clicked(bool state);
    void on_IOCtrlEnable_SV_5_clicked(bool state);
    void on_IOCtrlEnable_SV_6_clicked(bool state);
    void on_IOCtrlEnable_SV_7_clicked(bool state);
    void on_IOCtrlEnable_SV_8_clicked(bool state);
    void on_IOCtrlEnable_SV_9_clicked(bool state);
    void on_IOCtrlEnable_SV_10_clicked(bool state);
    void on_IOCtrlEnable_SV_11_clicked(bool state);
    void on_IOCtrlEnable_SV_12_clicked(bool state);
    void on_IOCtrlEnable_SV_13_clicked(bool state);
    void on_IOCtrlEnable_SV_14_clicked(bool state);
    void on_IOCtrlEnable_BL_1_clicked(bool state);
    void on_IOCtrlEnable_BL_2_clicked(bool state);
    void on_IOCtrlEnable_BL_3_clicked(bool state);
    void on_IOCtrlEnable_BL_4_clicked(bool state);
    void on_IOCtrlEnable_PMP_1_clicked(bool state);
    void on_IOCtrlEnable_PMP_2_clicked(bool state);
    void on_IOCtrlEnable_PMP_3_clicked(bool state);
    void on_IOCtrlEnable_PMP_4_clicked(bool state);
    void on_IOCtrlEnable_PMP_5_clicked(bool state);
    void on_IOCtrlEnable_RAD_1_clicked(bool state);
    void on_IOCtrlEnable_KM_1_clicked(bool state);

    void time_elapsed();

//    void on_speedControl_1_editingFinished();
//    void on_speedControl_2_editingFinished();
//    void on_speedControl_3_editingFinished();
//    void on_speedControl_4_editingFinished();
//    void on_speedControl_5_editingFinished();
//    void on_speedControl_6_editingFinished();
//    void on_speedControl_7_editingFinished();
//    void on_speedControl_8_editingFinished();
//    void on_speedControl_9_editingFinished();
//    void on_speedControl_10_editingFinished();


protected:
    virtual void timerEvent(QTimerEvent *);

Q_SIGNALS:
    void modbusErrorHappened(QModbusDevice::Error);

};

#endif // CONTROLPANEL_H

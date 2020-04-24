#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QDebug>
#include <QWidget>
#include <Modbus/modbusserial.h>
#include <QMessageBox>
#include "QRoundProgressBar/QRoundProgressBar.h"

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, uint8_t model = 0);
    ~ControlPanel();

public slots:
    void onReadyRead();

private:
    struct IOControls
    {
        quint16 io_ctrl_enable;
        quint16 io_ctrl;
        quint16 output_status_feedback;
    }Q_PACKED;

    struct SpeedControls
    {
        bool auto_control = false;
        quint16 speed_percentage;
        quint16 speed_rpm;
    }Q_PACKED;

    Ui::ControlPanel *ui;

    QString square_off_label = "QPushButton{width: 90px;height:28px;border:0px;background-image: url(:/square_off.png);}";
    QString square_on_label = "QPushButton {width: 90px;height:28px;border:0px;background-image: url(:/square_on.png);}";
    QString red_round_circle = "QLabel{min-width: 28px;min-height:28px;max-width:28px;max-height:28px;border-radius:14px;background:rgba(255,42,42,1);border:1px solid rgba(112,112,112,1);}";
    QString green_round_circle = "QLabel{QLabel {min-width: 28px;min-height:28px;max-width:28px;max-height:28px;border-radius:14px;background:rgba(81,223,0,1);border:1px solid rgba(112,112,112,1);}}";
    QString switch_on_label = "QPushButton{border:0px;image:url(:/switch_on.png);}";
    QString switch_off_label = "QPushButton{border:0px;image:url(:/switch_off.png);}";

    ModbusSerial *current_serial;
    uint8_t current_model;

    //0~13: SV01-SV14 14~17:BL01-BL04 18~22:PMP01-PMP05 23:RAD01 24:KM01
    IOControls io_controls[25];
    SpeedControls speed_controls[10];

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
};

#endif // CONTROLPANEL_H

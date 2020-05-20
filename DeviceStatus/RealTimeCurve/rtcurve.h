#ifndef RTCURVE_H
#define RTCURVE_H

#include <QWidget>
#include <QModbusDataUnit>
#include "Modbus/modbusserial.h"
#include "DeviceStatus/DevStatus_regs.h"
#include "AllBitsAndRegs.h"

namespace Ui {
class RTCurve;
}

class RTCurve : public QWidget
{
    Q_OBJECT

public:
    explicit RTCurve(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, Accounts account = SuperUser);
    ~RTCurve();

    void data_process(QModbusDataUnit unit);

private:
    Ui::RTCurve *ui;
    Accounts current_account;
    ModbusSerial *current_serial = nullptr;

    void set_widgets_size();
    void setup_charts_checkboxes(DisplayGroups group);

private slots:
    void on_tabWidget_currentChanged(int index);

protected:
    virtual void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
    void dataChanged(QString);
};

#endif // RTCURVE_H

#ifndef RTCURVE_H
#define RTCURVE_H

#include <QWidget>
#include <QModbusDataUnit>
#include "Modbus/modbusserial.h"
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

private slots:
    void on_tabWidget_currentChanged(int index);

Q_SIGNALS:
    void dataChanged(QString);
};

#endif // RTCURVE_H

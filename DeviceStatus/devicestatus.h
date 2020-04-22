#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include <QWidget>
#include "Modbus/modbusserial.h"
#include "DataOverview/dataoverview.h"
#include "RealTimeCurve/rtcurve.h"
#include "HistoryCurve/hiscurve.h"
#include "RealTimeValues/rtvalues.h"

namespace Ui {
class DeviceStatus;
}

class DeviceStatus : public QWidget
{
    Q_OBJECT

public:

    explicit DeviceStatus(QWidget *parent = nullptr, ModbusSerial *serial = nullptr,uint8_t model = 0);
    ~DeviceStatus();

public slots:
    void onReadyRead();

private:
    Ui::DeviceStatus *ui;

    ModbusSerial *current_serial;
    uint8_t current_model;

    DataOverview *dataOverview;
    RTCurve *rtCurve;
    HisCurve *hisCurve;
    RTValues *realTimeValues;

private slots:
//    void on_readBtn_clicked();
//    void index_changed();
};

#endif // DEVICESTATUS_H

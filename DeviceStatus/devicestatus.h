#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include <QWidget>
#include "Modbus/modbusserial.h"
#include "DataOverview/dataoverview.h"
#include "RealTimeCurve/rtcurve.h"
#include "RealTimeCurve/customerrtcurve.h"
#include "HistoryCurve/hiscurve.h"
#include "HistoryCurve/customer_historycurve.h"
#include "DeviceStatus/RealTimeValues/rtvalues.h"
#include "AllBitsAndRegs.h"

namespace Ui {
class DeviceStatus;
}

class DeviceStatus : public QWidget
{
    Q_OBJECT

public:

    explicit DeviceStatus(QWidget *parent = nullptr, ModbusSerial *serial = nullptr,uint8_t model = 0, Accounts account = Customer);
    ~DeviceStatus();

    DataOverview *dataOverview = nullptr;

    CustomerRTCurve *customer_rtCurve = nullptr;
    RTCurve *rtCurve = nullptr;

    customer_HistoryCurve *customer_hisCurve = nullptr;
    HisCurve *hisCurve = nullptr;

    RTValues *realTimeValues = nullptr;

public slots:
    void onReadyRead();

private:
    Ui::DeviceStatus *ui;

    ModbusSerial *current_serial;
    uint8_t current_model;
    Accounts current_account;

private slots:
//    void on_readBtn_clicked();
//    void index_changed();

    void on_tabWidget_currentChanged(int index);

Q_SIGNALS:
    void modbusErrorHappened(QModbusDevice::Error);
};

#endif // DEVICESTATUS_H

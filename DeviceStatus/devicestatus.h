#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include <QWidget>
#include "ModbusSerial/modbusserial.h"
#include "DataOverview/dataoverview.h"
#include "RealTimeCurve/rtcurve.h"
#include "RealTimeCurve/customerrtcurve.h"
#include "HistoryCurve/hiscurve.h"
#include "HistoryCurve/customer_historycurve.h"
#include "DeviceStatus/RealTimeValues/rtvalues.h"
#include "AllBitsAndRegs.h"
#include "DeviceLog/devicelog.h"

namespace Ui {
class DeviceStatus;
}

class DeviceStatus : public QWidget
{
    Q_OBJECT

public:

    explicit DeviceStatus(QWidget *parent = nullptr,
                          ModbusSerial *serial = nullptr,
                          uint8_t model = 0,
                          Accounts account = Customer,
                          DeviceLog *log_handler = nullptr);
    ~DeviceStatus();

    DataOverview *dataOverview = nullptr;

    CustomerRTCurve *customer_rtCurve = nullptr;
    RTCurve *rtCurve = nullptr;

    customer_HistoryCurve *customer_hisCurve = nullptr;
    HisCurve *hisCurve = nullptr;

    RTValues *realTimeValues = nullptr;

public slots:
//    void onReadyRead(const QModbusDataUnit&);
    void onReadyRead();

private:
    Ui::DeviceStatus *ui;

    ModbusSerial *current_serial;
    uint8_t current_model;
    Accounts current_account;
    DeviceLog *current_log_handler;

private slots:
//    void on_readBtn_clicked();
//    void index_changed();

    void on_tabWidget_currentChanged(int index);

protected:
    virtual void changeEvent(QEvent *);

Q_SIGNALS:
    void communicationRecord(QString, QString);

    void refresh_timeout_counter();
    void modbusErrorHappened(QModbusDevice::Error);
};

#endif // DEVICESTATUS_H

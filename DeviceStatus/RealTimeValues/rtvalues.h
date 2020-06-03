#ifndef RTVALUES_H
#define RTVALUES_H

#include <QDialog>
#include <QTimer>
#include <QModbusDataUnit>
#include "Modbus/modbusserial.h"
#include "AllBitsAndRegs.h"

namespace Ui {
class RTValues;
}

class RTValues : public QDialog
{
    Q_OBJECT

public:
    explicit RTValues(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, Accounts account = Customer);
    ~RTValues();

    void data_process(const QModbusDataUnit unit);
    void refreshCurrentPage();

private:
    Ui::RTValues *ui;

    ModbusSerial *current_serial = nullptr;
    Accounts current_account;

    QTimer *refresh_timer = nullptr;

    void start_refresh_timer(int sec);
    void stop_refresh_timer();

private slots:
    void time_elapsed();
};

#endif // RTVALUES_H

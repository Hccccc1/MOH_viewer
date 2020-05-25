#ifndef RTVALUES_H
#define RTVALUES_H

#include <QDialog>
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
};

#endif // RTVALUES_H

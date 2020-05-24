#ifndef PARAMETERCONFIGURATION_H
#define PARAMETERCONFIGURATION_H

#include <QDebug>
#include <QWidget>
#include <Modbus/modbusserial.h>

#include "AllBitsAndRegs.h"

namespace Ui {
class ParameterConfiguration;
}

class ParameterConfiguration : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterConfiguration(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, uint8_t model = 0, Accounts account = Customer);
    ~ParameterConfiguration();

    void refreshCurrentPage();

public slots:
    void onReadyRead();

private:
    struct Parameters
    {
        quint16 manufactor;
        quint16 product_name;
        quint16 product_type;
        quint16 product_serial_num[4];
        quint16 product_date[2];

//        quint16 firmware_version;
//        quint16 hardware_version;
        quint16 reforming_ID[4];
        quint16 stack_ID[4];
        quint16 dev_slave_addr;
        quint16 dev_IP_addr[2];

//        QSerialPort::DataBits databits;
//        QSerialPort::Parity parity;
//        QSerialPort::StopBits stopbits;
//        QSerialPort::BaudRate baud;

        quint16 serial_paras;

        quint16 power_mode;

        quint16 fc_output_current;
        quint16 fc_output_power;

        quint16 bat_charge_start_voltage;
        quint16 charge_start_delay;
        quint16 bat_charge_stop_voltage;
        quint16 charge_stop_delay;

        quint16 sd_storage_delay;

        quint16 low_pressure_pt03;
        quint16 high_pressure_pt03;
        quint16 high_pressure_pt04;
        quint16 high_temperature_tt17;
        quint16 high_temperature_tt31;
        quint16 high_conductivity;
        quint16 low_voltage_bat01;
        quint16 low_level_lt1;
        quint16 auto_liquid_low_limit_lt1;
        quint16 stop_liquid_limit_lt1;
        quint16 low_level_lt2;
    }Q_PACKED;

    struct RunningParameters {
        quint16 kp;
        quint16 ti;
        quint16 tsm;
    };

    Ui::ParameterConfiguration *ui;

    ModbusSerial *current_serial;
    uint8_t current_model;
    Accounts current_account;

    Parameters m_parameters;
    //0~3: BL01-BL04 4~8: PMP01-PMP05 9: RAD01
    RunningParameters running_para[10];

    void displayData();

private slots:
//    void on_readData_clicked();
    void on_saveToFile_clicked();
    void on_loadFromFile_clicked();
    void on_sendToLower_clicked();

    void on_Kp_BL01_editingFinished();
    void on_Ti_BL01_editingFinished();
    void on_Tsm_BL01_editingFinished();

    void on_Kp_BL02_editingFinished();
    void on_Ti_BL02_editingFinished();
    void on_Tsm_BL02_editingFinished();

    void on_Kp_BL03_editingFinished();
    void on_Ti_BL03_editingFinished();
    void on_Tsm_BL03_editingFinished();

    void on_Kp_BL04_editingFinished();
    void on_Ti_BL04_editingFinished();
    void on_Tsm_BL04_editingFinished();

    void on_Kp_PMP01_editingFinished();
    void on_Ti_PMP01_editingFinished();
    void on_Tsm_PMP01_editingFinished();

    void on_Kp_PMP02_editingFinished();
    void on_Ti_PMP02_editingFinished();
    void on_Tsm_PMP02_editingFinished();

    void on_Kp_PMP03_editingFinished();
    void on_Ti_PMP03_editingFinished();
    void on_Tsm_PMP03_editingFinished();

    void on_Kp_PMP04_editingFinished();
    void on_Ti_PMP04_editingFinished();
    void on_Tsm_PMP04_editingFinished();

    void on_Kp_PMP05_editingFinished();
    void on_Ti_PMP05_editingFinished();
    void on_Tsm_PMP05_editingFinished();

    void on_Kp_RAD01_editingFinished();
    void on_Ti_RAD01_editingFinished();
    void on_Tsm_RAD01_editingFinished();

    void on_lowPressure_PT03_editingFinished();
    void on_highPressure_PT03_editingFinished();
    void on_highPressure_PT04_editingFinished();
    void on_highTemperature_TT17_editingFinished();
    void on_highTemperature_TT31_editingFinished();
    void on_highConductivity_CS01_editingFinished();
    void on_lowVoltage_BAT01_editingFinished();
    void on_lowLevel_LT01_editingFinished();
    void on_autoLiquidLowLimit_LT01_editingFinished();
    void on_stopLiquidValue_LT01_editingFinished();
    void on_lowLevel_LT02_editingFinished();

Q_SIGNALS:
    void modbusErrorHappened(QModbusDevice::Error);
};

#endif // PARAMETERCONFIGURATION_H


#ifndef PARAMETERCONFIGURATION_H
#define PARAMETERCONFIGURATION_H

#include <QDebug>
#include <QWidget>
#include <Modbus/modbusserial.h>

namespace Ui {
class ParameterConfiguration;
}

class ParameterConfiguration : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterConfiguration(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, uint8_t model = 0);
    ~ParameterConfiguration();

public slots:
    void onReadyRead();

private:
    struct Parameters
    {
        QString manufactor;
        quint16 product_name;
        quint16 product_type;
        QString product_serial_num;
        QString product_date;

        QString firmware_version;
        QString hardware_version;
        QString reforming_ID;
        QString stack_ID;
        quint16 dev_communication_addr;
        quint16 dev_slave_addr;

        QSerialPort::DataBits databits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopbits;
        QSerialPort::BaudRate baud;

        quint16 power_mode;

        quint16 kp_bl01;
        quint16 ti_bl01;
        quint16 tsm_bl01;
        quint16 kp_bl02;
        quint16 ti_bl02;
        quint16 tsm_bl02;
        quint16 kp_bl03;
        quint16 ti_bl03;
        quint16 tsm_bl03;
        quint16 kp_bl04;
        quint16 ti_bl04;
        quint16 tsm_bl04;
        quint16 kp_pmp01;
        quint16 ti_pmp01;
        quint16 tsm_pmp01;
        quint16 kp_pmp02;
        quint16 ti_pmp02;
        quint16 tsm_pmp02;
        quint16 kp_pmp03;
        quint16 ti_pmp03;
        quint16 tsm_pmp03;
        quint16 kp_pmp04;
        quint16 ti_pmp04;
        quint16 tsm_pmp04;
        quint16 kp_pmp05;
        quint16 ti_pmp05;
        quint16 tsm_pmp05;
        quint16 kp_rad01;
        quint16 ti_rad01;
        quint16 tsm_rad01;
    }Q_PACKED;

    Ui::ParameterConfiguration *ui;

    ModbusSerial *current_serial;
    uint8_t current_model;

    Parameters m_parameters;

private slots:
    void on_readData_clicked();
};

#endif // PARAMETERCONFIGURATION_H

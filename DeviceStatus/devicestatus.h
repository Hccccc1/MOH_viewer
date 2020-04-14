#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include <QWidget>
#include <Modbus/modbusserial.h>

namespace Ui {
class DeviceStatus;
}

class DeviceStatus : public QWidget
{
    Q_OBJECT

public:
    struct SystemStatus
    {
        quint16 sys_stat;                       //系统状态
        quint16 fueltank_level_1;               //燃料箱液位1
        quint16 fueltank_level_2;               //燃料箱液位2
        quint16 fuelbat_voltage;                //燃料电池电压
        quint16 fuelbat_current;                //燃料电池电流
        quint16 fuelbat_generateing_power;      //燃料电池发电功率
        quint16 sys_output_voltage;             //系统输出电压
        quint16 sys_output_current;             //系统输出电流
        quint16 sys_output_power;               //系统输出功率
        quint16 bat_voltage;                    //电池电压
        quint16 bat_current;                    //电池电流
        quint32 single_power_produced;          //单次发电量
        quint32 total_power_produced;             //累计发电量
        quint16 fuel_consumption_rate;          //燃料消耗率
        quint32 total_fuel_consumption;         //燃料累计消耗量
    }Q_PACKED;

    explicit DeviceStatus(QWidget *parent = nullptr, ModbusSerial *current_serial = nullptr,uint8_t model = 0);
    ~DeviceStatus();

public slots:
    void onReadyRead();

private:
    Ui::DeviceStatus *ui;

    ModbusSerial *serial;
    uint8_t current_model;

    SystemStatus sys_status;

private slots:
//    void on_readBtn_clicked();
};

#endif // DEVICESTATUS_H

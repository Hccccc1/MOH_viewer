#ifndef DATAOVERVIEW_H
#define DATAOVERVIEW_H

#include <QDialog>
#include <QLabel>
#include <QModbusDataUnit>
#include "AllBitsAndRegs.h"
#include "Modbus/modbusserial.h"

namespace Ui {
class DataOverview;
}

class DataOverview : public QDialog
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
        quint16 total_boot_times;               //累计启动次数
        quint16 fuel_consumption_rate;          //燃料消耗率
        quint32 total_fuel_consumption;         //燃料累计消耗量

        quint16 sys_year;
        quint16 sys_month;
        quint16 sys_date;
        quint16 sys_hour;
        quint16 sys_min;
        quint16 sys_sec;
        quint16 sys_single_hour;
        quint16 sys_single_min;
        quint16 sys_single_sec;
        quint16 sys_total_hour;
        quint16 sys_total_min;
        quint16 sys_total_sec;
    }Q_PACKED;

    explicit DataOverview(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, Accounts account = Customer);
    ~DataOverview();

    void data_process(const QModbusDataUnit unit);

public slots:
    void refreshCurrentPage();

private:
    Ui::DataOverview *ui;

    QString labelPressed = "QLabel {background:rgba(87,192,255,1);font-size:18px;font-family:PingFang SC;font-weight:300;line-height:40px;color:rgba(255, 255, 255, 1);}";
    QString labelReleased = "QLabel {font-size:18px;font-family:PingFang SC;font-weight:300;line-height:40px;color:rgba(97,97,97,1);background:rgba(255,255,255,1);}";

    QLabel* last_label= nullptr;

    SystemStatus sys_status;

    ModbusSerial *current_serial;
    Accounts current_account;

    void showSysStatus();

private slots:
};

#endif // DATAOVERVIEW_H

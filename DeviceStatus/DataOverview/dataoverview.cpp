#include "dataoverview.h"
#include "ui_dataoverview.h"
#include "DeviceStatus/DevStatus_regs.h"

DataOverview::DataOverview(QWidget *parent, ModbusSerial *serial) :
    QDialog(parent),
    ui(new Ui::DataOverview),
    current_serial(serial)
{
    ui->setupUi(this);

    last_label = ui->ST_00_label;
}

DataOverview::~DataOverview()
{
    delete ui;
}

void DataOverview::showSysStatus()
{
    switch (sys_status.sys_stat)
    {
    case ST_00:
        last_label->setStyleSheet(labelReleased);
        ui->ST_00_label->setStyleSheet(labelPressed);
        last_label = ui->ST_00_label;
        break;
    case ST_10:
        last_label->setStyleSheet(labelReleased);
        ui->ST_10_label->setStyleSheet(labelPressed);
        last_label = ui->ST_10_label;
        break;
    case ST_11:
        last_label->setStyleSheet(labelReleased);
        ui->ST_11_label->setStyleSheet(labelPressed);
        last_label = ui->ST_11_label;
        break;
    case ST_20:
        last_label->setStyleSheet(labelReleased);
        ui->ST_20_label->setStyleSheet(labelPressed);
        last_label = ui->ST_20_label;
        break;
    case ST_30:
        last_label->setStyleSheet(labelReleased);
        ui->ST_30_label->setStyleSheet(labelPressed);
        last_label = ui->ST_30_label;
        break;
    case ST_40:
        last_label->setStyleSheet(labelReleased);
        ui->ST_40_label->setStyleSheet(labelPressed);
        last_label = ui->ST_40_label;
        break;
    case ST_50:
        last_label->setStyleSheet(labelReleased);
        ui->ST_50_label->setStyleSheet(labelPressed);
        last_label = ui->ST_50_label;
        break;
    case ST_60:
        last_label->setStyleSheet(labelReleased);
        ui->ST_60_label->setStyleSheet(labelPressed);
        last_label = ui->ST_60_label;
        break;
    case ST_70:
        last_label->setStyleSheet(labelReleased);
        ui->ST_70_label->setStyleSheet(labelPressed);
        last_label = ui->ST_70_label;
        break;
    case ST_80:
        last_label->setStyleSheet(labelReleased);
        ui->ST_80_label->setStyleSheet(labelPressed);
        last_label = ui->ST_80_label;
        break;
    case ST_90:
        last_label->setStyleSheet(labelReleased);
        ui->ST_90_label->setStyleSheet(labelPressed);
        last_label = ui->ST_90_label;
        break;
    case ST_100:
        last_label->setStyleSheet(labelReleased);
        ui->ST_100_label->setStyleSheet(labelPressed);
        last_label = ui->ST_100_label;
        break;
    case ST_110:
        last_label->setStyleSheet(labelReleased);
        ui->ST_110_label->setStyleSheet(labelPressed);
        last_label = ui->ST_110_label;
        break;
    case ST_120:
        last_label->setStyleSheet(labelReleased);
        ui->ST_120_label->setStyleSheet(labelPressed);
        last_label = ui->ST_120_label;
        break;
    }
}

void DataOverview::data_process(QModbusDataUnit unit)
{
    for (int i = 0, total = int(unit.valueCount()); i < total; i++)
    {
        int addr = i + unit.startAddress();

        switch (addr) {
        case InputRegs_SysStatus:
            sys_status.sys_stat = unit.value(i);
            showSysStatus();
            break;
        case InputRegs_LT_01:
            sys_status.fueltank_level_1 = unit.value(i);
            ui->fuelTankLeve_1->setText(QString::number(sys_status.fueltank_level_1));
            break;
        case InputRegs_LT_02:
            sys_status.fueltank_level_2 = unit.value(i);
            ui->fuelTankLeve_2->setText(QString::number(sys_status.fueltank_level_2));
            break;
        case InputRegs_VT_01:
            sys_status.fuelbat_voltage = unit.value(i);
            ui->fuelBatVoltage->setText(QString::number(sys_status.fuelbat_voltage));
            break;
        case InputRegs_IT_01:
            sys_status.fuelbat_current = unit.value(i);
            ui->fuelBatCurrent->setText(QString::number(sys_status.fuelbat_current));
            break;
        case InputRegs_FcPower:
            sys_status.fuelbat_generateing_power = unit.value(i);
            ui->fuelBatPower->setText(QString::number(sys_status.fuelbat_generateing_power));
            break;
        case InputRegs_OutVoltage:
            sys_status.sys_output_voltage = unit.value(i);
            ui->sysOutputVoltage->setText(QString::number(sys_status.sys_output_voltage));
            break;
        case InputRegs_OutCurrent:
            sys_status.sys_output_current = unit.value(i);
            ui->sysOutputCurrent->setText(QString::number(sys_status.sys_output_current));
            break;
        case InputRegs_OutPower:
            sys_status.sys_output_power = unit.value(i);
            ui->sysOutputPower->setText(QString::number(sys_status.sys_output_power));
            break;
        case InputRegs_VT_02:
            sys_status.bat_voltage = unit.value(i);
            ui->batVoltage->setText(QString::number(sys_status.bat_voltage));
            break;
        case InputRegs_IT_02:
            sys_status.bat_current = unit.value(i);
            ui->batCurrent->setText(QString::number(sys_status.bat_current));
            break;
        case InputRegs_SinglePowerProduced:
            sys_status.single_power_produced = unit.value(i);
            ui->singlePowerProduced->setText(QString::number(sys_status.single_power_produced));
            break;
        case InputRegs_TotalPowerProduced:
            sys_status.total_power_produced = unit.value(i);
            ui->totalPowerProduced->setText(QString::number(sys_status.total_power_produced));
            break;
        case InputRegs_TotalBootTimes:
            sys_status.total_boot_times = unit.value(i);
            ui->totalBootTimes->setText(QString::number(sys_status.total_boot_times));
            break;
        case InputRegs_FuelConsumption:
            sys_status.fuel_consumption_rate = unit.value(i);
            ui->fuelConsumptionRate->setText(QString::number(sys_status.fuel_consumption_rate));
            break;
        case InputRegs_TotalFuelConsumption:
            sys_status.total_fuel_consumption = unit.value(i);
            ui->totalFuelConsumption->setText(QString::number(sys_status.total_fuel_consumption));
            break;
        case HoldingRegs_SysTime:
            sys_status.sys_year = unit.value(i);
            sys_status.sys_month = (unit.value(i+1)&0xff00)>>8;
            sys_status.sys_date = unit.value(i+1)&0x00ff;
            sys_status.sys_hour = unit.value(i+2);
            sys_status.sys_min = (unit.value(i+3)&0xff00)>>8;
            sys_status.sys_sec = unit.value(i+3)&0x00ff;

            ui->sysTime->setText(QString("%1/%2/%3 %4:%5:%6").arg(QString::number(sys_status.sys_year))
                                                             .arg(QString::number(sys_status.sys_month))
                                                             .arg(QString::number(sys_status.sys_date))
                                                             .arg(QString::number(sys_status.sys_hour))
                                                             .arg(QString::number(sys_status.sys_min))
                                                             .arg(QString::number(sys_status.sys_sec)));
            break;
        case HoldingRegs_SysSingleTime:
            sys_status.sys_single_hour = unit.value(i);
            sys_status.sys_single_min = (unit.value(i+1)&0xff00)>>8;
            sys_status.sys_single_sec = unit.value(i+1)&0x00ff;

            ui->sysSingleTime->setText(QString("%1:%2:%3").arg(QString::number(sys_status.sys_single_hour))
                                                          .arg(QString::number(sys_status.sys_single_min))
                                                          .arg(QString::number(sys_status.sys_single_sec)));

            break;
        case HoldingRegs_SysTotalTime:
            sys_status.sys_total_hour = quint32(unit.value(i)<<16);
            sys_status.sys_total_hour |= unit.value(i+1);
            sys_status.sys_total_min = (unit.value(i+2)&0xff00)>>8;
            sys_status.sys_total_sec = unit.value(i+2)&0x00ff;

            ui->sysTotalTime->setText(QString("%1:%2:%3").arg(QString::number(sys_status.sys_total_hour))
                                                          .arg(QString::number(sys_status.sys_total_min))
                                                          .arg(QString::number(sys_status.sys_total_sec)));

            break;
        }
    }
}

void DataOverview::refreshCurrentPage()
{
    current_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_SysStatus, 17);
//    current_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_LT_01, 18);
    current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SysTime, 9);
}

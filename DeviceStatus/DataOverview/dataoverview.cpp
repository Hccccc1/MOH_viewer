#include "dataoverview.h"
#include "ui_dataoverview.h"
#include "DeviceStatus/DevStatus_regs.h"

DataOverview::DataOverview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataOverview)
{
    ui->setupUi(this);
}

DataOverview::~DataOverview()
{
    delete ui;
}

void DataOverview::data_process(QModbusDataUnit unit)
{
    for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
    {
        switch (unit.startAddress() + i) {
        case SysStatus:
            sys_status.sys_stat = unit.value(i);
            //TODO: change different pic
//                ui->sysStatus_label->setText(QString::number(sys_status.sys_stat));
            break;
        case LT_01:
            sys_status.fueltank_level_1 = unit.value(i);
            ui->fuelTankLevel_1->setText(QString::number(sys_status.fueltank_level_1));
            break;
        case LT_02:
            sys_status.fueltank_level_2 = unit.value(i);
            ui->fuelTankLevel_2->setText(QString::number(sys_status.fueltank_level_2));
            break;
        case VT_01:
           sys_status.fuelbat_voltage = unit.value(i);
           ui->fuelBatVol->setText(QString::number(sys_status.fuelbat_voltage));
           break;
        case IT_01:
            sys_status.fuelbat_current = unit.value(i);
            ui->fuelBatCur->setText(QString::number(sys_status.fuelbat_current));
            break;
        case FcPower:
            sys_status.fuelbat_generateing_power = unit.value(i);
            ui->fuelBatPow->setText(QString::number(sys_status.fuelbat_generateing_power));
            break;
        case OutVoltage:
            sys_status.sys_output_voltage = unit.value(i);
            ui->sysOutputVoltage->setText(QString::number(sys_status.sys_output_voltage));
            break;
        case OutCurrent:
            sys_status.sys_output_current = unit.value(i);
            ui->sysOutputCurrent->setText(QString::number(sys_status.sys_output_current));
            break;
        case OutPower:
            sys_status.sys_output_power = unit.value(i);
            ui->sysOutputPower->setText(QString::number(sys_status.sys_output_power));
            break;
        case VT_02:
            sys_status.bat_voltage = unit.value(i);
            ui->batVoltage->setText(QString::number(sys_status.bat_voltage));
            break;
        case IT_02:
            sys_status.bat_current = unit.value(i);
            ui->batCurrent->setText(QString::number(sys_status.bat_current));
            break;
        case SinglePowerProduced:
            sys_status.single_power_produced = unit.value(i);
            ui->singlePowerProduced->setText(QString::number(sys_status.single_power_produced));
            break;
        case TotalPowerProduced:
            sys_status.total_power_produced = unit.value(i);
            ui->totalPowerProduced->setText(QString::number(sys_status.total_power_produced));
            break;
        case FuelConsumption:
            sys_status.fuel_consumption_rate = unit.value(i);
            ui->fuelConsumptionRate->setText(QString::number(sys_status.fuel_consumption_rate));
            break;
        case TotalFuelConsumption:
            sys_status.total_fuel_consumption = unit.value(i);
            ui->totalFuelConsumption->setText(QString::number(sys_status.sys_output_voltage));
            break;
        }
    }
}

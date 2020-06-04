#include "rtvalues.h"
#include "ui_rtvalues.h"

RTValues::RTValues(QWidget *parent, ModbusSerial *serial, Accounts account) :
    QDialog(parent),
    ui(new Ui::RTValues),
    current_serial(serial),
    current_account(account)
{
    ui->setupUi(this);

    if (account == Customer)
    {
        ui->temperature_groupBox->hide();
        ui->pressure_groupBox->hide();
        ui->flow_groupBox->hide();
        ui->speed_groupBox->hide();

        ui->others_CM_1->hide();
        ui->others_CM_1_namelabel->hide();
        ui->others_CM_1_unitlabel->hide();
    }
}

RTValues::~RTValues()
{
    delete ui;
    delete current_serial;
}

void RTValues::start_refresh_timer(int sec)
{
    if (!refresh_timer)
        refresh_timer = new QTimer(this);

    if (refresh_timer->isActive())
        refresh_timer->stop();

    refresh_timer->start(sec*1000);

    connect(refresh_timer, &QTimer::timeout, this, &RTValues::time_elapsed);
}

void RTValues::stop_refresh_timer()
{
    if (refresh_timer != nullptr && refresh_timer->isActive())
    {
        refresh_timer->stop();

        refresh_timer->deleteLater();
    }
}

void RTValues::time_elapsed()
{
    refreshCurrentPage();
}

void RTValues::data_process(const QModbusDataUnit unit)
{
    for (int i = 0; i < int(unit.valueCount()); i++)
    {
        const int addr = unit.startAddress() + i;

        switch (addr) {
        case InputRegs_TT_01:ui->temperatue_TT_1->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_02:ui->temperatue_TT_2->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_03:ui->temperatue_TT_3->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_04:ui->temperatue_TT_4->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_05:ui->temperatue_TT_5->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_06:ui->temperatue_TT_6->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_07:ui->temperatue_TT_7->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_08:ui->temperatue_TT_8->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_09:ui->temperatue_TT_9->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_10:ui->temperatue_TT_10->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_11:ui->temperatue_TT_11->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_12:ui->temperatue_TT_12->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_13:ui->temperatue_TT_13->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_14:ui->temperatue_TT_14->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_15:ui->temperatue_TT_15->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_16:ui->temperatue_TT_16->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_17:ui->temperatue_TT_17->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_18:ui->temperatue_TT_18->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_19:ui->temperatue_TT_19->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_20:ui->temperatue_TT_20->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_21:ui->temperatue_TT_21->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_22:ui->temperatue_TT_22->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_23:ui->temperatue_TT_23->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_24:ui->temperatue_TT_24->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_25:ui->temperatue_TT_25->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_26:ui->temperatue_TT_26->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_27:ui->temperatue_TT_27->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_28:ui->temperatue_TT_28->setText(QString::number(unit.value(i)));break;
        case InputRegs_TT_29:ui->temperatue_TT_29->setText(QString::number(qint16(unit.value(i))));break;
        case InputRegs_TT_30:ui->temperatue_TT_30->setText(QString::number(qint16(unit.value(i))));break;
        case InputRegs_TT_31:ui->temperatue_TT_31->setText(QString::number(qint16(unit.value(i))));break;
        case InputRegs_TT_32:ui->temperatue_TT_32->setText(QString::number(qint16(unit.value(i))));break;
        case InputRegs_TT_33:ui->temperatue_TT_33->setText(QString::number(qint16(unit.value(i))));break;
        case InputRegs_TT_34:ui->temperatue_TT_34->setText(QString::number(qint16(unit.value(i))));break;
        case InputRegs_TT_35:ui->temperatue_TT_35->setText(QString::number(qint16(unit.value(i))));break;
        case InputRegs_TT_36:ui->temperatue_TT_36->setText(QString::number(qint16(unit.value(i))));break;

        case InputRegs_PT_01:ui->pressure_PT_1->setText(QString::number(double(unit.value(i))/100));break;
        case InputRegs_PT_02:ui->pressure_PT_2->setText(QString::number(double(unit.value(i))/100));break;
        case InputRegs_PT_03:ui->pressure_PT_3->setText(QString::number(double(unit.value(i))/100));break;
        case InputRegs_PT_04:ui->pressure_PT_4->setText(QString::number(double(unit.value(i))/100));break;
        case InputRegs_PT_05:ui->pressure_PT_5->setText(QString::number(double(unit.value(i))/100));break;
        case InputRegs_PT_06:ui->pressure_PT_6->setText(QString::number(double(unit.value(i))/100));break;

        case InputRegs_AFM_01:ui->flow_AFM_1->setText(QString::number(double(unit.value(i))/10));break;
        case InputRegs_AFM_02:ui->flow_AFM_2->setText(QString::number(double(unit.value(i))/10));break;
        case InputRegs_AFM_03:ui->flow_AFM_3->setText(QString::number(double(unit.value(i))/10));break;
        case InputRegs_AFM_04:ui->flow_AFM_4->setText(QString::number(double(unit.value(i))/10));break;
        case InputRegs_MFM_01:ui->flow_MFM_1->setText(QString::number(double(unit.value(i))/10));break;

        case InputRegs_BL_01:ui->speed_BL_1->setText(QString::number(unit.value(i)));break;
        case InputRegs_BL_02:ui->speed_BL_2->setText(QString::number(unit.value(i)));break;
        case InputRegs_BL_03:ui->speed_BL_3->setText(QString::number(unit.value(i)));break;
        case InputRegs_BL_04:ui->speed_BL_4->setText(QString::number(unit.value(i)));break;
        case InputRegs_PMP_01:ui->speed_PMP_1->setText(QString::number(unit.value(i)));break;
        case InputRegs_PMP_02:ui->speed_PMP_2->setText(QString::number(unit.value(i)));break;
        case InputRegs_PMP_03:ui->speed_PMP_3->setText(QString::number(unit.value(i)));break;
        case InputRegs_PMP_04:ui->speed_PMP_4->setText(QString::number(unit.value(i)));break;
        case InputRegs_PMP_05:ui->speed_PMP_5->setText(QString::number(unit.value(i)));break;
        case InputRegs_RAD_01:ui->speed_RAD_1->setText(QString::number(unit.value(i)));break;

        case InputRegs_CM_01:ui->others_CM_1->setText(QString::number(double(unit.value(i))/10));break;
        case InputRegs_LT_01:ui->others_LT_1->setText(QString::number(unit.value(i)));break;
        case InputRegs_LT_02:ui->others_LT_2->setText(QString::number(unit.value(i)));break;
        case InputRegs_VT_01:ui->others_VT_1->setText(QString::number(double(unit.value(i))/10));break;
        case InputRegs_IT_01:ui->others_IT_1->setText(QString::number(double(unit.value(i))/10));break;
        case InputRegs_VT_02:ui->others_VT_2->setText(QString::number(double(unit.value(i))/10));break;
        case InputRegs_IT_02:ui->others_IT_2->setText(QString::number(double(unit.value(i))/10));break;

        case HoldingRegs_SpeedCtrl_BL01:ui->speedPercentage_BL_1->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_BL02:ui->speedPercentage_BL_2->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_BL03:ui->speedPercentage_BL_3->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_BL04:ui->speedPercentage_BL_4->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_PMP01:ui->speedPercentage_PMP_1->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_PMP02:ui->speedPercentage_PMP_2->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_PMP03:ui->speedPercentage_PMP_3->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_PMP04:ui->speedPercentage_PMP_4->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_PMP05:ui->speedPercentage_PMP_5->setText(QString::number(double(unit.value(i))/10));break;
        case HoldingRegs_SpeedCtrl_RAD01:ui->speedPercentage_RAD_1->setText(QString::number(double(unit.value(i))/10));break;

        default:break;
        }
    }
}

void RTValues::refreshCurrentPage()
{
//    current_serial->read_from_modbus(QModbusDataUnit::Coils, )
    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_BL01, 10);
        current_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_TT_01, 77);
    }
}

void RTValues::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

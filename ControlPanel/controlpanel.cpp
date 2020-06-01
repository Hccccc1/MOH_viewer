#include "controlpanel.h"
#include "ui_controlpanel.h"

#include <QMouseEvent>

ControlPanel::ControlPanel(QWidget *parent, ModbusSerial* serial, uint8_t model, Accounts account) :
    QWidget(parent),
    ui(new Ui::ControlPanel),
    current_serial(serial),
    current_model(model),
    current_account(account)
{
    ui->setupUi(this);

    connect(ui->roundProgressBar_1, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_2, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_3, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_4, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_5, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_6, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_7, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_8, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_9, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);
    connect(ui->roundProgressBar_10, &QRoundProgressBar::barValueChanged, this, &ControlPanel::onValueChanged);

    QGradientStops gradientPoints;
    gradientPoints << QGradientStop(0, QColor::fromRgb(0,159,255));

    ui->roundProgressBar_1->setDataColors(gradientPoints);
    ui->roundProgressBar_2->setDataColors(gradientPoints);
    ui->roundProgressBar_3->setDataColors(gradientPoints);
    ui->roundProgressBar_4->setDataColors(gradientPoints);
    ui->roundProgressBar_5->setDataColors(gradientPoints);
    ui->roundProgressBar_6->setDataColors(gradientPoints);
    ui->roundProgressBar_7->setDataColors(gradientPoints);
    ui->roundProgressBar_8->setDataColors(gradientPoints);
    ui->roundProgressBar_9->setDataColors(gradientPoints);
    ui->roundProgressBar_10->setDataColors(gradientPoints);

    if (current_account == Customer)
    {
        ui->IOCtrl_groupBox->hide();
        ui->speedCtrl_groupbox->hide();
    }

//    startTimer(2000);
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::start_refresh_timer(int sec)
{
    if (!refresh_timer)
        refresh_timer = new QTimer(this);

    if (!refresh_timer->isActive())
        refresh_timer->start(sec*1000);

    connect(refresh_timer, &QTimer::timeout, this, &ControlPanel::time_elapsed);
}

void ControlPanel::stop_refresh_timer()
{
    if (refresh_timer != nullptr && refresh_timer->isActive())
    {
        refresh_timer->stop();

        delete refresh_timer;
    }
}

void ControlPanel::time_elapsed()
{
//    qDebug() << "Time elapsed";
    refreshCurrentPage();
}

void ControlPanel::onValueChanged(double value)
{
    //    qDebug() << __FILE__ << __LINE__ << action;
    QRoundProgressBar* roundProgressBar = qobject_cast<QRoundProgressBar *>(sender());

    if (roundProgressBar->objectName() == "roundProgressBar_1")
    {
        if (ui->checkBox_1->isChecked())
        {
            speed_controls[0].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_BL01, speed_controls[0].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_1->setValue(double(speed_controls[0].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_2")
    {
        if (ui->checkBox_2->isChecked())
        {
            speed_controls[1].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_BL02, speed_controls[1].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_2->setValue(double(speed_controls[1].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_3")
    {
        if (ui->checkBox_3->isChecked())
        {
            speed_controls[2].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_BL03, speed_controls[2].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_3->setValue(double(speed_controls[2].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_4")
    {
        if (ui->checkBox_4->isChecked())
        {
            speed_controls[3].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_BL04, speed_controls[3].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_4->setValue(double(speed_controls[3].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_5")
    {
        if (ui->checkBox_5->isChecked())
        {
            speed_controls[4].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_PMP01, speed_controls[4].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_5->setValue(double(speed_controls[4].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_6")
    {
        if (ui->checkBox_6->isChecked())
        {
            speed_controls[5].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_PMP02, speed_controls[5].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_6->setValue(double(speed_controls[5].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_7")
    {
        if (ui->checkBox_7->isChecked())
        {
            speed_controls[6].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_PMP03, speed_controls[6].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_7->setValue(double(speed_controls[6].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_8")
    {
        if (ui->checkBox_8->isChecked())
        {
            speed_controls[7].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_PMP04, speed_controls[7].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_8->setValue(double(speed_controls[7].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_9")
    {
        if (ui->checkBox_9->isChecked())
        {
            speed_controls[8].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_PMP05, speed_controls[8].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_9->setValue(double(speed_controls[8].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
    else if (roundProgressBar->objectName() == "roundProgressBar_10")
    {
        if (ui->checkBox_10->isChecked())
        {
            speed_controls[9].speed_percentage = quint16(value*10);
            current_serial->write_to_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_RAD01, speed_controls[9].speed_percentage);
        }
        else
        {
            ui->roundProgressBar_10->setValue(double(speed_controls[9].speed_percentage)/10);
            QMessageBox::warning(this, "警告", QString(tr("你不能进行该操作！")));
        }
    }
}

//void ControlPanel::on_pushButton_clicked()
//{
//    ui->roundProgressSlider_1->setValue(50);
//}

void ControlPanel::onReadyRead()
{
    auto reply = qobject_cast<QModbusReply *>(sender());

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();

        for (int i = 0, total = int(unit.valueCount()); i < total; i++)
        {
            const int addr = i + unit.startAddress();

            switch (addr) {

            case CoilsRegs_SV_01:
                io_controls[0].io_ctrl = unit.value(i);
                if (io_controls[0].io_ctrl)
                    ui->IOCtrl_SV_1->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_1->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_02:
                io_controls[1].io_ctrl = unit.value(i);
                if (io_controls[1].io_ctrl)
                    ui->IOCtrl_SV_2->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_2->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_03:
                io_controls[2].io_ctrl = unit.value(i);
                if (io_controls[2].io_ctrl)
                    ui->IOCtrl_SV_3->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_3->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_04:
                io_controls[3].io_ctrl = unit.value(i);
                if (io_controls[3].io_ctrl)
                    ui->IOCtrl_SV_4->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_4->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_05:
                io_controls[4].io_ctrl = unit.value(i);
                if (io_controls[4].io_ctrl)
                    ui->IOCtrl_SV_5->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_5->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_06:
                io_controls[5].io_ctrl = unit.value(i);
                if (io_controls[5].io_ctrl)
                    ui->IOCtrl_SV_6->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_6->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_07:
                io_controls[6].io_ctrl = unit.value(i);
                if (io_controls[6].io_ctrl)
                    ui->IOCtrl_SV_7->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_7->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_08:
                io_controls[7].io_ctrl = unit.value(i);
                if (io_controls[7].io_ctrl)
                    ui->IOCtrl_SV_8->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_8->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_09:
                io_controls[8].io_ctrl = unit.value(i);
                if (io_controls[8].io_ctrl)
                    ui->IOCtrl_SV_9->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_9->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_10:
                io_controls[9].io_ctrl = unit.value(i);
                if (io_controls[9].io_ctrl)
                    ui->IOCtrl_SV_10->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_10->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_11:
                io_controls[10].io_ctrl = unit.value(i);
                if (io_controls[10].io_ctrl)
                    ui->IOCtrl_SV_11->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_11->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_12:
                io_controls[11].io_ctrl = unit.value(i);
                if (io_controls[11].io_ctrl)
                    ui->IOCtrl_SV_12->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_12->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_13:
                io_controls[12].io_ctrl = unit.value(i);
                if (io_controls[12].io_ctrl)
                    ui->IOCtrl_SV_13->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_13->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_SV_14:
                io_controls[13].io_ctrl = unit.value(i);
                if (io_controls[13].io_ctrl)
                    ui->IOCtrl_SV_14->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_SV_14->setStyleSheet(square_off_label);
                break;

            case CoilsRegs_BL_01:
                io_controls[14].io_ctrl = unit.value(i);
                if (io_controls[14].io_ctrl)
                    ui->IOCtrl_BL_1->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_BL_1->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_BL_02:
                io_controls[15].io_ctrl = unit.value(i);
                if (io_controls[15].io_ctrl)
                    ui->IOCtrl_BL_2->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_BL_2->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_BL_03:
                io_controls[16].io_ctrl = unit.value(i);
                if (io_controls[16].io_ctrl)
                    ui->IOCtrl_BL_3->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_BL_3->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_BL_04:
                io_controls[17].io_ctrl = unit.value(i);
                if (io_controls[17].io_ctrl)
                    ui->IOCtrl_BL_4->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_BL_4->setStyleSheet(square_off_label);
                break;

            case CoilsRegs_PMP_01:
                io_controls[18].io_ctrl = unit.value(i);
                if (io_controls[18].io_ctrl)
                    ui->IOCtrl_PMP_1->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_PMP_1->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_PMP_02:
                io_controls[19].io_ctrl = unit.value(i);
                if (io_controls[19].io_ctrl)
                    ui->IOCtrl_PMP_2->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_PMP_2->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_PMP_03:
                io_controls[20].io_ctrl = unit.value(i);
                if (io_controls[20].io_ctrl)
                    ui->IOCtrl_PMP_3->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_PMP_3->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_PMP_04:
                io_controls[21].io_ctrl = unit.value(i);
                if (io_controls[21].io_ctrl)
                    ui->IOCtrl_PMP_4->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_PMP_4->setStyleSheet(square_off_label);
                break;
            case CoilsRegs_PMP_05:
                io_controls[22].io_ctrl = unit.value(i);
                if (io_controls[22].io_ctrl)
                    ui->IOCtrl_PMP_5->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_PMP_5->setStyleSheet(square_off_label);
                break;

            case CoilsRegs_RAD_01:
                io_controls[23].io_ctrl = unit.value(i);
                if (io_controls[23].io_ctrl)
                    ui->IOCtrl_RAD_1->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_RAD_1->setStyleSheet(square_off_label);
                break;

            case CoilsRegs_KM_01:
                io_controls[24].io_ctrl = unit.value(i);
                if (io_controls[24].io_ctrl)
                    ui->IOCtrl_KM_1->setStyleSheet(square_on_label);
                else
                    ui->IOCtrl_KM_1->setStyleSheet(square_off_label);
                break;

            case CoilsRegs_SV_01_CtrlEnable:
                io_controls[0].io_ctrl_enable = unit.value(i);
                if (io_controls[0].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_1->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_1->setChecked(false);
                //                qDebug() << __FILE__ << __LINE__ << ui->IOCtrlEnable_SV_1->styleSheet();
                break;
            case CoilsRegs_SV_02_CtrlEnable:
                io_controls[1].io_ctrl_enable = unit.value(i);
                if (io_controls[1].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_2->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_2->setChecked(false);
                //                qDebug() << __FILE__ << __LINE__ << ui->IOCtrlEnable_SV_2->styleSheet();
                break;
            case CoilsRegs_SV_03_CtrlEnable:
                io_controls[2].io_ctrl_enable = unit.value(i);
                if (io_controls[2].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_3->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_3->setChecked(false);
                //                qDebug() << __FILE__ << __LINE__ << ui->IOCtrlEnable_SV_3->styleSheet();
                break;
            case CoilsRegs_SV_04_CtrlEnable:
                io_controls[3].io_ctrl_enable = unit.value(i);
                if (io_controls[3].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_4->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_4->setChecked(false);
                break;
            case CoilsRegs_SV_05_CtrlEnable:
                io_controls[4].io_ctrl_enable = unit.value(i);
                if (io_controls[4].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_5->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_5->setChecked(false);
                break;
            case CoilsRegs_SV_06_CtrlEnable:
                io_controls[5].io_ctrl_enable = unit.value(i);
                if (io_controls[5].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_6->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_6->setChecked(false);
                break;
            case CoilsRegs_SV_07_CtrlEnable:
                io_controls[6].io_ctrl_enable = unit.value(i);
                if (io_controls[6].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_7->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_7->setChecked(false);
                break;
            case CoilsRegs_SV_08_CtrlEnable:
                io_controls[7].io_ctrl_enable = unit.value(i);
                if (io_controls[7].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_8->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_8->setChecked(false);
                break;
            case CoilsRegs_SV_09_CtrlEnable:
                io_controls[8].io_ctrl_enable = unit.value(i);
                if (io_controls[8].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_9->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_9->setChecked(false);
                break;
            case CoilsRegs_SV_10_CtrlEnable:
                io_controls[9].io_ctrl_enable = unit.value(i);
                if (io_controls[9].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_10->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_10->setChecked(false);
                break;
            case CoilsRegs_SV_11_CtrlEnable:
                io_controls[10].io_ctrl_enable = unit.value(i);
                if (io_controls[10].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_11->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_11->setChecked(false);
                break;
            case CoilsRegs_SV_12_CtrlEnable:
                io_controls[11].io_ctrl_enable = unit.value(i);
                if (io_controls[11].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_12->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_12->setChecked(false);
                break;
            case CoilsRegs_SV_13_CtrlEnable:
                io_controls[12].io_ctrl_enable = unit.value(i);
                if (io_controls[12].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_13->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_13->setChecked(false);
                break;
            case CoilsRegs_SV_14_CtrlEnable:
                io_controls[13].io_ctrl_enable = unit.value(i);
                if (io_controls[13].io_ctrl_enable)
                    ui->IOCtrlEnable_SV_14->setChecked(true);
                else
                    ui->IOCtrlEnable_SV_14->setChecked(false);
                break;

            case CoilsRegs_BL_01_CtrlEnable:
                io_controls[14].io_ctrl_enable = unit.value(i);
                if (io_controls[14].io_ctrl_enable)
                    ui->IOCtrlEnable_BL_1->setChecked(true);
                else
                    ui->IOCtrlEnable_BL_1->setChecked(false);
                break;
            case CoilsRegs_BL_02_CtrlEnable:
                io_controls[15].io_ctrl_enable = unit.value(i);
                if (io_controls[15].io_ctrl_enable)
                    ui->IOCtrlEnable_BL_2->setChecked(true);
                else
                    ui->IOCtrlEnable_BL_2->setChecked(false);
                break;
            case CoilsRegs_BL_03_CtrlEnable:
                io_controls[16].io_ctrl_enable = unit.value(i);
                if (io_controls[16].io_ctrl_enable)
                    ui->IOCtrlEnable_BL_3->setChecked(true);
                else
                    ui->IOCtrlEnable_BL_3->setChecked(false);
                break;
            case CoilsRegs_BL_04_CtrlEnable:
                io_controls[17].io_ctrl_enable = unit.value(i);
                if (io_controls[17].io_ctrl_enable)
                    ui->IOCtrlEnable_BL_4->setChecked(true);
                else
                    ui->IOCtrlEnable_BL_4->setChecked(false);
                break;

            case CoilsRegs_PMP_01_CtrlEnable:
                io_controls[18].io_ctrl_enable = unit.value(i);
                if (io_controls[18].io_ctrl_enable)
                    ui->IOCtrlEnable_PMP_1->setChecked(true);
                else
                    ui->IOCtrlEnable_PMP_1->setChecked(false);
                break;
            case CoilsRegs_PMP_02_CtrlEnable:
                io_controls[19].io_ctrl_enable = unit.value(i);
                if (io_controls[19].io_ctrl_enable)
                    ui->IOCtrlEnable_PMP_2->setChecked(true);
                else
                    ui->IOCtrlEnable_PMP_2->setChecked(false);
                break;
            case CoilsRegs_PMP_03_CtrlEnable:
                io_controls[20].io_ctrl_enable = unit.value(i);
                if (io_controls[20].io_ctrl_enable)
                    ui->IOCtrlEnable_PMP_3->setChecked(true);
                else
                    ui->IOCtrlEnable_PMP_3->setChecked(false);
                break;
            case CoilsRegs_PMP_04_CtrlEnable:
                io_controls[21].io_ctrl_enable = unit.value(i);
                if (io_controls[21].io_ctrl_enable)
                    ui->IOCtrlEnable_PMP_4->setChecked(true);
                else
                    ui->IOCtrlEnable_PMP_4->setChecked(false);
                break;
            case CoilsRegs_PMP_05_CtrlEnable:
                io_controls[22].io_ctrl_enable = unit.value(i);
                if (io_controls[22].io_ctrl_enable)
                    ui->IOCtrlEnable_PMP_5->setChecked(true);
                else
                    ui->IOCtrlEnable_PMP_5->setChecked(false);
                break;

            case CoilsRegs_RAD_01_CtrlEnable:
                io_controls[23].io_ctrl_enable = unit.value(i);
                if (io_controls[23].io_ctrl_enable)
                    ui->IOCtrlEnable_RAD_1->setChecked(true);
                else
                    ui->IOCtrlEnable_RAD_1->setChecked(false);
                break;

            case CoilsRegs_KM_01_CtrlEnable:
                io_controls[24].io_ctrl_enable = unit.value(i);
                if (io_controls[24].io_ctrl_enable)
                    ui->IOCtrlEnable_KM_1->setChecked(true);
                else
                    ui->IOCtrlEnable_KM_1->setChecked(false);
                break;

            case DiscreteInputs_OutputFeedback_SV01:
                io_controls[0].output_status_feedback = unit.value(i);
                if (io_controls[0].output_status_feedback)
                    ui->outputStatusFeedback_SV_1->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_1->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV02:
                io_controls[1].output_status_feedback = unit.value(i);
                if (io_controls[1].output_status_feedback)
                    ui->outputStatusFeedback_SV_2->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_2->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV03:
                io_controls[2].output_status_feedback = unit.value(i);
                if (io_controls[2].output_status_feedback)
                    ui->outputStatusFeedback_SV_3->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_3->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV04:
                io_controls[3].output_status_feedback = unit.value(i);
                if (io_controls[3].output_status_feedback)
                    ui->outputStatusFeedback_SV_4->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_4->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV05:
                io_controls[4].output_status_feedback = unit.value(i);
                if (io_controls[4].output_status_feedback)
                    ui->outputStatusFeedback_SV_5->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_5->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV06:
                io_controls[5].output_status_feedback = unit.value(i);
                if (io_controls[5].output_status_feedback)
                    ui->outputStatusFeedback_SV_6->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_6->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV07:
                io_controls[6].output_status_feedback = unit.value(i);
                if (io_controls[6].output_status_feedback)
                    ui->outputStatusFeedback_SV_7->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_7->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV08:
                io_controls[7].output_status_feedback = unit.value(i);
                if (io_controls[7].output_status_feedback)
                    ui->outputStatusFeedback_SV_8->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_8->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV09:
                io_controls[8].output_status_feedback = unit.value(i);
                if (io_controls[8].output_status_feedback)
                    ui->outputStatusFeedback_SV_9->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_9->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV10:
                io_controls[9].output_status_feedback = unit.value(i);
                if (io_controls[9].output_status_feedback)
                    ui->outputStatusFeedback_SV_10->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_10->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV11:
                io_controls[10].output_status_feedback = unit.value(i);
                if (io_controls[10].output_status_feedback)
                    ui->outputStatusFeedback_SV_11->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_11->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV12:
                io_controls[11].output_status_feedback = unit.value(i);
                if (io_controls[11].output_status_feedback)
                    ui->outputStatusFeedback_SV_12->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_12->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV13:
                io_controls[12].output_status_feedback = unit.value(i);
                if (io_controls[12].output_status_feedback)
                    ui->outputStatusFeedback_SV_13->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_13->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_SV14:
                io_controls[13].output_status_feedback = unit.value(i);
                if (io_controls[13].output_status_feedback)
                    ui->outputStatusFeedback_SV_14->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_SV_14->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_BL01:
                io_controls[14].output_status_feedback = unit.value(i);
                if (io_controls[14].output_status_feedback)
                    ui->outputStatusFeedback_BL_1->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_BL_1->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_BL02:
                io_controls[15].output_status_feedback = unit.value(i);
                if (io_controls[15].output_status_feedback)
                    ui->outputStatusFeedback_BL_2->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_BL_2->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_BL03:
                io_controls[16].output_status_feedback = unit.value(i);
                if (io_controls[16].output_status_feedback)
                    ui->outputStatusFeedback_BL_3->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_BL_3->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_BL04:
                io_controls[17].output_status_feedback = unit.value(i);
                if (io_controls[17].output_status_feedback)
                    ui->outputStatusFeedback_BL_4->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_BL_4->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_PMP01:
                io_controls[18].output_status_feedback = unit.value(i);
                if (io_controls[18].output_status_feedback)
                    ui->outputStatusFeedback_PMP_1->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_PMP_1->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_PMP02:
                io_controls[19].output_status_feedback = unit.value(i);
                if (io_controls[19].output_status_feedback)
                    ui->outputStatusFeedback_PMP_2->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_PMP_2->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_PMP03:
                io_controls[20].output_status_feedback = unit.value(i);
                if (io_controls[20].output_status_feedback)
                    ui->outputStatusFeedback_PMP_3->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_PMP_3->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_PMP04:
                io_controls[21].output_status_feedback = unit.value(i);
                if (io_controls[21].output_status_feedback)
                    ui->outputStatusFeedback_PMP_4->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_PMP_4->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_PMP05:
                io_controls[22].output_status_feedback = unit.value(i);
                if (io_controls[22].output_status_feedback)
                    ui->outputStatusFeedback_PMP_5->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_PMP_5->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_RAD01:
                io_controls[23].output_status_feedback = unit.value(i);
                if (io_controls[23].output_status_feedback)
                    ui->outputStatusFeedback_RAD_1->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_RAD_1->setStyleSheet(green_round_circle);
                break;
            case DiscreteInputs_OutputFeedback_KM01:
                io_controls[24].output_status_feedback = unit.value(i);
                if (io_controls[24].output_status_feedback)
                    ui->outputStatusFeedback_KM_1->setStyleSheet(red_round_circle);
                else
                    ui->outputStatusFeedback_KM_1->setStyleSheet(green_round_circle);
                break;

            case HoldingRegs_SpeedCtrl_BL01:
                speed_controls[0].speed_percentage = unit.value(i);
                ui->roundProgressBar_1->setValue(double(speed_controls[0].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_BL02:
                speed_controls[1].speed_percentage = unit.value(i);
                ui->roundProgressBar_2->setValue(double(speed_controls[1].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_BL03:
                speed_controls[2].speed_percentage = unit.value(i);
                ui->roundProgressBar_3->setValue(double(speed_controls[2].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_BL04:
                speed_controls[3].speed_percentage = unit.value(i);
                ui->roundProgressBar_4->setValue(double(speed_controls[3].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_PMP01:
                speed_controls[4].speed_percentage = unit.value(i);
                ui->roundProgressBar_5->setValue(double(speed_controls[4].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_PMP02:
                speed_controls[5].speed_percentage = unit.value(i);
                ui->roundProgressBar_6->setValue(double(speed_controls[5].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_PMP03:
                speed_controls[6].speed_percentage = unit.value(i);
                ui->roundProgressBar_7->setValue(double(speed_controls[6].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_PMP04:
                speed_controls[7].speed_percentage = unit.value(i);
                ui->roundProgressBar_8->setValue(double(speed_controls[7].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_PMP05:
                speed_controls[8].speed_percentage = unit.value(i);
                ui->roundProgressBar_9->setValue(double(speed_controls[8].speed_percentage)/10);
                break;
            case HoldingRegs_SpeedCtrl_RAD01:
                speed_controls[9].speed_percentage = unit.value(i);
                ui->roundProgressBar_10->setValue(double(speed_controls[9].speed_percentage)/10);
                break;

            case InputRegs_BL_01:
                speed_controls[0].speed_rpm = unit.value(i);
                ui->speedControl_1->setText(QString::number(speed_controls[0].speed_rpm));
                break;
            case InputRegs_BL_02:
                speed_controls[1].speed_rpm = unit.value(i);
                ui->speedControl_2->setText(QString::number(speed_controls[1].speed_rpm));
                break;
            case InputRegs_BL_03:
                speed_controls[2].speed_rpm = unit.value(i);
                ui->speedControl_3->setText(QString::number(speed_controls[2].speed_rpm));
                break;
            case InputRegs_BL_04:
                speed_controls[3].speed_rpm = unit.value(i);
                ui->speedControl_4->setText(QString::number(speed_controls[3].speed_rpm));
                break;
            case InputRegs_PMP_01:
                speed_controls[4].speed_rpm = unit.value(i);
                ui->speedControl_5->setText(QString::number(speed_controls[4].speed_rpm));
                break;
            case InputRegs_PMP_02:
                speed_controls[5].speed_rpm = unit.value(i);
                ui->speedControl_6->setText(QString::number(speed_controls[5].speed_rpm));
                break;
            case InputRegs_PMP_03:
                speed_controls[6].speed_rpm = unit.value(i);
                ui->speedControl_7->setText(QString::number(speed_controls[6].speed_rpm));
                break;
            case InputRegs_PMP_04:
                speed_controls[7].speed_rpm = unit.value(i);
                ui->speedControl_8->setText(QString::number(speed_controls[7].speed_rpm));
                break;
            case InputRegs_PMP_05:
                speed_controls[8].speed_rpm = unit.value(i);
                ui->speedControl_9->setText(QString::number(speed_controls[8].speed_rpm));
                break;
            case InputRegs_RAD_01:
                speed_controls[9].speed_rpm = unit.value(i);
                ui->speedControl_10->setText(QString::number(speed_controls[9].speed_rpm));
                break;

            case CoilsRegs_BL_01_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[0].auto_control = true;
                    ui->autoControl_1->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[0].auto_control = false;
                    ui->autoControl_1->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_BL_02_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[1].auto_control = true;
                    ui->autoControl_2->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[1].auto_control = false;
                    ui->autoControl_2->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_BL_03_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[2].auto_control = true;
                    ui->autoControl_3->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[2].auto_control = false;
                    ui->autoControl_3->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_BL_04_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[3].auto_control = true;
                    ui->autoControl_4->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[3].auto_control = false;
                    ui->autoControl_4->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_PMP_01_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[4].auto_control = true;
                    ui->autoControl_5->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[4].auto_control = false;
                    ui->autoControl_5->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_PMP_02_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[5].auto_control = true;
                    ui->autoControl_6->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[5].auto_control = false;
                    ui->autoControl_6->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_PMP_03_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[6].auto_control = true;
                    ui->autoControl_7->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[6].auto_control = false;
                    ui->autoControl_7->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_PMP_04_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[7].auto_control = true;
                    ui->autoControl_8->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[7].auto_control = false;
                    ui->autoControl_8->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_PMP_05_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[8].auto_control = true;
                    ui->autoControl_9->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[8].auto_control = false;
                    ui->autoControl_9->setStyleSheet(switch_off_label);
                }
                break;
            case CoilsRegs_RAD_01_AutoCtrl:
                if (unit.value(i))
                {
                    speed_controls[9].auto_control = true;
                    ui->autoControl_10->setStyleSheet(switch_on_label);
                }
                else
                {
                    speed_controls[9].auto_control = false;
                    ui->autoControl_10->setStyleSheet(switch_off_label);
                }
                break;
            default:
                break;
            }
        }
    }
    else
    {
        emit modbusErrorHappened(reply->error());
    }
}

void ControlPanel::on_autoControl_1_clicked()
{
    if (ui->checkBox_1->isChecked())
    {
        if (speed_controls[0].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01_AutoCtrl, 1, false);
            speed_controls[0].auto_control = false;
            ui->autoControl_1->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01_AutoCtrl, 1, true);
            speed_controls[0].auto_control = true;
            ui->autoControl_1->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[0].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01_AutoCtrl, 1, false);
                speed_controls[0].auto_control = false;
                ui->autoControl_1->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01_AutoCtrl, 1, true);
                speed_controls[0].auto_control = true;
                ui->autoControl_1->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_1->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_2_clicked()
{
    if (ui->checkBox_2->isChecked())
    {
        if (speed_controls[1].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_02_AutoCtrl, 1, false);
            speed_controls[1].auto_control = false;
            ui->autoControl_2->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_02_AutoCtrl, 1, true);
            speed_controls[1].auto_control = true;
            ui->autoControl_2->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[1].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_02_AutoCtrl, 1, false);
                speed_controls[1].auto_control = false;
                ui->autoControl_2->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_02_AutoCtrl, 1, true);
                speed_controls[1].auto_control = true;
                ui->autoControl_2->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_2->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_3_clicked()
{
    if (ui->checkBox_3->isChecked())
    {
        if (speed_controls[2].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_03_AutoCtrl, 1, false);
            speed_controls[2].auto_control = false;
            ui->autoControl_3->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_03_AutoCtrl, 1, true);
            speed_controls[2].auto_control = true;
            ui->autoControl_3->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[2].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_03_AutoCtrl, 1, false);
                speed_controls[2].auto_control = false;
                ui->autoControl_3->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_03_AutoCtrl, 1, true);
                speed_controls[2].auto_control = true;
                ui->autoControl_3->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_3->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_4_clicked()
{
    if (ui->checkBox_4->isChecked())
    {
        if (speed_controls[3].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_04_AutoCtrl, 1, false);
            speed_controls[3].auto_control = false;
            ui->autoControl_4->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_04_AutoCtrl, 1, true);
            speed_controls[3].auto_control = true;
            ui->autoControl_4->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[3].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_04_AutoCtrl, 1, false);
                speed_controls[3].auto_control = false;
                ui->autoControl_4->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_04_AutoCtrl, 1, true);
                speed_controls[3].auto_control = true;
                ui->autoControl_4->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_4->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_5_clicked()
{
    if (ui->checkBox_5->isChecked())
    {
        if (speed_controls[4].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_01_AutoCtrl, 1, false);
            speed_controls[4].auto_control = false;
            ui->autoControl_5->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_01_AutoCtrl, 1, true);
            speed_controls[4].auto_control = true;
            ui->autoControl_5->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[4].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_01_AutoCtrl, 1, false);
                speed_controls[4].auto_control = false;
                ui->autoControl_5->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_01_AutoCtrl, 1, true);
                speed_controls[4].auto_control = true;
                ui->autoControl_5->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_5->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_6_clicked()
{
    if (ui->checkBox_6->isChecked())
    {
        if (speed_controls[5].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_02_AutoCtrl, 1, false);
            speed_controls[5].auto_control = false;
            ui->autoControl_6->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_02_AutoCtrl, 1, true);
            speed_controls[5].auto_control = true;
            ui->autoControl_6->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[5].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_02_AutoCtrl, 1, false);
                speed_controls[5].auto_control = false;
                ui->autoControl_6->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_02_AutoCtrl, 1, true);
                speed_controls[5].auto_control = true;
                ui->autoControl_6->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_6->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_7_clicked()
{
    if (ui->checkBox_7->isChecked())
    {
        if (speed_controls[6].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_03_AutoCtrl, 1, false);
            speed_controls[6].auto_control = false;
            ui->autoControl_7->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_03_AutoCtrl, 1, true);
            speed_controls[6].auto_control = true;
            ui->autoControl_7->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[6].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_03_AutoCtrl, 1, false);
                speed_controls[6].auto_control = false;
                ui->autoControl_7->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_03_AutoCtrl, 1, true);
                speed_controls[6].auto_control = true;
                ui->autoControl_7->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_7->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_8_clicked()
{
    if (ui->checkBox_8->isChecked())
    {
        if (speed_controls[7].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_04_AutoCtrl, 1, false);
            speed_controls[7].auto_control = false;
            ui->autoControl_8->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_04_AutoCtrl, 1, true);
            speed_controls[7].auto_control = true;
            ui->autoControl_8->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[7].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_04_AutoCtrl, 1, false);
                speed_controls[7].auto_control = false;
                ui->autoControl_8->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_04_AutoCtrl, 1, true);
                speed_controls[7].auto_control = true;
                ui->autoControl_8->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_8->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_9_clicked()
{
    if (ui->checkBox_9->isChecked())
    {
        if (speed_controls[8].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_05_AutoCtrl, 1, false);
            speed_controls[8].auto_control = false;
            ui->autoControl_9->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_05_AutoCtrl, 1, true);
            speed_controls[8].auto_control = true;
            ui->autoControl_9->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[8].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_05_AutoCtrl, 1, false);
                speed_controls[8].auto_control = false;
                ui->autoControl_9->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_05_AutoCtrl, 1, true);
                speed_controls[8].auto_control = true;
                ui->autoControl_9->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_9->setChecked(true);
        }
    }
}

void ControlPanel::on_autoControl_10_clicked()
{
    if (ui->checkBox_10->isChecked())
    {
        if (speed_controls[9].auto_control)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_RAD_01_AutoCtrl, 1, false);
            speed_controls[9].auto_control = false;
            ui->autoControl_10->setStyleSheet(switch_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_RAD_01_AutoCtrl, 1, true);
            speed_controls[9].auto_control = true;
            ui->autoControl_10->setStyleSheet(switch_on_label);
        }
    }
    else
    {
        if (QMessageBox::question(this, tr("提示"), tr("确定该操作吗？")) == QMessageBox::Yes)
        {
            if (speed_controls[9].auto_control)
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_RAD_01_AutoCtrl, 1, false);
                speed_controls[9].auto_control = false;
                ui->autoControl_10->setStyleSheet(switch_off_label);
            }
            else
            {
                current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_RAD_01_AutoCtrl, 1, true);
                speed_controls[9].auto_control = true;
                ui->autoControl_10->setStyleSheet(switch_on_label);
            }
            //            ui->checkBox_10->setChecked(true);
        }
    }
}

void ControlPanel::refreshCurrentPage()
{
    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    {
        current_serial->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_01, 64);
        current_serial->read_from_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01_AutoCtrl, 10);
        current_serial->read_from_modbus(QModbusDataUnit::DiscreteInputs, DiscreteInputs_OutputFeedback_SV01, 32);
        current_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_BL_01, 10);
        current_serial->read_from_modbus(QModbusDataUnit::HoldingRegisters, HoldingRegs_SpeedCtrl_BL01, 10);
    }
}


void ControlPanel::on_IOCtrl_SV_1_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[0].io_ctrl_enable)
    {
        if (io_controls[0].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_01, 1, false);
            io_controls[0].io_ctrl = false;
            ui->IOCtrl_SV_1->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_01, 1, true);
            io_controls[0].io_ctrl = true;
            ui->IOCtrl_SV_1->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_2_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[1].io_ctrl_enable)
    {
        if (io_controls[1].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_02, 1, false);
            io_controls[1].io_ctrl = false;
            ui->IOCtrl_SV_2->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_02, 1, true);
            io_controls[1].io_ctrl = true;
            ui->IOCtrl_SV_2->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_3_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[2].io_ctrl_enable)
    {
        if (io_controls[2].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_03, 1, false);
            io_controls[2].io_ctrl = false;
            ui->IOCtrl_SV_3->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_03, 1, true);
            io_controls[2].io_ctrl = true;
            ui->IOCtrl_SV_3->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_4_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[3].io_ctrl_enable)
    {
        if (io_controls[3].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_04, 1, false);
            io_controls[3].io_ctrl = false;
            ui->IOCtrl_SV_4->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_04, 1, true);
            io_controls[3].io_ctrl = true;
            ui->IOCtrl_SV_4->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_5_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[4].io_ctrl_enable)
    {
        if (io_controls[4].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_05, 1, false);
            io_controls[4].io_ctrl = false;
            ui->IOCtrl_SV_5->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_05, 1, true);
            io_controls[4].io_ctrl = true;
            ui->IOCtrl_SV_5->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_6_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[5].io_ctrl_enable)
    {
        if (io_controls[5].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_06, 1, false);
            io_controls[5].io_ctrl = false;
            ui->IOCtrl_SV_6->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_06, 1, true);
            io_controls[5].io_ctrl = true;
            ui->IOCtrl_SV_6->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_7_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[6].io_ctrl_enable)
    {
        if (io_controls[6].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_07, 1, false);
            io_controls[6].io_ctrl = false;
            ui->IOCtrl_SV_7->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_07, 1, true);
            io_controls[6].io_ctrl = true;
            ui->IOCtrl_SV_7->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_8_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[7].io_ctrl_enable)
    {
        if (io_controls[7].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_08, 1, false);
            io_controls[7].io_ctrl = false;
            ui->IOCtrl_SV_8->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_08, 1, true);
            io_controls[7].io_ctrl = true;
            ui->IOCtrl_SV_8->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_9_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[8].io_ctrl_enable)
    {
        if (io_controls[8].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_09, 1, false);
            io_controls[8].io_ctrl = false;
            ui->IOCtrl_SV_9->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_09, 1, true);
            io_controls[8].io_ctrl = true;
            ui->IOCtrl_SV_9->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_10_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[9].io_ctrl_enable)
    {
        if (io_controls[9].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_10, 1, false);
            io_controls[9].io_ctrl = false;
            ui->IOCtrl_SV_10->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_10, 1, true);
            io_controls[9].io_ctrl = true;
            ui->IOCtrl_SV_10->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_11_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[10].io_ctrl_enable)
    {
        if (io_controls[10].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_11, 1, false);
            io_controls[10].io_ctrl = false;
            ui->IOCtrl_SV_11->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_11, 1, true);
            io_controls[10].io_ctrl = true;
            ui->IOCtrl_SV_11->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_12_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[11].io_ctrl_enable)
    {
        if (io_controls[11].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_12, 1, false);
            io_controls[11].io_ctrl = false;
            ui->IOCtrl_SV_12->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_12, 1, true);
            io_controls[11].io_ctrl = true;
            ui->IOCtrl_SV_12->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_13_clicked()
{

    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[12].io_ctrl_enable)
    {
        if (io_controls[12].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_13, 1, false);
            io_controls[12].io_ctrl = false;
            ui->IOCtrl_SV_13->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_13, 1, true);
            io_controls[12].io_ctrl = true;
            ui->IOCtrl_SV_13->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_SV_14_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[13].io_ctrl_enable)
    {
        if (io_controls[13].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_14, 1, false);
            io_controls[13].io_ctrl = false;
            ui->IOCtrl_SV_14->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_14, 1, true);
            io_controls[13].io_ctrl = true;
            ui->IOCtrl_SV_14->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_BL_1_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[14].io_ctrl_enable)
    {
        if (io_controls[14].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01, 1, false);
            io_controls[14].io_ctrl = false;
            ui->IOCtrl_BL_1->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01, 1, true);
            io_controls[14].io_ctrl = true;
            ui->IOCtrl_BL_1->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_BL_2_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[15].io_ctrl_enable)
    {
        if (io_controls[15].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_02, 1, false);
            io_controls[15].io_ctrl = false;
            ui->IOCtrl_BL_2->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_02, 1, true);
            io_controls[15].io_ctrl = true;
            ui->IOCtrl_BL_2->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_BL_3_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[16].io_ctrl_enable)
    {
        if (io_controls[16].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_03, 1, false);
            io_controls[16].io_ctrl = false;
            ui->IOCtrl_BL_3->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_03, 1, true);
            io_controls[16].io_ctrl = true;
            ui->IOCtrl_BL_3->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_BL_4_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[17].io_ctrl_enable)
    {
        if (io_controls[17].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_04, 1, false);
            io_controls[17].io_ctrl = false;
            ui->IOCtrl_BL_4->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_04, 1, true);
            io_controls[17].io_ctrl = true;
            ui->IOCtrl_BL_4->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_PMP_1_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[18].io_ctrl_enable)
    {
        if (io_controls[18].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_01, 1, false);
            io_controls[18].io_ctrl = false;
            ui->IOCtrl_PMP_1->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_01, 1, true);
            io_controls[18].io_ctrl = true;
            ui->IOCtrl_PMP_1->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_PMP_2_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[19].io_ctrl_enable)
    {
        if (io_controls[19].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_02, 1, false);
            io_controls[19].io_ctrl = false;
            ui->IOCtrl_PMP_2->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_02, 1, true);
            io_controls[19].io_ctrl = true;
            ui->IOCtrl_PMP_2->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_PMP_3_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[20].io_ctrl_enable)
    {
        if (io_controls[20].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_03, 1, false);
            io_controls[20].io_ctrl = false;
            ui->IOCtrl_PMP_3->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_03, 1, true);
            io_controls[20].io_ctrl = true;
            ui->IOCtrl_PMP_3->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_PMP_4_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[21].io_ctrl_enable)
    {
        if (io_controls[21].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_04, 1, false);
            io_controls[21].io_ctrl = false;
            ui->IOCtrl_PMP_4->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_04, 1, true);
            io_controls[21].io_ctrl = true;
            ui->IOCtrl_PMP_4->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_PMP_5_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[22].io_ctrl_enable)
    {
        if (io_controls[22].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_05, 1, false);
            io_controls[22].io_ctrl = false;
            ui->IOCtrl_PMP_5->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_05, 1, true);
            io_controls[22].io_ctrl = true;
            ui->IOCtrl_PMP_5->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_RAD_1_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[23].io_ctrl_enable)
    {
        if (io_controls[23].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_RAD_01, 1, false);
            io_controls[23].io_ctrl = false;
            ui->IOCtrl_RAD_1->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_RAD_01, 1, true);
            io_controls[23].io_ctrl = true;
            ui->IOCtrl_RAD_1->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrl_KM_1_clicked()
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    if (io_controls[24].io_ctrl_enable)
    {
        if (io_controls[24].io_ctrl)
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_KM_01, 1, false);
            io_controls[24].io_ctrl = false;
            ui->IOCtrl_KM_1->setStyleSheet(square_off_label);
        }
        else
        {
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_KM_01, 1, true);
            io_controls[24].io_ctrl = true;
            ui->IOCtrl_KM_1->setStyleSheet(square_on_label);
        }
    }
    else
        QMessageBox::critical(this, tr("错误！"), tr("你不能进行该操作。"));
}

void ControlPanel::on_IOCtrlEnable_SV_1_clicked(bool state)
{
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[0].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_01_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[0].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_01_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_2_clicked(bool state) {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[1].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_02_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[1].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_02_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_3_clicked(bool state) {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[2].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_03_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[2].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_03_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_4_clicked(bool state) {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[3].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_04_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[3].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_04_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_5_clicked(bool state) {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[4].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_05_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[4].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_05_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_6_clicked(bool state) {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[5].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_06_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[5].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_06_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_7_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[6].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_07_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[6].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_07_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_8_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[7].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_08_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[7].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_08_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_9_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[8].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_09_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[8].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_09_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_10_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[9].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_10_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[9].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_10_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_11_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[10].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_11_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[10].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_11_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_12_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[11].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_12_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[11].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_12_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_13_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[12].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_13_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[12].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_13_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_SV_14_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[13].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_14_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[13].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_SV_14_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_BL_1_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[14].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[14].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_01_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_BL_2_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[15].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_02_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[15].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_02_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_BL_3_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[16].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_03_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[16].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_03_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_BL_4_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[17].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_04_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[17].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_BL_04_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_PMP_1_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[18].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_01_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[18].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_01_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_PMP_2_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[19].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_02_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[19].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_02_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_PMP_3_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[20].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_03_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[20].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_03_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_PMP_4_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[21].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_04_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[21].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_04_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_PMP_5_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[22].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_05_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[22].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_PMP_05_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_RAD_1_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[23].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_RAD_01_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[23].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_RAD_01_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::on_IOCtrlEnable_KM_1_clicked(bool state)  {
    //    if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
    {
        if (state)
        {
            io_controls[24].io_ctrl_enable = true;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_KM_01_CtrlEnable, 1, true);
        }
        else
        {
            io_controls[24].io_ctrl_enable = false;
            current_serial->write_to_modbus(QModbusDataUnit::Coils, CoilsRegs_KM_01_CtrlEnable, 1, false);
        }
    }
}

void ControlPanel::timerEvent(QTimerEvent *)
{
    //    qDebug() << "timer elapsed";

    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
        this->refreshCurrentPage();
}

//void ControlPanel::on_speedControl_1_editingFinished()
//{
//    if (ui->checkBox_1->isChecked())
//    {
//        speed_controls[0].speed_rpm = ui->speedControl_1->value();
//        ui->speedControl_1->setValue(speed_controls[0].speed_rpm);
////        current_serial->write_to_modbus(QModbusDataUnit::)
//    }
//    else
//    {
//        if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
//        {
//            speed_controls[0].speed_rpm = ui->speedControl_1->value();
//            ui->speedControl_1->setValue(speed_controls[0].speed_rpm);
//        }
//    }
//}

//void ControlPanel::on_speedControl_2_editingFinished()
//{
//    if (ui->checkBox_2->isChecked())
//    {
//        speed_controls[1].speed_rpm = ui->speedControl_2->value();
//        ui->speedControl_2->setValue(speed_controls[1].speed_rpm);
//    }
//    else
//    {
//        if (QMessageBox::question(this, "提示", "确定该操作吗？") == QMessageBox::Yes)
//        {
//            speed_controls[1].speed_rpm = ui->speedControl_2->value();
//            ui->speedControl_2->setValue(speed_controls[1].speed_rpm);
//        }
//    }
//}

//void ControlPanel::on_speedControl_3_editingFinished()
//{

//}

//void ControlPanel::on_speedControl_4_editingFinished()
//{

//}

//void ControlPanel::on_speedControl_5_editingFinished()
//{

//}

//void ControlPanel::on_speedControl_6_editingFinished()
//{

//}

//void ControlPanel::on_speedControl_7_editingFinished()
//{

//}

//void ControlPanel::on_speedControl_8_editingFinished()
//{

//}

//void ControlPanel::on_speedControl_9_editingFinished()
//{

//}

//void ControlPanel::on_speedControl_10_editingFinished()
//{

//}


#include "rtcurve.h"
#include "ui_rtcurve.h"
#include "DeviceStatus/DevStatus_regs.h"

RTCurve::RTCurve(QWidget *parent, ModbusSerial *serial, Accounts account) :
    QWidget(parent),
    ui(new Ui::RTCurve),
    current_account(account),
    current_serial(serial)
{
    ui->setupUi(this);

}

RTCurve::~RTCurve()
{
    delete ui;
    delete current_serial;
}

void RTCurve::data_process(QModbusDataUnit unit)
{
}

void RTCurve::on_tabWidget_currentChanged(int index)
{
    qDebug() << sender()->objectName();

    QWidget *tmp_widget = ui->tabWidget->currentWidget();

    ui->groupBox->setParent(tmp_widget);
    ui->groupBox->show();

    ui->gridLayout_3->setParent(tmp_widget);
    ui->tableView->setParent(tmp_widget);
    ui->tableView->show();

    switch (index) {
    case TT01_TT08:ui->checkBox_chart_1->setText("TT01_TT08");break;
    case TT09_TT16:ui->checkBox_chart_1->setText("TT09_TT16");break;
    case TT17_TT24:ui->checkBox_chart_1->setText("TT17_TT24");break;
    case TT25_TT32:ui->checkBox_chart_1->setText("TT25_TT32");break;
    case TT33_TT36:ui->checkBox_chart_1->setText("TT33_TT36");break;
    case PressureChart:break;
    case FlowChart:break;
    case SpeedChart_1:break;
    case SpeedChart_2:break;
    case OthersChart:break;
    }
}

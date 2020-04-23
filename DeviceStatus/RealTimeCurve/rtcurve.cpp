#include "rtcurve.h"
#include "ui_rtcurve.h"
#include "AllBitsAndRegs.h"

RTCurve::RTCurve(QWidget *parent, ModbusSerial *serial) :
    QDialog(parent),
    ui(new Ui::RTCurve),
    current_serial(serial)
{
    ui->setupUi(this);

    QPen p0;
    p0.setWidth(1);

    for (int i = 0; i < max_charts_num; i++)
    {
        chart[i] = new QChart();
        series[i] = new QLineSeries();
        axis_x[i] = new QValueAxis();
        axis_y[i] = new QValueAxis();

        chart[i]->addSeries(series[i]);

        chart[i]->addAxis(axis_x[i], Qt::AlignBottom);
        chart[i]->addAxis(axis_y[i], Qt::AlignLeft);

        series[i]->attachAxis(axis_x[i]);
        series[i]->attachAxis(axis_y[i]);

//        axis_x[
        axis_x[i]->setRange(0, 10);
        axis_y[i]->setRange(0, 500);

        qDebug() << axis_x[i]->max() << axis_y[i]->min();

        chart[i]->setMargins(QMargins(0, 0, 0, 0));

        switch (i)
        {
        case 0:
            chart[i]->setTitle(tr("TT-01"));
            ui->checkBox_chart_1->setText(tr("TT-01"));
            p0.setColor(QColor::fromRgb(87,192,255));
            series[i]->setPen(p0);
            ui->realTimeCurve_1->setChart(chart[0]);
            break;
        case 1:
            chart[i]->setTitle(tr("TT-02"));
            ui->checkBox_chart_2->setText(tr("TT-02"));
            p0.setColor(QColor::fromRgb(81,223,0));
            series[i]->setPen(p0);
            ui->realTimeCurve_2->setChart(chart[i]);
            break;
        case 2:
            chart[i]->setTitle(tr("TT-03"));
            ui->checkBox_chart_3->setText(tr("TT-03"));
            p0.setColor(QColor::fromRgb(255,87,193));
            series[i]->setPen(p0);
            ui->realTimeCurve_3->setChart(chart[i]);
            break;
        case 3:
            chart[i]->setTitle(tr("TT-04"));
            ui->checkBox_chart_4->setText(tr("TT-04"));
            p0.setColor(QColor::fromRgb(252,43,43));
            series[i]->setPen(p0);
            ui->realTimeCurve_4->setChart(chart[i]);
            break;
        case 4:
            chart[i]->setTitle(tr("TT-05"));
            ui->checkBox_chart_5->setText(tr("TT-05"));
            p0.setColor(QColor::fromRgb(255,199,87));
            series[i]->setPen(p0);
            ui->realTimeCurve_5->setChart(chart[i]);
            break;
        case 5:
            chart[i]->setTitle(tr("TT-06"));
            ui->checkBox_chart_6->setText(tr("TT-06"));
            p0.setColor(QColor::fromRgb(30,206,226));
            series[i]->setPen(p0);
            ui->realTimeCurve_6->setChart(chart[i]);
            break;
        case 6:
            chart[i]->setTitle(tr("TT-07"));
            ui->checkBox_chart_7->setText(tr("TT-07"));
            p0.setColor(QColor::fromRgb(87,121,255));
            series[i]->setPen(p0);
            ui->realTimeCurve_7->setChart(chart[i]);
            break;
        case 7:
            chart[i]->setTitle(tr("TT-08"));
            ui->checkBox_chart_8->setText(tr("TT-08"));
            p0.setColor(QColor::fromRgb(200,87,255));
            series[i]->setPen(p0);
            ui->realTimeCurve_8->setChart(chart[i]);
            break;
        }
    }

    lastGroup = TT01_TT08;
    ui->TT01_TT08_btn->setStyleSheet(pressed_stylesheet);
    ui->TT09_TT16_btn->setStyleSheet(released_stylesheet);
    ui->TT17_TT24_btn->setStyleSheet(released_stylesheet);
    ui->TT25_TT32_btn->setStyleSheet(released_stylesheet);
    ui->TT33_TT36_btn->setStyleSheet(released_stylesheet);
    ui->pressure_btn->setStyleSheet(released_stylesheet);
    ui->flow_btn->setStyleSheet(released_stylesheet);
    ui->speed_1_btn->setStyleSheet(released_stylesheet);
    ui->speed_2_btn->setStyleSheet(released_stylesheet);
    ui->others_btn->setStyleSheet(released_stylesheet);
}

void RTCurve::setup_stylesheet(const DisplayGroups current_group, const DisplayGroups last_group)
{
    switch (last_group) {
    case TT01_TT08:
        ui->TT01_TT08_btn->setStyleSheet(released_stylesheet);
        break;
    case TT09_TT16:
        ui->TT09_TT16_btn->setStyleSheet(released_stylesheet);
        break;
    case TT17_TT24:
        ui->TT17_TT24_btn->setStyleSheet(released_stylesheet);
        break;
    case TT25_TT32:
        ui->TT25_TT32_btn->setStyleSheet(released_stylesheet);
        break;
    case TT33_TT36:
        ui->TT33_TT36_btn->setStyleSheet(released_stylesheet);
        break;
    case PressureChart:
        ui->pressure_btn->setStyleSheet(released_stylesheet);
        break;
    case FlowChart:
        ui->flow_btn->setStyleSheet(released_stylesheet);
        break;
    case SpeedChart_1:
        ui->speed_1_btn->setStyleSheet(released_stylesheet);
        break;
    case SpeedChart_2:
        ui->speed_2_btn->setStyleSheet(released_stylesheet);
        break;
    case OthersChart:
        ui->others_btn->setStyleSheet(released_stylesheet);
        break;
    }

    switch (current_group) {
    case TT01_TT08:
        ui->TT01_TT08_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT09_TT16:
        ui->TT09_TT16_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT17_TT24:
        ui->TT17_TT24_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT25_TT32:
        ui->TT25_TT32_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT33_TT36:
        ui->TT33_TT36_btn->setStyleSheet(pressed_stylesheet);
        break;
    case PressureChart:
        ui->pressure_btn->setStyleSheet(pressed_stylesheet);
        break;
    case FlowChart:
        ui->flow_btn->setStyleSheet(pressed_stylesheet);
        break;
    case SpeedChart_1:
        ui->speed_1_btn->setStyleSheet(pressed_stylesheet);
        break;
    case SpeedChart_2:
        ui->speed_2_btn->setStyleSheet(pressed_stylesheet);
        break;
    case OthersChart:
        ui->others_btn->setStyleSheet(pressed_stylesheet);
        break;
    }
}

void RTCurve::setup_charts_and_buttton(const DisplayGroups group)
{
    switch(group)
    {
    case TT01_TT08:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-01"));chart[i]->setTitle(tr("TT-01"));break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-02"));chart[i]->setTitle(tr("TT-02"));break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-03"));chart[i]->setTitle(tr("TT-03"));break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-04"));chart[i]->setTitle(tr("TT-04"));break;
            case 4:ui->checkBox_chart_5->setText(tr("TT-05"));chart[i]->setTitle(tr("TT-05"));chart[i]->show();break;
            case 5:ui->checkBox_chart_6->setText(tr("TT-06"));chart[i]->setTitle(tr("TT-06"));chart[i]->show();break;
            case 6:ui->checkBox_chart_7->setText(tr("TT-07"));chart[i]->setTitle(tr("TT-07"));chart[i]->show();break;
            case 7:ui->checkBox_chart_8->setText(tr("TT-08"));chart[i]->setTitle(tr("TT-08"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT01_TT08, lastGroup);
        lastGroup = TT01_TT08;

        break;
    case TT09_TT16:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-09"));chart[i]->setTitle(tr("TT-09"));break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-10"));chart[i]->setTitle(tr("TT-10"));break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-11"));chart[i]->setTitle(tr("TT-11"));break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-12"));chart[i]->setTitle(tr("TT-12"));break;
            case 4:ui->checkBox_chart_5->setText(tr("TT-13"));chart[i]->setTitle(tr("TT-13"));chart[i]->show();break;
            case 5:ui->checkBox_chart_6->setText(tr("TT-14"));chart[i]->setTitle(tr("TT-14"));chart[i]->show();break;
            case 6:ui->checkBox_chart_7->setText(tr("TT-15"));chart[i]->setTitle(tr("TT-15"));chart[i]->show();break;
            case 7:ui->checkBox_chart_8->setText(tr("TT-16"));chart[i]->setTitle(tr("TT-16"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT09_TT16, lastGroup);
        lastGroup = TT09_TT16;

        break;
    case TT17_TT24:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-17"));chart[i]->setTitle(tr("TT-17"));break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-18"));chart[i]->setTitle(tr("TT-18"));break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-19"));chart[i]->setTitle(tr("TT-19"));break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-20"));chart[i]->setTitle(tr("TT-20"));break;
            case 4:ui->checkBox_chart_5->setText(tr("TT-21"));chart[i]->setTitle(tr("TT-21"));chart[i]->show();break;
            case 5:ui->checkBox_chart_6->setText(tr("TT-22"));chart[i]->setTitle(tr("TT-22"));chart[i]->show();break;
            case 6:ui->checkBox_chart_7->setText(tr("TT-23"));chart[i]->setTitle(tr("TT-23"));chart[i]->show();break;
            case 7:ui->checkBox_chart_8->setText(tr("TT-24"));chart[i]->setTitle(tr("TT-24"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT17_TT24, lastGroup);
        lastGroup = TT17_TT24;

        break;
    case TT25_TT32:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-25"));chart[i]->setTitle(tr("TT-25"));break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-26"));chart[i]->setTitle(tr("TT-26"));break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-27"));chart[i]->setTitle(tr("TT-27"));break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-28"));chart[i]->setTitle(tr("TT-28"));break;
            case 4:ui->checkBox_chart_5->setText(tr("TT-29"));chart[i]->setTitle(tr("TT-29"));chart[i]->show();break;
            case 5:ui->checkBox_chart_6->setText(tr("TT-30"));chart[i]->setTitle(tr("TT-30"));chart[i]->show();break;
            case 6:ui->checkBox_chart_7->setText(tr("TT-31"));chart[i]->setTitle(tr("TT-31"));chart[i]->show();break;
            case 7:ui->checkBox_chart_8->setText(tr("TT-32"));chart[i]->setTitle(tr("TT-32"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT25_TT32, lastGroup);
        lastGroup = TT25_TT32;

        break;
    case TT33_TT36:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-33"));chart[i]->setTitle(tr("TT-33"));break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-34"));chart[i]->setTitle(tr("TT-34"));break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-35"));chart[i]->setTitle(tr("TT-35"));break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-36"));chart[i]->setTitle(tr("TT-36"));break;
            case 4:ui->checkBox_chart_5->setText(tr("NULL"));chart[i]->hide();break;
            case 5:ui->checkBox_chart_6->setText(tr("NULL"));chart[i]->hide();break;
            case 6:ui->checkBox_chart_7->setText(tr("NULL"));chart[i]->hide();break;
            case 7:ui->checkBox_chart_8->setText(tr("NULL"));chart[i]->hide();break;
            }
        }

        setup_stylesheet(TT33_TT36, lastGroup);
        lastGroup = TT33_TT36;

        break;
    case PressureChart:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("PT-01"));chart[i]->setTitle(tr("PT-01"));break;
            case 1:ui->checkBox_chart_2->setText(tr("PT-02"));chart[i]->setTitle(tr("PT-02"));break;
            case 2:ui->checkBox_chart_3->setText(tr("PT-03"));chart[i]->setTitle(tr("PT-03"));break;
            case 3:ui->checkBox_chart_4->setText(tr("PT-04"));chart[i]->setTitle(tr("PT-04"));break;
            case 4:ui->checkBox_chart_5->setText(tr("PT-05"));chart[i]->setTitle(tr("PT-05"));chart[i]->show();break;
            case 5:ui->checkBox_chart_6->setText(tr("PT-06"));chart[i]->setTitle(tr("PT-06"));chart[i]->show();break;
            case 6:ui->checkBox_chart_7->setText(tr("NULL"));chart[i]->hide();break;
            case 7:ui->checkBox_chart_8->setText(tr("NULL"));chart[i]->hide();break;
            }
        }

        setup_stylesheet(PressureChart, lastGroup);
        lastGroup = PressureChart;

        break;
    case FlowChart:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("AFM-01"));chart[i]->setTitle(tr("AFM-01"));break;
            case 1:ui->checkBox_chart_2->setText(tr("AFM-02"));chart[i]->setTitle(tr("AFM-02"));break;
            case 2:ui->checkBox_chart_3->setText(tr("AFM-03"));chart[i]->setTitle(tr("AFM-03"));break;
            case 3:ui->checkBox_chart_4->setText(tr("AFM-04"));chart[i]->setTitle(tr("AFM-04"));break;
            case 4:ui->checkBox_chart_5->setText(tr("MFM-01"));chart[i]->setTitle(tr("MFM-01"));chart[i]->show();break;
            case 5:ui->checkBox_chart_6->setText(tr("NULL"));chart[i]->hide();break;
            case 6:ui->checkBox_chart_7->setText(tr("NULL"));chart[i]->hide();break;
            case 7:ui->checkBox_chart_8->setText(tr("NULL"));chart[i]->hide();break;
            }
        }

        setup_stylesheet(FlowChart, lastGroup);
        lastGroup = FlowChart;

        break;
    case SpeedChart_1:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("BL-01"));chart[i]->setTitle(tr("BL-01"));break;
            case 1:ui->checkBox_chart_2->setText(tr("BL-02"));chart[i]->setTitle(tr("BL-02"));break;
            case 2:ui->checkBox_chart_3->setText(tr("BL-03"));chart[i]->setTitle(tr("BL-03"));break;
            case 3:ui->checkBox_chart_4->setText(tr("BL-04"));chart[i]->setTitle(tr("BL-04"));break;
            case 4:ui->checkBox_chart_5->setText(tr("NULL"));chart[i]->hide();break;
            case 5:ui->checkBox_chart_6->setText(tr("NULL"));chart[i]->hide();break;
            case 6:ui->checkBox_chart_7->setText(tr("NULL"));chart[i]->hide();break;
            case 7:ui->checkBox_chart_8->setText(tr("NULL"));chart[i]->hide();break;
            }
        }

        setup_stylesheet(SpeedChart_1, lastGroup);
        lastGroup = SpeedChart_1;

        break;
    case SpeedChart_2:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("PMP-01"));chart[i]->setTitle(tr("PMP-01"));break;
            case 1:ui->checkBox_chart_2->setText(tr("PMP-02"));chart[i]->setTitle(tr("PMP-02"));break;
            case 2:ui->checkBox_chart_3->setText(tr("PMP-03"));chart[i]->setTitle(tr("PMP-03"));break;
            case 3:ui->checkBox_chart_4->setText(tr("PMP-04"));chart[i]->setTitle(tr("PMP-04"));break;
            case 4:ui->checkBox_chart_5->setText(tr("PMP-05"));chart[i]->setTitle(tr("PMP-05"));chart[i]->show();break;
            case 5:ui->checkBox_chart_6->setText(tr("RAD-01"));chart[i]->setTitle(tr("RAD-01"));chart[i]->show();break;
            case 6:ui->checkBox_chart_7->setText(tr("NULL"));chart[i]->hide();break;
            case 7:ui->checkBox_chart_8->setText(tr("NULL"));chart[i]->hide();break;
            }
        }

        setup_stylesheet(SpeedChart_2, lastGroup);
        lastGroup = SpeedChart_2;

        break;
    case OthersChart:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("CM-01"));chart[i]->setTitle(tr("CM-01"));break;
            case 1:ui->checkBox_chart_2->setText(tr("LT-01"));chart[i]->setTitle(tr("LT-01"));break;
            case 2:ui->checkBox_chart_3->setText(tr("LT-02"));chart[i]->setTitle(tr("LT-02"));break;
            case 3:ui->checkBox_chart_4->setText(tr("VT-01"));chart[i]->setTitle(tr("VT-01"));break;
            case 4:ui->checkBox_chart_5->setText(tr("IT-01"));chart[i]->setTitle(tr("IT-01"));chart[i]->show();break;
            case 5:ui->checkBox_chart_6->setText(tr("VT-02"));chart[i]->setTitle(tr("VT-02"));chart[i]->show();break;
            case 6:ui->checkBox_chart_7->setText(tr("IT-01"));chart[i]->setTitle(tr("IT-02"));chart[i]->show();break;
            case 7:ui->checkBox_chart_8->setText(tr("NULL"));chart[i]->hide();break;
            }
        }

        setup_stylesheet(OthersChart, lastGroup);
        lastGroup = OthersChart;

        break;
    }
}

RTCurve::~RTCurve()
{
    delete ui;
}

void RTCurve::on_readButton_clicked()
{
    current_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_TT_01, 8);

//    series[0]->append(QDateTime::currentSecsSinceEpoch()%100, QDateTime::currentSecsSinceEpoch()%10);
}

void RTCurve::data_process(const QModbusDataUnit unit)
{
    for (int i = 0, total = int(unit.valueCount()); i < total; i++)
    {
        const int addr = unit.startAddress() + i;

        qDebug() << __LINE__ << QDateTime::currentSecsSinceEpoch()%10;

        switch (addr) {
        case InputRegs_TT_01:
            qDebug() << __LINE__ << unit.value(i);
            series[0]->append(QDateTime::currentSecsSinceEpoch()%10, unit.value(i));
            break;
        case InputRegs_TT_02:
            qDebug() << __LINE__ << unit.value(i);
            series[1]->append(QDateTime::currentSecsSinceEpoch()%10, unit.value(i));
            break;
        case InputRegs_TT_03:
            qDebug() << __LINE__ << unit.value(i);
            series[2]->append(QDateTime::currentSecsSinceEpoch()%10, unit.value(i));
            break;
        case InputRegs_TT_04:
            qDebug() << __LINE__ << unit.value(i);
            series[3]->append(QDateTime::currentSecsSinceEpoch()%10, unit.value(i));
            break;
        case InputRegs_TT_05:
            qDebug() << __LINE__ << unit.value(i);
            series[4]->append(QDateTime::currentSecsSinceEpoch()%10, unit.value(i));
            break;
        case InputRegs_TT_06:
            qDebug() << __LINE__ << unit.value(i);
            series[5]->append(QDateTime::currentSecsSinceEpoch()%10, unit.value(i));
            break;
        case InputRegs_TT_07:
            qDebug() << __LINE__ << unit.value(i);
            series[6]->append(QDateTime::currentSecsSinceEpoch()%10, unit.value(i));
            break;
        case InputRegs_TT_08:
            series[7]->append(QDateTime::currentSecsSinceEpoch()%10, unit.value(i));
            break;
        default:break;
        }
    }
}

void RTCurve::on_TT01_TT08_btn_clicked()
{
    setup_charts_and_buttton(TT01_TT08);
}

void RTCurve::on_TT09_TT16_btn_clicked()
{
    setup_charts_and_buttton(TT09_TT16);
}

void RTCurve::on_TT17_TT24_btn_clicked()
{
    setup_charts_and_buttton(TT17_TT24);
}

void RTCurve::on_TT25_TT32_btn_clicked()
{
    setup_charts_and_buttton(TT25_TT32);
}

void RTCurve::on_TT33_TT36_btn_clicked()
{
    setup_charts_and_buttton(TT33_TT36);
}

void RTCurve::on_pressure_btn_clicked()
{
    setup_charts_and_buttton(PressureChart);
}

void RTCurve::on_flow_btn_clicked()
{
    setup_charts_and_buttton(FlowChart);
}

void RTCurve::on_speed_1_btn_clicked()
{
    setup_charts_and_buttton(SpeedChart_1);
}
void RTCurve::on_speed_2_btn_clicked()
{
    setup_charts_and_buttton(SpeedChart_2);
}

void RTCurve::on_others_btn_clicked()
{
    setup_charts_and_buttton(OthersChart);
}

#include "rtcurve.h"
#include "ui_rtcurve.h"

RTCurve::RTCurve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RTCurve)
{
    ui->setupUi(this);

    QPen p0;
    p0.setWidth(1);

    //    ui->realTimeCurve_1->setChart()

    for (int i = 0; i < 6; i++)
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

        axis_y[i]->setRange(0, 100);

        chart[i]->setMargins(QMargins(0, 0, 0, 0));

        switch (i)
        {
        case 0:
            chart[i]->setTitle(tr("TT-01"));
            p0.setColor(QColor::fromRgb(87,192,255));
            series[i]->setPen(p0);
            ui->realTimeCurve_1->setChart(chart[0]);
            break;
        case 1:
            chart[i]->setTitle(tr("TT-02"));
            p0.setWidth(1);
            p0.setColor(QColor::fromRgb(81,223,0));
            series[i]->setPen(p0);
            ui->realTimeCurve_2->setChart(chart[i]);
            break;
        case 2:
            chart[i]->setTitle(tr("TT-03"));
            p0.setWidth(1);
            p0.setColor(QColor::fromRgb(255,87,193));
            series[i]->setPen(p0);
            ui->realTimeCurve_3->setChart(chart[i]);
            break;
        case 3:
            chart[i]->setTitle(tr("TT-04"));
            p0.setWidth(1);
            p0.setColor(QColor::fromRgb(252,43,43));
            series[i]->setPen(p0);
            ui->realTimeCurve_4->setChart(chart[i]);
            break;
        case 4:
            chart[i]->setTitle(tr("TT-05"));
            p0.setWidth(1);
            p0.setColor(QColor::fromRgb(255,199,87));
            series[i]->setPen(p0);
            ui->realTimeCurve_5->setChart(chart[i]);
            break;
        case 5:
            chart[i]->setTitle(tr("TT-06"));
            p0.setWidth(1);
            p0.setColor(QColor::fromRgb(30,206,226));
            series[i]->setPen(p0);
            ui->realTimeCurve_6->setChart(chart[i]);
            break;
        }
    }

    qDebug()<< ui->TT01_TT06_btn->styleSheet();
    qDebug()<< ui->TT07_TT12_btn->styleSheet();

    lastGroup = TT01_TT06;
    ui->TT01_TT06_btn->setStyleSheet(pressed_stylesheet);
    ui->TT07_TT12_btn->setStyleSheet(released_stylesheet);
    ui->TT13_TT18_btn->setStyleSheet(released_stylesheet);
    ui->TT19_TT24_btn->setStyleSheet(released_stylesheet);
    ui->TT25_TT30_btn->setStyleSheet(released_stylesheet);
    ui->TT31_TT36_btn->setStyleSheet(released_stylesheet);
    ui->pressure_btn->setStyleSheet(released_stylesheet);
    ui->flow_btn->setStyleSheet(released_stylesheet);
    ui->speed_1_btn->setStyleSheet(released_stylesheet);
    ui->speed_2_btn->setStyleSheet(released_stylesheet);
    ui->others_btn->setStyleSheet(released_stylesheet);
}

void RTCurve::setup_stylesheet(const DisplayGroups current_group, const DisplayGroups last_group)
{
    switch (last_group) {
    case TT01_TT06:
        ui->TT01_TT06_btn->setStyleSheet(released_stylesheet);
        break;
    case TT07_TT12:
        ui->TT07_TT12_btn->setStyleSheet(released_stylesheet);
        break;
    case TT13_TT18:
        ui->TT13_TT18_btn->setStyleSheet(released_stylesheet);
        break;
    case TT19_TT24:
        ui->TT19_TT24_btn->setStyleSheet(released_stylesheet);
        break;
    case TT25_TT30:
        ui->TT25_TT30_btn->setStyleSheet(released_stylesheet);
        break;
    case TT31_TT36:
        ui->TT31_TT36_btn->setStyleSheet(released_stylesheet);
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
    case TT01_TT06:
        ui->TT01_TT06_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT07_TT12:
        ui->TT07_TT12_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT13_TT18:
        ui->TT13_TT18_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT19_TT24:
        ui->TT19_TT24_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT25_TT30:
        ui->TT25_TT30_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT31_TT36:
        ui->TT31_TT36_btn->setStyleSheet(pressed_stylesheet);
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
    case TT01_TT06:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("TT-01"));break;
            case 1:chart[i]->setTitle(tr("TT-02"));break;
            case 2:chart[i]->setTitle(tr("TT-03"));break;
            case 3:chart[i]->setTitle(tr("TT-04"));break;
            case 4:chart[i]->setTitle(tr("TT-05"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("TT-06"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT01_TT06, lastGroup);
        lastGroup = TT01_TT06;

        break;
    case TT07_TT12:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("TT-07"));break;
            case 1:chart[i]->setTitle(tr("TT-08"));break;
            case 2:chart[i]->setTitle(tr("TT-09"));break;
            case 3:chart[i]->setTitle(tr("TT-10"));break;
            case 4:chart[i]->setTitle(tr("TT-11"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("TT-12"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT07_TT12, lastGroup);
        lastGroup = TT07_TT12;

        break;
    case TT13_TT18:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("TT-13"));break;
            case 1:chart[i]->setTitle(tr("TT-14"));break;
            case 2:chart[i]->setTitle(tr("TT-15"));break;
            case 3:chart[i]->setTitle(tr("TT-16"));break;
            case 4:chart[i]->setTitle(tr("TT-17"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("TT-18"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT13_TT18, lastGroup);
        lastGroup = TT13_TT18;

        break;
    case TT19_TT24:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("TT-19"));break;
            case 1:chart[i]->setTitle(tr("TT-20"));break;
            case 2:chart[i]->setTitle(tr("TT-21"));break;
            case 3:chart[i]->setTitle(tr("TT-22"));break;
            case 4:chart[i]->setTitle(tr("TT-23"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("TT-24"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT19_TT24, lastGroup);
        lastGroup = TT19_TT24;

        break;
    case TT25_TT30:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("TT-25"));break;
            case 1:chart[i]->setTitle(tr("TT-26"));break;
            case 2:chart[i]->setTitle(tr("TT-27"));break;
            case 3:chart[i]->setTitle(tr("TT-28"));break;
            case 4:chart[i]->setTitle(tr("TT-29"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("TT-30"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT25_TT30, lastGroup);
        lastGroup = TT25_TT30;

        break;
    case TT31_TT36:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("TT-31"));break;
            case 1:chart[i]->setTitle(tr("TT-32"));break;
            case 2:chart[i]->setTitle(tr("TT-33"));break;
            case 3:chart[i]->setTitle(tr("TT-34"));break;
            case 4:chart[i]->setTitle(tr("TT-35"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("TT-36"));chart[i]->show();break;
            }
        }

        setup_stylesheet(TT31_TT36, lastGroup);
        lastGroup = TT31_TT36;

        break;
    case PressureChart:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("PT-01"));break;
            case 1:chart[i]->setTitle(tr("PT-02"));break;
            case 2:chart[i]->setTitle(tr("PT-03"));break;
            case 3:chart[i]->setTitle(tr("PT-04"));break;
            case 4:chart[i]->setTitle(tr("PT-05"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("PT-06"));chart[i]->show();break;
            }
        }

        setup_stylesheet(PressureChart, lastGroup);
        lastGroup = PressureChart;

        break;
    case FlowChart:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("AFM-01"));break;
            case 1:chart[i]->setTitle(tr("AFM-02"));break;
            case 2:chart[i]->setTitle(tr("AFM-03"));break;
            case 3:chart[i]->setTitle(tr("AFM-04"));break;
            case 4:chart[i]->setTitle(tr("AFM-05"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("AFM-06"));chart[i]->show();break;
            }
        }

        setup_stylesheet(FlowChart, lastGroup);
        lastGroup = FlowChart;

        break;
    case SpeedChart_1:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("BL-01"));break;
            case 1:chart[i]->setTitle(tr("BL-02"));break;
            case 2:chart[i]->setTitle(tr("BL-03"));break;
            case 3:chart[i]->setTitle(tr("BL-04"));break;
            case 4:chart[i]->hide();break;
            case 5:chart[i]->hide();break;
            }
        }

        setup_stylesheet(SpeedChart_1, lastGroup);
        lastGroup = SpeedChart_1;

        break;
    case SpeedChart_2:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("PMP-01"));break;
            case 1:chart[i]->setTitle(tr("PMP-02"));break;
            case 2:chart[i]->setTitle(tr("PMP-03"));break;
            case 3:chart[i]->setTitle(tr("PMP-04"));break;
            case 4:chart[i]->setTitle(tr("PMP-05"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("PMP-06"));chart[i]->show();break;
            }
        }

        setup_stylesheet(SpeedChart_2, lastGroup);
        lastGroup = SpeedChart_2;

        break;
    case OthersChart:
        for (int i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:chart[i]->setTitle(tr("CM-01"));break;
            case 1:chart[i]->setTitle(tr("LT-01"));break;
            case 2:chart[i]->setTitle(tr("LT-02"));break;
            case 3:chart[i]->setTitle(tr("VT-01"));break;
            case 4:chart[i]->setTitle(tr("IT-01"));chart[i]->show();break;
            case 5:chart[i]->setTitle(tr("VT-02"));chart[i]->show();break;
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

void RTCurve::data_process(const QModbusDataUnit unit)
{

}

void RTCurve::on_TT01_TT06_btn_clicked()
{
    setup_charts_and_buttton(TT01_TT06);
}

void RTCurve::on_TT07_TT12_btn_clicked()
{
    setup_charts_and_buttton(TT07_TT12);
}

void RTCurve::on_TT13_TT18_btn_clicked()
{
    setup_charts_and_buttton(TT13_TT18);
}

void RTCurve::on_TT19_TT24_btn_clicked()
{
    setup_charts_and_buttton(TT19_TT24);
}

void RTCurve::on_TT25_TT30_btn_clicked()
{
    setup_charts_and_buttton(TT25_TT30);
}

void RTCurve::on_TT31_TT36_btn_clicked()
{
    setup_charts_and_buttton(TT31_TT36);
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

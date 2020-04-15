#include "rtcurve.h"
#include "ui_rtcurve.h"

RTCurve::RTCurve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RTCurve)
{
    ui->setupUi(this);

//    ui->realTimeCurve_1->setChart()

    QXYSeries *series = new QLineSeries();
    series->append(0, 8);
    series->append(2, 2);
    series->append(3, 8);
    series->append(4, 5);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(NULL);

    ui->realTimeCurve_1->setRenderHint(QPainter::Antialiasing);
    ui->realTimeCurve_1->setChart(chart);

    ui->realTimeCurve_10->hide();
}

RTCurve::~RTCurve()
{
    delete ui;
}

void RTCurve::data_process(QModbusDataUnit unit)
{

}

#include "rtcurve.h"
#include "ui_rtcurve.h"

RTCurve::RTCurve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RTCurve)
{
    ui->setupUi(this);
}

RTCurve::~RTCurve()
{
    delete ui;
}

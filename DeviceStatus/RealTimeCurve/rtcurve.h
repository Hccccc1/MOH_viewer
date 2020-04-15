#ifndef RTCURVE_H
#define RTCURVE_H

#include <QDialog>
#include <QtCharts>
#include <QChartView>
#include <QModbusDataUnit>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class RTCurve;
}

class RTCurve : public QDialog
{
    Q_OBJECT

public:
    explicit RTCurve(QWidget *parent = nullptr);
    ~RTCurve();

    void data_process(QModbusDataUnit unit);

private:
    Ui::RTCurve *ui;
};

#endif // RTCURVE_H

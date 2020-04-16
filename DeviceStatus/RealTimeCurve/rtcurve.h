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
    enum DisplayGroups
    {
        TT01_TT06,
        TT07_TT12,
        TT13_TT18,
        TT19_TT24,
        TT25_TT30,
        TT31_TT36,

        PressureChart,
        FlowChart,
        SpeedChart_1,
        SpeedChart_2,
        OthersChart,
    };

    explicit RTCurve(QWidget *parent = nullptr);
    ~RTCurve();

    void data_process(const QModbusDataUnit unit);

    QChart *chart[6];
    QLineSeries *series[6];
    QValueAxis *axis_x[6];
    QValueAxis *axis_y[6];

private:
    Ui::RTCurve *ui;

    DisplayGroups lastGroup;

    QString pressed_stylesheet = "QPushButton { background:rgba(87,192,255,1);          \
                                                border:1px solid rgba(204,204,204,1);   \
                                                border-radius:20px;                     \
                                                font-size:14px;                         \
                                                font-family:PingFang SC;                \
                                                font-weight:500;                        \
                                                line-height:34px;                       \
                                                color:rgba(255,255,255,1);}";

    QString released_stylesheet = "QPushButton { background:rgba(241,241,241,1);        \
                                                 border:1px solid rgba(204,204,204,1);  \
                                                 border-radius:20px;                    \
                                                 font-size:14px;                        \
                                                 font-family:PingFang SC;               \
                                                 font-weight:500;                       \
                                                 line-height:34px;                      \
                                                 color:rgba(97,97,97,1);}";

    void setup_charts_and_buttton(const DisplayGroups group);
    void setup_stylesheet(const DisplayGroups current_group, const DisplayGroups last_group);

private slots:
    void on_TT01_TT06_btn_clicked();
    void on_TT07_TT12_btn_clicked();
    void on_TT13_TT18_btn_clicked();
    void on_TT19_TT24_btn_clicked();
    void on_TT25_TT30_btn_clicked();
    void on_TT31_TT36_btn_clicked();
    void on_pressure_btn_clicked();
    void on_flow_btn_clicked();
    void on_speed_1_btn_clicked();
    void on_speed_2_btn_clicked();
    void on_others_btn_clicked();
};

#endif // RTCURVE_H

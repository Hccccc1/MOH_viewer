#ifndef RTCURVE_H
#define RTCURVE_H

#include <QDialog>
#include <QModbusDataUnit>
#include "DeviceStatus/DevStatus_regs.h"
#include "Modbus/modbusserial.h"
#include "3rdparty/QCustomPlot/qcustomplot.h"
#include "DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h"
#include "AllBitsAndRegs.h"

//QT_CHARTS_USE_NAMESPACE

namespace Ui {
class RTCurve;
}

class RTCurve : public QDialog
{
    Q_OBJECT

public:
    explicit RTCurve(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, Accounts account = Customer);
    ~RTCurve();

    void data_process(const QModbusDataUnit unit);

    QCustomPlot *plots[max_charts_num];
    QCPTextElement *title[max_charts_num];

private:
    Ui::RTCurve *ui;

    DisplayGroups lastGroup;

    QString pressed_stylesheet = "QPushButton { background:rgba(87,192,255,1);          \
                                                border:1px solid rgba(204,204,204,1);   \
                                                border-radius:20px;                     \
                                                font-size:14px;                         \
                                                font-family:PingFang SC;                \
                                                font-weight:300;                        \
                                                line-height:34px;                       \
                                                color:rgba(255,255,255,1);}";

    QString released_stylesheet = "QPushButton { background:rgba(241,241,241,1);        \
                                                 border:1px solid rgba(204,204,204,1);  \
                                                 border-radius:20px;                    \
                                                 font-size:14px;                        \
                                                 font-family:PingFang SC;               \
                                                 font-weight:300;                       \
                                                 line-height:34px;                      \
                                                 color:rgba(97,97,97,1);}";

    ModbusSerial *current_serial;
    Accounts current_account;

    QVector<double> data;

//    HistoryValuesDatabase *db = new HistoryValuesDatabase();

    void plot_set_color();
    void setup_charts_and_buttton(const DisplayGroups group);
    void setup_stylesheet(const DisplayGroups current_group, const DisplayGroups last_group);

private slots:
    void on_TT01_TT08_btn_clicked();
    void on_TT09_TT16_btn_clicked();
    void on_TT17_TT24_btn_clicked();
    void on_TT25_TT32_btn_clicked();
    void on_TT33_TT36_btn_clicked();
    void on_pressure_btn_clicked();
    void on_flow_btn_clicked();
    void on_speed_1_btn_clicked();
    void on_speed_2_btn_clicked();
    void on_others_btn_clicked();
//    void on_readButton_clicked();

    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

    void refreshCurrentPage();

    void on_checkBox_chart_1_stateChanged(int state);
    void on_checkBox_chart_2_stateChanged(int state);
    void on_checkBox_chart_3_stateChanged(int state);
    void on_checkBox_chart_4_stateChanged(int state);
    void on_checkBox_chart_5_stateChanged(int state);
    void on_checkBox_chart_6_stateChanged(int state);
    void on_checkBox_chart_7_stateChanged(int state);
    void on_checkBox_chart_8_stateChanged(int state);

protected:
    void resizeEvent(QResizeEvent *event);

    virtual void timerEvent(QTimerEvent *);

Q_SIGNALS:
    void dataChanged(QString);

};

#endif // RTCURVE_H

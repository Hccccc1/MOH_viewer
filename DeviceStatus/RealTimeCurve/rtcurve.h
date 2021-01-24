#ifndef RTCURVE_H
#define RTCURVE_H

#include <QWidget>
#include <QTimer>
#include <QModbusDataUnit>
#include "ModbusSerial/modbusserial.h"
#include "DeviceStatus/DevStatus_regs.h"
#include "AllBitsAndRegs.h"
#include "DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h"

#include "3rdparty/QCustomPlot/qcustomplot.h"

namespace Ui {
class RTCurve;
}

class RTCurve : public QWidget
{
    Q_OBJECT

public:
    explicit RTCurve(QWidget *parent = nullptr, ModbusSerial *serial = nullptr, Accounts account = SuperUser);
    ~RTCurve();

    void refreshCurrentPage();
    void data_process(QModbusDataUnit unit);

public slots:
    void start_save_timer();
    void stop_save_timer();

private:
    Ui::RTCurve *ui;
    Accounts current_account;
    ModbusSerial *current_serial = nullptr;

    QVector<QCheckBox *> checkboxes;
    QVector<QLabel *> pic_labels;
    QVector<QLabel *> text_labels;
    QVector<QCustomPlot *> plots;
    QVector<QCPTextElement *> title;

    QTimer *save_timer = new QTimer(this);

    QVector<QVector<quint16>> last_record;
    QQueue<QVector<QVector<quint16>>> buffer;

//    void set_widgets_size();
    void plot_set_color();
    void setup_charts_checkboxes(DisplayGroups group);

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_checkBox_chart_1_stateChanged(int state);
    void on_checkBox_chart_2_stateChanged(int state);
    void on_checkBox_chart_3_stateChanged(int state);
    void on_checkBox_chart_4_stateChanged(int state);
    void on_checkBox_chart_5_stateChanged(int state);
    void on_checkBox_chart_6_stateChanged(int state);
    void on_checkBox_chart_7_stateChanged(int state);
    void on_checkBox_chart_8_stateChanged(int state);

    void plots_mouseMove(QMouseEvent *event);

    void save_timer_timeout();

protected:
    virtual void timerEvent(QTimerEvent *);
    virtual void changeEvent(QEvent *);

Q_SIGNALS:
//    void dataChanged(QString);
};

#endif // RTCURVE_H

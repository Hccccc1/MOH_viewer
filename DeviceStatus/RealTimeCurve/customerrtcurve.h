#ifndef CUSTOMERRTCURVE_H
#define CUSTOMERRTCURVE_H

#include <QWidget>
#include "ModbusSerial/modbusserial.h"
#include "AllBitsAndRegs.h"
#include "DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h"

#include "3rdparty/QCustomPlot/qcustomplot.h"

namespace Ui {
class CustomerRTCurve;
}

class CustomerRTCurve : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerRTCurve(QWidget *parent = nullptr, ModbusSerial *serial = nullptr);
    ~CustomerRTCurve();

    void refreshCurrentPage();
    void data_process(QModbusDataUnit unit);

public slots:
    void start_save_timer();
    void stop_save_timer();

private:
    Ui::CustomerRTCurve *ui;
    ModbusSerial *current_serial = nullptr;

//    QVector<QCheckBox *> checkboxes;
//    QVector<QLabel *> pic_labels;
//    QVector<QLabel *> text_labels;
    QVector<QCustomPlot *> plots;
    QVector<QCPTextElement *> title;

    QTimer *save_timer = new QTimer(this);

    QVector<QVector<quint16>> last_record;
    QQueue<QVector<QVector<quint16>>> buffer;

    void plot_set_color();

private slots:
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
    virtual void changeEvent(QEvent *);
    virtual void timerEvent(QTimerEvent *);
};

#endif // CUSTOMERRTCURVE_H

#ifndef CUSTOMER_HISTORYCURVE_H
#define CUSTOMER_HISTORYCURVE_H

#include <QWidget>

//#include "DeviceStatus/DevStatus_regs.h"
#include "DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h"

#include "3rdparty/QCustomPlot/qcustomplot.h"

namespace Ui {
class customer_HistoryCurve;
}

class customer_HistoryCurve : public QWidget
{
    Q_OBJECT

public:
    explicit customer_HistoryCurve(QWidget *parent = nullptr, QMutex *ope_mutex = nullptr);
    ~customer_HistoryCurve();

private:
    Ui::customer_HistoryCurve *ui;

    QVector<QCustomPlot *> plots;
    QVector<QCPTextElement *> titles;

    QMutex *operation_mutex = nullptr;

    void plot_set_color();
    QVector<qint64> get_time_interval(int index, QDateTime start, QDateTime end);

private slots:
    void on_checkBox_customer_chart_1_stateChanged(int state);
    void on_checkBox_customer_chart_2_stateChanged(int state);
    void on_checkBox_customer_chart_3_stateChanged(int state);
    void on_checkBox_customer_chart_4_stateChanged(int state);
    void on_checkBox_customer_chart_5_stateChanged(int state);
    void on_checkBox_customer_chart_6_stateChanged(int state);
    void on_checkBox_customer_chart_7_stateChanged(int state);
    void on_checkBox_customer_chart_8_stateChanged(int state);

    void on_searchData_customer_clicked();
    void on_exportData_customer_clicked();
    void on_quickSearch_customer_currentIndexChanged(int index);

    void display_history_values(QVector<QVector<double>> result);

    void plots_mouseMove(QMouseEvent *event);

protected:
    virtual void changeEvent(QEvent *);
};

#endif // CUSTOMER_HISTORYCURVE_H

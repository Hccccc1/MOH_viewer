#ifndef HISCURVE_H
#define HISCURVE_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>

#include "DeviceStatus/DevStatus_regs.h"
#include "DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h"

#include "3rdparty/QCustomPlot/qcustomplot.h"

namespace Ui {
class HisCurve;
}

class HisCurve : public QWidget
{
    Q_OBJECT

public:
    explicit HisCurve(QWidget *parent = nullptr, QMutex *ope_mutex = nullptr);
    ~HisCurve();

private:
    Ui::HisCurve *ui;

    QVector<QCheckBox *> checkboxes;
    QVector<QLabel *> pic_labels;
//    QVector<QLabel *> text_labels;
    QVector<QCustomPlot *> plots;
    QVector<QCPTextElement *> title;

    QMutex *operation_mutex = nullptr;

    void plot_set_color();
    void setup_charts_checkboxes(DisplayGroups group);

    QVector<qint64> get_time_interval(int index, QDateTime start, QDateTime end);

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

    void on_searchData_clicked();
    void on_exportData_clicked();

    void display_history_values(QVector<QVector<double>> result);
    void on_quickSearch_currentIndexChanged(int index);

    void plots_mouseMove(QMouseEvent *event);

Q_SIGNALS:
//    void dataChanged(QString);
    void operation_needs_lock();
    void operation_release_lock();

protected:
    virtual void changeEvent(QEvent *);
};

#endif // HISCURVE_H

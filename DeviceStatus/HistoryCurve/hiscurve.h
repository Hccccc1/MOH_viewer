#ifndef HISCURVE_H
#define HISCURVE_H

#include <QDialog>
#include "DeviceStatus/DevStatus_regs.h"
#include "3rdparty/QCustomPlot/qcustomplot.h"
#include "DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h"

namespace Ui {
class HisCurve;
}

class HisCurve : public QDialog
{
    Q_OBJECT

public:
    explicit HisCurve(QWidget *parent = nullptr);
    ~HisCurve();

    QCustomPlot *plots[max_charts_num];
    QCPTextElement *title[max_charts_num];

//    HistoryValuesDatabase db;

//    void open_database();
//    void close_database();

private:
    Ui::HisCurve *ui;

    QVector<double> data;

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

    void plot_set_color();
    void setup_charts_and_buttton(const DisplayGroups group);
    void setup_stylesheet(const DisplayGroups current_group, const DisplayGroups last_group);

    void display_history_values(QVector<QVector<double>> result);

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

    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

    void on_searchData_clicked();
    void on_quickSearch_currentIndexChanged(int index);

protected:
    void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
    void dataChanged(QString);
};

#endif // HISCURVE_H

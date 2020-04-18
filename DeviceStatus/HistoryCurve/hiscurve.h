#ifndef HISCURVE_H
#define HISCURVE_H

#include <QDialog>
#include "DeviceStatus/DevStatus_regs.h"

namespace Ui {
class HisCurve;
}

class HisCurve : public QDialog
{
    Q_OBJECT

public:
    explicit HisCurve(QWidget *parent = nullptr);
    ~HisCurve();

    QChart *chart[max_charts_num];
    QLineSeries *series[max_charts_num];
    QValueAxis *axis_x[max_charts_num], *axis_y[max_charts_num];

private:
    Ui::HisCurve *ui;

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
};

#endif // HISCURVE_H

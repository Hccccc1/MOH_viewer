#include "rtcurve.h"
#include "ui_rtcurve.h"
#include "3rdparty/QCustomPlot/qcustomplot.h"

RTCurve::RTCurve(QWidget *parent, ModbusSerial *serial, Accounts account) :
    QWidget(parent),
    ui(new Ui::RTCurve),
    current_account(account),
    current_serial(serial)
{
    ui->setupUi(this);

    checkboxes.resize(max_charts_num);
    pic_labels.resize(max_charts_num);
    text_labels.resize(max_charts_num);

    checkboxes[0] = ui->checkBox_chart_1;
    checkboxes[1] = ui->checkBox_chart_2;
    checkboxes[2] = ui->checkBox_chart_3;
    checkboxes[3] = ui->checkBox_chart_4;
    checkboxes[4] = ui->checkBox_chart_5;
    checkboxes[5] = ui->checkBox_chart_6;
    checkboxes[6] = ui->checkBox_chart_7;
    checkboxes[7] = ui->checkBox_chart_8;

    pic_labels[0] = ui->picLabel_1;
    pic_labels[1] = ui->picLabel_2;
    pic_labels[2] = ui->picLabel_3;
    pic_labels[3] = ui->picLabel_4;
    pic_labels[4] = ui->picLabel_5;
    pic_labels[5] = ui->picLabel_6;
    pic_labels[6] = ui->picLabel_7;
    pic_labels[7] = ui->picLabel_8;

    text_labels[0] = ui->real_time_value_1;
    text_labels[1] = ui->real_time_value_2;
    text_labels[2] = ui->real_time_value_3;
    text_labels[3] = ui->real_time_value_4;
    text_labels[4] = ui->real_time_value_5;
    text_labels[5] = ui->real_time_value_6;
    text_labels[6] = ui->real_time_value_7;
    text_labels[7] = ui->real_time_value_8;

    plots.resize(max_charts_num);
    title.resize(max_charts_num);

    for (int i = 0; i < plots.size(); i++)
    {
        plots[i] = new QCustomPlot(ui->plots_groupbox);
        title[i] = new QCPTextElement(plots[i], "", QFont("PingFang SC", 17, 300));

        plots[i]->plotLayout()->insertRow(0);
        plots[i]->plotLayout()->addElement(0, 0, title[i]);

        plots[i]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        plots[i]->addGraph();

        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("MM-dd hh:mm");
        plots[i]->xAxis->setTicker(dateTicker);

        if (i < 4)
            ui->gridLayout_4->addWidget(plots[i], i, 0, 1, 1);
        else
            ui->gridLayout_4->addWidget(plots[i], i-4, 1, 1, 1);

        connect(plots[i], &QCustomPlot::mouseMove, this, &RTCurve::plots_mouseMove);
    }

    setup_charts_checkboxes(TT01_TT08);

    //    startTimer(1000);
}

RTCurve::~RTCurve()
{
    delete ui;
    delete current_serial;
}

void RTCurve::data_process(QModbusDataUnit unit)
{
    QVector<QVector<quint16>> values(10);
    //    values.resize(10);

    QVector<double> time(1), value(1);

    time[0] = QDateTime::currentMSecsSinceEpoch()/1000;

    for (int i = 0; i < int(unit.valueCount()); i++)
    {
        const int addr = unit.startAddress() + i;

        switch (addr) {
        case InputRegs_TT_01:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT01_TT08)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_02:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT01_TT08)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_03:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT01_TT08)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_04:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT01_TT08)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_05:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT01_TT08)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();

                ui->real_time_value_5->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_06:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT01_TT08)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();

                ui->real_time_value_6->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_07:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT01_TT08)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();

                ui->real_time_value_7->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_08:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT01_TT08)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();

                ui->real_time_value_8->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_09:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT09_TT16)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_10:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT09_TT16)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_11:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT09_TT16)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_12:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT09_TT16)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_13:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT09_TT16)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();

                ui->real_time_value_5->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_14:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT09_TT16)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();

                ui->real_time_value_6->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_15:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT09_TT16)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();

                ui->real_time_value_7->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_16:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT09_TT16)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();

                ui->real_time_value_8->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_17:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT17_TT24)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_18:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT17_TT24)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_19:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT17_TT24)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_20:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT17_TT24)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_21:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT17_TT24)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();

                ui->real_time_value_5->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_22:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT17_TT24)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();

                ui->real_time_value_6->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_23:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT17_TT24)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();

                ui->real_time_value_7->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_24:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT17_TT24)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();

                ui->real_time_value_8->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_25:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT25_TT32)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_26:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT25_TT32)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_27:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT25_TT32)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_28:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT25_TT32)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1°C").arg(unit.value(i)));
            }
            break;
        case InputRegs_TT_29:
            value[0] = qint16(unit.value(i));

            values[TT25_TT32].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT25_TT32)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();

                ui->real_time_value_5->setText(QString("%1°C").arg(qint16(unit.value(i))));
            }
            break;
        case InputRegs_TT_30:
            value[0] = qint16(unit.value(i));

            values[TT25_TT32].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT25_TT32)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();

                ui->real_time_value_6->setText(QString("%1°C").arg(qint16(unit.value(i))));
            }
            break;
        case InputRegs_TT_31:
            value[0] = qint16(unit.value(i));

            values[TT25_TT32].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT25_TT32)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();

                ui->real_time_value_7->setText(QString("%1°C").arg(qint16(unit.value(i))));
            }
            break;
        case InputRegs_TT_32:
            value[0] = qint16(unit.value(i));

            values[TT25_TT32].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT25_TT32)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();

                ui->real_time_value_8->setText(QString("%1°C").arg(qint16(unit.value(i))));
            }
            break;
        case InputRegs_TT_33:
            value[0] = qint16(unit.value(i));

            values[TT33_TT36].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT33_TT36)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1°C").arg(qint16(unit.value(i))));
            }
            break;
        case InputRegs_TT_34:
            value[0] = qint16(unit.value(i));

            values[TT33_TT36].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT33_TT36)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1°C").arg(qint16(unit.value(i))));
            }
            break;
        case InputRegs_TT_35:
            value[0] = qint16(unit.value(i));

            values[TT33_TT36].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT33_TT36)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1°C").arg(qint16(unit.value(i))));
            }
            break;
        case InputRegs_TT_36:
            value[0] = qint16(unit.value(i));

            values[TT33_TT36].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == TT33_TT36)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1°C").arg(qint16(unit.value(i))));
            }
            break;
        case InputRegs_PT_01:
            value[0] = double(qint16(unit.value(i)))/100;

            values[PressureChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == PressureChart)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1KPa").arg(value[0]));
            }
            break;
        case InputRegs_PT_02:
            value[0] = double(qint16(unit.value(i)))/100;

            values[PressureChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == PressureChart)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1KPa").arg(value[0]));
            }
            break;
        case InputRegs_PT_03:
            value[0] = double(qint16(unit.value(i)))/100;

            values[PressureChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == PressureChart)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1KPa").arg(value[0]));
            }
            break;
        case InputRegs_PT_04:
            value[0] = double(qint16(unit.value(i)))/100;

            values[PressureChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == PressureChart)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1KPa").arg(value[0]));
            }
            break;
        case InputRegs_PT_05:
            value[0] = double(qint16(unit.value(i)))/100;

            values[PressureChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == PressureChart)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();

                ui->real_time_value_5->setText(QString("%1KPa").arg(value[0]));
            }
            break;
        case InputRegs_PT_06:
            value[0] = double(qint16(unit.value(i)))/100;

            values[PressureChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == PressureChart)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();

                ui->real_time_value_6->setText(QString("%1KPa").arg(value[0]));
            }
            break;

        case InputRegs_AFM_01:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == FlowChart)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1slm").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_AFM_02:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == FlowChart)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1slm").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_AFM_03:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == FlowChart)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1slm").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_AFM_04:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == FlowChart)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1slm").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_MFM_01:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == FlowChart)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();

                ui->real_time_value_5->setText(QString("%1g/min").arg(double(unit.value(i))/10));
            }
            break;

        case InputRegs_BL_01:
            value[0] = unit.value(i);

            values[SpeedChart_1].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_1)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;
        case InputRegs_BL_02:
            value[0] = unit.value(i);

            values[SpeedChart_1].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_1)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;
        case InputRegs_BL_03:
            value[0] = unit.value(i);

            values[SpeedChart_1].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_1)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;
        case InputRegs_BL_04:
            value[0] = unit.value(i);

            values[SpeedChart_1].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_1)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;

        case InputRegs_PMP_01:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_2)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;
        case InputRegs_PMP_02:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_2)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;
        case InputRegs_PMP_03:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_2)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;
        case InputRegs_PMP_04:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_2)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;
        case InputRegs_PMP_05:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_2)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();

                ui->real_time_value_5->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;
        case InputRegs_RAD_01:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == SpeedChart_2)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();

                ui->real_time_value_6->setText(QString("%1rpm").arg(unit.value(i)));
            }
            break;

            //        case InputRegs_CM_01:
            //            value[0] = double(unit.value(i))/10;

            //            values[OthersChart].push_back(unit.value(i));

            //            if (ui->tabWidget->currentIndex() == OthersChart)
            //            {
            //                plots[0]->graph(0)->addData(time, value);
            //                plots[0]->graph(0)->rescaleAxes();
            //                plots[0]->replot();

            //                ui->real_time_value_1->setText(QString("%1us/cm").arg(double(unit.value(i))/10));
            //            }
            //            break;
        case InputRegs_LT_01:

            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == OthersChart)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();

                ui->real_time_value_1->setText(QString("%1cm").arg(unit.value(i)));
            }
            break;
        case InputRegs_LT_02:

            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == OthersChart)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();

                ui->real_time_value_2->setText(QString("%1cm").arg(unit.value(i)));
            }
            break;
        case InputRegs_VT_01:

            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == OthersChart)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();

                ui->real_time_value_3->setText(QString("%1V").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_IT_01:

            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == OthersChart)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();

                ui->real_time_value_4->setText(QString("%1A").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_FcPower:

            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == OthersChart)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();

                ui->real_time_value_5->setText(QString("%1W").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_OutPower:

            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == OthersChart)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();

                ui->real_time_value_6->setText(QString("%1W").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_VT_02:

            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == OthersChart)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();

                ui->real_time_value_7->setText(QString("%1V").arg(double(unit.value(i))/10));
            }
            break;
        case InputRegs_IT_02:

            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            if (ui->tabWidget->currentIndex() == OthersChart)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();

                ui->real_time_value_8->setText(QString("%1A").arg(double(unit.value(i))/10));
            }
            break;

        default:break;
        }
    }

    if (unit.valueCount() == 77)
    {
        int save_time_ms = current_serial->settings().save_interval * 1000 - 100;
        HistoryValuesDatabase db(current_serial->settings().slave_addr);

        if (save_timer == Q_NULLPTR)
        {
            save_timer = new QTimer(this);
            save_timer->start(save_time_ms);
            save_timer->setSingleShot(true);
            for (int i = 0; i < 1000000;i++)
            db.insert_values_to_tables(values);
        }
        else if (save_timer  && !save_timer->isActive())
        {
            save_timer->start(save_time_ms);
            db.insert_values_to_tables(values);
            qDebug() << QDateTime::currentDateTime() << "saved";
        }
        else
        {

        }
    }

}

void RTCurve::on_tabWidget_currentChanged(int index)
{
    QWidget *tmp_widget = ui->tabWidget->currentWidget();

    ui->groupBox->setParent(tmp_widget);
    ui->groupBox->show();

    ui->gridLayout_3->setParent(tmp_widget);
    ui->plots_groupbox->setParent(tmp_widget);
    ui->plots_groupbox->show();

    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    setup_charts_checkboxes(DisplayGroups(index));

    plot_set_color();
}

void RTCurve::setup_charts_checkboxes(DisplayGroups group)
{
    switch (group) {
    case TT01_TT08:
        for (int i = 0; i < plots.size(); i++)
        {
            checkboxes[i]->setText(QString("TT-%1").arg(i+1));
            title[i]->setText(QString("TT-%1(°C)").arg(i+1));
            plots[i]->replot();

            if (i < 8)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
                plots[i]->hide();
        }

        plot_set_color();

        break;
    case TT09_TT16:
        for (int i = 0; i < plots.size(); i++)
        {
            checkboxes[i]->setText(QString("TT-%1").arg(i+9));
            title[i]->setText(QString("TT-%1(°C)").arg(i+9));
            plots[i]->replot();

            if (i < 8)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
                plots[i]->hide();
        }

        plot_set_color();

        break;
    case TT17_TT24:
        for (int i = 0; i < plots.size(); i++)
        {
            checkboxes[i]->setText(QString("TT-%1").arg(i+17));
            title[i]->setText(QString("TT-%1(°C)").arg(i+17));
            plots[i]->replot();

            if (i < 8)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
                plots[i]->hide();
        }

        plot_set_color();

        break;
    case TT25_TT32:
        for (int i = 0; i < plots.size(); i++)
        {
            checkboxes[i]->setText(QString("TT-%1").arg(i+25));
            title[i]->setText(QString("TT-%1(°C)").arg(i+25));
            plots[i]->replot();

            if (i < 8)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
                plots[i]->hide();
        }

        plot_set_color();

        break;
    case TT33_TT36:
        for (int i = 0; i < plots.size(); i++)
        {
            checkboxes[i]->setText(QString("TT-%1").arg(i+33));
            title[i]->setText(QString("TT-%1(°C)").arg(i+33));
            plots[i]->replot();

            if (i < 4)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
                text_labels[i]->hide();
                plots[i]->hide();
            }
        }

        plot_set_color();

        break;
    case PressureChart:
        for (int i = 0; i < plots.size(); i++)
        {
            checkboxes[i]->setText(QString("PT-%1").arg(i+1));
            title[i]->setText(QString("PT-%1(KPa)").arg(i+1));
            plots[i]->replot();

            if (i < 6)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
                text_labels[i]->hide();
                plots[i]->hide();
            }
        }

        plot_set_color();

        break;
    case FlowChart:
        for (int i = 0; i < plots.size(); i++)
        {
            if (i < 4)
            {
                checkboxes[i]->setText(QString("AFM-%1").arg(i+1));
                title[i]->setText(QString("AFM-%1(slm)").arg(i+1));
            }
            else
            {
                checkboxes[i]->setText(QString("MFM-1"));
                title[i]->setText(QString("MFM-1(g/min)"));
            }

            plots[i]->replot();

            if (i < 5)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
                text_labels[i]->hide();
                plots[i]->hide();
            }
        }

        plot_set_color();

        break;
    case SpeedChart_1:
        for (int i = 0; i < plots.size(); i++)
        {
            checkboxes[i]->setText(QString("BL-%1").arg(i+1));
            title[i]->setText(QString("BL-%1(rpm)").arg(i+1));
            plots[i]->replot();

            if (i < 4)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
                text_labels[i]->hide();
                plots[i]->hide();
            }
        }

        plot_set_color();

        break;
    case SpeedChart_2:
        for (int i = 0; i < plots.size(); i++)
        {
            if (i < 5)
            {
                checkboxes[i]->setText(QString("PMP-%1").arg(i+1));
                title[i]->setText(QString("PMP-%1(rpm)").arg(i+1));
            }
            else
            {
                checkboxes[i]->setText(QString("RAD-%1").arg(1));
                title[i]->setText(QString("RAD-%1(rpm)").arg(1));
            }

            plots[i]->replot();

            if (i < 6)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
                text_labels[i]->hide();
                plots[i]->hide();
            }
        }

        plot_set_color();

        break;
    case OthersChart:
        //        checkboxes[0]->setText("CM-01");
        //        title[0]->setText("CM-01(us/cm)");

        checkboxes[0]->setText("LT-01");
        title[0]->setText("LT-01(cm)");

        checkboxes[1]->setText("LT-02");
        title[1]->setText("LT-02(cm)");

        checkboxes[2]->setText("VT-01");
        title[2]->setText("VT-01(V)");

        checkboxes[3]->setText("IT-01");
        title[3]->setText("IT-01(A)");

        checkboxes[4]->setText("FCPower");
        title[4]->setText("FCPower(W)");

        checkboxes[5]->setText("OutPower");
        title[5]->setText("OutPower(W)");

        checkboxes[6]->setText("VT-02");
        title[6]->setText("VT-02(V)");

        checkboxes[7]->setText("IT-02");
        title[7]->setText("IT-02(A)");


        for (int i = 0; i < plots.size(); i++)
        {
            //            if (i < 7)
            //            {
            checkboxes[i]->show();
            pic_labels[i]->show();
            text_labels[i]->show();
            checkboxes[i]->setChecked(true);
            plots[i]->show();
            //            }

            plots[i]->replot();
        }

        //        checkboxes[7]->hide();
        //        pic_labels[7]->hide();
        //        text_labels[7]->hide();
        //        plots[7]->hide();

        plot_set_color();

        break;
    }
}

void RTCurve::plot_set_color()
{
    QPen p0;
    p0.setWidth(3);

    for (int i = 0; i < plots.size(); i++)
    {
        switch (i)
        {
        case 0:
            p0.setColor(QColor::fromRgb(87,192,255));
            plots[i]->graph(0)->setPen(p0);
            break;
        case 1:
            p0.setColor(QColor::fromRgb(81,223,0));
            plots[i]->graph(0)->setPen(p0);
            break;
        case 2:
            p0.setColor(QColor::fromRgb(255,87,193));
            plots[i]->graph(0)->setPen(p0);
            break;
        case 3:
            p0.setColor(QColor::fromRgb(252,43,43));
            plots[i]->graph(0)->setPen(p0);
            break;
        case 4:
            p0.setColor(QColor::fromRgb(255,199,87));
            plots[i]->graph(0)->setPen(p0);
            break;
        case 5:
            p0.setColor(QColor::fromRgb(30,206,226));
            plots[i]->graph(0)->setPen(p0);
            break;
        case 6:
            p0.setColor(QColor::fromRgb(87,121,255));
            plots[i]->graph(0)->setPen(p0);
            break;
        case 7:
            p0.setColor(QColor::fromRgb(200,87,255));
            plots[i]->graph(0)->setPen(p0);
            break;
        }
    }
}

void RTCurve::on_checkBox_chart_1_stateChanged(int state)
{

    if (state == Qt::Checked)
        plots[0]->show();
    else
        plots[0]->hide();
}

void RTCurve::on_checkBox_chart_2_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[1]->show();
    else
        plots[1]->hide();
}

void RTCurve::on_checkBox_chart_3_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[2]->show();
    else
        plots[2]->hide();
}

void RTCurve::on_checkBox_chart_4_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[3]->show();
    else
        plots[3]->hide();
}

void RTCurve::on_checkBox_chart_5_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[4]->show();
    else
        plots[4]->hide();
}

void RTCurve::on_checkBox_chart_6_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[5]->show();
    else
        plots[5]->hide();
}

void RTCurve::on_checkBox_chart_7_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[6]->show();
    else
        plots[6]->hide();
}

void RTCurve::on_checkBox_chart_8_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[7]->show();
    else
        plots[7]->hide();
}

void RTCurve::refreshCurrentPage()
{
    //    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    if (current_serial->is_serial_connected())
        current_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_TT_01, 77);
}

void RTCurve::timerEvent(QTimerEvent *)
{
    refreshCurrentPage();
}

void RTCurve::plots_mouseMove(QMouseEvent *event)
{
    QCustomPlot* plot = qobject_cast<QCustomPlot *>(sender());
    QSharedPointer<QCPGraphDataContainer> real_data = plot->graph(0)->data();

    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    //    qDebug() << __func__ << __LINE__ << x << y;

    for (int i = 0; i < real_data->size(); i++)
    {
        if ( qint64(real_data->findBegin(x)->key*1000) == qint64(real_data->at(i)->key*1000) )
        {
            if ( qAbs(qint64(y*10) - qint64(real_data->at(i)->value*10)) < 10 )
            {
                QString toolTips = QDateTime::fromMSecsSinceEpoch(qint64(real_data->at(i)->key*1000)).toString("yyyy-MM-dd hh:mm:ss");
                toolTips += " ";
                toolTips += QString::number(real_data->at(i)->value, 'f', 1);
                //                setAttribute(Qt::WA_AlwaysShowToolTips);
                //                setToolTip(toolTips);
                QToolTip::showText(event->globalPos(), toolTips, plot);
            }
            else
                setToolTip("");
        }
    }
}

void RTCurve::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

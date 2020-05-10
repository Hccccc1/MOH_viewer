#include "hiscurve.h"
#include "ui_hiscurve.h"

#include "LoginInterface/logininterface.h"
#include "DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h"

HisCurve::HisCurve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HisCurve)
{
    ui->setupUi(this);

    ui->tableWidget->setRowCount(4);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();

    for (int i = 0; i < max_charts_num; i++)
    {
        plots[i] = new QCustomPlot();
        title[i] = new QCPTextElement(plots[i], QString("TT-%01").arg(i), QFont("PingFang SC", 17, 300));
        plots[i]->plotLayout()->insertRow(0);
        plots[i]->plotLayout()->addElement(0, 0, title[i]);

        plots[i]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        plots[i]->addGraph();

        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("hh:mm:ss");
        plots[i]->xAxis->setTicker(dateTicker);
    }

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            if ( j == 1 )
                ui->tableWidget->setCellWidget(i, j, plots[i*j+4]);
            else
                ui->tableWidget->setCellWidget(i, j, plots[i]);
        }
    }

    for (const auto *plot : plots)
    {
        connect(plot, &QCustomPlot::plottableClick, this, &HisCurve::graphClicked);
    }

    lastGroup = TT01_TT08;
    ui->TT01_TT08_btn->setStyleSheet(pressed_stylesheet);
    ui->TT09_TT16_btn->setStyleSheet(released_stylesheet);
    ui->TT17_TT24_btn->setStyleSheet(released_stylesheet);
    ui->TT25_TT32_btn->setStyleSheet(released_stylesheet);
    ui->TT33_TT36_btn->setStyleSheet(released_stylesheet);
    ui->pressure_btn->setStyleSheet(released_stylesheet);
    ui->flow_btn->setStyleSheet(released_stylesheet);
    ui->speed_1_btn->setStyleSheet(released_stylesheet);
    ui->speed_2_btn->setStyleSheet(released_stylesheet);
    ui->others_btn->setStyleSheet(released_stylesheet);

    plot_set_color();

    ui->checkBox_chart_1->setCheckState(Qt::Checked);
    ui->checkBox_chart_2->setCheckState(Qt::Checked);
    ui->checkBox_chart_3->setCheckState(Qt::Checked);
    ui->checkBox_chart_4->setCheckState(Qt::Checked);
    ui->checkBox_chart_5->setCheckState(Qt::Checked);
    ui->checkBox_chart_6->setCheckState(Qt::Checked);
    ui->checkBox_chart_7->setCheckState(Qt::Checked);
    ui->checkBox_chart_8->setCheckState(Qt::Checked);

    ui->startDateTimeEdit->setDisabled(true);
    ui->endDateTimeEdit->setDisabled(true);
}

HisCurve::~HisCurve()
{
    delete ui;

    delete plots[8];
    delete title[8];
}

void HisCurve::setup_stylesheet(const DisplayGroups current_group, const DisplayGroups last_group)
{
    switch (last_group) {
    case TT01_TT08:
        ui->TT01_TT08_btn->setStyleSheet(released_stylesheet);
        break;
    case TT09_TT16:
        ui->TT09_TT16_btn->setStyleSheet(released_stylesheet);
        break;
    case TT17_TT24:
        ui->TT17_TT24_btn->setStyleSheet(released_stylesheet);
        break;
    case TT25_TT32:
        ui->TT25_TT32_btn->setStyleSheet(released_stylesheet);
        break;
    case TT33_TT36:
        ui->TT33_TT36_btn->setStyleSheet(released_stylesheet);
        break;
    case PressureChart:
        ui->pressure_btn->setStyleSheet(released_stylesheet);
        break;
    case FlowChart:
        ui->flow_btn->setStyleSheet(released_stylesheet);
        break;
    case SpeedChart_1:
        ui->speed_1_btn->setStyleSheet(released_stylesheet);
        break;
    case SpeedChart_2:
        ui->speed_2_btn->setStyleSheet(released_stylesheet);
        break;
    case OthersChart:
        ui->others_btn->setStyleSheet(released_stylesheet);
        break;
    }

    switch (current_group) {
    case TT01_TT08:
        ui->TT01_TT08_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT09_TT16:
        ui->TT09_TT16_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT17_TT24:
        ui->TT17_TT24_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT25_TT32:
        ui->TT25_TT32_btn->setStyleSheet(pressed_stylesheet);
        break;
    case TT33_TT36:
        ui->TT33_TT36_btn->setStyleSheet(pressed_stylesheet);
        break;
    case PressureChart:
        ui->pressure_btn->setStyleSheet(pressed_stylesheet);
        break;
    case FlowChart:
        ui->flow_btn->setStyleSheet(pressed_stylesheet);
        break;
    case SpeedChart_1:
        ui->speed_1_btn->setStyleSheet(pressed_stylesheet);
        break;
    case SpeedChart_2:
        ui->speed_2_btn->setStyleSheet(pressed_stylesheet);
        break;
    case OthersChart:
        ui->others_btn->setStyleSheet(pressed_stylesheet);
        break;
    }
}

void HisCurve::setup_charts_and_buttton(const DisplayGroups group)
{
    switch(group)
    {
    case TT01_TT08:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-1"));title[i]->setText("TT-1");break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-2"));title[i]->setText("TT-2");break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-3"));title[i]->setText("TT-3");break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-4"));title[i]->setText("TT-4");break;
            case 4:ui->checkBox_chart_5->setText(tr("TT-5"));title[i]->setText("TT-5");
                plots[i]->show();
                ui->checkBox_chart_5->show();
                ui->label_9->show();
                ui->label_10->show();
                break;
            case 5:ui->checkBox_chart_6->setText(tr("TT-6"));title[i]->setText("TT-6");
                plots[i]->show();
                ui->checkBox_chart_6->show();
                ui->label_11->show();
                ui->label_12->show();
                break;
            case 6:ui->checkBox_chart_7->setText(tr("TT-7"));title[i]->setText("TT-7");
                plots[i]->show();
                ui->checkBox_chart_7->show();
                ui->label_13->show();
                ui->label_14->show();
                break;
            case 7:ui->checkBox_chart_8->setText(tr("TT-8"));title[i]->setText("TT-8");
                plots[i]->show();
                ui->checkBox_chart_8->show();
                ui->label_15->show();
                ui->label_16->show();
                break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(TT01_TT08, lastGroup);
        lastGroup = TT01_TT08;

        break;
    case TT09_TT16:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-9"));title[i]->setText("TT-9");break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-10"));title[i]->setText("TT-10");break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-11"));title[i]->setText("TT-11");break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-12"));title[i]->setText("TT-12");break;
            case 4:ui->checkBox_chart_5->setText(tr("TT-13"));title[i]->setText("TT-13");
                plots[i]->show();
                ui->checkBox_chart_5->show();
                ui->label_9->show();
                ui->label_10->show();
                break;
            case 5:ui->checkBox_chart_6->setText(tr("TT-14"));title[i]->setText("TT-14");
                plots[i]->show();
                ui->checkBox_chart_6->show();
                ui->label_11->show();
                ui->label_12->show();
                break;
            case 6:ui->checkBox_chart_7->setText(tr("TT-15"));title[i]->setText("TT-15");
                plots[i]->show();
                ui->checkBox_chart_7->show();
                ui->label_13->show();
                ui->label_14->show();
                break;
            case 7:ui->checkBox_chart_8->setText(tr("TT-16"));title[i]->setText("TT-16");
                plots[i]->show();
                ui->checkBox_chart_8->show();
                ui->label_15->show();
                ui->label_16->show();
                break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(TT09_TT16, lastGroup);
        lastGroup = TT09_TT16;

        break;
    case TT17_TT24:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-17"));title[i]->setText("TT-17");break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-18"));title[i]->setText("TT-18");break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-19"));title[i]->setText("TT-19");break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-20"));title[i]->setText("TT-20");break;
            case 4:ui->checkBox_chart_5->setText(tr("TT-21"));title[i]->setText("TT-21");
                plots[i]->show();
                ui->checkBox_chart_5->show();
                ui->label_9->show();
                ui->label_10->show();
                break;
            case 5:ui->checkBox_chart_6->setText(tr("TT-22"));title[i]->setText("TT-22");
                plots[i]->show();
                ui->checkBox_chart_6->show();
                ui->label_11->show();
                ui->label_12->show();
                break;
            case 6:ui->checkBox_chart_7->setText(tr("TT-23"));title[i]->setText("TT-23");
                plots[i]->show();
                ui->checkBox_chart_7->show();
                ui->label_13->show();
                ui->label_14->show();
                break;
            case 7:ui->checkBox_chart_8->setText(tr("TT-24"));title[i]->setText("TT-24");
                plots[i]->show();
                ui->checkBox_chart_8->show();
                ui->label_15->show();
                ui->label_16->show();
                break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(TT17_TT24, lastGroup);
        lastGroup = TT17_TT24;

        break;
    case TT25_TT32:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-25"));title[i]->setText("TT-25");break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-26"));title[i]->setText("TT-26");break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-27"));title[i]->setText("TT-27");break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-28"));title[i]->setText("TT-28");break;
            case 4:ui->checkBox_chart_5->setText(tr("TT-29"));title[i]->setText("TT-29");
                plots[i]->show();
                ui->checkBox_chart_5->show();
                ui->label_9->show();
                ui->label_10->show();
                break;
            case 5:ui->checkBox_chart_6->setText(tr("TT-30"));title[i]->setText("TT-30");
                plots[i]->show();
                ui->checkBox_chart_6->show();
                ui->label_11->show();
                ui->label_12->show();
                break;
            case 6:ui->checkBox_chart_7->setText(tr("TT-31"));title[i]->setText("TT-31");
                plots[i]->show();
                ui->checkBox_chart_7->show();
                ui->label_13->show();
                ui->label_14->show();
                break;
            case 7:ui->checkBox_chart_8->setText(tr("TT-32"));title[i]->setText("TT-32");
                plots[i]->show();
                ui->checkBox_chart_8->show();
                ui->label_15->show();
                ui->label_16->show();
                break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(TT25_TT32, lastGroup);
        lastGroup = TT25_TT32;

        break;
    case TT33_TT36:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("TT-33"));title[i]->setText("TT-33");break;
            case 1:ui->checkBox_chart_2->setText(tr("TT-34"));title[i]->setText("TT-34");break;
            case 2:ui->checkBox_chart_3->setText(tr("TT-35"));title[i]->setText("TT-35");break;
            case 3:ui->checkBox_chart_4->setText(tr("TT-36"));title[i]->setText("TT-36");break;
            case 4:plots[i]->hide();ui->checkBox_chart_5->hide();ui->label_9->hide();ui->label_10->hide();break;
            case 5:plots[i]->hide();ui->checkBox_chart_6->hide();ui->label_11->hide();ui->label_12->hide();break;
            case 6:plots[i]->hide();ui->checkBox_chart_7->hide();ui->label_13->hide();ui->label_14->hide();break;
            case 7:plots[i]->hide();ui->checkBox_chart_8->hide();ui->label_15->hide();ui->label_16->hide();break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(TT33_TT36, lastGroup);
        lastGroup = TT33_TT36;

        break;
    case PressureChart:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("PT-01"));title[i]->setText("PT-01");break;
            case 1:ui->checkBox_chart_2->setText(tr("PT-02"));title[i]->setText("PT-02");break;
            case 2:ui->checkBox_chart_3->setText(tr("PT-03"));title[i]->setText("PT-03");break;
            case 3:ui->checkBox_chart_4->setText(tr("PT-04"));title[i]->setText("PT-04");break;
            case 4:ui->checkBox_chart_5->setText(tr("PT-05"));title[i]->setText("PT-05");
                plots[i]->show();
                ui->checkBox_chart_5->show();
                ui->label_9->show();
                ui->label_10->show();
                break;
            case 5:ui->checkBox_chart_6->setText(tr("PT-06"));title[i]->setText("PT-06");
                plots[i]->show();
                ui->checkBox_chart_6->show();
                ui->label_11->show();
                ui->label_12->show();
                break;
            case 6:plots[i]->hide();ui->checkBox_chart_7->hide();ui->label_13->hide();ui->label_14->hide();break;
            case 7:plots[i]->hide();ui->checkBox_chart_8->hide();ui->label_15->hide();ui->label_16->hide();break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(PressureChart, lastGroup);
        lastGroup = PressureChart;

        break;
    case FlowChart:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("AFM-01"));title[i]->setText("AFM-01");break;
            case 1:ui->checkBox_chart_2->setText(tr("AFM-02"));title[i]->setText("AFM-02");break;
            case 2:ui->checkBox_chart_3->setText(tr("AFM-03"));title[i]->setText("AFM-03");break;
            case 3:ui->checkBox_chart_4->setText(tr("AFM-04"));title[i]->setText("AFM-04");break;
            case 4:ui->checkBox_chart_5->setText(tr("MFM-01"));title[i]->setText("MFM-01");
                plots[i]->show();
                ui->checkBox_chart_5->show();
                ui->label_9->show();
                ui->label_10->show();
                break;
            case 5:plots[i]->hide();ui->checkBox_chart_6->hide();ui->label_11->hide();ui->label_12->hide();break;
            case 6:plots[i]->hide();ui->checkBox_chart_7->hide();ui->label_13->hide();ui->label_14->hide();break;
            case 7:plots[i]->hide();ui->checkBox_chart_8->hide();ui->label_15->hide();ui->label_16->hide();break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(FlowChart, lastGroup);
        lastGroup = FlowChart;

        break;
    case SpeedChart_1:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("BL-01"));title[i]->setText("BL-01");break;
            case 1:ui->checkBox_chart_2->setText(tr("BL-02"));title[i]->setText("BL-02");break;
            case 2:ui->checkBox_chart_3->setText(tr("BL-03"));title[i]->setText("BL-03");break;
            case 3:ui->checkBox_chart_4->setText(tr("BL-04"));title[i]->setText("BL-04");break;
            case 4:plots[i]->hide();ui->checkBox_chart_5->hide();ui->label_9->hide();ui->label_10->hide();break;
            case 5:plots[i]->hide();ui->checkBox_chart_6->hide();ui->label_11->hide();ui->label_12->hide();break;
            case 6:plots[i]->hide();ui->checkBox_chart_7->hide();ui->label_13->hide();ui->label_14->hide();break;
            case 7:plots[i]->hide();ui->checkBox_chart_8->hide();ui->label_15->hide();ui->label_16->hide();break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(SpeedChart_1, lastGroup);
        lastGroup = SpeedChart_1;

        break;
    case SpeedChart_2:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("PMP-01"));title[i]->setText("PMP-01");break;
            case 1:ui->checkBox_chart_2->setText(tr("PMP-02"));title[i]->setText("PMP-02");break;
            case 2:ui->checkBox_chart_3->setText(tr("PMP-03"));title[i]->setText("PMP-03");break;
            case 3:ui->checkBox_chart_4->setText(tr("PMP-04"));title[i]->setText("PMP-04");break;
            case 4:ui->checkBox_chart_5->setText(tr("PMP-05"));title[i]->setText("PMP-05");
                plots[i]->show();
                ui->checkBox_chart_5->show();
                ui->label_9->show();
                ui->label_10->show();
                break;
            case 5:ui->checkBox_chart_6->setText(tr("RAD-01"));title[i]->setText("RAD-01");
                plots[i]->show();
                ui->checkBox_chart_6->show();
                ui->label_11->show();
                ui->label_12->show();
                break;
            case 6:plots[i]->hide();ui->checkBox_chart_7->hide();ui->label_13->hide();ui->label_14->hide();break;
            case 7:plots[i]->hide();ui->checkBox_chart_8->hide();ui->label_15->hide();ui->label_16->hide();break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(SpeedChart_2, lastGroup);
        lastGroup = SpeedChart_2;

        break;
    case OthersChart:
        for (int i = 0; i < max_charts_num; i++)
        {
            switch (i)
            {
            case 0:ui->checkBox_chart_1->setText(tr("CM-01"));title[i]->setText("CM-01");break;
            case 1:ui->checkBox_chart_2->setText(tr("LT-01"));title[i]->setText("LT-01");break;
            case 2:ui->checkBox_chart_3->setText(tr("LT-02"));title[i]->setText("LT-02");break;
            case 3:ui->checkBox_chart_4->setText(tr("VT-01"));title[i]->setText("VT-01");break;
            case 4:ui->checkBox_chart_5->setText(tr("IT-01"));title[i]->setText("IT-01");
                plots[i]->show();
                ui->checkBox_chart_5->show();
                ui->label_9->show();
                ui->label_10->show();
                break;
            case 5:ui->checkBox_chart_6->setText(tr("VT-02"));title[i]->setText("VT-02");
                plots[i]->show();
                ui->checkBox_chart_6->show();
                ui->label_11->show();
                ui->label_12->show();
                break;
            case 6:ui->checkBox_chart_7->setText(tr("IT-01"));title[i]->setText("IT-01");
                plots[i]->show();
                ui->checkBox_chart_7->show();
                ui->label_13->show();
                ui->label_14->show();
                break;
            case 7:plots[i]->hide();ui->checkBox_chart_8->hide();ui->label_15->hide();ui->label_16->hide();break;
            }
            plots[i]->replot();
        }

        setup_stylesheet(OthersChart, lastGroup);
        lastGroup = OthersChart;

        break;
    }
}

void HisCurve::on_TT01_TT08_btn_clicked()
{
    setup_charts_and_buttton(TT01_TT08);

//    qDebug() << __FILE__ << __LINE__ << ui->checkBox_chart_1->size();
}

void HisCurve::on_TT09_TT16_btn_clicked()
{
    setup_charts_and_buttton(TT09_TT16);
}

void HisCurve::on_TT17_TT24_btn_clicked()
{
    setup_charts_and_buttton(TT17_TT24);
}

void HisCurve::on_TT25_TT32_btn_clicked()
{
    setup_charts_and_buttton(TT25_TT32);
}

void HisCurve::on_TT33_TT36_btn_clicked()
{
    setup_charts_and_buttton(TT33_TT36);
}

void HisCurve::on_pressure_btn_clicked()
{
    setup_charts_and_buttton(PressureChart);
}

void HisCurve::on_flow_btn_clicked()
{
    setup_charts_and_buttton(FlowChart);
}

void HisCurve::on_speed_1_btn_clicked()
{
    setup_charts_and_buttton(SpeedChart_1);
}
void HisCurve::on_speed_2_btn_clicked()
{
    setup_charts_and_buttton(SpeedChart_2);
}

void HisCurve::on_others_btn_clicked()
{
    setup_charts_and_buttton(OthersChart);
}

void HisCurve::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
    double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
    emit dataChanged(message);
}

void HisCurve::plot_set_color()
{
    QPen p0;
    p0.setWidth(3);

    for (int i = 0; i < max_charts_num; i++)
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

void HisCurve::resizeEvent(QResizeEvent */*event*/)
{
    int height = ui->tableWidget->size().height() / 4;
    int width = ui->tableWidget->size().width() / 2 - 1;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            ui->tableWidget->setRowHeight(i, height);
            ui->tableWidget->setColumnWidth(j, width);

            plots[i*j]->replot();
        }
    }
}

void HisCurve::on_searchData_clicked()
{
    QDateTime startDateTime, endDateTime;
    qint64 start_time, end_time;

    switch (ui->quickSearch->currentIndex()) {
    case CustomDates:
        if (ui->endDateTimeEdit->dateTime().toMSecsSinceEpoch() < ui->startDateTimeEdit->dateTime().toMSecsSinceEpoch())
        {
            QMessageBox::critical(this, "错误", "请选择正确的查询时间段");
        }
        else
        {
            startDateTime = ui->startDateTimeEdit->dateTime();
            endDateTime = ui->endDateTimeEdit->dateTime();
        }
        break;
    case Today:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));

        endDateTime = QDateTime::currentDateTime();
        break;

    case Yesterday:
        endDateTime = QDateTime::currentDateTime();
        endDateTime.setTime(QTime(0, 0, 0, 0));

        startDateTime = endDateTime;
        startDateTime = endDateTime.addDays(-1);

        break;

    case LastSevenDays:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime = startDateTime.addDays(-7);

        endDateTime = QDateTime::currentDateTime();
//        endDateTime.setTime(QTime(0, 0, 0, 0));

        break;

    case CurrentMonth:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime.setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));
        endDateTime = QDateTime::currentDateTime();
//        endDateTime.setTime(QTime(0, 0, 0, 0));
        break;
    case LastMonth:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime.setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month()-1, 1));

        endDateTime.QDateTime::currentDateTime();
        endDateTime.setTime(QTime(0, 0, 0, 0));
        endDateTime.setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));

        break;
    }

    start_time = startDateTime.toMSecsSinceEpoch();
    end_time = endDateTime.toMSecsSinceEpoch();

    qDebug() << startDateTime << endDateTime;
}

void HisCurve::on_quickSearch_currentIndexChanged(int index)
{
//    ui->quickSearch
    if (index == CustomDates)
    {
        ui->startDateTimeEdit->setEnabled(true);
        ui->endDateTimeEdit->setEnabled(false);
    }
    else
    {
        ui->startDateTimeEdit->setDisabled(true);
        ui->endDateTimeEdit->setDisabled(true);
    }
}

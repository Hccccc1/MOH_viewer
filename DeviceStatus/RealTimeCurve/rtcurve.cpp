#include "rtcurve.h"
#include "ui_rtcurve.h"
#include "AllBitsAndRegs.h"
#include "MOH_viewer/moh_viewer.h"

RTCurve::RTCurve(QWidget *parent, ModbusSerial *serial) :
    QDialog(parent),
    ui(new Ui::RTCurve),
    current_serial(serial)
{
    ui->setupUi(this);

//    QPen p0;
    //    p0.setWidth(3);

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
        dateTicker->setDateTimeFormat("yyyy-MM-dd hh:mm:ss");
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

    startTimer(1000, Qt::CoarseTimer);

    for (const auto *plot : plots)
    {
        connect(plot, &QCustomPlot::plottableClick, this, &RTCurve::graphClicked);
    }

    for (int i = 0; i < max_charts_num; i++)
    {
        switch (i)
        {
        case 0:ui->checkBox_chart_1->setText(tr("TT-1"));break;
        case 1:ui->checkBox_chart_2->setText(tr("TT-2"));break;
        case 2:ui->checkBox_chart_3->setText(tr("TT-3"));break;
        case 3:ui->checkBox_chart_4->setText(tr("TT-4"));break;
        case 4:ui->checkBox_chart_5->setText(tr("TT-5"));break;
        case 5:ui->checkBox_chart_6->setText(tr("TT-6"));break;
        case 6:ui->checkBox_chart_7->setText(tr("TT-7"));break;
        case 7:ui->checkBox_chart_8->setText(tr("TT-8"));break;
        }
    }
    lastGroup = TT01_TT08;

    plot_set_color();

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

    ui->checkBox_chart_1->setCheckState(Qt::Checked);
    ui->checkBox_chart_2->setCheckState(Qt::Checked);
    ui->checkBox_chart_3->setCheckState(Qt::Checked);
    ui->checkBox_chart_4->setCheckState(Qt::Checked);
    ui->checkBox_chart_5->setCheckState(Qt::Checked);
    ui->checkBox_chart_6->setCheckState(Qt::Checked);
    ui->checkBox_chart_7->setCheckState(Qt::Checked);
    ui->checkBox_chart_8->setCheckState(Qt::Checked);

    //    connect(ui->checkBox_chart_1, &QCheckBox::stateChanged, this, &RTCurve::on_checkBox_stateChan)
}

void RTCurve::resizeEvent(QResizeEvent* /*event*/)
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

void RTCurve::plot_set_color()
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

void RTCurve::timerEvent(QTimerEvent *)
{
    //    qDebug() << e->timerId();

    //    QVector<double> time(1), value(1);

    //    for (QCustomPlot *plot : plots)
    //    {
    //        time[0] = QDateTime::currentSecsSinceEpoch();
    //        value[0] = qrand() % 100;

    //        plot->graph(0)->addData(time, value);
    //        plot->graph(0)->rescaleAxes();

    //        plot->replot();
    //    }

    //    plots[0]->graph(0)->addData(time, value);
    //    plots[0]->graph(0)->rescaleAxes();

    //    plots[0]->replot();

    refreshCurrentPage();

}

void RTCurve::setup_stylesheet(const DisplayGroups current_group, const DisplayGroups last_group)
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

void RTCurve::setup_charts_and_buttton(const DisplayGroups group)
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
            case 6:ui->checkBox_chart_7->setText(tr("IT-02"));title[i]->setText("IT-02");
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

RTCurve::~RTCurve()
{
    delete ui;

    delete plots[8];
    delete title[8];

//    delete db;
}

//void RTCurve::on_readButton_clicked()
//{
//    emit dataChanged("Hello world");

//    QPushButton *btn = qobject_cast<QPushButton *>(sender());

//    qDebug() << btn->objectName();
//}

void RTCurve::data_process(const QModbusDataUnit unit)
{
    QVector<QVector<quint16>> values;

    values.resize(10);

    QVector<double> time(1), value(1);

    time[0] = QDateTime::currentMSecsSinceEpoch();

    for (int i = 0, total = int(unit.valueCount()); i < total; i++)
    {
        const int addr = unit.startAddress() + i;

        switch (addr) {
        case InputRegs_TT_01:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (lastGroup == TT01_TT08)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_TT_02:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (lastGroup == TT01_TT08)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_TT_03:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (lastGroup == TT01_TT08)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_TT_04:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (lastGroup == TT01_TT08)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_TT_05:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (lastGroup == TT01_TT08)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();
            }
            break;
        case InputRegs_TT_06:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (lastGroup == TT01_TT08)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();
            }
            break;
        case InputRegs_TT_07:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (lastGroup == TT01_TT08)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();
            }
            break;
        case InputRegs_TT_08:
            value[0] = unit.value(i);

            values[TT01_TT08].push_back(unit.value(i));

            if (lastGroup == TT01_TT08)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();
            }
            break;
        case InputRegs_TT_09:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (lastGroup == TT09_TT16)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_TT_10:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (lastGroup == TT09_TT16)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_TT_11:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (lastGroup == TT09_TT16)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_TT_12:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (lastGroup == TT09_TT16)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_TT_13:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (lastGroup == TT09_TT16)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();
            }
            break;
        case InputRegs_TT_14:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (lastGroup == TT09_TT16)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();
            }
            break;
        case InputRegs_TT_15:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (lastGroup == TT09_TT16)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();
            }
            break;
        case InputRegs_TT_16:
            value[0] = unit.value(i);

            values[TT09_TT16].push_back(unit.value(i));

            if (lastGroup == TT09_TT16)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();
            }
            break;
        case InputRegs_TT_17:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (lastGroup == TT17_TT24)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_TT_18:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (lastGroup == TT17_TT24)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_TT_19:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (lastGroup == TT17_TT24)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_TT_20:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (lastGroup == TT17_TT24)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_TT_21:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (lastGroup == TT17_TT24)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();
            }
            break;
        case InputRegs_TT_22:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (lastGroup == TT17_TT24)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();
            }
            break;
        case InputRegs_TT_23:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (lastGroup == TT17_TT24)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();
            }
            break;
        case InputRegs_TT_24:
            value[0] = unit.value(i);

            values[TT17_TT24].push_back(unit.value(i));

            if (lastGroup == TT17_TT24)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();
            }
            break;
        case InputRegs_TT_25:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (lastGroup == TT25_TT32)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_TT_26:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (lastGroup == TT25_TT32)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_TT_27:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (lastGroup == TT25_TT32)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_TT_28:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (lastGroup == TT25_TT32)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_TT_29:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (lastGroup == TT25_TT32)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();
            }
            break;
        case InputRegs_TT_30:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (lastGroup == TT25_TT32)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();
            }
            break;
        case InputRegs_TT_31:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (lastGroup == TT25_TT32)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();
            }
            break;
        case InputRegs_TT_32:
            value[0] = unit.value(i);

            values[TT25_TT32].push_back(unit.value(i));

            if (lastGroup == TT25_TT32)
            {
                plots[7]->graph(0)->addData(time, value);
                plots[7]->graph(0)->rescaleAxes();
                plots[7]->replot();
            }
            break;
        case InputRegs_TT_33:
            value[0] = unit.value(i);

            values[TT33_TT36].push_back(unit.value(i));

            if (lastGroup == TT33_TT36)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_TT_34:
            value[0] = unit.value(i);

            values[TT33_TT36].push_back(unit.value(i));

            if (lastGroup == TT33_TT36)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_TT_35:
            value[0] = unit.value(i);

            values[TT33_TT36].push_back(unit.value(i));

            if (lastGroup == TT33_TT36)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_TT_36:
            value[0] = unit.value(i);

            values[TT33_TT36].push_back(unit.value(i));

            if (lastGroup == TT33_TT36)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_PT_01:
            value[0] = double(unit.value(i))/100;

            values[PressureChart].push_back(unit.value(i));

            if (lastGroup == PressureChart)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_PT_02:
            value[0] = double(unit.value(i))/100;

            values[PressureChart].push_back(unit.value(i));

            if (lastGroup == PressureChart)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_PT_03:
            value[0] = double(unit.value(i))/100;

            values[PressureChart].push_back(unit.value(i));

            if (lastGroup == PressureChart)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_PT_04:
            value[0] = double(unit.value(i))/100;

            values[PressureChart].push_back(unit.value(i));

            if (lastGroup == PressureChart)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_PT_05:
            value[0] = double(unit.value(i))/100;

            values[PressureChart].push_back(unit.value(i));

            if (lastGroup == PressureChart)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();
            }
            break;
        case InputRegs_PT_06:
            value[0] = double(unit.value(i))/100;

            values[PressureChart].push_back(unit.value(i));

            if (lastGroup == PressureChart)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();
            }
            break;

        case InputRegs_AFM_01:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (lastGroup == FlowChart)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_AFM_02:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (lastGroup == FlowChart)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_AFM_03:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (lastGroup == FlowChart)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_AFM_04:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (lastGroup == FlowChart)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_MFM_01:
            value[0] = double(unit.value(i))/10;

            values[FlowChart].push_back(unit.value(i));

            if (lastGroup == FlowChart)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();
            }
            break;

        case InputRegs_BL_01:
            value[0] = unit.value(i);

            values[SpeedChart_1].push_back(unit.value(i));

            if (lastGroup == SpeedChart_1)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_BL_02:
            value[0] = unit.value(i);

            values[SpeedChart_1].push_back(unit.value(i));

            if (lastGroup == SpeedChart_1)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_BL_03:
            value[0] = unit.value(i);

            values[SpeedChart_1].push_back(unit.value(i));

            if (lastGroup == SpeedChart_1)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_BL_04:
            value[0] = unit.value(i);

            values[SpeedChart_1].push_back(unit.value(i));

            if (lastGroup == SpeedChart_1)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;

        case InputRegs_PMP_01:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (lastGroup == SpeedChart_2)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_PMP_02:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (lastGroup == SpeedChart_2)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_PMP_03:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (lastGroup == SpeedChart_2)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_PMP_04:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (lastGroup == SpeedChart_2)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_PMP_05:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (lastGroup == SpeedChart_2)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();
            }
            break;
        case InputRegs_RAD_01:
            value[0] = unit.value(i);

            values[SpeedChart_2].push_back(unit.value(i));

            if (lastGroup == SpeedChart_2)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();
            }
            break;

        case InputRegs_CM_01:
            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            if (lastGroup == OthersChart)
            {
                plots[0]->graph(0)->addData(time, value);
                plots[0]->graph(0)->rescaleAxes();
                plots[0]->replot();
            }
            break;
        case InputRegs_LT_01:
            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            if (lastGroup == OthersChart)
            {
                plots[1]->graph(0)->addData(time, value);
                plots[1]->graph(0)->rescaleAxes();
                plots[1]->replot();
            }
            break;
        case InputRegs_LT_02:
            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            if (lastGroup == OthersChart)
            {
                plots[2]->graph(0)->addData(time, value);
                plots[2]->graph(0)->rescaleAxes();
                plots[2]->replot();
            }
            break;
        case InputRegs_VT_01:
            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            if (lastGroup == OthersChart)
            {
                plots[3]->graph(0)->addData(time, value);
                plots[3]->graph(0)->rescaleAxes();
                plots[3]->replot();
            }
            break;
        case InputRegs_IT_01:
            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            if (lastGroup == OthersChart)
            {
                plots[4]->graph(0)->addData(time, value);
                plots[4]->graph(0)->rescaleAxes();
                plots[4]->replot();
            }
            break;
        case InputRegs_VT_02:
            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            if (lastGroup == OthersChart)
            {
                plots[5]->graph(0)->addData(time, value);
                plots[5]->graph(0)->rescaleAxes();
                plots[5]->replot();
            }
            break;
        case InputRegs_IT_02:
            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            if (lastGroup == OthersChart)
            {
                plots[6]->graph(0)->addData(time, value);
                plots[6]->graph(0)->rescaleAxes();
                plots[6]->replot();
            }
            break;

        default:break;
        }
    }

    HistoryValuesDatabase db;
    db.insert_values_to_tables(values);
}

void RTCurve::on_TT01_TT08_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(TT01_TT08);
}

void RTCurve::on_TT09_TT16_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(TT09_TT16);
}

void RTCurve::on_TT17_TT24_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(TT17_TT24);
}

void RTCurve::on_TT25_TT32_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(TT25_TT32);
}

void RTCurve::on_TT33_TT36_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(TT33_TT36);
}

void RTCurve::on_pressure_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(PressureChart);
}

void RTCurve::on_flow_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(FlowChart);
}

void RTCurve::on_speed_1_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(SpeedChart_1);
}
void RTCurve::on_speed_2_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(SpeedChart_2);
}

void RTCurve::on_others_btn_clicked()
{
    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();

    setup_charts_and_buttton(OthersChart);
}

void RTCurve::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
    double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
    emit dataChanged(message);
}

void RTCurve::refreshCurrentPage()
{
    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
        current_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_TT_01, 77);
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


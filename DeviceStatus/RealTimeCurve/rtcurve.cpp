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

    //    ui->tableView->setRowHeight()
    //    ui->tableWidget->setCellWidget();
    //    ui->tableWidget->setCellWidget(0, 0, new QCustomPlot());

    ui->tableWidget->setRowCount(4);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();

    ui->tableWidget->resize(1356, 563);

//    connect(ui->tableWidget, &QTableWidget::)

    setup_charts_checkboxes(TT01_TT08);

//    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
//    {
//        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
//        {
//            ui->tableWidget->setCellWidget(i, j, new QCustomPlot(ui->tableWidget));

//            QCustomPlot *plot = qobject_cast<QCustomPlot *>(ui->tableWidget->cellWidget(i, j));

//            plot->plotLayout()->insertRow(0);
//            plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i), QFont("PingFang SC", 17, 300)));

//            plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

//            QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
//            dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
//            plot->xAxis->setTicker(dateTicker);
//        }
//    }
}

RTCurve::~RTCurve()
{
    delete ui;
    delete current_serial;
}

void RTCurve::data_process(QModbusDataUnit unit)
{
}

void RTCurve::on_tabWidget_currentChanged(int index)
{
//    qDebug() << sender()->objectName();

    QWidget *tmp_widget = ui->tabWidget->currentWidget();

    ui->groupBox->setParent(tmp_widget);
    ui->groupBox->show();

    ui->gridLayout_3->setParent(tmp_widget);
    ui->tableWidget->setParent(tmp_widget);
    ui->tableWidget->show();

    setup_charts_checkboxes(DisplayGroups(index));

//    (ui->tableWidget == nullptr) ? (qDebug() << "Null") : (qDebug() << "Something");
}

void RTCurve::set_widgets_size()
{
    int total_height = ui->tableWidget->height()-10;
    int total_width = ui->tableWidget->width()-10;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->setRowHeight(i, total_height/ui->tableWidget->rowCount());

        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            ui->tableWidget->setColumnWidth(j, total_width/ui->tableWidget->columnCount());
        }
    }
}

void RTCurve::resizeEvent(QResizeEvent */*event*/)
{
    //    qDebug() << event->size();
    qDebug() << __FILE__ << __LINE__ << ui->tableWidget->size();

    set_widgets_size();
}

void RTCurve::setup_charts_checkboxes(DisplayGroups group)
{
    switch (group) {
    case TT01_TT08:
        ui->tableWidget->setRowCount(4);
        ui->tableWidget->setColumnCount(2);
        set_widgets_size();

        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if (ui->tableWidget->cellWidget(i, j) != nullptr)
                    delete ui->tableWidget->cellWidget(i, j);

//                (ui->tableWidget->cellWidget(i, j) == nullptr) ? qDebug() << ("Null") : qDebug() << ("Something");

                ui->tableWidget->setCellWidget(i, j, new QCustomPlot(ui->tableWidget));

                QCustomPlot *plot = qobject_cast<QCustomPlot *>(ui->tableWidget->cellWidget(i, j));

                plot->plotLayout()->insertRow(0);
                if (j == 1)
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i*j+5), QFont("PingFang SC", 17, 300)));
                else
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i+1), QFont("PingFang SC", 17, 300)));

                plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
                plot->xAxis->setTicker(dateTicker);
            }
        }
        break;
    case TT09_TT16:
        ui->tableWidget->setRowCount(4);
        ui->tableWidget->setColumnCount(2);
        set_widgets_size();

        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if (ui->tableWidget->cellWidget(i, j) != nullptr)
                    delete ui->tableWidget->cellWidget(i, j);

                ui->tableWidget->setCellWidget(i, j, new QCustomPlot(ui->tableWidget));

                QCustomPlot *plot = qobject_cast<QCustomPlot *>(ui->tableWidget->cellWidget(i, j));

                plot->plotLayout()->insertRow(0);
                if (j == 1)
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i*j+13), QFont("PingFang SC", 17, 300)));
                else
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i+9), QFont("PingFang SC", 17, 300)));

                plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
                plot->xAxis->setTicker(dateTicker);
            }
        }
        break;
    case TT17_TT24:
        ui->tableWidget->setRowCount(4);
        ui->tableWidget->setColumnCount(2);
        set_widgets_size();

        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if (ui->tableWidget->cellWidget(i, j) != nullptr)
                    delete ui->tableWidget->cellWidget(i, j);

                ui->tableWidget->setCellWidget(i, j, new QCustomPlot(ui->tableWidget));

                QCustomPlot *plot = qobject_cast<QCustomPlot *>(ui->tableWidget->cellWidget(i, j));

                plot->plotLayout()->insertRow(0);
                if (j == 1)
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i*j+21), QFont("PingFang SC", 17, 300)));
                else
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i+17), QFont("PingFang SC", 17, 300)));

                plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
                plot->xAxis->setTicker(dateTicker);
            }
        }
        break;
    case TT25_TT32:
        ui->tableWidget->setRowCount(4);
        ui->tableWidget->setColumnCount(2);
        set_widgets_size();

        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if (ui->tableWidget->cellWidget(i, j) != nullptr)
                    delete ui->tableWidget->cellWidget(i, j);

                ui->tableWidget->setCellWidget(i, j, new QCustomPlot(ui->tableWidget));

                QCustomPlot *plot = qobject_cast<QCustomPlot *>(ui->tableWidget->cellWidget(i, j));

                plot->plotLayout()->insertRow(0);
                if (j == 1)
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i*j+29), QFont("PingFang SC", 17, 300)));
                else
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i+25), QFont("PingFang SC", 17, 300)));

                plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
                plot->xAxis->setTicker(dateTicker);
            }
        }
        break;
    case TT33_TT36:
        ui->tableWidget->setRowCount(2);
        ui->tableWidget->setColumnCount(2);
        set_widgets_size();

        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if (ui->tableWidget->cellWidget(i, j) != nullptr)
                    delete ui->tableWidget->cellWidget(i, j);

                ui->tableWidget->setCellWidget(i, j, new QCustomPlot(ui->tableWidget));

                QCustomPlot *plot = qobject_cast<QCustomPlot *>(ui->tableWidget->cellWidget(i, j));

                plot->plotLayout()->insertRow(0);
                if (j == 1)
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i*j+35), QFont("PingFang SC", 17, 300)));
                else
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("TT-%1(°C)").arg(i+33), QFont("PingFang SC", 17, 300)));

                plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
                plot->xAxis->setTicker(dateTicker);
            }
        }
        break;
    case PressureChart:
        ui->tableWidget->setRowCount(3);
        ui->tableWidget->setColumnCount(2);
        set_widgets_size();

        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if (ui->tableWidget->cellWidget(i, j) != nullptr)
                    delete ui->tableWidget->cellWidget(i, j);

                ui->tableWidget->setCellWidget(i, j, new QCustomPlot(ui->tableWidget));

                QCustomPlot *plot = qobject_cast<QCustomPlot *>(ui->tableWidget->cellWidget(i, j));

                plot->plotLayout()->insertRow(0);
                if (j == 1)
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("PT-%1(°C)").arg(i*j+4), QFont("PingFang SC", 17, 300)));
                else
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("PT-%1(°C)").arg(i+1), QFont("PingFang SC", 17, 300)));

                plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
                plot->xAxis->setTicker(dateTicker);
            }
        }
        break;
    case FlowChart:
        ui->tableWidget->setRowCount(3);
        ui->tableWidget->setColumnCount(2);
        set_widgets_size();

        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if (ui->tableWidget->cellWidget(i, j) != nullptr)
                    delete ui->tableWidget->cellWidget(i, j);

                ui->tableWidget->setCellWidget(i, j, new QCustomPlot(ui->tableWidget));

//                if (i == 2 && j == 1)
//                    ui->tableWidget->cellWidget(2, 1)->hide();

                QCustomPlot *plot = qobject_cast<QCustomPlot *>(ui->tableWidget->cellWidget(i, j));

                plot->plotLayout()->insertRow(0);
                if (j == 1 && i == 1)
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("MFM-%1(°C)").arg(1), QFont("PingFang SC", 17, 300)));
                else if (j == 1)
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("AFM-%1(°C)").arg(i*j+1), QFont("PingFang SC", 17, 300)));
                else
                    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, QString("AFM-%1(°C)").arg(i+1), QFont("PingFang SC", 17, 300)));

                plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
                plot->xAxis->setTicker(dateTicker);
            }
        }
        break;
    case SpeedChart_1:break;
    case SpeedChart_2:break;
    case OthersChart:break;
    }
}

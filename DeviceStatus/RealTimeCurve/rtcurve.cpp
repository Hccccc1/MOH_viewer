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
        dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
        plots[i]->xAxis->setTicker(dateTicker);

        if (i < 4)
            ui->gridLayout_4->addWidget(plots[i], i, 0, 1, 1);
        else
            ui->gridLayout_4->addWidget(plots[i], i-4, 1, 1, 1);
    }

    //    ui->gridLayout_3->addWidget()

    //    ui->plots_groupboxplots[0]

    //    ui->tableView->setRowHeight()
    //    ui->tableWidget->setCellWidget();
    //    ui->tableWidget->setCellWidget(0, 0, new QCustomPlot());

    //    ui->tableWidget->setRowCount(4);
    //    ui->tableWidget->setColumnCount(2);
    //    ui->tableWidget->horizontalHeader()->hide();
    //    ui->tableWidget->verticalHeader()->hide();

    //    ui->tableWidget->resize(1356, 563);

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
    ui->plots_groupbox->setParent(tmp_widget);
    ui->plots_groupbox->show();
    //    ui->tableWidget->setParent(tmp_widget);
    //    ui->tableWidget->show();

    setup_charts_checkboxes(DisplayGroups(index));

    //    (ui->tableWidget == nullptr) ? (qDebug() << "Null") : (qDebug() << "Something");
}

void RTCurve::set_widgets_size()
{
    //    int total_height = ui->tableWidget->height()-10;
    //    int total_width = ui->tableWidget->width()-10;

    //    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    //    {
    //        ui->tableWidget->setRowHeight(i, total_height/ui->tableWidget->rowCount());

    //        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
    //        {
    //            ui->tableWidget->setColumnWidth(j, total_width/ui->tableWidget->columnCount());
    //        }
    //    }
}

void RTCurve::resizeEvent(QResizeEvent */*event*/)
{

}

void RTCurve::setup_charts_checkboxes(DisplayGroups group)
{
    switch (group) {
    case TT01_TT08:
        for (int i = 0; i < plots.size(); i++)
        {
            title[i]->setText(QString("TT-%1").arg(i+1));
            plots[i]->replot();

            if (i < 8)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case TT09_TT16:
        for (int i = 0; i < plots.size(); i++)
        {
            title[i]->setText(QString("TT-%1").arg(i+9));
            plots[i]->replot();

            if (i < 8)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case TT17_TT24:
        for (int i = 0; i < plots.size(); i++)
        {
            title[i]->setText(QString("TT-%1").arg(i+17));
            plots[i]->replot();

            if (i < 8)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case TT25_TT32:
        for (int i = 0; i < plots.size(); i++)
        {
            title[i]->setText(QString("TT-%1").arg(i+25));
            plots[i]->replot();

            if (i < 8)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case TT33_TT36:
        for (int i = 0; i < plots.size(); i++)
        {
            title[i]->setText(QString("TT-%1").arg(i+33));
            plots[i]->replot();

            if (i < 4)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case PressureChart:
        for (int i = 0; i < plots.size(); i++)
        {
            title[i]->setText(QString("PT-%1").arg(i+1));
            plots[i]->replot();

            if (i < 6)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case FlowChart:
        for (int i = 0; i < plots.size(); i++)
        {
            if (i < 4)
                title[i]->setText(QString("AFM-%1").arg(i+1));
            else
                title[i]->setText(QString("MFM-%1").arg(1));

            plots[i]->replot();

            if (i < 5)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case SpeedChart_1:
        for (int i = 0; i < plots.size(); i++)
        {
            title[i]->setText(QString("BL-%1").arg(i+1));
            plots[i]->replot();

            if (i < 4)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case SpeedChart_2:
        for (int i = 0; i < plots.size(); i++)
        {
            if (i < 5)
                title[i]->setText(QString("PMP-%1").arg(i+1));
            else
                title[i]->setText(QString("RAD-%1").arg(1));

            plots[i]->replot();

            if (i < 6)
                plots[i]->show();
            else
                plots[i]->hide();
        }
        break;
    case OthersChart:
        title[0]->setText("CM-01");
        title[1]->setText("LT-01");
        title[2]->setText("LT-02");
        title[3]->setText("VT-01");
        title[4]->setText("IT-01");
        title[5]->setText("VT-02");
        title[6]->setText("IT-02");

        for (int i = 0; i < plots.size(); i++)
        {
            if (i > 3)
                plots[i]->show();

            plots[i]->replot();
        }

        plots[7]->hide();

        break;
    default:break;
    }
}

#include "customerrtcurve.h"
#include "ui_customerrtcurve.h"

CustomerRTCurve::CustomerRTCurve(QWidget *parent, ModbusSerial *serial) :
    QWidget(parent),
    ui(new Ui::CustomerRTCurve),
    current_serial(serial)
{
    ui->setupUi(this);

    plots.resize(8);
    title.resize(8);

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

//        if (i < 3)
//            ui->gridLayout_4->addWidget(plots[i], i, 0, 1, 1);
//        else
//            ui->gridLayout_4->addWidget(plots[i], i-3, 1, 1, 1);

        if (i < 4)
            ui->gridLayout_4->addWidget(plots[i], i, 0, 1, 1);
        else
            ui->gridLayout_4->addWidget(plots[i], i-4, 1, 1, 1);

        connect(plots[i], &QCustomPlot::mouseMove, this, &CustomerRTCurve::plots_mouseMove);
    }

    plot_set_color();

    ui->checkBox_chart_1->setText("LT-01");
    title[0]->setText("LT-01(cm)");

    ui->checkBox_chart_2->setText("LT-02");
    title[1]->setText("LT-02(cm)");

    ui->checkBox_chart_3->setText("VT-01");
    title[2]->setText("VT-01(V)");

    ui->checkBox_chart_4->setText("IT-01");
    title[3]->setText("IT-01(A)");

    ui->checkBox_chart_5->setText("FCPower");
    title[4]->setText("FCPower(W)");

    ui->checkBox_chart_6->setText("OutPower");
    title[5]->setText("OutPower(W)");

    ui->checkBox_chart_7->setText("VT-02");
    title[6]->setText("VT-02(V)");

    ui->checkBox_chart_8->setText("IT-02");
    title[7]->setText("IT-02(A)");

}

CustomerRTCurve::~CustomerRTCurve()
{
    delete ui;
}

void CustomerRTCurve::timerEvent(QTimerEvent *)
{
    refreshCurrentPage();
}

void CustomerRTCurve::refreshCurrentPage()
{
//    if (current_serial->modbus_client->state() == QModbusDevice::ConnectedState)
    if (current_serial->is_serial_connected())
    {
        current_serial->read_from_modbus(QModbusDataUnit::InputRegisters, InputRegs_CM_01, 11);
    }
}

void CustomerRTCurve::plot_set_color()
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

void CustomerRTCurve::data_process(QModbusDataUnit unit)
{
    QVector<QVector<quint16>> values(10);
    QVector<double> time(1), value(1);

    time[0] = QDateTime::currentMSecsSinceEpoch() / 1000;

    for (int i = 0; i < int(unit.valueCount()); i++)
    {
        const int addr = unit.startAddress() + i;

        switch (addr) {
//        case InputRegs_CM_01:
//            values[OthersChart].push_back(unit.value(i));
//            break;

        case InputRegs_LT_01:
            value[0] = unit.value(i);

            values[OthersChart].push_back(unit.value(i));

            plots[0]->graph(0)->addData(time, value);
            plots[0]->graph(0)->rescaleAxes();
            plots[0]->replot();
            ui->real_time_value_1->setText(QString("%1cm").arg(unit.value(i)));

            break;

        case InputRegs_LT_02:
            value[0] = unit.value(i);
            values[OthersChart].push_back(unit.value(i));

            plots[1]->graph(0)->addData(time, value);
            plots[1]->graph(0)->rescaleAxes();
            plots[1]->replot();

            ui->real_time_value_2->setText(QString("%1cm").arg(unit.value(i)));
            break;

        case InputRegs_VT_01:
            value[0] = double(unit.value(i))/10;
            values[OthersChart].push_back(unit.value(i));

            plots[2]->graph(0)->addData(time, value);
            plots[2]->graph(0)->rescaleAxes();
            plots[2]->replot();

            ui->real_time_value_3->setText(QString("%1V").arg(double(unit.value(i))/10));
            break;

        case InputRegs_IT_01:
            value[0] = double(unit.value(i))/10;
            values[OthersChart].push_back(unit.value(i));

            plots[3]->graph(0)->addData(time, value);
            plots[3]->graph(0)->rescaleAxes();
            plots[3]->replot();

            ui->real_time_value_4->setText(QString("%1A").arg(double(unit.value(i))/10));
            break;
        case InputRegs_FcPower:
            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            plots[4]->graph(0)->addData(time, value);
            plots[4]->graph(0)->rescaleAxes();
            plots[4]->replot();
            ui->real_time_value_5->setText(QString("%1W").arg(double(unit.value(i))/10));

            break;
        case InputRegs_OutPower:
            value[0] = double(unit.value(i))/10;

            values[OthersChart].push_back(unit.value(i));

            plots[5]->graph(0)->addData(time, value);
            plots[5]->graph(0)->rescaleAxes();
            plots[5]->replot();
            ui->real_time_value_6->setText(QString("%1W").arg(double(unit.value(i))/10));

            break;
        case InputRegs_VT_02:
            value[0] = double(unit.value(i))/10;
            values[OthersChart].push_back(unit.value(i));

            plots[6]->graph(0)->addData(time, value);
            plots[6]->graph(0)->rescaleAxes();
            plots[6]->replot();

            ui->real_time_value_7->setText(QString("%1V").arg(double(unit.value(i))/10));
            break;

        case InputRegs_IT_02:
            value[0] = double(unit.value(i))/10;
            values[OthersChart].push_back(unit.value(i));

            plots[7]->graph(0)->addData(time, value);
            plots[7]->graph(0)->rescaleAxes();
            plots[7]->replot();

            ui->real_time_value_8->setText(QString("%1A").arg(double(unit.value(i))/10));
            break;


        default:break;
        }
    }

    HistoryValuesDatabase his_db(current_serial->settings().slave_addr);
    his_db.insert_values_to_tables(values);
}

void CustomerRTCurve::on_checkBox_chart_1_stateChanged(int state)
{

    if (state == Qt::Checked)
        plots[0]->show();
    else
        plots[0]->hide();
}

void CustomerRTCurve::on_checkBox_chart_2_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[1]->show();
    else
        plots[1]->hide();
}

void CustomerRTCurve::on_checkBox_chart_3_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[2]->show();
    else
        plots[2]->hide();
}

void CustomerRTCurve::on_checkBox_chart_4_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[3]->show();
    else
        plots[3]->hide();
}

void CustomerRTCurve::on_checkBox_chart_5_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[4]->show();
    else
        plots[4]->hide();
}

void CustomerRTCurve::on_checkBox_chart_6_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[5]->show();
    else
        plots[5]->hide();
}


void CustomerRTCurve::on_checkBox_chart_7_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[6]->show();
    else
        plots[6]->hide();
}

void CustomerRTCurve::on_checkBox_chart_8_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[7]->show();
    else
        plots[7]->hide();
}

void CustomerRTCurve::plots_mouseMove(QMouseEvent *event)
{
    QCustomPlot* plot = qobject_cast<QCustomPlot *>(sender());
    QSharedPointer<QCPGraphDataContainer> real_data = plot->graph(0)->data();

    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    for (int i = 0; i < real_data->size(); i++)
    {
        if ( qint64(real_data->findBegin(x)->key*1000) == qint64(real_data->at(i)->key*1000) )
        {
            if ( qAbs(qint64(y*10) - qint64(real_data->at(i)->value*10)) < 10 )
            {
                QString toolTips = QDateTime::fromMSecsSinceEpoch(qint64(real_data->at(i)->key*1000)).toString("yyyy-MM-dd hh:mm:ss");
                toolTips += " ";
                toolTips += QString::number(real_data->at(i)->value, 'f', 1);
                setToolTip(toolTips);
            }
            else
                setToolTip("");
        }
    }
}

void CustomerRTCurve::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

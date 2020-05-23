#include "hiscurve.h"
#include "ui_hiscurve.h"

#include "LoginInterface/logininterface.h"

HisCurve::HisCurve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HisCurve)
{
    ui->setupUi(this);

    checkboxes.resize(max_charts_num);
    pic_labels.resize(max_charts_num);
//    text_labels.resize(max_charts_num);

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

//    text_labels[0] = ui->real_time_value_1;
//    text_labels[1] = ui->real_time_value_2;
//    text_labels[2] = ui->real_time_value_3;
//    text_labels[3] = ui->real_time_value_4;
//    text_labels[4] = ui->real_time_value_5;
//    text_labels[5] = ui->real_time_value_6;
//    text_labels[6] = ui->real_time_value_7;
//    text_labels[7] = ui->real_time_value_8;

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

        connect(plots[i], &QCustomPlot::mouseMove, this, &HisCurve::on_plots_mouseMove);
    }

    ui->startDateTimeEdit->setDisabled(true);
    ui->endDateTimeEdit->setDisabled(true);

    setup_charts_checkboxes(TT01_TT08);

}

HisCurve::~HisCurve()
{
    delete ui;
}

void HisCurve::on_tabWidget_currentChanged(int index)
{
    QWidget *tmp_widget = ui->tabWidget->currentWidget();

    ui->groupBox->setParent(tmp_widget);
    ui->groupBox->show();

    ui->gridLayout_3->setParent(tmp_widget);
    ui->plots_groupbox->setParent(tmp_widget);
    ui->plots_groupbox->show();

    setup_charts_checkboxes(DisplayGroups(index));

    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    plot_set_color();
}

void HisCurve::setup_charts_checkboxes(DisplayGroups group)
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
//                text_labels[i]->show();
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
//                text_labels[i]->show();
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
//                text_labels[i]->show();
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
//                text_labels[i]->show();
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
//                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
//                text_labels[i]->hide();
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
//                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
//                text_labels[i]->hide();
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
                title[i]->setText(QString("AFM-%1(m/s)").arg(i+1));
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
//                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
//                text_labels[i]->hide();
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
//                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
//                text_labels[i]->hide();
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
//                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }
            else
            {
                checkboxes[i]->hide();
                pic_labels[i]->hide();
//                text_labels[i]->hide();
                plots[i]->hide();
            }
        }

        plot_set_color();

        break;
    case OthersChart:
        checkboxes[0]->setText("CM-01");
        title[0]->setText("CM-01(us/cm)");

        checkboxes[1]->setText("LT-01");
        title[1]->setText("LT-01(cm)");

        checkboxes[2]->setText("LT-02");
        title[2]->setText("LT-02(cm)");

        checkboxes[3]->setText("VT-01");
        title[3]->setText("VT-01(V)");

        checkboxes[4]->setText("IT-01");
        title[4]->setText("IT-01(A)");

        checkboxes[5]->setText("VT-02");
        title[5]->setText("VT-02(V)");

        checkboxes[6]->setText("IT-02");
        title[6]->setText("IT-02(A)");

        for (int i = 0; i < plots.size(); i++)
        {
            if (i < 7)
            {
                checkboxes[i]->show();
                pic_labels[i]->show();
//                text_labels[i]->show();
                checkboxes[i]->setChecked(true);
                plots[i]->show();
            }

            plots[i]->replot();
        }

        checkboxes[7]->hide();
        pic_labels[7]->hide();
//        text_labels[7]->hide();
        plots[7]->hide();

        plot_set_color();

        break;
    }
}

void HisCurve::plot_set_color()
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

void HisCurve::on_checkBox_chart_1_stateChanged(int state)
{

    if (state == Qt::Checked)
        plots[0]->show();
    else
        plots[0]->hide();
}

void HisCurve::on_checkBox_chart_2_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[1]->show();
    else
        plots[1]->hide();
}

void HisCurve::on_checkBox_chart_3_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[2]->show();
    else
        plots[2]->hide();
}

void HisCurve::on_checkBox_chart_4_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[3]->show();
    else
        plots[3]->hide();
}

void HisCurve::on_checkBox_chart_5_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[4]->show();
    else
        plots[4]->hide();
}

void HisCurve::on_checkBox_chart_6_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[5]->show();
    else
        plots[5]->hide();
}

void HisCurve::on_checkBox_chart_7_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[6]->show();
    else
        plots[6]->hide();
}

void HisCurve::on_checkBox_chart_8_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[7]->show();
    else
        plots[7]->hide();
}

QVector<qint64> HisCurve::get_time_interval(int index, QDateTime start, QDateTime end)
{
    QDateTime startDateTime, endDateTime;
    QVector<qint64> time;

    switch (index) {
    case CustomDates:
        if (end.toMSecsSinceEpoch() < start.toMSecsSinceEpoch())
        {
            QMessageBox::critical(this, "错误", "请选择正确的查询时间段");
        }
        else
        {
            startDateTime = start;
            endDateTime = end;
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

        break;

    case CurrentMonth:
        startDateTime = QDateTime::currentDateTime();
        startDateTime.setTime(QTime(0, 0, 0, 0));
        startDateTime.setDate(QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1));
        endDateTime = QDateTime::currentDateTime();
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

    time.append(startDateTime.toMSecsSinceEpoch());
    time.append(endDateTime.toMSecsSinceEpoch());

    return time;
}

void HisCurve::on_searchData_clicked()
{
    QVector<qint64> time(2);
    HistoryValuesDatabase db;

    time = get_time_interval(ui->quickSearch->currentIndex(), ui->startDateTimeEdit->dateTime(), ui->endDateTimeEdit->dateTime());
    display_history_values(db.search_values_from_tables(DisplayGroups(ui->tabWidget->currentIndex()), time[0], time[1]));
}

void HisCurve::on_exportData_clicked()
{
    QVector<qint64> time(2);
    time = get_time_interval(ui->quickSearch->currentIndex(), ui->startDateTimeEdit->dateTime(), ui->endDateTimeEdit->dateTime());

    HistoryValuesDatabase db;
    db.search_values_from_tables(DisplayGroups(ui->tabWidget->currentIndex()), time[0], time[1]);
}

void HisCurve::display_history_values(QVector<QVector<double>> result)
{
    foreach (auto tmp, result)
    {
        if (tmp.isEmpty())
        {
            QMessageBox::critical(this, "错误", "数据库中没有数据！");
            return;
        }
    }

    for (auto &tmp : result[0])
    {
        tmp /= double(1000.f);
//        qDebug() << QDateTime::fromSecsSinceEpoch(qint64(tmp)).toString("MM-dd hh:mm:ss.zzz");
    }

    for (int i = 0; i < result.size()-1; i++)
    {
        plots[i]->graph(0)->setData(result[0], result[i+1]);
        plots[i]->graph(0)->rescaleAxes();
        plots[i]->replot();
    }
}

void HisCurve::on_quickSearch_currentIndexChanged(int index)
{
//    ui->quickSearch
    if (index == CustomDates)
    {
        ui->startDateTimeEdit->setEnabled(true);
        ui->endDateTimeEdit->setEnabled(true);
    }
    else
    {
        ui->startDateTimeEdit->setDisabled(true);
        ui->endDateTimeEdit->setDisabled(true);
    }
}

void HisCurve::on_plots_mouseMove(QMouseEvent *event)
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

//    setToolTipDuration()
//    setToolTip("");

//    qDebug() << real_data->at(0)->key;

//    qDebug() << QDateTime::fromMSecsSinceEpoch(qint64(x)) << y;
}

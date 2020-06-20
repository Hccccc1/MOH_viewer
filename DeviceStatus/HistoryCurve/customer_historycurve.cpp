#include "customer_historycurve.h"
#include "ui_customer_historycurve.h"

#include "LoginInterface/logininterface.h"

customer_HistoryCurve::customer_HistoryCurve(QWidget *parent, QMutex *ope_mutex) :
    QWidget(parent),
    ui(new Ui::customer_HistoryCurve),
    operation_mutex(ope_mutex)
{
    ui->setupUi(this);

    plots.resize(6);
    titles.resize(6);

    for (int i = 0; i < plots.size(); i++)
    {
        plots[i] = new QCustomPlot(ui->plots_groupbox_customer);
        titles[i] = new QCPTextElement(plots[i], "", QFont("PingFang SC", 17, 300));

        plots[i]->plotLayout()->insertRow(0);
        plots[i]->plotLayout()->addElement(0, 0, titles[i]);

        plots[i]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        plots[i]->addGraph();

        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("MM-dd hh:mm");
        plots[i]->xAxis->setTicker(dateTicker);

        if (i < 3)
            ui->gridLayout_5->addWidget(plots[i], i, 0, 1, 1);
        else
            ui->gridLayout_5->addWidget(plots[i], i-3, 1, 1, 1);

        connect(plots[i], &QCustomPlot::mouseMove, this, &customer_HistoryCurve::plots_mouseMove);
    }

    ui->startDateTimeEdit_customer->setDisabled(true);
    ui->endDateTimeEdit_customer->setDisabled(true);

    plot_set_color();

    titles[0]->setText("LT-01(cm)");
    titles[1]->setText("LT-02(cm)");
    titles[2]->setText("VT-01(V)");
    titles[3]->setText("IT-01(A)");
    titles[4]->setText("VT-02(V)");
    titles[5]->setText("IT-02(A)");
}

customer_HistoryCurve::~customer_HistoryCurve()
{
    delete ui;
}

void customer_HistoryCurve::plot_set_color()
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
        }
    }
}

QVector<qint64> customer_HistoryCurve::get_time_interval(int index, QDateTime start, QDateTime end)
{
    QDateTime startDateTime, endDateTime;
    QVector<qint64> time;

    switch (index) {
    case CustomDates:
        if (end.toMSecsSinceEpoch() < start.toMSecsSinceEpoch())
        {
            QMessageBox::critical(this, tr("错误"), tr("请选择正确的查询时间段"));
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

void customer_HistoryCurve::on_checkBox_customer_chart_1_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[0]->show();
    else
        plots[0]->hide();
}

void customer_HistoryCurve::on_checkBox_customer_chart_2_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[1]->show();
    else
        plots[1]->hide();
}

void customer_HistoryCurve::on_checkBox_customer_chart_3_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[2]->show();
    else
        plots[2]->hide();
}

void customer_HistoryCurve::on_checkBox_customer_chart_4_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[3]->show();
    else
        plots[3]->hide();
}

void customer_HistoryCurve::on_checkBox_customer_chart_5_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[4]->show();
    else
        plots[4]->hide();
}

void customer_HistoryCurve::on_checkBox_customer_chart_6_stateChanged(int state)
{
    if (state == Qt::Checked)
        plots[5]->show();
    else
        plots[5]->hide();
}


void customer_HistoryCurve::on_searchData_customer_clicked()
{
    QVector<qint64> time(2);
    HistoryValuesDatabase his_db;

    time = get_time_interval(ui->quickSearch_customer->currentIndex(), ui->startDateTimeEdit_customer->dateTime(), ui->endDateTimeEdit_customer->dateTime());
    display_history_values(his_db.search_values_from_tables(OthersChart, time[0], time[1]));
}

void customer_HistoryCurve::on_exportData_customer_clicked()
{
    QVector<qint64> time(2);
    QVector<QVector<double>> result;

    time = get_time_interval(ui->quickSearch_customer->currentIndex(), ui->startDateTimeEdit_customer->dateTime(), ui->endDateTimeEdit_customer->dateTime());

    HistoryValuesDatabase db;
    result = db.search_values_from_tables(OthersChart, time[0], time[1]);

    //    for (int i = 0; i < result[0].size(); i++)
    //        qDebug() << __FILE__ << __LINE__ << QDateTime::fromMSecsSinceEpoch(result[0][i]);

    for (auto tmp : result)
    {
        if (tmp.isEmpty())
        {
            QMessageBox::critical(this, tr("错误"), tr("数据库中没有数据！"));
            return;
        }
    }

    QFile save_file;

    operation_mutex->lock();

    QString save_filename = QFileDialog::getSaveFileName(this, tr("保存至"), "", tr("Excel data file (*.csv)"));

    operation_mutex->unlock();

    save_file.setFileName(save_filename);
    //    if (QFile::open(save_filename))

    if (!save_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        QMessageBox::critical(this, tr("错误"), tr("文件打开失败！"));
    else
    {
        QTextStream stream(&save_file);

        stream << ',' << "LT01(cm)" << ',' << "LT02(cm)" << ',' << "VT01(V)" << ',' << "IT01(A)" << ',' << "VT02(V)" << ',' << "IT02(A)" << '\n';

        for (int i = 0; i < result[0].size(); i++)
        {
            for (int j = 0; j < result.size(); j++)
            {
                if ( j == 0 )
                {
                    stream << QDateTime::fromMSecsSinceEpoch(qint64(result[j][i])).toString("\tyyyy-MM-dd HH:mm:ss\t") << ',';
                }
                else if ( j == 1 )
                    continue;
                else
                    stream << result[j][i] << ',';
            }
            stream << '\n';
        }

        save_file.close();
    }
}


void customer_HistoryCurve::display_history_values(QVector<QVector<double>> result)
{
    foreach (auto tmp, result)
    {
        if (tmp.isEmpty())
        {
            QMessageBox::critical(this, tr("错误"), tr("数据库中没有数据！"));
            return;
        }
    }

    for (auto &tmp : result[0])
    {
        tmp /= double(1000.f);
        //        qDebug() << QDateTime::fromSecsSinceEpoch(qint64(tmp)).toString("MM-dd hh:mm:ss.zzz");
    }

    for (int i = 1; i < result.size()-1; i++)
    {
        plots[i-1]->graph(0)->setData(result[0], result[i+1]);
        plots[i-1]->graph(0)->rescaleAxes();
        plots[i-1]->replot();
    }
}

void customer_HistoryCurve::on_quickSearch_customer_currentIndexChanged(int index)
{
    if (index == CustomDates)
    {
        ui->startDateTimeEdit_customer->setEnabled(true);
        ui->endDateTimeEdit_customer->setEnabled(true);
    }
    else
    {
        ui->startDateTimeEdit_customer->setDisabled(true);
        ui->endDateTimeEdit_customer->setDisabled(true);
    }
}


void customer_HistoryCurve::plots_mouseMove(QMouseEvent *event)
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

void customer_HistoryCurve::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}


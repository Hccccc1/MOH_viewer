#include "hiscurve.h"
#include "ui_hiscurve.h"

#include "LoginInterface/logininterface.h"

HisCurve::HisCurve(QWidget *parent, QMutex *ope_mutex) :
    QWidget(parent),
    ui(new Ui::HisCurve),
    operation_mutex(ope_mutex)
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
        dateTicker->setDateTimeFormat("MM-dd hh:mm");
        plots[i]->xAxis->setTicker(dateTicker);

        if (i < 4)
            ui->gridLayout_4->addWidget(plots[i], i, 0, 1, 1);
        else
            ui->gridLayout_4->addWidget(plots[i], i-4, 1, 1, 1);

        connect(plots[i], &QCustomPlot::mouseMove, this, &HisCurve::plots_mouseMove);
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

    for (auto *plot : plots)
    {
        plot->clearGraphs();
        plot->addGraph();
    }

    setup_charts_checkboxes(DisplayGroups(index));

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

    //    if (QMessageBox::question())
//    QMessageBox msg_box(this);
//    msg_box.setIcon(QMessageBox::Question);
//    QPushButton *currentPage_btn = msg_box.addButton(tr("当前页面数据"), QMessageBox::DestructiveRole);
//    QPushButton *allPages_btn = msg_box.addButton(tr("所有数据"), QMessageBox::DestructiveRole);
//    //    msg_box.addButton("当前页面数据", QMessageBox::DestructiveRole);
//    //    msg_box.addButton("该时间段内所有数据", QMessageBox::DestructiveRole);

//    msg_box.exec();

//    if (msg_box.clickedButton() == currentPage_btn)
//        qDebug() << "currentPage_btn";
//    else if (msg_box.clickedButton() == allPages_btn)
//        qDebug() << "all page btn";

    //    if (msg)

    //    connect(&msg_box, &QMessageBox::buttonClicked, this, [=](QAbstractButton *button) {
    //                qDebug() << button->text();
    //            });

    //    int result = msg_box.exec();

//    if (QMessageBox::question(this, tr("数据导出"), tr("选择需要导出的数据"), tr("当前页面数据"), tr("所有数据")) == 0)
    if (ui->current_or_all->currentIndex() == 0)
    {
        QVector<QVector<double>> result;
        HistoryValuesDatabase db;
        result = db.search_values_from_tables(DisplayGroups(ui->tabWidget->currentIndex()), time[0], time[1]);

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

        if (!save_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            QMessageBox::critical(this, tr("错误"), tr("文件打开失败！"));
        else
        {
            QTextStream stream(&save_file);

            switch (DisplayGroups(ui->tabWidget->currentIndex())) {
            case TT01_TT08:stream << ',' << "TT01(\260C)" << ',' << "TT02(\260C)" << ',' << "TT03(\260C)" << ',' << "TT04(\260C)" << ',' << "TT05(\260C)" << ',' << "TT06(\260C)" << ',' << "TT07(\260C)" << ',' << "TT08(\260C)" << '\n';break;
            case TT09_TT16:stream << ',' << "TT09(\260C)" << ',' << "TT10(\260C)" << ',' << "TT11(\260C)" << ',' << "TT12(\260C)" << ',' << "TT13(\260C)" << ',' << "TT14(\260C)" << ',' << "TT15(\260C)" << ',' << "TT16(\260C)" << '\n';break;
            case TT17_TT24:stream << ',' << "TT17(\260C)" << ',' << "TT18(\260C)" << ',' << "TT19(\260C)" << ',' << "TT20(\260C)" << ',' << "TT21(\260C)" << ',' << "TT22(\260C)" << ',' << "TT23(\260C)" << ',' << "TT24(\260C)" << '\n';break;
            case TT25_TT32:stream << ',' << "TT25(\260C)" << ',' << "TT26(\260C)" << ',' << "TT27(\260C)" << ',' << "TT28(\260C)" << ',' << "TT29(\260C)" << ',' << "TT30(\260C)" << ',' << "TT31(\260C)" << ',' << "TT32(\260C)" << '\n';break;
            case TT33_TT36:stream << ',' << "TT33(\260C)" << ',' << "TT34(\260C)" << ',' << "TT35(\260C)" << ',' << "TT36(\260C)" << '\n';break;
            case PressureChart:stream << ',' << "PT01(KPa)" << ',' << "PT02(KPa)" << ',' << "PT03(KPa)" << ',' << "PT04(KPa)" << ',' << "PT05(KPa)" << ',' << "PT06(KPa)" << '\n';break;
            case FlowChart:stream << ',' << "AFM01(m/s)" << ',' << "AFM02(m/s)" << ',' << "AFM03(m/s)" << ',' << "AFM04(m/s)" << ',' << "MFM01(g/min)" << '\n';break;
            case SpeedChart_1:stream << ',' << "BL01(rpm)" << ',' << "BL02(rpm)" << ',' << "BL03(rpm)" << ',' << "BL04(rpm)" << '\n';break;
            case SpeedChart_2:stream << ',' << "PMP01(rpm)" << ',' << "PMP02(rpm)" << ',' << "PMP03(rpm)" << ',' << "PMP04(rpm)" << ',' << "PMP05(rpm)" << ',' << "RAD01(rpm)" << '\n';break;
            case OthersChart:stream << ',' << "CM01(us/cm)" << ',' << "LT01(cm)" << ',' << "LT02(cm)" << ',' << "VT01(V)" << ',' << "IT01(A)" << ',' << "VT02(V)" << ',' << "IT02(A)" << '\n';break;
                //        default:break;
            }

            for (int i = 0; i < result[0].size(); i++)
            {
                for (int j = 0; j < result.size(); j++)
                {
                    if ( j == 0 )
                        stream << QDateTime::fromMSecsSinceEpoch(qint64(result[j][i])).toString("\tyyyy-MM-dd HH:mm:ss\t") << ',';
                    else
                        stream << result[j][i] << ',';
                }
                stream << '\n';
            }

            save_file.close();
        }
    }
    else
    {
        //        QVector<QVector<QVector<quint16>>>
        QVector<QVector<double>> result[10];
        for (int i = TT01_TT08; i <= OthersChart; i++)
        {
            HistoryValuesDatabase db;
            result[i] = db.search_values_from_tables(DisplayGroups(i), time[0], time[1]);

            for (auto tmp : result[i])
            {
                if (tmp.isEmpty())
                {
                    QMessageBox::critical(this, tr("错误"), tr("数据库中没有数据！"));
                    return;
                }
            }
        }

        QFile save_file;

        operation_mutex->lock();

        QString save_filename = QFileDialog::getSaveFileName(this, tr("保存至"), "", tr("Excel data file (*.csv)"));

        operation_mutex->unlock();

        save_file.setFileName(save_filename);

        if (!save_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            QMessageBox::critical(this, tr("错误"), tr("文件打开失败！"));
        else
        {
            QTextStream stream(&save_file);

            stream << ',' << "TT01(\260C)" << ',' << "TT02(\260C)" << ',' << "TT03(\260C)" << ',' << "TT04(\260C)" << ',' << "TT05(\260C)" << ',' << "TT06(\260C)" << ',' << "TT07(\260C)" << ',' << "TT08(\260C)" \
                   << ',' << "TT09(\260C)" << ',' << "TT10(\260C)" << ',' << "TT11(\260C)" << ',' << "TT12(\260C)" << ',' << "TT13(\260C)" << ',' << "TT14(\260C)" << ',' << "TT15(\260C)" << ',' << "TT16(\260C)" \
                   << ',' << "TT17(\260C)" << ',' << "TT18(\260C)" << ',' << "TT19(\260C)" << ',' << "TT20(\260C)" << ',' << "TT21(\260C)" << ',' << "TT22(\260C)" << ',' << "TT23(\260C)" << ',' << "TT24(\260C)" \
                   << ',' << "TT25(\260C)" << ',' << "TT26(\260C)" << ',' << "TT27(\260C)" << ',' << "TT28(\260C)" << ',' << "TT29(\260C)" << ',' << "TT30(\260C)" << ',' << "TT31(\260C)" << ',' << "TT32(\260C)" \
                   << ',' << "TT33(\260C)" << ',' << "TT34(\260C)" << ',' << "TT35(\260C)" << ',' << "TT36(\260C)" \
                   << ',' << "PT01(KPa)" << ',' << "PT02(KPa)" << ',' << "PT03(KPa)" << ',' << "PT04(KPa)" << ',' << "PT05(KPa)" << ',' << "PT06(KPa)" \
                   << ',' << "AFM01(m/s)" << ',' << "AFM02(m/s)" << ',' << "AFM03(m/s)" << ',' << "AFM04(m/s)" << ',' << "MFM01(g/min)" \
                   << ',' << "BL01(rpm)" << ',' << "BL02(rpm)" << ',' << "BL03(rpm)" << ',' << "BL04(rpm)" \
                   << ',' << "PMP01(rpm)" << ',' << "PMP02(rpm)" << ',' << "PMP03(rpm)" << ',' << "PMP04(rpm)" << ',' << "PMP05(rpm)" << ',' << "RAD01(rpm)" \
                   << ',' << "CM01(us/cm)" << ',' << "LT01(cm)" << ',' << "LT02(cm)" << ',' << "VT01(V)" << ',' << "IT01(A)" << ',' << "VT02(V)" << ',' << "IT02(A)" \
                   << '\n';

            // i -> cow number; j -> group number; k -> column
            for ( int i = 0; i < result[0][0].size(); i++)
            {
                for ( int j = 0; j < 10; j++)
                {
                    for ( int k = 0; k < result[j].size(); k++)
                    {
                        if ( j == 0 && k == 0)
                            stream << QDateTime::fromMSecsSinceEpoch(qint64(result[j][k][i])).toString("\tyyyy-MM-dd HH:mm:ss\t") << ',';
                        else if ( k == 0 )
                            continue;
                        else
                            stream << result[j][k][i] << ',';
                    }
                }
                stream << '\n';
            }
            save_file.close();
        }
    }
}

void HisCurve::display_history_values(QVector<QVector<double>> result)
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

void HisCurve::plots_mouseMove(QMouseEvent *event)
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

void HisCurve::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

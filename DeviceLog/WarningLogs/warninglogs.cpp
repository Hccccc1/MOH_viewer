#include <QFileDialog>

#include "warninglogs.h"
#include "ui_warninglogs.h"

WarningLogs::WarningLogs(QWidget *parent, int slave_addr) :
    QDialog(parent),
    ui(new Ui::WarningLogs),
    m_slave_addr(slave_addr)
{
    ui->setupUi(this);

    db_name = tmp_db_name.arg(m_slave_addr);

    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("内容")));
    model->setItem(0, 2, new QStandardItem(tr("等级")));

    //    model->setItem(1, 0, new QStandardItem(tr("2020/4/19 4:17")));
    //    model->setItem(1, 1, new QStandardItem(tr("1asdqweqasdfcvzxfasdf")));
    //    model->setItem(1, 2, new QStandardItem(tr("SuperUser")));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();

    ui->startDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->startDateTimeEdit->setDisabled(true);
    ui->endDateTimeEdit->setDisabled(true);
}

WarningLogs::~WarningLogs()
{
    delete ui;
}

void WarningLogs::resizeEvent(QResizeEvent *event)
{
    //    qDebug() << __FILE__ << __LINE__ << event->size();

    column_time = static_cast<int>(event->size().width()*0.12);
    column_content = static_cast<int>(event->size().width()*0.77);
    column_level = event->size().width() - column_time - column_content-20;

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);
}

void WarningLogs::addWarningRecord(QString first_column, QString second_column)
{
//    if (sender() == parent()->parent()->parent()->parent()->parent()->parent()->parent())
    {
        LogDatabase warning_database = LogDatabase(db_name, table_name, WarningLog);

        QVector<QString> res = warning_database.get_newest_data();
        QString tmp_warning, tmp_first_column;

        if (first_column.contains(tr("报警消除")))
        {
            tmp_warning = first_column;
            tmp_first_column = first_column.mid(0, first_column.indexOf(tr("报警消除")));
        }
        else
        {
            tmp_first_column = first_column;
            tmp_warning = first_column + tr("报警消除");
        }

        quint8 pos_warn = 0xff, pos_dis = 0;

        for (int i = res.size() - 1; i >= 0; i--)
        {
            if (res[i] == tmp_first_column)
            {
                pos_warn = i+1;
            }
            if (res[i] == tmp_warning)
            {
                pos_dis = i+1;
            }
        }

        switch (pos_warn) {
        case 0xff:
            warning_database.insert_values_into_table(table_name, first_column, second_column);
            break;
        default:
            if (pos_warn < pos_dis)
            {
                if (first_column.contains("报警消除"))
                {
                    warning_database.insert_values_into_table(table_name, first_column, second_column);
                }
                else
                    return;
            }
//            else if (pos_dis != 0 && pos_warn > pos_dis)
            else
            {
                if (pos_dis != 0)
                {
                    if (first_column.contains("报警消除"))
                        return;
                    else
                        warning_database.insert_values_into_table(table_name, first_column, second_column);
                }
                else
                {
                    if (first_column.contains("报警消除"))
                        warning_database.insert_values_into_table(table_name, first_column, second_column);
                }
            }
            break;
        }

    }


//    if ( (!pos_dis && pos_warn) ||  pos_warn < pos_dis )
//    {
//        qDebug() << "pos_dis = " << pos_dis << "pos_warn = " << pos_warn;
//        warning_database.insert_values_into_table(table_name, first_column, second_column);
//    }
}

void WarningLogs::reset_model()
{
    model->clear();
    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("内容")));
    model->setItem(0, 2, new QStandardItem(tr("等级")));

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);

    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();
}

void WarningLogs::on_getDataBtn_clicked()
{
    QDateTime startDateTime, endDateTime;
    qint64 start_time, end_time;
    //    QVector<QVector<QString>> tmp_result;

    switch (ui->quickSearch->currentIndex()) {
    case CustomDates:
        if (ui->endDateTimeEdit->dateTime().toMSecsSinceEpoch() < ui->startDateTimeEdit->dateTime().toMSecsSinceEpoch())
        {
            QMessageBox::critical(this, tr("错误"), tr("请选择正确的查询时间段"));
            return;
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

    start_time = startDateTime.toMSecsSinceEpoch();
    end_time = endDateTime.toMSecsSinceEpoch();

//    qDebug() << startDateTime << endDateTime;

    QString search_db_name = tmp_db_name.arg(ui->slaveAddSpinBox->value());

    LogDatabase warning_database = LogDatabase(search_db_name, table_name, WarningLog);
    search_result = warning_database.get_columns_by_time(start_time, end_time);

    if (search_result[0].isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("没有数据！"));
    }
    else
    {

//        ui->tableView->setModel(model);
        reset_model();

        ui->jump_to_page->setText(QString::number(1));
        if (search_result[0].size()%records_per_page == 0)
        {
            total_pages = search_result[0].size()/records_per_page;
            records_not_full = 0;
            ui->total_num_pages->setText(QString::number(total_pages));
        }
        else
        {
            total_pages = search_result[0].size()/records_per_page + 1;
            records_not_full = search_result[0].size()%records_per_page;
            ui->total_num_pages->setText(QString::number(total_pages));
        }

        for (quint64 i = 0; i < ((total_pages==1&&(records_not_full!=0))?(records_not_full):(records_per_page)); i++)
        {
            QStandardItem *result_show = new QStandardItem(search_result[1][i]);

            if (search_result[1][i].contains(tr("报警消除")))
                result_show->setForeground(QBrush(Qt::green));
            else
                result_show->setForeground(QBrush(Qt::red));

            QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem(i+1, 0, new QStandardItem(first_column));
            model->setItem(i+1, 1, result_show);
            model->setItem(i+1, 2, new QStandardItem(search_result[2][i]));
        }

        //        model->item()
    }
}

void WarningLogs::on_quickSearch_currentIndexChanged(int index)
{
    if (index != CustomDates)
    {
        ui->startDateTimeEdit->setDisabled(true);
        ui->endDateTimeEdit->setDisabled(true);
    }
    else
    {
        ui->startDateTimeEdit->setEnabled(true);
        ui->endDateTimeEdit->setEnabled(true);
    }
}

void WarningLogs::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        model->item(0, 0)->setText(tr("时间"));
        model->item(0, 1)->setText(tr("内容"));
        model->item(0, 2)->setText(tr("等级"));

        ui->retranslateUi(this);
    }
}

void WarningLogs::on_last_page_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toInt();

        if (current_page > 1)
        {
            current_page--;

            ui->jump_to_page->setText(QString::number(current_page));

//            model->removeRows(1, 18);
            reset_model();

            for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
            {
                QStandardItem *result_show = new QStandardItem(search_result[1][i]);

                if (search_result[1][i].contains(tr("报警消除")))
                    result_show->setForeground(QBrush(Qt::green));
                else
                    result_show->setForeground(QBrush(Qt::red));

                QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
                model->setItem((i-(current_page-1)*records_per_page)+1, 1, result_show);
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(search_result[2][i]));
            }
        }
    }
}

void WarningLogs::on_move_to_last_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->total_num_pages->text().toUInt();
        ui->jump_to_page->setText(QString::number(current_page));

//        model->removeRows(1, 18);

        reset_model();

        for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
        {
            QStandardItem *result_show = new QStandardItem(search_result[1][i]);

            if (search_result[1][i].contains(tr("报警消除")))
                result_show->setForeground(QBrush(Qt::green));
            else
                result_show->setForeground(QBrush(Qt::red));

            QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
            model->setItem((i-(current_page-1)*records_per_page)+1, 1, result_show);
            model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(search_result[2][i]));
        }
    }
}

void WarningLogs::on_move_to_first_clicked()
{
    if (!search_result.isEmpty())
    {
        ui->jump_to_page->setText(QString::number(1));

//        model->removeRows(1, 18);
        reset_model();

        for (quint64 i = 0; i < ((1==total_pages) ? (records_not_full):(records_per_page)); i++)
        {
            QStandardItem *result_show = new QStandardItem(search_result[1][i]);

            if (search_result[1][i].contains(tr("报警消除")))
                result_show->setForeground(QBrush(Qt::green));
            else
                result_show->setForeground(QBrush(Qt::red));

            QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem(i+1, 0, new QStandardItem(first_column));
            model->setItem(i+1, 1, result_show);
            model->setItem(i+1, 2, new QStandardItem(search_result[2][i]));
        }
    }
}

void WarningLogs::on_next_page_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toUInt();

        if (current_page != ui->total_num_pages->text().toUInt())
        {
            current_page++;

            ui->jump_to_page->setText(QString::number(current_page));

//            model->removeRows(1, 18);
            reset_model();

            for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
            {
                QStandardItem *result_show = new QStandardItem(search_result[1][i]);

                if (search_result[1][i].contains(tr("报警消除")))
                    result_show->setForeground(QBrush(Qt::green));
                else
                    result_show->setForeground(QBrush(Qt::red));

                QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
                model->setItem((i-(current_page-1)*records_per_page)+1, 1, result_show);
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(search_result[2][i]));
            }
        }
    }
}

void WarningLogs::on_jump_to_page_btn_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toUInt();

//        model->removeRows(1, 18);
        reset_model();

        if (current_page > total_pages || current_page < 1)
        {
            ui->jump_to_page->setText(QString::number(0));
            QMessageBox::warning(this, tr("错误！"), tr("请输入正确的页数"));
        }
        else
        {
            for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
            {
                QStandardItem *result_show = new QStandardItem(search_result[1][i]);

                if (search_result[1][i].contains(tr("报警消除")))
                    result_show->setForeground(QBrush(Qt::green));
                else
                    result_show->setForeground(QBrush(Qt::red));

                QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
                model->setItem((i-(current_page-1)*records_per_page)+1, 1, result_show);
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(search_result[2][i]));
            }
        }
    }
}

void WarningLogs::on_exportDataBtn_clicked()
{
    QDateTime startDateTime, endDateTime;
    qint64 start_time, end_time;
    //    QVector<QVector<QString>> tmp_result;

    switch (ui->quickSearch->currentIndex()) {
    case CustomDates:
        if (ui->endDateTimeEdit->dateTime().toMSecsSinceEpoch() < ui->startDateTimeEdit->dateTime().toMSecsSinceEpoch())
        {
            QMessageBox::critical(this, tr("错误"), tr("请选择正确的查询时间段"));
            return;
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

    start_time = startDateTime.toMSecsSinceEpoch();
    end_time = endDateTime.toMSecsSinceEpoch();

//    qDebug() << startDateTime << endDateTime;

    QString search_db_name = tmp_db_name.arg(ui->slaveAddSpinBox->value());

    LogDatabase warning_database = LogDatabase(search_db_name, table_name, WarningLog);
    search_result = warning_database.get_columns_by_time(start_time, end_time);

    if (search_result[0].isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("数据库中没有数据，请输入正确的查询时间段"));
        return;
    }

    QFile warningDataFile;

    emit operation_needs_lock();

    QString warningDataPath = QFileDialog::getSaveFileName(this, "Choose file to save.", "", tr("WarningDataFile (*.csv)"));

    warningDataFile.setFileName(warningDataPath);

    if (warningDataPath.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("文件打开失败"));
        emit operation_release_lock();
    }
    else
    {
        if (!warningDataFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QMessageBox::critical(this, tr("错误"), tr("文件打开失败"));
            emit operation_release_lock();
        }
        else
        {
            emit operation_release_lock();

            QTextStream stream(&warningDataFile);

            stream << tr("时间") << "," << tr("内容") << "," << tr("等级") << '\n';

            for (int i = 0; i < search_result[0].size(); i++)
            {
                for (int j = 0; j < search_result.size(); j++)
                {
                    if (j == 0)
                    {
                        //                    bool ok;
                        //                    qDebug() << search_result[j][i].toDouble();
                        stream << QDateTime::fromMSecsSinceEpoch(search_result[j][i].toDouble()).toString("\tyyyy-MM-dd HH:mm:ss\t") << ',';
                    }

                    else
                        stream << search_result[j][i] << ',';
                }
                stream << '\n';
            }

            warningDataFile.close();
        }
    }
}

void WarningLogs::change_slave_addr(int slave_addr)
{
    m_slave_addr = slave_addr;

    db_name = tmp_db_name.arg(m_slave_addr);
}


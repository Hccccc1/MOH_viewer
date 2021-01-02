#include "operationlogs.h"
#include "ui_operationlogs.h"

#include <QFileDialog>
#include <QStandardItemModel>

OperationLogs::OperationLogs(QWidget *parent, int slave_addr) :
    QDialog(parent),
    ui(new Ui::OperationLogs),
    m_slave_addr(slave_addr)
{
    ui->setupUi(this);

    db_name = tmp_db_name.arg(m_slave_addr);

    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("内容")));
    model->setItem(0, 2, new QStandardItem(tr("操作员")));

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

    //    database.create_database_table(db_name, table_name, OperationLog);
}

OperationLogs::~OperationLogs()
{
    delete ui;
}

void OperationLogs::resizeEvent(QResizeEvent *event)
{
    column_time = static_cast<int>(event->size().width()*0.12);
    column_content = static_cast<int>(event->size().width()*0.77);
    column_level = event->size().width() - column_time - column_content-20;

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);
}

void OperationLogs::addOperationRecord(QString first_column, Accounts account)
{
//    qDebug() << sender() << parent()->parent()->parent()->parent()->parent()->parent()->parent();

//    if (sender() == parent()->parent()->parent()->parent()->parent()->parent()->parent())
    {
        QString second_column;

        switch (account) {
        case SuperUser:second_column = QString("SuperUser");break;
        case Technician:second_column = QString("Technician");break;
        case Customer:second_column = QString("Customer");break;
        }

        LogDatabase operation_database = LogDatabase(db_name, table_name, OperationLog);
        operation_database.insert_values_into_table(table_name, first_column, second_column);
    }
}

void OperationLogs::reset_model()
{
    model->clear();
    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("内容")));
    model->setItem(0, 2, new QStandardItem(tr("操作员")));

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);

    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();
}

void OperationLogs::on_getDataBtn_clicked()
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

    LogDatabase operation_database = LogDatabase(search_db_name, table_name, OperationLog);
    search_result = operation_database.get_columns_by_time(start_time, end_time);

    if (search_result[0].isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("没有数据！"));
    }
    else
    {
//        model->removeRows(1, 18);

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
            QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem(i+1, 0, new QStandardItem(first_column));
            model->setItem(i+1, 1, new QStandardItem(search_result[1][i]));
            model->setItem(i+1, 2, new QStandardItem(search_result[2][i]));
        }
    }

    qDebug() << QDateTime::currentDateTime() << ":" << "查询结束并返回。";
}

void OperationLogs::on_dataExportBtn_clicked()
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

    QString search_db_name = tmp_db_name.arg(ui->slaveAddSpinBox->value());

    LogDatabase operation_database = LogDatabase(search_db_name, table_name, OperationLog);
    search_result = operation_database.get_columns_by_time(start_time, end_time);

    if (search_result[0].isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("数据库中没有数据，请输入正确的查询时间段"));
        return;
    }

    QFile operationDataFile;

    emit operation_needs_lock();

    QString operationDataPath = QFileDialog::getSaveFileName(this, "Choose file to save.", "", tr("OperationDataFile (*.csv)"));

    operationDataFile.setFileName(operationDataPath);

    if (operationDataPath.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("文件打开失败"));
        emit operation_release_lock();
    }
    else
    {
        if (!operationDataFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QMessageBox::critical(this, tr("错误"), tr("文件打开失败"));
            emit operation_release_lock();
        }
        else
        {
            emit operation_release_lock();

            QTextStream stream(&operationDataFile);

            stream << tr("时间") << "," << tr("内容") << "," << tr("操作员") << "\n";

            for (int i = 0; i < search_result[0].size(); i++)
            {
                for (int j = 0; j < search_result.size(); j++)
                {
                    if (j == 0)
                    {
                        stream << QDateTime::fromMSecsSinceEpoch(search_result[j][i].toDouble()).toString("\tyyyy-MM-dd HH:mm:ss\t") << ",";
                    }
                    else
                    {
                        stream << search_result[j][i] << ",";
                    }
                }
                stream << "\n";
            }

            operationDataFile.close();
        }
    }
}

void OperationLogs::on_quickSearch_currentIndexChanged(int index)
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

void OperationLogs::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        model->item(0, 0)->setText(tr("时间"));
        model->item(0, 1)->setText(tr("内容"));
        model->item(0, 2)->setText(tr("操作员"));

        ui->retranslateUi(this);
    }
}

void OperationLogs::on_last_page_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toInt();

        if (current_page > 1)
        {
            current_page--;

            ui->jump_to_page->setText(QString::number(current_page));

            reset_model();

            for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
            {
                QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
                model->setItem((i-(current_page-1)*records_per_page)+1, 1, new QStandardItem(search_result[1][i]));
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(search_result[2][i]));
            }
        }
    }
}

void OperationLogs::on_move_to_last_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->total_num_pages->text().toUInt();
        ui->jump_to_page->setText(QString::number(current_page));

        reset_model();

        for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
        {
            QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
            model->setItem((i-(current_page-1)*records_per_page)+1, 1, new QStandardItem(search_result[1][i]));
            model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(search_result[2][i]));
        }
    }
}

void OperationLogs::on_move_to_first_clicked()
{
    if (!search_result.isEmpty())
    {
        ui->jump_to_page->setText(QString::number(1));

        reset_model();

        for (quint64 i = 0; i < ((1==total_pages) ? (records_not_full):(records_per_page)); i++)
        {
            QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem(i+1, 0, new QStandardItem(first_column));
            model->setItem(i+1, 1, new QStandardItem(search_result[1][i]));
            model->setItem(i+1, 2, new QStandardItem(search_result[2][i]));
        }
    }
}

void OperationLogs::on_next_page_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toUInt();

        if (current_page != ui->total_num_pages->text().toUInt())
        {
            current_page++;

            ui->jump_to_page->setText(QString::number(current_page));

            reset_model();

            for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
            {
                QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
                model->setItem((i-(current_page-1)*records_per_page)+1, 1, new QStandardItem(search_result[1][i]));
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(search_result[2][i]));
            }
        }
    }
}

void OperationLogs::on_jump_to_page_btn_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toInt();

        reset_model();

        if (current_page > total_pages || current_page < 1U)
        {
            ui->jump_to_page->setText(QString::number(0));
            QMessageBox::warning(this, tr("错误！"), tr("请输入正确的页数"));
        }
        else
        {
            for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
            {
                QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
                model->setItem((i-(current_page-1)*records_per_page)+1, 1, new QStandardItem(search_result[1][i]));
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(search_result[2][i]));
            }
        }
    }
}

void OperationLogs::change_slave_addr(int slave_addr)
{
    m_slave_addr = slave_addr;

    db_name = tmp_db_name.arg(m_slave_addr);
}

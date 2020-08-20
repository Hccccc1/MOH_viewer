#include "communicationlogs.h"
#include "ui_communicationlogs.h"

#include <QFileDialog>
#include <QMessageBox>

CommunicationLogs::CommunicationLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicationLogs)
{
    ui->setupUi(this);

    model->setItem(0, 0, new QStandardItem(tr("时间")));
    model->setItem(0, 1, new QStandardItem(tr("类型")));
    model->setItem(0, 2, new QStandardItem(tr("帧数据")));

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();

    ui->startDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    ui->startDateTimeEdit->setDisabled(true);
    ui->endDateTimeEdit->setDisabled(true);

    //            ui->last_page
    //            ui->move_to_last
    //            ui->move_to_first
    //            ui->next_page
    //            ui->jump_to_page
    //            ui->total_num_pages
}

CommunicationLogs::~CommunicationLogs()
{
    delete ui;
}

void CommunicationLogs::resizeEvent(QResizeEvent *event)
{
    int column_time = static_cast<int>(event->size().width()*0.20);
    int column_content = static_cast<int>(event->size().width()*0.12);
    int column_level = event->size().width() - column_time - column_content-20;

    ui->tableView->setColumnWidth(0, column_time);
    ui->tableView->setColumnWidth(1, column_content);
    ui->tableView->setColumnWidth(2, column_level);

}

void CommunicationLogs::addCommunicationRecord(QString first_column, QString second_column)
{
    commu_database.insert_values_into_table(table_name, first_column, second_column);
}

//void CommunicationLogs::on_pushButton_clicked()
//{
//    addCommunicationRecord("asdasd", "qweqrtr");
//}

void CommunicationLogs::on_getDataBtn_clicked()
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

    search_result = commu_database.get_columns_by_time(start_time, end_time);

    if (search_result[0].isEmpty())
    {
        //        QString dbg_msg = "在"
        QMessageBox::critical(this, tr("错误"), tr("数据库中没有数据！"));
    }
    else
    {
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
            //            qDebug() << tmp_result[2][i];
            QString tmp_array;
            for (int j = 0; j < search_result[2][i].size()-1; j += 2)
            {
                tmp_array.append(search_result[2][i][j]);
                tmp_array.append(search_result[2][i][j+1]);
                tmp_array.append(" ");
            }
            model->setItem(i+1, 2, new QStandardItem(tmp_array));
        }
    }
}

void CommunicationLogs::on_exportDataBtn_clicked()
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

    qDebug() << startDateTime << endDateTime;

    search_result = commu_database.get_columns_by_time(start_time, end_time);

    if (search_result[0].isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("数据库中没有数据，请输入正确的查询时间段！"));
        return;
    }

    QFile commuDataFile;

    emit operation_needs_lock();

    QString commuDataPath = QFileDialog::getSaveFileName(this, "Choose file to save.", "", tr("CommunicationDataFile (*.csv)"));
//    QString operationDataPath = QFileDialog::getSaveFileName(this, "Choose file to save.", "", tr("OperationDataFile (*.csv)"));

    commuDataFile.setFileName(commuDataPath);

    if (commuDataPath.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("文件打开失败"));
        emit operation_release_lock();
    }
    else
    {
        if (!commuDataFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QMessageBox::critical(this, tr("错误"), tr("文件打开失败"));
            emit operation_release_lock();
        }
        else
        {
            emit operation_release_lock();

            QTextStream stream(&commuDataFile);

            stream << tr("时间") << "," << tr("内容") << "," << tr("帧数据") << "\n";

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
            commuDataFile.close();
        }
    }
}

void CommunicationLogs::on_quickSearch_currentIndexChanged(int index)
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

void CommunicationLogs::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        model->item(0, 0)->setText(tr("时间"));
        model->item(0, 1)->setText(tr("类型"));
        model->item(0, 2)->setText(tr("帧数据"));

        ui->retranslateUi(this);
    }
}

void CommunicationLogs::on_last_page_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toInt();

        if (current_page > 1)
        {
            current_page--;

            ui->jump_to_page->setText(QString::number(current_page));

            model->removeRows(1, 18);

            for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
            {
                QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
                model->setItem((i-(current_page-1)*records_per_page)+1, 1, new QStandardItem(search_result[1][i]));
                //            qDebug() << tmp_result[2][i];
                QString tmp_array;
                for (int j = 0; j < search_result[2][i].size()-1; j += 2)
                {
                    tmp_array.append(search_result[2][i][j]);
                    tmp_array.append(search_result[2][i][j+1]);
                    tmp_array.append(" ");
                }
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(tmp_array));
            }
        }
    }
}

void CommunicationLogs::on_move_to_last_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->total_num_pages->text().toUInt();
        ui->jump_to_page->setText(QString::number(current_page));

        model->removeRows(1, 18);

        for (quint64 i = (current_page-1)*records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
        {
            QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
            model->setItem((i-(current_page-1)*records_per_page)+1, 1, new QStandardItem(search_result[1][i]));

            QString tmp_array;
            for (int j = 0; j < search_result[2][i].size()-1; j += 2)
            {
                tmp_array.append(search_result[2][i][j]);
                tmp_array.append(search_result[2][i][j+1]);
                tmp_array.append(" ");
            }
            model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(tmp_array));
        }
    }
}

void CommunicationLogs::on_move_to_first_clicked()
{
    if (!search_result.isEmpty())
    {
        ui->jump_to_page->setText(QString::number(1));

        model->removeRows(1, 18);

        for (quint64 i = 0; i < ((1==total_pages) ? (records_not_full):(records_per_page)); i++)
        {
            QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
            model->setItem(i+1, 0, new QStandardItem(first_column));
            model->setItem(i+1, 1, new QStandardItem(search_result[1][i]));
            //            qDebug() << tmp_result[2][i];
            QString tmp_array;
            for (int j = 0; j < search_result[2][i].size()-1; j += 2)
            {
                tmp_array.append(search_result[2][i][j]);
                tmp_array.append(search_result[2][i][j+1]);
                tmp_array.append(" ");
            }
            model->setItem(i+1, 2, new QStandardItem(tmp_array));
        }
    }
}

void CommunicationLogs::on_next_page_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toUInt();

        if (current_page != ui->total_num_pages->text().toUInt())
        {
            current_page++;

            ui->jump_to_page->setText(QString::number(current_page));

            model->removeRows(1, 18);

            for (quint64 i = (current_page-1) * records_per_page; i < ((current_page==total_pages&&records_not_full) ? (((current_page-1)*records_per_page)+records_not_full):(current_page*records_per_page)); i++)
            {
                QString first_column = QDateTime::fromMSecsSinceEpoch(search_result[0][i].toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                model->setItem((i-(current_page-1)*records_per_page)+1, 0, new QStandardItem(first_column));
                model->setItem((i-(current_page-1)*records_per_page)+1, 1, new QStandardItem(search_result[1][i]));
                //            qDebug() << tmp_result[2][i];
                QString tmp_array;
                for (int j = 0; j < search_result[2][i].size()-1; j += 2)
                {
                    tmp_array.append(search_result[2][i][j]);
                    tmp_array.append(search_result[2][i][j+1]);
                    tmp_array.append(" ");
                }
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(tmp_array));
            }
        }
    }
}

void CommunicationLogs::on_jump_to_page_btn_clicked()
{
    if (!search_result.isEmpty())
    {
        quint64 current_page = ui->jump_to_page->text().toUInt();

        model->removeRows(1, 18);

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

                QString tmp_array;
                for (int j = 0; j < search_result[2][i].size()-1; j += 2)
                {
                    tmp_array.append(search_result[2][i][j]);
                    tmp_array.append(search_result[2][i][j+1]);
                    tmp_array.append(" ");
                }
                model->setItem((i-(current_page-1)*records_per_page)+1, 2, new QStandardItem(tmp_array));
            }
        }
    }
}

//void CommunicationLogs::on_jump_to_page_editingFinished()
//{
//    //    qDebug() << ui->jump_to_page->text().toInt() << ui->total_num_pages->text().toInt();

//}


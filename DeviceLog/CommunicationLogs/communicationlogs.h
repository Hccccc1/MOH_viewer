#ifndef COMMUNICATIONLOGS_H
#define COMMUNICATIONLOGS_H

#include <QDialog>
#include <QResizeEvent>
#include <QDebug>
#include <QStandardItem>

#include "DeviceLog/LogDatabase/logdatabase.h"

namespace Ui {
class CommunicationLogs;
}

class CommunicationLogs : public QDialog
{
    Q_OBJECT

public:
    explicit CommunicationLogs(QWidget *parent = nullptr, int slave_addr = 0x01);
    ~CommunicationLogs();

    void change_slave_addr(int slave_addr);

public slots:
    void addCommunicationRecord(QString first_column, QString second_column);

private:
    Ui::CommunicationLogs *ui;

    int m_slave_addr = 0x01;

    QString db_name;
    QString tmp_db_name = "communication_log_%1.db";
    QString table_name = "communication_table";

    quint64 total_pages;
    quint16 records_not_full;
    QVector<QVector<QString>> search_result;

    QStandardItemModel *model = new QStandardItemModel(this);

    int column_time;
    int column_content;
    int column_level;

    void reset_model();
//    LogDatabase commu_database = LogDatabase(db_name, table_name, CommunicaitionLog);

private slots:
    void on_getDataBtn_clicked();
    void on_exportDataBtn_clicked();
//    void on_pushButton_clicked();

    void on_quickSearch_currentIndexChanged(int index);

    void on_last_page_clicked();
    void on_move_to_last_clicked();
    void on_move_to_first_clicked();
    void on_next_page_clicked();
    void on_jump_to_page_btn_clicked();

protected:
    virtual void changeEvent(QEvent *);
    void resizeEvent(QResizeEvent *);

Q_SIGNALS:
    void operation_needs_lock();
    void operation_release_lock();

};

#endif // COMMUNICATIONLOGS_H

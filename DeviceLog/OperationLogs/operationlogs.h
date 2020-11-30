#ifndef OPERATIONLOGS_H
#define OPERATIONLOGS_H

#include <QDialog>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QMessageBox>
#include "DeviceLog/LogDatabase/logdatabase.h"
#include "AllBitsAndRegs.h"

namespace Ui {
class OperationLogs;
}

class OperationLogs : public QDialog
{
    Q_OBJECT

public:
    explicit OperationLogs(QWidget *parent = nullptr, int slave_addr = 0x01);
    ~OperationLogs();

    void change_slave_addr(int slave_addr);

public slots:
    void addOperationRecord(QString first_column, Accounts account);

private:
    Ui::OperationLogs *ui;

    int m_slave_addr = 0x01;

    QString db_name;
    QString tmp_db_name = "operation_log_%1.db";
    QString table_name = "operation_table";

    quint64 total_pages;
    quint16 records_not_full;
    QVector<QVector<QString>> search_result;

    int column_time;
    int column_content;
    int column_level;

    QStandardItemModel *model = new QStandardItemModel(this);

    void reset_model();
//    LogDatabase operation_database = LogDatabase(db_name, table_name, OperationLog);

private slots:
    void on_getDataBtn_clicked();
    void on_dataExportBtn_clicked();
    void on_quickSearch_currentIndexChanged(int index);

    void on_last_page_clicked();
    void on_move_to_last_clicked();
    void on_move_to_first_clicked();
    void on_next_page_clicked();
    void on_jump_to_page_btn_clicked();

protected:
    virtual void changeEvent(QEvent *);
    void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
    void operation_needs_lock();
    void operation_release_lock();
};

#endif // OPERATIONLOGS_H

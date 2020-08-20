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
    explicit OperationLogs(QWidget *parent = nullptr);
    ~OperationLogs();

public slots:
    void addOperationRecord(QString first_column, Accounts account);

private:
    Ui::OperationLogs *ui;

    QString db_name = "operation_log.db";
    QString table_name = "operation_table";

    quint64 total_pages;
    quint16 records_not_full;
    QVector<QVector<QString>> search_result;

    QStandardItemModel *model = new QStandardItemModel(this);
    LogDatabase operation_database = LogDatabase(db_name, table_name, OperationLog);

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

#ifndef WARNINGLOGS_H
#define WARNINGLOGS_H

#include <QDebug>
#include <QDialog>
#include <QStandardItemModel>
#include <QResizeEvent>
#include <QMessageBox>

#include "DeviceLog/LogDatabase/logdatabase.h"

namespace Ui {
class WarningLogs;
}

class WarningLogs : public QDialog
{
    Q_OBJECT

public:
    explicit WarningLogs(QWidget *parent = nullptr);
    ~WarningLogs();

public slots:
    void addWarningRecord(QString first_column, QString second_column);

private:
    Ui::WarningLogs *ui;

    QString db_name = "warning_log.db";
    QString table_name = "warninglog_table";

    quint64 total_pages;
    quint16 records_not_full;
    QVector<QVector<QString>> search_result;

    QStandardItemModel *model = new QStandardItemModel(this);
    LogDatabase warning_database = LogDatabase(db_name, table_name, WarningLog);

private slots:
    void on_getDataBtn_clicked();
    void on_quickSearch_currentIndexChanged(int index);

    void on_last_page_clicked();
    void on_move_to_last_clicked();
    void on_move_to_first_clicked();
    void on_next_page_clicked();
    void on_jump_to_page_btn_clicked();

    void on_exportDataBtn_clicked();

protected:
    virtual void changeEvent(QEvent *);
    void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
    void operation_needs_lock();
    void operation_release_lock();
};

#endif // WARNINGLOGS_H

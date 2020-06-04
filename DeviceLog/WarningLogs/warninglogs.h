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

    QStandardItemModel *model = new QStandardItemModel(this);
    LogDatabase warning_database = LogDatabase(db_name, table_name, WarningLog);

private slots:
    void on_getDataBtn_clicked();
    void on_quickSearch_currentIndexChanged(int index);

protected:
    virtual void changeEvent(QEvent *);
    void resizeEvent(QResizeEvent *event);
};

#endif // WARNINGLOGS_H

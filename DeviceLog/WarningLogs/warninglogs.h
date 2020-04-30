#ifndef WARNINGLOGS_H
#define WARNINGLOGS_H

#include <QDebug>
#include <QDialog>
#include <QStandardItemModel>
#include <QResizeEvent>
#include "LoginInterface/userdatabase.h"

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
    void addWarningRecord(QString content, Accounts user);

private:
    Ui::WarningLogs *ui;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // WARNINGLOGS_H

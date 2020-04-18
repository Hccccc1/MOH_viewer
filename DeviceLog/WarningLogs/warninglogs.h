#ifndef WARNINGLOGS_H
#define WARNINGLOGS_H

#include <QDebug>
#include <QDialog>
#include <QStandardItemModel>
#include <QResizeEvent>

namespace Ui {
class WarningLogs;
}

class WarningLogs : public QDialog
{
    Q_OBJECT

public:
    explicit WarningLogs(QWidget *parent = nullptr);
    ~WarningLogs();

private:
    Ui::WarningLogs *ui;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // WARNINGLOGS_H

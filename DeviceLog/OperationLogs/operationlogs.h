#ifndef OPERATIONLOGS_H
#define OPERATIONLOGS_H

#include <QDialog>

namespace Ui {
class OperationLogs;
}

class OperationLogs : public QDialog
{
    Q_OBJECT

public:
    explicit OperationLogs(QWidget *parent = nullptr);
    ~OperationLogs();

private:
    Ui::OperationLogs *ui;
};

#endif // OPERATIONLOGS_H

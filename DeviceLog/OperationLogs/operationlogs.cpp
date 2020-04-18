#include "operationlogs.h"
#include "ui_operationlogs.h"

OperationLogs::OperationLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OperationLogs)
{
    ui->setupUi(this);
}

OperationLogs::~OperationLogs()
{
    delete ui;
}

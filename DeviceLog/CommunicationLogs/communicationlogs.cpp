#include "communicationlogs.h"
#include "ui_communicationlogs.h"

CommunicationLogs::CommunicationLogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicationLogs)
{
    ui->setupUi(this);
}

CommunicationLogs::~CommunicationLogs()
{
    delete ui;
}

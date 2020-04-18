#ifndef COMMUNICATIONLOGS_H
#define COMMUNICATIONLOGS_H

#include <QDialog>

namespace Ui {
class CommunicationLogs;
}

class CommunicationLogs : public QDialog
{
    Q_OBJECT

public:
    explicit CommunicationLogs(QWidget *parent = nullptr);
    ~CommunicationLogs();

private:
    Ui::CommunicationLogs *ui;
};

#endif // COMMUNICATIONLOGS_H

#ifndef DEVICELOG_H
#define DEVICELOG_H

#include <QWidget>

namespace Ui {
class DeviceLog;
}

class DeviceLog : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceLog(QWidget *parent = nullptr, uint8_t model = 0);
    ~DeviceLog();

private:
    Ui::DeviceLog *ui;

    uint8_t current_model;
};

#endif // DEVICELOG_H

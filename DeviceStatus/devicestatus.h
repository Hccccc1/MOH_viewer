#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include <QWidget>

namespace Ui {
class DeviceStatus;
}

class DeviceStatus : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceStatus(QWidget *parent = nullptr, uint8_t model = 0);
    ~DeviceStatus();

private:
    Ui::DeviceStatus *ui;

    uint8_t current_model;
};

#endif // DEVICESTATUS_H

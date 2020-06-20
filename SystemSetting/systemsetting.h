#ifndef SYSTEMSETTING_H
#define SYSTEMSETTING_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTranslator>

#include "Modbus/modbusserial.h"

namespace Ui {
class SystemSetting;
}

class SystemSetting : public QWidget
{
    Q_OBJECT

public:
    explicit SystemSetting(QWidget *parent = nullptr, uint8_t model = 0, ModbusSerial *serial = nullptr, QTranslator *trans = nullptr);
    ~SystemSetting();

public slots:
    void refresh_port();
    void on_confirm_btn_clicked();
    void on_errorHappened(QModbusDevice::Error);

private:
    Ui::SystemSetting *ui;

    uint8_t current_model;
    QTranslator *current_trans = nullptr;

    ModbusSerial *current_serial = nullptr;

    void open_port();
    void close_port();

    void save_language_settings_to_file();

private slots:
    void on_disconnectBtn_clicked();
    void on_languageChangeBtn_clicked();

protected:
    virtual void changeEvent(QEvent *);

Q_SIGNALS:
    void serial_connected();
    void serial_disconnected();
};

#endif // SYSTEMSETTING_H

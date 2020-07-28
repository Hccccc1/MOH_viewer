#ifndef SYSTEMSETTING_H
#define SYSTEMSETTING_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTranslator>

#include "Modbus/modbusserial.h"

#include "3rdparty/YModem/YmodemFileTransmit.h"

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

//public slots:
//    void do_upgrade(bool);

private:
    Ui::SystemSetting *ui;

    uint8_t current_model;
    QTranslator *current_trans = nullptr;

    ModbusSerial *current_serial = nullptr;

    QSerialPort *dete_serial = nullptr;

    int timer_id;
    bool is_in_boot = false;
    QModbusDevice::Error pri_error;

    void open_port();
    void close_port();

    void save_language_settings_to_file();

private slots:
    void on_disconnectBtn_clicked();
    void on_languageChangeBtn_clicked();

    void on_upgradeNow_clicked();

protected:
    virtual void timerEvent(QTimerEvent *);
    virtual void changeEvent(QEvent *);

Q_SIGNALS:
    void serial_connected();
    void serial_disconnected();

    //升级时停止自动刷新，完成后重新开启
    void start_timer();
    void stop_timer();

    void upgrade_now();
    void switch_to_upgrade();
};

#endif // SYSTEMSETTING_H

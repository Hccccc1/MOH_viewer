#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include <QTranslator>

#include "MOH_Viewer/moh_viewer.h"
#include "ModbusSerial/modbusserial.h"
#include "ModbusSerial/modbusserialprivate.h"

class Manager : public QWidget
{
    Q_OBJECT
public:
    explicit Manager(QWidget *parent = nullptr, Accounts account = Customer, uint8_t model = 0, QTranslator* trans = nullptr);
    ~Manager();

private:
//    ModbusSerial::Settings serial_settings;
    quint8 moh_counter = 0;

    uint8_t current_model = 0;
    Accounts current_account;
    QTranslator *current_trans = nullptr;

    ModbusSerialPrivate *m_serialPrivate = new ModbusSerialPrivate(this);

//    QVector<ModbusSerial* > m_serial;
    QVector<MOH_Viewer*> moh_viewers;

private slots:
//    void connectToSerial();
    void creat_new_moh(int);

protected:
//    void keyPressEvent(QKeyEvent *event);
};

#endif // MANAGER_H

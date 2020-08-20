#ifndef WARNINGSOUND_H
#define WARNINGSOUND_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QSound>

enum WarningType
{
    LowPressure_PT03 = 0x0001,
    HighPressure_PT03 = 0x0002,
    HighPressure_PT05 = 0x0004,
    HighTemperature_TT17 = 0x0008,
    HighTemperature_TT31 = 0x0010,
    ConductivityAbnormal_CS01 = 0x0020,
    LowVoltage_BAT01 = 0x0040,
    LowLevel_LT1 = 0x0080,
    LowLevel_LT2 = 0x0100,
    LowLoading = 0x0200,

    AllWarningMask = 0x03ff,
};

class WarningSound : public QThread
{
    Q_OBJECT
public:
    WarningSound(quint16 warning_msg = 0);

    void warning_msg_detected(WarningType type);

public slots:
    void clear_warning_msg();

private:
    bool warningState = false;
    qint16 current_warning_msg;
    QSound *warning_sound = new QSound(":/Smoke_Alarm.wav");;

    QMutex *ope_mutex = new QMutex(QMutex::NonRecursive);

private slots:
    void play_alarm_sound();

protected:
    void run() override;

Q_SIGNALS:
    void alarm_sound();
    void change_text();
    void change_color(bool);
    void warningRecord(QString, QString);
};

#endif // WARNINGSOUND_H

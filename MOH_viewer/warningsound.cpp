#include "warningsound.h"

WarningSound::WarningSound(quint16 warning_msg) :
    current_warning_msg(warning_msg)
{
    //    warning_sound = new QSound(":/Smoke_Alarm.wav");

    //    warning_sound->play();

    connect(this, &WarningSound::alarm_sound, this, &WarningSound::play_alarm_sound);
}

void WarningSound::warning_msg_detected(WarningType type)
{
    current_warning_msg |= (type&AllWarningMask);
}

void WarningSound::run()
{
    static uint8_t change_text_counter = 0;

    while (1)
    {
        if (current_warning_msg&AllWarningMask)
        {
            if (warning_sound && warning_sound->isFinished())
            {
                qDebug() << __FILE__ << __LINE__;

                emit alarm_sound();
            }

            if (change_text_counter % 2 != 0)
            {
                emit change_text();
            }

            change_text_counter++;

            emit change_color(warningState);
            warningState = !warningState;

        }
        msleep(500);
    }
}

void WarningSound::clear_warning_msg()
{
    qDebug() << __FILE__ << __LINE__;

    if (current_warning_msg&LowPressure_PT03)
        emit warningRecord(tr("PT-04压力低报警消除"), "1");
    if (current_warning_msg&HighPressure_PT03)
        emit warningRecord(tr("PT-04压力高报警消除"), "1");
    if (current_warning_msg&HighPressure_PT05)
        emit warningRecord(tr("PT-05压力高报警消除"), "1");
    if (current_warning_msg&HighTemperature_TT17)
        emit warningRecord(tr("TT-17温度高报警消除"), "1");
    if (current_warning_msg&HighTemperature_TT31)
        emit warningRecord(tr("TT-18温度高报警消除"), "1");
    if (current_warning_msg&ConductivityAbnormal_CS01)
        emit warningRecord(tr("电导率异常报警消除"), "1");
    if (current_warning_msg&LowVoltage_BAT01)
        emit warningRecord(tr("BAT-01电池电压低报警消除"), "1");
    if (current_warning_msg&LowLevel_LT1)
        emit warningRecord(tr("LT1低液位报警消除"), "1");
    if (current_warning_msg&LowLevel_LT2)
        emit warningRecord(tr("LT2低液位报警消除"), "1");
    if (current_warning_msg&LowLoading)
        emit warningRecord(tr("低负载报警消除"), "1");

    current_warning_msg &= 0x0;

    warningState = false;
    emit change_color(warningState);

    if (warning_sound && !warning_sound->isFinished())
    {
        warning_sound->stop();
    }
}

void WarningSound::play_alarm_sound()
{
    if (warning_sound)
    {
        warning_sound->play();
    }
}

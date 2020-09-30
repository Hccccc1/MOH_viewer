#include "warningsound.h"
#include <QDateTime>

WarningSound::WarningSound(QObject* parent, quint16 warning_msg) :
    QThread(parent),
    current_warning_msg(warning_msg)
{
    //    warning_sound = new QSound(":/Smoke_Alarm.wav");

    //    warning_sound->play();

    connect(this, &WarningSound::alarm_sound, this, &WarningSound::play_alarm_sound);
}

void WarningSound::warning_msg_detected(WarningType type)
{

    if (!wmv_enabled && !(type|current_warning_msg))
    {
        wmv_enabled = true;
    }

    current_warning_msg |= (type&AllWarningMask);
}

void WarningSound::warning_msg_dissmissed(WarningType type)
{
    current_warning_msg &= ~(type&AllWarningMask);
}

void WarningSound::run()
{
    static uint8_t change_text_counter = 0;

    while (1)
    {
        if (current_warning_msg&AllWarningMask)
        {
            if (wmv_enabled && warning_sound && warning_sound->isFinished())
            {
//                qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime();

                emit alarm_sound();
            }

//            qDebug() << __func__ << __LINE__ << this;

            if (change_text_counter % 2 != 0)
            {
                emit change_text();
                msleep(100);
            }

            change_text_counter++;

            emit change_color(warningState);
            warningState = !warningState;

        }
        else
        {
            warningState = false;
            emit change_color(warningState);
        }
        msleep(400);
    }
}

void WarningSound::clear_warning_msg()
{
//    qDebug() << __FILE__ << __LINE__;

//    if (current_warning_msg&LowPressure_PT03)
//        emit warningRecord(tr("PT-04压力低报警消除"), "1");
//    if (current_warning_msg&HighPressure_PT03)
//        emit warningRecord(tr("PT-04压力高报警消除"), "1");
//    if (current_warning_msg&HighPressure_PT05)
//        emit warningRecord(tr("PT-05压力高报警消除"), "1");
//    if (current_warning_msg&HighTemperature_TT17)
//        emit warningRecord(tr("TT-17温度高报警消除"), "1");
//    if (current_warning_msg&HighTemperature_TT31)
//        emit warningRecord(tr("TT-18温度高报警消除"), "1");
//    if (current_warning_msg&ConductivityAbnormal_CS01)
//        emit warningRecord(tr("电导率异常报警消除"), "1");
//    if (current_warning_msg&LowVoltage_BAT01)
//        emit warningRecord(tr("BAT-01电池电压低报警消除"), "1");
//    if (current_warning_msg&LowLevel_LT1)
//        emit warningRecord(tr("LT1低液位报警消除"), "1");
//    if (current_warning_msg&LowLevel_LT2)
//        emit warningRecord(tr("LT2低液位报警消除"), "1");
//    if (current_warning_msg&LowLoading)
//        emit warningRecord(tr("低负载报警消除"), "1");

//    current_warning_msg &= 0x0;

//    warningState = false;
//    emit change_color(warningState);

    wmv_enabled = false;

//    timer_id = startTimer(30000);

//    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime();

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

//void WarningSound::timerEvent(QTimerEvent *e)
//{
//    Q_UNUSED(e);

//    wmv_enabled = true;

//    killTimer(timer_id);
//}

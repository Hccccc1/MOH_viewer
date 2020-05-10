QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql serialport serialbus printsupport multimedia

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    3rdparty/QCustomPlot/qcustomplot.cpp \
    3rdparty/QRoundProgressBar/QRoundProgressBar.cpp \
    DeviceLog/CommunicationLogs/communicationlogs.cpp \
    DeviceLog/LogDatabase/logdatabase.cpp \
    DeviceLog/OperationLogs/operationlogs.cpp \
    DeviceLog/WarningLogs/warninglogs.cpp \
    DeviceStatus/DataOverview/dataoverview.cpp \
    DeviceStatus/HistoryCurve/hiscurve.cpp \
    DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.cpp \
    DeviceStatus/RealTimeCurve/rtcurve.cpp \
    DeviceStatus/RealTimeValues/rtvalues.cpp \
    DeviceStatus/devicestatus.cpp \
    LoginInterface/logininterface.cpp \
    LoginInterface/userdatabase.cpp \
    MOH_viewer/moh_viewer.cpp \
    Modbus/modbusserial.cpp \
    main.cpp \
    ControlPanel/controlpanel.cpp \
    DeviceLog/devicelog.cpp \
    ParameterConfiguration/parameterconfiguration.cpp \
    SystemSetting/systemsetting.cpp \
    MOH_viewer/modelselector.cpp

HEADERS += \
    3rdparty/QCustomPlot/qcustomplot.h \
    3rdparty/QRoundProgressBar/QRoundProgressBar.h \
    AllBitsAndRegs.h \
    DeviceLog/CommunicationLogs/communicationlogs.h \
    DeviceLog/LogDatabase/logdatabase.h \
    DeviceLog/OperationLogs/operationlogs.h \
    DeviceLog/WarningLogs/warninglogs.h \
    DeviceStatus/DataOverview/dataoverview.h \
    DeviceStatus/DevStatus_regs.h \
    DeviceStatus/HistoryCurve/hiscurve.h \
    DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h \
    DeviceStatus/RealTimeCurve/rtcurve.h \
    DeviceStatus/RealTimeValues/rtvalues.h \
    LoginInterface/logininterface.h \
    LoginInterface/userdatabase.h \
    DeviceStatus/devicestatus.h \
    ControlPanel/controlpanel.h \
    DeviceLog/devicelog.h \
    MOH_viewer/moh_viewer.h \
    Modbus/modbusserial.h \
    ParameterConfiguration/parameterconfiguration.h \
    SystemSetting/systemsetting.h \
    MOH_viewer/modelselector.h

FORMS += \
    DeviceLog/CommunicationLogs/communicationlogs.ui \
    DeviceLog/OperationLogs/operationlogs.ui \
    DeviceLog/WarningLogs/warninglogs.ui \
    DeviceStatus/DataOverview/dataoverview.ui \
    DeviceStatus/HistoryCurve/hiscurve.ui \
    DeviceStatus/RealTimeCurve/rtcurve.ui \
    DeviceStatus/RealTimeValues/rtvalues.ui \
    LoginInterface/logininterface.ui \
    MOH_viewer/moh_viewer.ui \
    DeviceStatus/devicestatus.ui \
    ControlPanel/controlpanel.ui \
    DeviceLog/devicelog.ui \
    Modbus/modbusserial.ui \
    ParameterConfiguration/parameterconfiguration.ui \
    SystemSetting/systemsetting.ui \
    MOH_viewer/modelselector.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images/images.qrc \
    Sound/Sound.qrc

DISTFILES +=

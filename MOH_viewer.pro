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
    3rdparty/YModem/Ymodem.cpp \
    3rdparty/YModem/YmodemFileReceive.cpp \
    3rdparty/YModem/YmodemFileTransmit.cpp \
    ControlPanel/controlpanel.cpp \
    DeviceLog/CommunicationLogs/communicationlogs.cpp \
    DeviceLog/LogDatabase/logdatabase.cpp \
    DeviceLog/OperationLogs/operationlogs.cpp \
    DeviceLog/WarningLogs/warninglogs.cpp \
    DeviceLog/devicelog.cpp \
    DeviceStatus/DataOverview/dataoverview.cpp \
    DeviceStatus/HistoryCurve/customer_historycurve.cpp \
    DeviceStatus/HistoryCurve/hiscurve.cpp \
    DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.cpp \
    DeviceStatus/RealTimeCurve/customerrtcurve.cpp \
    DeviceStatus/RealTimeCurve/rtcurve.cpp \
    DeviceStatus/RealTimeValues/rtvalues.cpp \
    DeviceStatus/devicestatus.cpp \
    LoginInterface/logininterface.cpp \
    LoginInterface/modelselector.cpp \
    LoginInterface/userdatabase.cpp \
    MOH_Viewer/moh_viewer.cpp \
    MOH_Viewer/warningsound.cpp \
    ModbusSerial/modbusserial.cpp \
    ModbusSerial/modbusserialprivate.cpp \
    ParameterConfiguration/parameterconfiguration.cpp \
    SystemSetting/serialupgrade.cpp \
    SystemSetting/systemsetting.cpp \
    main.cpp \
    manager.cpp

HEADERS += \
    3rdparty/QCustomPlot/qcustomplot.h \
    3rdparty/QRoundProgressBar/QRoundProgressBar.h \
    3rdparty/YModem/Ymodem.h \
    3rdparty/YModem/YmodemFileReceive.h \
    3rdparty/YModem/YmodemFileTransmit.h \
    AllBitsAndRegs.h \
    ControlPanel/controlpanel.h \
    DeviceLog/CommunicationLogs/communicationlogs.h \
    DeviceLog/LogDatabase/logdatabase.h \
    DeviceLog/OperationLogs/operationlogs.h \
    DeviceLog/WarningLogs/warninglogs.h \
    DeviceLog/devicelog.h \
    DeviceStatus/DataOverview/dataoverview.h \
    DeviceStatus/DevStatus_regs.h \
    DeviceStatus/HistoryCurve/customer_historycurve.h \
    DeviceStatus/HistoryCurve/hiscurve.h \
    DeviceStatus/HistoryValuesDatabase/historyvaluesdatabase.h \
    DeviceStatus/RealTimeCurve/customerrtcurve.h \
    DeviceStatus/RealTimeCurve/rtcurve.h \
    DeviceStatus/RealTimeValues/rtvalues.h \
    DeviceStatus/devicestatus.h \
    LoginInterface/logininterface.h \
    LoginInterface/modelselector.h \
    LoginInterface/userdatabase.h \
    MOH_Viewer/moh_viewer.h \
    MOH_Viewer/warningsound.h \
    ModbusSerial/modbusserial.h \
    ModbusSerial/modbusserialprivate.h \
    ParameterConfiguration/parameterconfiguration.h \
    SystemSetting/serialupgrade.h \
    SystemSetting/systemsetting.h \
    manager.h

FORMS += \
    ControlPanel/controlpanel.ui \
    DeviceLog/CommunicationLogs/communicationlogs.ui \
    DeviceLog/OperationLogs/operationlogs.ui \
    DeviceLog/WarningLogs/warninglogs.ui \
    DeviceLog/devicelog.ui \
    DeviceStatus/DataOverview/dataoverview.ui \
    DeviceStatus/HistoryCurve/customer_historycurve.ui \
    DeviceStatus/HistoryCurve/hiscurve.ui \
    DeviceStatus/RealTimeCurve/customerrtcurve.ui \
    DeviceStatus/RealTimeCurve/rtcurve.ui \
    DeviceStatus/RealTimeValues/rtvalues.ui \
    DeviceStatus/devicestatus.ui \
    LoginInterface/logininterface.ui \
    LoginInterface/modelselector.ui \
    MOH_Viewer/moh_viewer.ui \
    ParameterConfiguration/parameterconfiguration.ui \
    SystemSetting/serialupgrade.ui \
    SystemSetting/systemsetting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images/images.qrc \
    Sound/Sound.qrc \
    Translators/Translator.qrc

DISTFILES += \
    Translators/chinese.ts \
    Translators/english.ts

RC_FILE += exe_icon.rc

TRANSLATIONS += english.ts chinese.ts

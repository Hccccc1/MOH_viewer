#ifndef ALLBITSANDREGS_H
#define ALLBITSANDREGS_H

enum CoilsRegs
{
    //系统控制命令字
    CoilsRegs_SysCtrlSelfCheck = 0x0000,
    CoilsRegs_SysCtrlStart,
    CoilsRegs_SysCtrlRun,
    CoilsRegs_SysCtrlShutDown,
    CoilsRegs_SysCtrlEmergencyShutDown,
    CoilsRegs_SysCtrlReset,

    //IO输出位段1
    CoilsRegs_SV_01 = 0x0010,
    CoilsRegs_SV_02,
    CoilsRegs_SV_03,
    CoilsRegs_SV_04,
    CoilsRegs_SV_05,
    CoilsRegs_SV_06,
    CoilsRegs_SV_07,
    CoilsRegs_SV_08,
    CoilsRegs_SV_09,
    CoilsRegs_SV_10,
    CoilsRegs_SV_11,
    CoilsRegs_SV_12,
    CoilsRegs_SV_13,
    CoilsRegs_SV_14,

    //IO输出位段2
    CoilsRegs_BL_01 = 0x0020,
    CoilsRegs_BL_02,
    CoilsRegs_BL_03,
    CoilsRegs_BL_04,
    CoilsRegs_PMP_01,
    CoilsRegs_PMP_02,
    CoilsRegs_PMP_03,
    CoilsRegs_PMP_04,
    CoilsRegs_PMP_05,
    CoilsRegs_RAD_01,
    CoilsRegs_KM_01,

    //IO输出控制使能位段1
    CoilsRegs_SV_01_CtrlEnable = 0x0030,
    CoilsRegs_SV_02_CtrlEnable,
    CoilsRegs_SV_03_CtrlEnable,
    CoilsRegs_SV_04_CtrlEnable,
    CoilsRegs_SV_05_CtrlEnable,
    CoilsRegs_SV_06_CtrlEnable,
    CoilsRegs_SV_07_CtrlEnable,
    CoilsRegs_SV_08_CtrlEnable,
    CoilsRegs_SV_09_CtrlEnable,
    CoilsRegs_SV_10_CtrlEnable,
    CoilsRegs_SV_11_CtrlEnable,
    CoilsRegs_SV_12_CtrlEnable,
    CoilsRegs_SV_13_CtrlEnable,
    CoilsRegs_SV_14_CtrlEnable,

    //IO输出控制使能位段2
    CoilsRegs_BL_01_CtrlEnable = 0x0040,
    CoilsRegs_BL_02_CtrlEnable,
    CoilsRegs_BL_03_CtrlEnable,
    CoilsRegs_BL_04_CtrlEnable,
    CoilsRegs_PMP_01_CtrlEnable,
    CoilsRegs_PMP_02_CtrlEnable,
    CoilsRegs_PMP_03_CtrlEnable,
    CoilsRegs_PMP_04_CtrlEnable,
    CoilsRegs_PMP_05_CtrlEnable,
    CoilsRegs_RAD_01_CtrlEnable,
    CoilsRegs_KM_01_CtrlEnable,

    //模式控制位
    CoilsRegs_AutoCtrl = 0x0050,
    CoilsRegs_AutoCharge,
    CoilsRegs_BL_01_AutoCtrl,
    CoilsRegs_BL_02_AutoCtrl,
    CoilsRegs_BL_03_AutoCtrl,
    CoilsRegs_BL_04_AutoCtrl,
    CoilsRegs_PMP_01_AutoCtrl,
    CoilsRegs_PMP_02_AutoCtrl,
    CoilsRegs_PMP_03_AutoCtrl,
    CoilsRegs_PMP_04_AutoCtrl,
    CoilsRegs_PMP_05_AutoCtrl,
    CoilsRegs_RAD_01_AutoCtrl,

    //报警开关控制位
    CoilsRegs_LT_01_AlarmCtrl = 0x0060,
    CoilsRegs_LT_02_AlarmCtrl,
};

enum DiscreteInputs
{
    //IO输入
    DiscreteInputs_IOInput00 = 0x1000,
    DiscreteInputs_IOInput01,
    DiscreteInputs_IOInput02,
    DiscreteInputs_IOInput03,
    DiscreteInputs_IOInput04,

    //IO输出状态反馈位段1
    DiscreteInputs_OutputFeedback_SV01 = 0x1010,
    DiscreteInputs_OutputFeedback_SV02,
    DiscreteInputs_OutputFeedback_SV03,
    DiscreteInputs_OutputFeedback_SV04,
    DiscreteInputs_OutputFeedback_SV05,
    DiscreteInputs_OutputFeedback_SV06,
    DiscreteInputs_OutputFeedback_SV07,
    DiscreteInputs_OutputFeedback_SV08,
    DiscreteInputs_OutputFeedback_SV09,
    DiscreteInputs_OutputFeedback_SV10,
    DiscreteInputs_OutputFeedback_SV11,
    DiscreteInputs_OutputFeedback_SV12,
    DiscreteInputs_OutputFeedback_SV13,
    DiscreteInputs_OutputFeedback_SV14,

    //IO输出状态反馈位段2
    DiscreteInputs_OutputFeedback_BL01 = 0x1020,
    DiscreteInputs_OutputFeedback_BL02,
    DiscreteInputs_OutputFeedback_BL03,
    DiscreteInputs_OutputFeedback_BL04,
    DiscreteInputs_OutputFeedback_PMP01,
    DiscreteInputs_OutputFeedback_PMP02,
    DiscreteInputs_OutputFeedback_PMP03,
    DiscreteInputs_OutputFeedback_PMP04,
    DiscreteInputs_OutputFeedback_PMP05,
    DiscreteInputs_OutputFeedback_RAD01,
    DiscreteInputs_OutputFeedback_KM01,

    //通讯状态码
    DiscreteInputs_Status_Can = 0x1030,
    DiscreteInputs_Status_RS485_1,
    DiscreteInputs_Status_RS485_2,
    DiscreteInputs_Status_RS485_3,
    DiscreteInputs_Status_4G,
    DiscreteInputs_Status_Enternet,

    //传感器设备自检码1
    DiscreteInputs_SelfCheck_TT03 = 0x1040,
    DiscreteInputs_SelfCheck_TT05,
    DiscreteInputs_SelfCheck_TT15,
    DiscreteInputs_SelfCheck_TT16,
    DiscreteInputs_SelfCheck_TT17,
    DiscreteInputs_SelfCheck_TT19,
    DiscreteInputs_SelfCheck_TT23,
    DiscreteInputs_SelfCheck_TT24,
    DiscreteInputs_SelfCheck_TT25,
    DiscreteInputs_SelfCheck_TT27,
    DiscreteInputs_SelfCheck_TT29,
    DiscreteInputs_SelfCheck_TT31,
    DiscreteInputs_SelfCheck_TT33,
    DiscreteInputs_SelfCheck_TT34,
    DiscreteInputs_SelfCheck_TT37,

    //传感器设备自检码2
    DiscreteInputs_SelfCheck_PT01 = 0x1050,
    DiscreteInputs_SelfCheck_PT02,
    DiscreteInputs_SelfCheck_PT03,
    DiscreteInputs_SelfCheck_PT04,
    DiscreteInputs_SelfCheck_PT05,
    DiscreteInputs_SelfCheck_PT06,
    DiscreteInputs_SelfCheck_AFM01,
    DiscreteInputs_SelfCheck_AFM02,
    DiscreteInputs_SelfCheck_AFM03,
    DiscreteInputs_SelfCheck_AFM04,
    DiscreteInputs_SelfCheck_MFM04,
    DiscreteInputs_SelfCheck_CM01,

    //传感器设备自检码3
    DiscreteInputs_SelfCheck_VT01 = 0x1060,
    DiscreteInputs_SelfCheck_VT02,
    DiscreteInputs_SelfCheck_IT01,
    DiscreteInputs_SelfCheck_IT02,
    DiscreteInputs_SelfCheck_LT01,
    DiscreteInputs_SelfCheck_LT02,

    //系统实时报警码1
    DiscreteInputs_LowPressure_PT03 = 0x1070,
    DiscreteInputs_HighPressure_PT03,
    DiscreteInputs_HighPressure_PT05,
    DiscreteInputs_HighTemperature_TT17,
    DiscreteInputs_HighTemperature_TT18,
    DiscreteInputs_ConductivityAbnormal_CS01,
    DiscreteInputs_LowVoltage_BAT01,
    DiscreteInputs_LowLevel_LT1,
    DiscreteInputs_LowLevel_LT2,
    DiscreteInputs_LowLoading,
};

enum InputRegs
{
    InputRegs_TT_01 = 0x1fff,     //温度01
    InputRegs_TT_02,
    InputRegs_TT_03,
    InputRegs_TT_04,
    InputRegs_TT_05,
    InputRegs_TT_06,
    InputRegs_TT_07,
    InputRegs_TT_08,
    InputRegs_TT_09,
    InputRegs_TT_10,
    InputRegs_TT_11,
    InputRegs_TT_12,
    InputRegs_TT_13,
    InputRegs_TT_14,
    InputRegs_TT_15,
    InputRegs_TT_16,
    InputRegs_TT_17,
    InputRegs_TT_18,
    InputRegs_TT_19,
    InputRegs_TT_20,
    InputRegs_TT_21,
    InputRegs_TT_22,
    InputRegs_TT_23,
    InputRegs_TT_24,
    InputRegs_TT_25,
    InputRegs_TT_26,
    InputRegs_TT_27,
    InputRegs_TT_28,
    InputRegs_TT_29,
    InputRegs_TT_30,
    InputRegs_TT_31,
    InputRegs_TT_32,
    InputRegs_TT_33,
    InputRegs_TT_34,
    InputRegs_TT_35,
    InputRegs_TT_36,

    InputRegs_PT_01,     //压力01
    InputRegs_PT_02,
    InputRegs_PT_03,
    InputRegs_PT_04,
    InputRegs_PT_05,
    InputRegs_PT_06,

    InputRegs_AFM_01,    //空气流量1
    InputRegs_AFM_02,
    InputRegs_AFM_03,
    InputRegs_AFM_04,
    InputRegs_MFM_01,    //质量流量1

    InputRegs_BL_01,     //燃烧腔空气进气风机反馈速度
    InputRegs_BL_02,     //BL-02 反馈速度
    InputRegs_BL_03,     //阴极空气泵反馈速度
    InputRegs_BL_04,     //PROX 温度风扇反馈速度
    InputRegs_PMP_01,    //燃料进液泵
    InputRegs_PMP_02,    //过滤回路泵反馈速度
    InputRegs_PMP_03,    //电堆冷却水泵反馈速度
    InputRegs_PMP_04,    //外部换热泵反馈速度
    InputRegs_PMP_05,    //外部换热泵反馈速度
    InputRegs_RAD_01,    //换热器风机反馈速度

    InputRegs_SysStatus, //系统运行状态
    InputRegs_CM_01,     //电导率
    InputRegs_LT_01,     //燃料箱液位1
    InputRegs_LT_02,     //燃料箱液位2
    InputRegs_VT_01,     //燃料电池电压
    InputRegs_IT_01,     //燃料电池电流
    InputRegs_FcPower,   //燃料电池发电功率
    InputRegs_OutVoltage,    //系统输出电压
    InputRegs_OutCurrent,    //系统输出电流
    InputRegs_OutPower,      //系统输出功率
    InputRegs_VT_02,     //电池电压
    InputRegs_IT_02,     //电池电流
    InputRegs_SinglePowerProduced = 0x2045,   //单次发电量
    InputRegs_TotalPowerProduced = 0x2047,    //累计发电量
    InputRegs_TotalBootTimes = 0x2049,        //累计启动次数
    InputRegs_FuelConsumption,       //燃料消耗率
    InputRegs_TotalFuelConsumption,  //燃料累计消耗量
};

enum HoldingRegs
{
    //产品参数
    HoldingRegs_Manufacturer = 0x3000,
    HoldingRegs_ProductName,
    HoldingRegs_ProductType,
    HoldingRegs_ProductSerialNum,
    HoldingRegs_ProductDate,

    //设备参数
    HoldingRegs_FirmwareVersion,
    HoldingRegs_HardwareVersion,
    HoldingRegs_ReformingID = 0x300b,    //4bytes
    HoldingRegs_StackID = 0x300f,    //4 bytes
    HoldingRegs_DevSlaveAddr = 0x3013,
    HoldingRegs_DevIPAddr = 0x3014,  //2 bytes
    HoldingRegs_SerialPara = 0x3016,    //2 bytes

    //时间参数
    HoldingRegs_SysTime = 0x3018, //4bytes 1:年 2:高位月份 低位日 3:小时 4: 秒
    HoldingRegs_SysSingleTime = 0x301c, //2 bytes 1:小时 2:高位分 低位秒
    HoldingRegs_SysTotalTime = 0x301e, //3bytes 1-2:小时 2:高位分 低位秒

    //模式参数
    HoldingRegs_PowerMode = 0x3023,

    //调速设备控速信息
    HoldingRegs_SpeedCtrl_BL01 = 0x3024,
    HoldingRegs_SpeedCtrl_BL02,
    HoldingRegs_SpeedCtrl_BL03,
    HoldingRegs_SpeedCtrl_BL04,
    HoldingRegs_SpeedCtrl_PMP01,
    HoldingRegs_SpeedCtrl_PMP02,
    HoldingRegs_SpeedCtrl_PMP03,
    HoldingRegs_SpeedCtrl_PMP04,
    HoldingRegs_SpeedCtrl_PMP05,
    HoldingRegs_SpeedCtrl_RAD01,

    //PI控制参数
    HoldingRegs_Kp_BL01,
    HoldingRegs_Ti_BL01,
    HoldingRegs_Tsm_BL01,
    HoldingRegs_Kp_BL02,
    HoldingRegs_Ti_BL02,
    HoldingRegs_Tsm_BL02,
    HoldingRegs_Kp_BL03,
    HoldingRegs_Ti_BL03,
    HoldingRegs_Tsm_BL03,
    HoldingRegs_Kp_BL04,
    HoldingRegs_Ti_BL04,
    HoldingRegs_Tsm_BL04,
    HoldingRegs_Kp_PMP01,
    HoldingRegs_Ti_PMP01,
    HoldingRegs_Tsm_PMP01,
    HoldingRegs_Kp_PMP02,
    HoldingRegs_Ti_PMP02,
    HoldingRegs_Tsm_PMP02,
    HoldingRegs_Kp_PMP03,
    HoldingRegs_Ti_PMP03,
    HoldingRegs_Tsm_PMP03,
    HoldingRegs_Kp_PMP04,
    HoldingRegs_Ti_PMP04,
    HoldingRegs_Tsm_PMP04,
    HoldingRegs_Kp_PMP05,
    HoldingRegs_Ti_PMP05,
    HoldingRegs_Tsm_PMP05,
    HoldingRegs_Kp_RAD01,
    HoldingRegs_Ti_RAD01,
    HoldingRegs_Tsm_RAD01,

    //燃料电池参数
    HoldingRegs_FCOutCurrent = 0x304c,
    HoldingRegs_FCOutPower,

    //蓄电池参数
    HoldingRegs_BatChargeStartVoltage,
    HoldingRegs_BatChargeStartDelay,
    HoldingRegs_BatChargeStopVoltage,
    HoldingRegs_BatChargeStopDelay,

    //数据保存间隔
    HoldingRegs_DataStorageCycle,

    //设备报警参数
    HoldingRegs_LowPressure_PT03,
    HoldingRegs_HighPressure_PT03,
    HoldingRegs_HighPressure_PT04,
    HoldingRegs_HighTemperature_TT17,
    HoldingRegs_HighTemperature_TT31,
    HoldingRegs_HighConductivity_CS01,
    HoldingRegs_LowVoltage_BAT01,
    HoldingRegs_LowLevel_LT01,
    HoldingRegs_AutoLiquidLowLimit_LT01,
    HoldingRegs_StopLiquidValue_LT01,
    HoldingRegs_LowLevel_LT02,
};

enum ST_Status
{
    ST_00 = 0x00,
    ST_10 = 0x0a,
    ST_11 = 0x0b,
    ST_20 = 0x14,
    ST_30 = 0x1e,
    ST_40 = 0x28,
    ST_50 = 0x32,
    ST_60 = 0x3c,
    ST_70 = 0x46,
    ST_80 = 0x50,
    ST_90 = 0x5a,
    ST_100 = 0x64,
    ST_110 = 0x6e,
    ST_120 = 0x78,
};

#endif // ALLBITSANDREGS_H

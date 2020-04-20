#ifndef ALLBITSANDREGS_H
#define ALLBITSANDREGS_H

enum CoilsRegs
{
    //系统控制命令字
    Coils_SysCtrlSelfCheck = 0x0000,
    Coils_SysCtrlStart,
    Coils_SysCtrlRun,
    Coils_SysCtrlShutDown,
    Coils_SysCtrlEmergencyShutDown,
    Coils_SysCtrlReset,

    //IO输出位段1
    Coils_SV_01 = 0x0010,
    Coils_SV_02,
    Coils_SV_03,
    Coils_SV_04,
    Coils_SV_05,
    Coils_SV_06,
    Coils_SV_07,
    Coils_SV_08,
    Coils_SV_09,
    Coils_SV_10,
    Coils_SV_11,
    Coils_SV_12,
    Coils_SV_13,
    Coils_SV_14,

    //IO输出位段2
    Coils_BL_01 = 0x0020,
    Coils_BL_02,
    Coils_BL_03,
    Coils_BL_04,
    Coils_PMP_01,
    Coils_PMP_02,
    Coils_PMP_03,
    Coils_PMP_04,
    Coils_PMP_05,
    Coils_RAD_01,
    Coils_KM_01,

    //IO输出控制使能位段1
    Coils_SV_01_CtrlEnable = 0x0030,
    Coils_SV_02_CtrlEnable,
    Coils_SV_03_CtrlEnable,
    Coils_SV_04_CtrlEnable,
    Coils_SV_05_CtrlEnable,
    Coils_SV_06_CtrlEnable,
    Coils_SV_07_CtrlEnable,
    Coils_SV_08_CtrlEnable,
    Coils_SV_09_CtrlEnable,
    Coils_SV_10_CtrlEnable,
    Coils_SV_11_CtrlEnable,
    Coils_SV_12_CtrlEnable,
    Coils_SV_13_CtrlEnable,
    Coils_SV_14_CtrlEnable,

    //IO输出控制使能位段2
    Coils_BL_01_CtrlEnable = 0x0040,
    Coils_BL_02_CtrlEnable,
    Coils_BL_03_CtrlEnable,
    Coils_BL_04_CtrlEnable,
    Coils_PMP_01_CtrlEnable,
    Coils_PMP_02_CtrlEnable,
    Coils_PMP_03_CtrlEnable,
    Coils_PMP_04_CtrlEnable,
    Coils_PMP_05_CtrlEnable,
    Coils_RAD_01_CtrlEnable,
    Coils_KM_01_CtrlEnable,

    //模式控制位
    CoilsBits_AutoCtrl = 0x0050,
    CoilsBits_AutoCharge,
    CoilsBits_BL_01_AutoCtrl,
    CoilsBits_BL_02_AutoCtrl,
    CoilsBits_BL_03_AutoCtrl,
    CoilsBits_BL_04_AutoCtrl,
    CoilsBits_PMP_01_AutoCtrl,
    CoilsBits_PMP_02_AutoCtrl,
    CoilsBits_PMP_03_AutoCtrl,
    CoilsBits_PMP_04_AutoCtrl,
    CoilsBits_PMP_05_AutoCtrl,
    CoilsBits_RAD_01_AutoCtrl,

    //报警开关控制位
    CoilsBits_LT_01_AlarmCtrl = 0x0060,
    CoilsBits_LT_02_AlarmCtrl,
};

enum DevStatusRegs
{
    TT_01 = 0x2001,     //温度01
    TT_02 = 0x2002,
    TT_03 = 0x2003,
    TT_04 = 0x2004,
    TT_05 = 0x2005,
    TT_06 = 0x2006,
    TT_07 = 0x2007,
    TT_08 = 0x2008,
    TT_09 = 0x2009,
    TT_10 = 0x200a,
    TT_11 = 0x200b,
    TT_12 = 0x200c,
    TT_13 = 0x200d,
    TT_14 = 0x200e,
    TT_15 = 0x200f,
    TT_16 = 0x2010,
    TT_17 = 0x2011,
    TT_18 = 0x2012,
    TT_19 = 0x2013,
    TT_20 = 0x2014,
    TT_21 = 0x2015,
    TT_22 = 0x2016,
    TT_23 = 0x2017,
    TT_24 = 0x2018,
    TT_25 = 0x2019,
    TT_26 = 0x201a,
    TT_27 = 0x201b,
    TT_28 = 0x201c,
    TT_29 = 0x201d,
    TT_30 = 0x201e,
    TT_31 = 0x201f,
    TT_32 = 0x2020,
    TT_33 = 0x2021,
    TT_34 = 0x2022,
    TT_35 = 0x2023,
    TT_36 = 0x2024,

    PT_01 = 0x2025,     //压力01
    PT_02 = 0x2026,
    PT_03 = 0x2027,
    PT_04 = 0x2028,
    PT_05 = 0x2029,
    PT_06 = 0x202a,

    AFM_01 = 0x202b,    //空气流量1
    AFM_02 = 0x202c,
    AFM_03 = 0x202d,
    AFM_04 = 0x202e,
    MFM_01 = 0x202f,    //质量流量1

    BL_01 = 0x2030,     //燃烧腔空气进气风机反馈速度
    BL_02 = 0x2031,     //BL-02 反馈速度
    BL_03 = 0x2032,     //阴极空气泵反馈速度
    BL_04 = 0x2033,     //PROX 温度风扇反馈速度
    PMP_01 = 0x2034,    //燃料进液泵
    PMP_02 = 0x2035,    //过滤回路泵反馈速度
    PMP_03 = 0x2036,    //电堆冷却水泵反馈速度
    PMP_04 = 0x2037,    //外部换热泵反馈速度
    PMP_05 = 0x2038,    //外部换热泵反馈速度
    RAD_01 = 0x2039,    //换热器风机反馈速度

    SysStatus = 0x203a, //系统运行状态
    CM_01 = 0x203b,     //电导率
    LT_01 = 0x203c,     //燃料箱液位1
    LT_02 = 0x203d,     //燃料箱液位2
    VT_01 = 0x203e,     //燃料电池电压
    IT_01 = 0x203f,     //燃料电池电流
    FcPower = 0x2040,   //燃料电池发电功率
    OutVoltage = 0x2041,    //系统输出电压
    OutCurrent = 0x2042,    //系统输出电流
    OutPower = 0x2043,      //系统输出功率
    VT_02 = 0x2044,     //电池电压
    IT_02 = 0x2045,     //电池电流
    SinglePowerProduced = 0x2046,   //单次发电量
    TotalPowerProduced = 0x2048,    //累计发电量
    TotalBootTimes = 0x204a,        //累计启动次数
    FuelConsumption = 0x204b,       //燃料消耗率
    TotalFuelConsumption = 0x204c,  //燃料累计消耗量
};


#endif // ALLBITSANDREGS_H

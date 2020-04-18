#ifndef DEVSTATUS_REGS_H
#define DEVSTATUS_REGS_H

#include <QtCharts>
#include <QChartView>

enum DisplayGroups
{
    TT01_TT08,
    TT09_TT16,
    TT17_TT24,
    TT25_TT32,
    TT33_TT36,

    PressureChart,
    FlowChart,
    SpeedChart_1,
    SpeedChart_2,
    OthersChart,
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

const int max_charts_num = 8;

#endif // DEVSTATUS_REGS_H

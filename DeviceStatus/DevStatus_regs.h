#ifndef DEVSTATUS_REGS_H
#define DEVSTATUS_REGS_H

enum DevStatusRegs
{
    TT_01 = 0x2001,
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

    PT_01 = 0x2025,
    PT_02 = 0x2026,
    PT_03 = 0x2027,
    PT_04 = 0x2028,
    PT_05 = 0x2029,
    PT_06 = 0x202a,

    AFM_01 = 0x202b,
    AFM_02 = 0x202c,
    AFM_03 = 0x202d,
    AFM_04 = 0x202e,
    MFM_01 = 0x202f,

    BL_01 = 0x2030,
    BL_02 = 0x2031,
    BL_03 = 0x2032,
    BL_04 = 0x2033,
    PMP_01 = 0x2034,
    PMP_02 = 0x2035,
    PMP_03 = 0x2036,
    PMP_04 = 0x2037,
    PMP_05 = 0x2038,
    RAD_01 = 0x2039,

    SysStatus = 0x203a,
    CM_01 = 0x203b,
    LT_01 = 0x203c,
    LT_02 = 0x203d,
    VT_01 = 0x203e,
    IT_01 = 0x203f,
    FcPower = 0x2040,
    OutVoltage = 0x2041,
    OutCurrent = 0x2042,
    OutPower = 0x2041,
    VT_02 = 0x2044,
    IT_02 = 0x2045,
    SinglePowerProduced = 0x2046,
    TotalPowerProduced = 0x2048,
    TotalBootTimes = 0x204a,
    FuelConsumption = 0x204b,
    TotalFuelConsumption = 0x204c,
};

#endif // DEVSTATUS_REGS_H

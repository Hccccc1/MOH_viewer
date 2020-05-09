#ifndef DEVSTATUS_REGS_H
#define DEVSTATUS_REGS_H

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

const int max_charts_num = 8;

#endif // DEVSTATUS_REGS_H

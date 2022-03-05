#ifndef SIGNAL_H
#define SIGNAL_H

#include <game_project_1/types/str.hpp>

union SignalParam{
    int ival;
    float fval;
    void* pval;
    wchar* strval;
};

struct EventSignal{
    const static int SIGNAL_PARAM_COUNT=3;
    int type;
    SignalParam params[SIGNAL_PARAM_COUNT];
};










#endif
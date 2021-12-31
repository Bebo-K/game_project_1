#ifndef SIGNAL_H
#define SIGNAL_H

#include "struct/str.h"

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
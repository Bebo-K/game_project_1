export module Signal;
import String;

export union SignalParam{
    int ival;
    float fval;
    void* pval;
    wchar* strval;
};


export struct EventSignal{
    static constexpr int SIGNAL_PARAM_COUNT=3;
    int type;
    SignalParam params[SIGNAL_PARAM_COUNT];
};










#endif
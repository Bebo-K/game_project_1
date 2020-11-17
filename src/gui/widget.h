#ifndef WIDGET_H
#define WIDGET_H

#include "layout.h"
#include "../input.h"

class Widget{
    public:
    Layout layout;

    virtual void Paint(){}
    virtual void OnUpdate(int frames){}
    virtual bool OnInput(InputEvent event_type){return false;}
    inline void Resize(){layout.Resize();OnResize();}
    virtual void OnResize(){}
    virtual bool OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}

    virtual ~Widget(){}
};




#endif
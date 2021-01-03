#ifndef TEXT_WIDGET_H
#define TEXT_WIDGET_H

#include "../../gfx/ui_text.h"
#include "../widget.h"

class TextWidget:public Widget{
    public:
    UIText text;

    TextWidget(char* text,int h, int w);
    ~TextWidget();

    void Paint();
    void OnUpdate(int frames);
    bool OnInput(InputCode event_type);
    void OnResize();
    bool OnSignal(int signal_id,int metadata_len, byte* metadata);

};



#endif
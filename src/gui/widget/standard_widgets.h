#ifndef SIMPLE_BUTTON_WIDGET_H
#define SIMPLE_BUTTON_WIDGET_H

#include "../widget.h"

namespace UI{    

class SimpleButton:public Widget{
    public:
    void (*select_callback)();

    SimpleButton(char* name, wchar* label, float w, float h, vec4 color, void (*callback)());
    ~SimpleButton();

};

}

#endif
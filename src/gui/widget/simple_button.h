#ifndef SIMPLE_BUTTON_WIDGET_H
#define SIMPLE_BUTTON_WIDGET_H

#include "widget.h"

namespace UI{
    
Widget* BuildSimpleButton(char* name, char* label, int w, int h, vec4 color, void (*callback)());



}



#endif
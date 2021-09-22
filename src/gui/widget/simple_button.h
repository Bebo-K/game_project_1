#ifndef SIMPLE_BUTTON_WIDGET_H
#define SIMPLE_BUTTON_WIDGET_H

#include "../widget.h"

namespace UI{    
Widget* BuildSimpleButton(char* name, char* label, float w, float h, vec4 color, void (*callback)());
}



#endif
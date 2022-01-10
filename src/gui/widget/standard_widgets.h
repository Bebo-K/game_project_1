#ifndef SIMPLE_BUTTON_WIDGET_H
#define SIMPLE_BUTTON_WIDGET_H

#include "../widget.h"
#include "component_clickable.h"
#include "component_selectable.h"

namespace UI{    


class SimpleButton:public Widget{

    SimpleButton(char* name, char* label, float w, float h, vec4 color, void (*callback)());
    ~SimpleButton();

    SelectableComponent* GetSelectionInfo();
    ClickableComponent* GetClickInfo();
};


#endif
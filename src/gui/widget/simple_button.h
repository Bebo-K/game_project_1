#ifndef SIMPLE_BUTTON_WIDGET_H
#define SIMPLE_BUTTON_WIDGET_H

#include "widget.h"

#include "widget/rect_component.h"
#include "widget/text_component.h"

namespace UI{
    
Widget* BuildSimpleButton(char* name, char* label, int w, int h, vec4 color, void (*callback)()){
    Widget* ret = new Widget(name);
    ret->layout.w=w;
    ret->layout.h=h;

    ret->components.Add(new RectComponent(color));
    ret->components.Add(new TextComponent(label));
    ret->components.Add(new ButtonComponent(callback));

    return ret;
}

}



#endif
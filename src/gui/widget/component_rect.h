#ifndef COMPONENT_RECT_H
#define COMPONENT_RECT_H

#include "../widget.h"
#include "../../gfx/ui_shape.h"

namespace UI{
    
class RectComponent: public WidgetComponent{
    UI_Rect shape;
    public:
    RectComponent(vec4 colour);
    ~RectComponent();

    void OnPaint(Widget* w);
    void OnResize(Widget* w);
};

}

#endif
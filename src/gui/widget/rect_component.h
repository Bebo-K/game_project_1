#ifndef SHAPE_VIEW_H
#define SHAPE_VIEW_H

#include "../widget.h"
#include "../../gfx/ui_shape.h"

namespace UI{
    
class RectComponent: public WidgetComponent{
    UI_Rect shape;
    public:
    RectComponent(vec4 colour);

    void OnPaint(Widget* w);
    void OnResize(Widget* w);
};

}

#endif
#ifndef SHAPE_VIEW_H
#define SHAPE_VIEW_H

#include "../widget.h"
#include "../../gfx/ui_shape.h"

class RectWidget: public Widget{
    UI_Rect shape;
    public:
    RectWidget(vec4 colour);
    RectWidget(int w, int h, vec4 colour);
    ~RectWidget();

    void OnPaint();
    void OnResize();
};

















#endif
#ifndef UI_SHAPE_H
#define UI_SHAPE_H
//Simple flat shapes for use in drawing UI

#include "drawable.h"
#include "../struct/2d_types.h"

class UI_Rect{
    public:
    rect_i      rect;
    point_f     center;
    vec4        color={1.0f,1.0f,1.0f,1.0f};
    float       rotation;
    float       scale;
    
    UI_Rect();
    UI_Rect(int w,int h);
    ~UI_Rect();
 
    void Draw();
};


#endif
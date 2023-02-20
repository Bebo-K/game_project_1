#ifndef UI_SHAPE_H
#define UI_SHAPE_H
//Simple flat shapes for use in drawing UI

#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/types/2d_types.hpp>


//UI_Rect: A UI primitive for drawing a colored rectangle to the screen
class UI_Rect{
    public:
    rect_i      rect;
    point_f     center;
    color_f     color;
    float       rotation;
    float       scale;
    
    UI_Rect();
    UI_Rect(int w,int h);
    ~UI_Rect();
 
    void Draw();
};


#endif
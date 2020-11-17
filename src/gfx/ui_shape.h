#ifndef UI_SHAPE_H
#define UI_SHAPE_H
//Simple flat shapes for use in drawing UI

#include "drawable.h"

class UI_Rect{
    public:
    int         x,y,width,height;
	float		center_x;//offset from rect's top left, used for center of rotation
	float		center_y;
    vec4        color={1.0f,1.0f,1.0f,1.0f};
    float       rotation;
    float       scale;
    
    UI_Rect();
    UI_Rect(int w,int h);
    ~UI_Rect();
 
    void Draw();
};


#endif
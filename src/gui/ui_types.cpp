#include "ui_types.h"

#include "../config.h"

using namespace UI;


Layout::Layout(){
    parent=nullptr;
    relative = {0,0,1,1};
    w=config::window_width;
    h=config::window_height;
    center.x=w/2;
    center.y=h/2;
}
Layout::Layout(Layout* parent_layout){
    parent = parent_layout;
    x_pos_scale= Relative;y_pos_scale= Relative;
    width_scale = Relative;height_scale = Relative;
    relative = {0,0,1,1};
    w=parent->w;
    h=parent->h;
    center.x=parent->center.x;
    center.y=parent->center.y;
}
void Layout::SetParent(Layout* parent_layout){
    parent = parent_layout;
    if(parent != nullptr){
        relative.x = (center.x-parent->center.x)/parent->w;
        relative.y = (center.y-parent->center.y)/parent->h;
        relative.w = w/parent->w;
        relative.h = h/parent->h;
        Resize();
    }
}
void Layout::SetOffsetMode(ScaleMode x, ScaleMode y){
    x_pos_scale =x;
    y_pos_scale=y;
}
void Layout::SetOffset(float x_pos,float y_pos){
    if(x_pos_scale == Relative){
        relative.x = x_pos;
        if(parent != nullptr){center.x = parent->center.x + parent->w*relative.x;}
    }
    else{
        center.x = x_pos;
        if(parent != nullptr){relative.x = (center.x-parent->center.x)/parent->w;}
    }
    if(y_pos_scale == Relative){
        relative.y = y_pos;
        if(parent != nullptr){center.y = parent->center.y + parent->h*relative.y;}
    }
    else{
        center.y = y_pos;
        if(parent != nullptr){relative.y = (center.y-parent->center.y)/parent->h;}
    }
}

void Layout::SetSizeMode(ScaleMode w, ScaleMode h){
    width_scale =w;
    height_scale=h;
}
void Layout::SetSize(float width, float height){
    if(width_scale == Relative){
        relative.w = width;
        if(parent != nullptr){w = relative.w * parent->w;}
    }
    else{
        w = width;
        if(parent != nullptr){relative.w = w/parent->w;}
    }
    if(height_scale == Relative){
        relative.h = height;
        if(parent != nullptr){h = relative.h * parent->h;}
    }
    else{
        h = height;
        if(parent != nullptr){relative.h = h/parent->h;}
    }
}

void Layout::Resize(){
    if(parent == nullptr || parent->w == 0 || parent->h ==0){return;}
    if(x_pos_scale==Relative){center.x = parent->center.x + parent->w*relative.x;}
     else{relative.x = (center.x-parent->center.x)/parent->w;}
    if(y_pos_scale==Relative){center.y = parent->center.y + parent->h*relative.y;}
     else{relative.y = (center.y-parent->center.y)/parent->h;}
    if(width_scale==Relative){w = relative.w * parent->w;}
     else{relative.w = w/parent->w;}
    if(height_scale==Relative){h = relative.h * parent->h;}
     else{relative.h = h/parent->h;}
}
rect_f Layout::GetRect(){
    float x = center.x-(w/2);
    float y = center.y-(h/2);
    return {x,y,w,h};
}
void Layout::MoveTo(Layout* l2,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset){
    center.x = l2->center.x + offset.x;
    center.y = l2->center.y - offset.y;

    switch (hmode){
        case UI::Center_H:  break;
        case UI::Left:      center.x -= ((l2->w/2) - (w/2));break;
        case UI::Right:     center.x += ((l2->w/2) - (w/2));break;
        case UI::To_Left:   center.x -= ((l2->w/2) + (w/2));break;
        case UI::To_Right:  center.x += ((l2->w/2) + (w/2));break;
        default:break;
    };
    switch (vmode){
        case UI::Center_V:  break;
        case UI::Top:       center.y += ((l2->h/2) - (h/2)); break;
        case UI::Bottom:    center.y -= ((l2->h/2) - (h/2)); break;
        case UI::Above:     center.y += ((l2->h/2) + (h/2)); break;
        case UI::Below:     center.y -= ((l2->h/2) + (h/2)); break;
        default:break;
    };
    
    if(parent != nullptr){
        relative.x = (center.x-parent->center.x)/parent->w;
        relative.y = (center.y-parent->center.y)/parent->h;
    }
}
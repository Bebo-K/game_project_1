#include "ui_types.h"



using namespace UI;


Layout::Layout(){
    parent=nullptr;
    width_scale = Absolute;
    height_scale = Absolute;
    x_pos_scale= Absolute;
    y_pos_scale= Absolute;
    relative = {0,0,1,1};
    x=y=0;
    w=h=1;
}
Layout::Layout(Layout* parent_layout){
    parent = parent_layout;
    width_scale = Absolute;
    height_scale = Absolute;
    x_pos_scale= Absolute;
    y_pos_scale= Absolute;
}
void Layout::SetParent(Layout* parent_layout){
    parent = parent_layout;
    Resize();
}
void Layout::SetOffset(int x_pos,int y_pos){
    x=x_pos;
    y=y_pos;
    if(parent != nullptr){
        x += parent->x;
        y += parent->y;
    }
}
void Layout::Resize(){
    if(parent == nullptr || parent->w == 0 || parent->h ==0){return;}
    if(x_pos_scale==Relative){x = relative.x * parent->w;}
     else{relative.x = x/parent->w;}
    if(y_pos_scale==Relative){y = relative.y * parent->h;}
     else{relative.y = y/parent->h;}
    if(width_scale==Relative){w = relative.w * parent->w;}
     else{relative.w = w/parent->w;}
    if(height_scale==Relative){h = relative.h * parent->h;}
     else{relative.h = h/parent->h;}
}
rect_i Layout::GetRect(){
    return {x,y,w,h};
}
void Layout::MoveTo(Layout* l2,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset){
    x = l2->x + offset.x;
    y = l2->y + offset.y;

    switch (hmode){
        case UI::Center_H:  x += (l2->w/2) - (w/2);break;
        case UI::Left:      break;
        case UI::Right:     x += l2->w - w;break;
        case UI::To_Left:   x -= w;
        case UI::To_Right:  x += l2->w; break;
        default:break;
    };
    switch (vmode){
        case UI::Center_V:  y += (l2->w/2) - (w/2);break;
        case UI::Top:       break;
        case UI::Bottom:    y += l2->w -w; break;
        case UI::Above:     y -= w; break;
        case UI::Below:     y += l2->w; break;
        default:break;
    };
}
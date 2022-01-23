#include "layout.h"
#include "../os.h"

using namespace UI;

point_i UI::ScreenToClientSpace(point_i screen_pos){
    unsigned int x = UI_WIDTH*screen_pos.x;
    unsigned int y = UI_HEIGHT*screen_pos.y;
    return {(int)(x/Window::width),(int)(y/Window::height)};
}

point_i UI::ClientToScreenSpace(point_f client_pos){
    float x = Window::width*client_pos.x;
    float  y = Window::height*client_pos.y;
    return {(int)(x/UI_WIDTH),(int)(y/UI_HEIGHT)};
}

Layout::Layout(){
    center = {0,0};
    width=0;
    height=0;
}


Layout::Layout(int w,int h){
    width=w;
    height=h;
    center = {(w/2),(h/2)};
}

void Layout::MoveTo(Layout* l2,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset){
    center.x = l2->center.x + offset.x;
    center.y = l2->center.y - offset.y;

    switch (hmode){
        case UI::Center_H:  break;
        case UI::Left:      center.x -= ((l2->width/2) - (width/2));break;
        case UI::Right:     center.x += ((l2->width/2) - (width/2));break;
        case UI::To_Left:   center.x -= ((l2->width/2) + (width/2));break;
        case UI::To_Right:  center.x += ((l2->width/2) + (width/2));break;
        default:break;
    };
    switch (vmode){
        case UI::Center_V:  break;
        case UI::Top:       center.y += ((l2->height/2) - (height/2)); break;
        case UI::Bottom:    center.y -= ((l2->height/2) - (height/2)); break;
        case UI::Above:     center.y += ((l2->height/2) + (height/2)); break;
        case UI::Below:     center.y -= ((l2->height/2) + (height/2)); break;
        default:break;
    };
}



rect_i Layout::Rect(){return {center.x-(width/2),center.y-(height/2),width,height};}


rect_i Layout::ScreenRect(){
    return {
        (center.x-(width/2)*Window::width)/UI_WIDTH,
        (center.y-(height/2)*Window::height)/UI_HEIGHT,
        (width*Window::width)/UI_WIDTH,
        (height*Window::height)/UI_HEIGHT
    };
}



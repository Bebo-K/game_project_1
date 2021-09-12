#include "layout.h"



using namespace UI;


Layout::Layout(){
    parent=nullptr;
    width_scale = ABSOLUTE;
    height_scale = ABSOLUTE;
    x_pos_scale= ABSOLUTE;
    y_pos_scale= ABSOLUTE;
    relative = {0,0,1,1};
    x=y=0;
    w=h=1;
}
Layout::Layout(Layout* parent_layout){
    parent = parent_layout;
    width_scale = ABSOLUTE;
    height_scale = ABSOLUTE;
    x_pos_scale= ABSOLUTE;
    y_pos_scale= ABSOLUTE;
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
    if(x_pos_scale==RELATIVE){x = relative.x * parent->w;}
     else{relative.x = x/parent->w;}
    if(y_pos_scale==RELATIVE){y = relative.y * parent->h;}
     else{relative.y = y/parent->h;}
    if(width_scale==RELATIVE){w = relative.w * parent->w;}
     else{relative.w = w/parent->w;}
    if(height_scale==RELATIVE){h = relative.h * parent->h;}
     else{relative.h = h/parent->h;}
}
rect_i Layout::GetRect(){
    return {x,y,w,h};
}
void Layout::MoveTo(Layout* l2,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset){
    x = l2->x + offset.x;
    y = l2->y + offset.y;

    switch (hmode){
        case UI::CENTER_H:  x += (l2->w/2) - (w/2);break;
        case UI::LEFT:      break;
        case UI::RIGHT:     x += l2->w - w;break;
        case UI::TO_LEFT:   x -= w;
        case UI::TO_RIGHT:  x += l2->w; break;
        default:break;
    };
    switch (vmode){
        case UI::CENTER_V:  y += (l2->w/2) - (w/2);break;
        case UI::TOP:       break;
        case UI::BOTTOM:    y += l2->w -w; break;
        case UI::ABOVE:     y -= w; break;
        case UI::BELOW:     y += l2->w; break;
        default:break;
    };
}




/*

Layout::Layout(){
    offset = {0,0};
    absolute = {0,0,1,1};
    relative = {0,0,1,1};
    parent=nullptr;
    width_scale = UI::RATIO;
    height_scale =UI::RATIO;
    h_offset_type = UI::CENTER_H;
    v_offset_type = UI::CENTER_V;
    h_offset_scale = UI::RATIO;
    v_offset_scale = UI::RATIO;
}
Layout::Layout(Layout* parent_layout){
    parent=parent_layout;
    offset = {0,0};
    absolute = {0,0,1,1};
    relative = {0,0,1,1};
    width_scale = UI::RATIO;
    height_scale =UI::RATIO;
    h_offset_type = UI::CENTER_H;
    v_offset_type = UI::CENTER_V;
    h_offset_scale = UI::RATIO;
    v_offset_scale = UI::RATIO;
    
    SetSize({1,1});
}

void Layout::SetHorizontalOffset(UI::HorizontalOrigin origin,int pos){
    h_offset_type = origin;
    SetOffset({pos,absolute.y});
}
void Layout::SetVerticalOffset(UI::VerticalOrigin origin,int pos){
    v_offset_type = origin;
    SetOffset({absolute.x,pos});
}

void Layout::SetSize(point_i new_size){
    absolute.w=new_size.x;
    absolute.h=new_size.y;

    if(absolute.w > 0 && absolute.h > 0 && parent != nullptr){
        if(parent->absolute.w > 0){relative.w = (float)absolute.w/parent->absolute.w;}
        if(parent->absolute.h > 0){relative.h = (float)absolute.h/parent->absolute.h;}
    }
    Resize();
}
void Layout::SetOffset(point_i new_offset){
    offset.x=new_offset.x;
    offset.y=new_offset.y;
    Resize();
}
void Layout::SetRelativeSize(point_f new_size){
    relative.w = new_size.x;
    relative.h = new_size.y;

    if(parent != nullptr){
        absolute.w = relative.w*parent->relative.w;
        absolute.h = relative.h*parent->relative.h;
    }
    Resize();
}
void Layout::SetRelativeOffset(point_f new_offset){
    relative.x = new_offset.x;
    relative.y = new_offset.y;

    if(parent != nullptr){
        offset.x = relative.w*parent->relative.x;
        offset.y = relative.h*parent->relative.y;
    }
    Resize();
}

void Layout::SetParent(Layout* new_parent){
    parent = new_parent;
    Resize();
}

void Layout::Resize(){
    absolute.x=offset.x;
    absolute.y=offset.y;
    if(parent == nullptr){return;}
    if(h_offset_scale==UI::RATIO){offset.x = parent->absolute.w*relative.x;}
    if(v_offset_scale==UI::RATIO){offset.y = parent->absolute.h*relative.y;}
    if(width_scale==UI::RATIO){absolute.w = parent->absolute.w*relative.w;}
    if(height_scale==UI::RATIO){absolute.h = parent->absolute.h*relative.h;}
    


}





*/






/*
Layout::Offset::Offset(){
    parent=nullptr;
    horizontal_mode=H_CENTERED;
    vertical_mode=V_CENTERED;
    x=y=0;
}
Layout::Offset::Offset(LayoutRect* p){
    parent=p;
    horizontal_mode=H_CENTERED;
    vertical_mode=V_CENTERED;
    x=y=0;
}
Layout::Offset::Offset(int offset_x,int offset_y,LayoutRect* parent){
    parent=parent;
    horizontal_mode=H_CENTERED;
    vertical_mode=V_CENTERED;
    x=offset_x;
    y=offset_y;
}
Layout::Offset::Offset(LayoutRect* parent,HorizontalMode horizontal_mode,VerticalMode vertical_mode){
    parent=parent;
    horizontal_mode=horizontal_mode;
    vertical_mode=vertical_mode;
    x=y=0;
}
Layout::Offset::Offset(int offset_x,int offset_y,LayoutRect* p,
                HorizontalMode horizontal_mode,VerticalMode vertical_mode){
    parent=p;
    horizontal_mode=horizontal_mode;
    vertical_mode=vertical_mode;
    x=offset_x;
    y=offset_y;
}

LayoutRect::LayoutRect(){
    offset = Layout::Offset();
    h_pos_mode = Layout::NOSCALE;
    v_pos_mode = Layout::NOSCALE;
    h_fill_mode = Layout::NOSCALE;
    v_fill_mode = Layout::NOSCALE;
    X=Y=W=H=0;
    relative = {1,1,0,0};
}
LayoutRect::LayoutRect(int pX,int pY,int pW,int pH){
    offset = Layout::Offset();
    h_pos_mode = Layout::NOSCALE;
    v_pos_mode = Layout::NOSCALE;
    h_fill_mode = Layout::NOSCALE;
    v_fill_mode = Layout::NOSCALE;
    X=pX;Y=pY;W=pW;H=pH;
    relative = {1,1,0,0};
}
LayoutRect::LayoutRect(int pW,int pH, Layout::Offset parent_offset){
    offset = parent_offset;
    h_pos_mode = Layout::NOSCALE;
    v_pos_mode = Layout::NOSCALE;
    h_fill_mode = Layout::NOSCALE;
    v_fill_mode = Layout::NOSCALE;
    X=Y=0;
    W=pW;H=pH;
    relative = {1,1,0,0};
    if(parent_offset.parent != nullptr){
        relative.x = offset.x/offset.parent->W;
        relative.y = offset.y/offset.parent->H;
        relative.w = W/offset.parent->W;
        relative.h = H/offset.parent->H;
    }
}

LayoutRect::LayoutRect(int pW,int pH, Layout::Offset parent_offset,Layout::ScaleMode pos_fill_modes[4]){
    offset = parent_offset;
    h_pos_mode = pos_fill_modes[0];
    v_pos_mode = pos_fill_modes[1];
    h_fill_mode = pos_fill_modes[2];
    v_fill_mode = pos_fill_modes[3];
    X=Y=0;
    W=pW;H=pH;
    relative = {1,1,0,0};
    if(parent_offset.parent != nullptr){
        relative.x = offset.x/offset.parent->W;
        relative.y = offset.y/offset.parent->H;
        relative.w = W/offset.parent->W;
        relative.h = H/offset.parent->H;
    }
}

void LayoutRect::Resize(){
     X=offset.x;
     Y=offset.y;
    if(offset.parent == nullptr){return;}

    switch(h_fill_mode){
        case Layout::NOSCALE: break;
        case Layout::MATCH_PARENT: W = offset.parent->W - offset.x; break;
        case Layout::RATIO: W = offset.parent->W * relative.w;break;
        default:break;
    }
    switch(v_fill_mode){
        case Layout::NOSCALE: break;
        case Layout::MATCH_PARENT: H = offset.parent->H - offset.y; break;
        case Layout::RATIO: H = offset.parent->H * relative.h;break;
        default:break;
    }
    switch(h_pos_mode){
        case Layout::NOSCALE: X = offset.x;break;
        case Layout::MATCH_PARENT: X = offset.parent->X; offset.x=0;break;
        case Layout::RATIO: X = relative.x * offset.parent->W;break;
        default:break;
    }
    switch(v_pos_mode){
        case Layout::NOSCALE: Y = offset.y; break;
        case Layout::MATCH_PARENT: Y = offset.parent->Y;offset.y=0; break;
        case Layout::RATIO: Y = relative.y * offset.parent->H;break;
        default:break;
    }
    switch (offset.horizontal_mode){
        case Layout::H_CENTERED:  X += offset.parent->X +(offset.parent->W/2) - (W/2);break;
        case Layout::H_LEFT:      X += offset.parent->X; break;
        case Layout::H_RIGHT:     X += offset.parent->X + offset.parent->W - W;break;
        case Layout::H_TO_LEFT:   X += offset.parent->X - W;
        case Layout::H_TO_RIGHT:  X += offset.parent->X + offset.parent->W; break;
        default:break;
    };
    switch (offset.vertical_mode){
        case Layout::V_CENTERED:  Y += offset.parent->Y +(offset.parent->H/2) - (H/2);break;
        case Layout::V_TOP:       Y += offset.parent->Y + offset.parent->H - H; break;
        case Layout::V_BOTTOM:    Y += offset.parent->Y;break;
        case Layout::V_ABOVE:     Y += offset.parent->Y + offset.parent->H;
        case Layout::V_BELOW:     Y += offset.parent->Y - H; break;
        default:break;
    }
    if(offset.parent->W <= 0 || offset.parent->H <= 0){
        relative.x=0;
        relative.y=0;
        relative.w=1;
        relative.h=1;
        return;
    }
    relative.x = offset.x/offset.parent->W;
    relative.y = offset.y/offset.parent->H;
    relative.w = W/offset.parent->W;
    relative.h = H/offset.parent->H;
}

*/
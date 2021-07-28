#include "layout.h"

LayoutOffset::LayoutOffset(){
    parent=nullptr;
    horizontal_mode=HLayoutMode::H_CENTERED;
    vertical_mode=VLayoutMode::V_CENTERED;
    x=y=0;
}
LayoutOffset::LayoutOffset(Layout* p){
    parent=p;
    horizontal_mode=HLayoutMode::H_CENTERED;
    vertical_mode=VLayoutMode::V_CENTERED;
    x=y=0;
}
LayoutOffset::LayoutOffset(int hmode,int vmode,Layout* p){
    parent=p;
    horizontal_mode=hmode;
    vertical_mode=vmode;
    x=y=0;
}
LayoutOffset::LayoutOffset(Layout* p,int offset_x,int offset_y){
    parent=p;
    horizontal_mode=HLayoutMode::H_CENTERED;
    vertical_mode=VLayoutMode::V_CENTERED;
    x=offset_x;
    y=offset_y;
}
LayoutOffset::LayoutOffset(int offset_x,int offset_y,Layout* p,int hmode,int vmode){
    parent=p;
    horizontal_mode=hmode;
    vertical_mode=vmode;
    x=offset_x;
    y=offset_y;
}

Layout::Layout(){
    offset = LayoutOffset();
    h_pos_mode = LayoutScaleMode::NOSCALE;
    v_pos_mode = LayoutScaleMode::NOSCALE;
    h_fill_mode = LayoutScaleMode::NOSCALE;
    v_fill_mode = LayoutScaleMode::NOSCALE;
    X=Y=W=H=0;
    relative = {1,1,0,0};
}
Layout::Layout(int pX,int pY,int pW,int pH){
    offset = LayoutOffset();
    h_pos_mode = LayoutScaleMode::NOSCALE;
    v_pos_mode = LayoutScaleMode::NOSCALE;
    h_fill_mode = LayoutScaleMode::NOSCALE;
    v_fill_mode = LayoutScaleMode::NOSCALE;
    X=pX;Y=pY;W=pW;H=pH;
    relative = {1,1,0,0};
}
Layout::Layout(int pW,int pH, LayoutOffset parent_offset){
    offset = parent_offset;
    h_pos_mode = LayoutScaleMode::NOSCALE;
    v_pos_mode = LayoutScaleMode::NOSCALE;
    h_fill_mode = LayoutScaleMode::NOSCALE;
    v_fill_mode = LayoutScaleMode::NOSCALE;
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

Layout::Layout(int pW,int pH, LayoutOffset parent_offset,int pos_fill_modes[4]){
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

void Layout::Resize(){
     X=offset.x;
     Y=offset.y;
    if(offset.parent == nullptr){return;}

    switch(h_pos_mode){
        case LayoutScaleMode::NOSCALE: X = offset.x;break;
        case LayoutScaleMode::MATCH_PARENT: X = offset.parent->X; offset.x=0;break;
        case LayoutScaleMode::RATIO: X = relative.x * offset.parent->W;break;
        default:break;
    }
    switch(v_pos_mode){
        case LayoutScaleMode::NOSCALE: Y = offset.y; break;
        case LayoutScaleMode::MATCH_PARENT: Y = offset.parent->Y;offset.y=0; break;
        case LayoutScaleMode::RATIO: Y = relative.y * offset.parent->H;break;
        default:break;
    }
    switch (offset.horizontal_mode){
        case HLayoutMode::H_CENTERED:  X += offset.parent->X +(offset.parent->W/2) - (W/2);break;
        case HLayoutMode::H_LEFT:      X += offset.parent->X; break;
        case HLayoutMode::H_RIGHT:     X += offset.parent->X + offset.parent->W - W;break;
        case HLayoutMode::H_TO_LEFT:   X += offset.parent->X - W;
        case HLayoutMode::H_TO_RIGHT:  X += offset.parent->X + offset.parent->W; break;
        default:break;
    };
    switch (offset.vertical_mode){
        case VLayoutMode::V_CENTERED:  Y += offset.parent->Y +(offset.parent->H/2) - (H/2);break;
        case VLayoutMode::V_TOP:       Y += offset.parent->Y; break;
        case VLayoutMode::V_BOTTOM:    Y += offset.parent->Y + offset.parent->H - H;break;
        case VLayoutMode::V_ABOVE:     Y += offset.parent->Y - H;
        case VLayoutMode::V_BELOW:     Y += offset.parent->Y + offset.parent->H; break;
        default:break;
    }
    switch(h_fill_mode){
        case LayoutScaleMode::NOSCALE: break;
        case LayoutScaleMode::MATCH_PARENT: W = offset.parent->W - offset.x; break;
        case LayoutScaleMode::RATIO: W = offset.parent->W * relative.w;break;
        default:break;
    }
    switch(v_fill_mode){
        case LayoutScaleMode::NOSCALE: break;
        case LayoutScaleMode::MATCH_PARENT: H = offset.parent->H - offset.y; break;
        case LayoutScaleMode::RATIO: H = offset.parent->H * relative.h;break;
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
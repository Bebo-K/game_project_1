#include "layout.h"


LayoutOffset::LayoutOffset(){parent=nullptr;horizontal_mode=H_CENTERED;vertical_mode=V_CENTERED;x=y=0;}
LayoutOffset::LayoutOffset(Layout* p){parent=p;horizontal_mode=H_CENTERED;vertical_mode=V_CENTERED;x=y=0;}
LayoutOffset::LayoutOffset(Layout* p,HLayoutMode hmode,VLayoutMode vmode){parent=p;horizontal_mode=hmode;vertical_mode=vmode;x=y=0;}
LayoutOffset::LayoutOffset(Layout* p,int offset_x,int offset_y,HLayoutMode hmode,VLayoutMode vmode){parent=p;horizontal_mode=hmode;vertical_mode=vmode;x=offset_x;y=offset_y;}
LayoutOffset::LayoutOffset(Layout* p,int offset_x,int offset_y){parent=p;horizontal_mode=H_CENTERED;vertical_mode=V_CENTERED;x=offset_x;y=offset_y;}

Layout::Layout(){
    offset = LayoutOffset();
    X=Y=W=H=0;
}
void Layout::Resize(){
     X=offset.x;
     Y=offset.y;
    if(offset.parent == nullptr){return;}

    switch (offset.horizontal_mode){
        case H_CENTERED:  X += offset.parent->X +(offset.parent->W/2) - (W/2);break;
        case H_LEFT:      X += offset.parent->X; break;
        case H_RIGHT:     X += offset.parent->X + offset.parent->W - W;break;
        case H_TO_LEFT:   X += offset.parent->X - W;
        case H_TO_RIGHT:  X += offset.parent->X + offset.parent->W; break;
        default:break;
    };
    switch (offset.vertical_mode){
        case V_CENTERED:  Y += offset.parent->X +(offset.parent->W/2) - (W/2);break;
        case V_TOP:       Y += offset.parent->X; break;
        case V_BOTTOM:    Y += offset.parent->X + offset.parent->W - W;break;
        case V_ABOVE:     Y += offset.parent->X - W;
        case V_BELOW:     Y += offset.parent->X + offset.parent->W; break;
        default:break;
    }
}
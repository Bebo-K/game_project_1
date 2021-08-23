#ifndef LAYOUT_H
#define LAYOUT_H

#include "../struct/2d_types.h"

namespace UI{
    enum ScaleMode{NO_SCALE, RATIO};
    enum HorizontalOrigin{CENTER_H,RIGHT,LEFT,TO_RIGHT,TO_LEFT};
    enum VerticalOrigin{CENTER_V,TOP,BOTTOM,ABOVE,BELOW};
}

//A general struct for self-sizing rectangles to position menus/panels/widgets
struct Layout{
    struct Layout* parent;
    UI::ScaleMode width_scale,height_scale;
    UI::ScaleMode h_offset_scale,v_offset_scale;
    UI::HorizontalOrigin h_offset_type;
    UI::VerticalOrigin v_offset_type;

    point_i offset;
    rect_i absolute;
    rect_f relative;

    Layout();
    Layout(Layout* parent);

    void SetHorizontalOffset(UI::HorizontalOrigin origin,int pos);
    void SetVerticalOffset(UI::VerticalOrigin origin,int pos);
    void SetSize(point_i new_size);
    void SetOffset(point_i new_offset);
    void SetRelativeSize(point_f new_size);
    void SetRelativeOffset(point_f new_offset);
    void SetParent(Layout* new_parent);
    void Resize();
};


/*

namespace Layout{
    enum HorizontalMode{
        H_CENTERED=0,H_RIGHT=1,H_LEFT=2,H_TO_RIGHT=3,H_TO_LEFT=4,H_CUSTOM=5
    };
    enum VerticalMode{
        V_CENTERED=0,V_TOP=1,V_BOTTOM=2,V_ABOVE=3,V_BELOW=4,V_CUSTOM=5
    };
    enum ScaleMode{
        NOSCALE=0,RATIO=1,MATCH_PARENT=2
    };

    struct Offset
    {
        LayoutRect* parent;
        int x,y;
        HorizontalMode horizontal_mode;
        VerticalMode vertical_mode;

        Offset();//0,0
        Offset(LayoutRect* parent);//no offset, centered on parent
        Offset(int offset_x,int offset_y,LayoutRect* parent);//offset from center of parent
        Offset(LayoutRect* parent,HorizontalMode horizontal_mode,VerticalMode vertical_mode);//no offset from horizontal/vertical layout mode
        Offset(int offset_x,int offset_y,LayoutRect* parent,HorizontalMode horizontal_mode,VerticalMode vertical_mode);//offset from horizontal/vertical layout mode
    };
}


struct LayoutRect{
    Layout::Offset offset;    
    Layout::ScaleMode  h_pos_mode;
    Layout::ScaleMode  v_pos_mode;
    Layout::ScaleMode  h_fill_mode;
    Layout::ScaleMode  v_fill_mode;
    void SetSize(int px,int py);
    void SetOffset(int px,int py);
    void Resize();
    LayoutRect();
    LayoutRect(int X,int Y,int W,int H);
    LayoutRect(int W,int H, Layout::Offset offset);
    LayoutRect(int W,int H, Layout::Offset offset,Layout::ScaleMode pos_fill_modes[4]);//horizontal position mode,vertical position mode,horizontal fill mode, vertical fill mode
};

*/





#endif
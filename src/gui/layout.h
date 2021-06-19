#ifndef LAYOUT_H
#define LAYOUT_H

#include "../struct/2d_types.h"

namespace HLayoutMode{
    const int H_CENTERED=0,H_RIGHT=1,H_LEFT=2,H_TO_RIGHT=3,H_TO_LEFT=4,H_CUSTOM=5;
}
namespace VLayoutMode{
    const static int V_CENTERED=0,V_TOP=1,V_BOTTOM=2,V_ABOVE=3,V_BELOW=4,V_CUSTOM=5;
}
namespace LayoutScaleMode{
    const static int NOSCALE=0,RATIO=1,MATCH_PARENT=2;
}

//A general class for self-sizing rectangles to position menus/widgets.
struct Layout;

struct LayoutOffset
{
    Layout* parent;
    int x,y;
    int horizontal_mode;
    int vertical_mode;

    LayoutOffset();//0,0
    LayoutOffset(Layout* parent);//no offset, centered on parent
    LayoutOffset(int offset_x,int offset_y,Layout* parent);//offset from center of parent
    LayoutOffset(Layout* parent,int horizontal_mode,int vertical_mode);//no offset from horizontal/vertical layout mode
    LayoutOffset(int offset_x,int offset_y,Layout* parent,int horizontal_mode,int vertical_mode);//offset from horizontal/vertical layout mode
};


struct Layout{
    LayoutOffset offset;    
    int  h_fill_mode;
    int  v_fill_mode;
    int  h_pos_mode;
    int  v_pos_mode;
    rect_f relative;
    int X,Y,W,H;
    void SetSize(int px,int py);
    void SetOffset(int px,int py);
    void Resize();
    Layout();
    Layout(int X,int Y,int W,int H);
    Layout(int W,int H, LayoutOffset offset);
    Layout(int W,int H, LayoutOffset offset,int pos_fill_modes[4]);//horizontal position mode,vertical position mode,horizontal fill mode, vertical fill mode
};


#endif
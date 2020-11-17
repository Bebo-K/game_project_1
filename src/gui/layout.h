#ifndef LAYOUT_H
#define LAYOUT_H

enum HLayoutMode{H_CENTERED,H_RIGHT,H_LEFT,H_TO_RIGHT,H_TO_LEFT,H_CUSTOM};
enum VLayoutMode{V_CENTERED,V_TOP,V_BOTTOM,V_ABOVE,V_BELOW,V_CUSTOM};


//A general class for self-sizing rectangles to position menus/widgets.
struct Layout;

struct LayoutOffset
{
    Layout* parent;
    int x,y;
    HLayoutMode horizontal_mode;
    VLayoutMode vertical_mode;

    LayoutOffset();//no offset, centerered
    LayoutOffset(Layout* parent);
    LayoutOffset(Layout* parent,HLayoutMode horizontal_mode,VLayoutMode vertical_mode);
    LayoutOffset(Layout* parent,int offset_x,int offset_y,HLayoutMode horizontal_mode,VLayoutMode vertical_mode);
    LayoutOffset(Layout* parent,int offset_x,int offset_y);//offset from center
};

struct Layout{
    public:
    LayoutOffset offset;    
    int X,Y,W,H;
    void Resize();
    Layout();
};


#endif
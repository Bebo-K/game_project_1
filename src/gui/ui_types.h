#ifndef UI_TYPES_H
#define UI_TYPES_H

#include "../struct/2d_types.h"

namespace UI{

enum ScaleMode{Absolute, Relative};
enum HorizontalOrigin{Center_H,Right,Left,To_Right,To_Left};
enum VerticalOrigin{Center_V,Top,Bottom,Above,Below};

//A data packet class that can be sent to the UI for custom event handling
struct Signal{
    int signal_id;
    int metadata_len;
    byte* metadata;
};

//A general struct for self-sizing rectangles to position menus/panels/widgets
class Layout{
    public: 
    class Layout* parent;
    ScaleMode width_scale,height_scale;
    ScaleMode x_pos_scale,y_pos_scale;
    rect_f relative;

    int x,y,w,h;
    Layout();
    Layout(Layout* parent);

    void SetParent(Layout* parent);
    void SetOffset(int x,int y);
    void Resize();
    rect_i GetRect();

    void MoveTo(Layout* l2,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset);
};


}













#endif
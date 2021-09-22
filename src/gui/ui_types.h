#ifndef UI_TYPES_H
#define UI_TYPES_H

#include "../struct/2d_types.h"
#include "../struct/data_types.h"

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
    point_f center;//center in absolute coordinates.
    float w,h;
    rect_f relative;

    Layout();
    Layout(Layout* parent);

    void SetParent(Layout* parent);
    void SetOffsetMode(ScaleMode x, ScaleMode y);
    void SetOffset(float x,float y);
    void SetSizeMode(ScaleMode w, ScaleMode h);
    void SetSize(float w,float h);
    void Resize();
    int  X();
    int  Y();
    rect_f GetRect();

    void MoveTo(Layout* l2,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset);
};


}













#endif
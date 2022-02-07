#ifndef LAYOUT_H
#define LAYOUT_H

#include "../struct/2d_types.h"

namespace UI{
    enum HorizontalOrigin{Center_H,Right,Left,To_Right,To_Left};
    enum VerticalOrigin{Center_V,Top,Bottom,Above,Below};

    const int UI_WIDTH= 1280;//640;
    const int UI_HEIGHT=720;//360;

    point_i ScreenToClientSpace(point_i screen_pos);
    point_i ClientToScreenSpace(point_f client_pos);

    struct Layout{
        point_i center;
        int width;
        int height;
        void MoveTo(Layout* l2,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset);
        rect_i Rect();
        rect_i ScreenRect();
        point_i LowerLeft();

        Layout();
        Layout(int width,int height);
    };

};




#endif
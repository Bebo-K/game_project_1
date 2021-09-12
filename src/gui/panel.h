#ifndef PANEL_H
#define PANEL_H

#include "layout.h"
#include "widget.h"

namespace UI{


//A collection of layouts arranged in a relative way.
class Panel{
    Layout master_layout;
    int layout_count;
    Layout* slots;

    void PlaceSlot();
    void AddToSlot();

    //Construct a panel with an evenly laid out grid of elements.
    static Panel* Grid(int w, int h, int rows, int columns, int x_padding, int y_padding);

};

}


#endif
#ifndef PANEL_H
#define PANEL_H

#include "layout.h"
#include "widget.h"

//A collection of layout elements (Widgets + other panels) whose positions are organized as a collection (In a grid/row/etc.)
class Panel{
    Layout master_layout;
    int layout_count;
    Layout* slots;
    Widget* widgets;


    void PlaceSlot();

};



#endif
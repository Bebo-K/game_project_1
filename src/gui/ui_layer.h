#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "menu.h"
#include "../input.h"
#include "../structs/list.h"


class UILayer{
    public:
    bool   active;
    Layout screen_rect;

    List<Menu>  menus;
    StringMap   widgets;

    UILayer();

    Widget* AddWidget(Widget* w,char* name);
    Widget* AddWidget(Widget* w,char* name,LayoutOffset pos);
    Widget* GetWidget(char* name);
    Widget* RemoveWidget(char* name);
    //Widget* GetWidgetByPos(int x,int y);

    void Update(int frames);
    void Paint();
    void Resize(int screen_w,int screen_h);
    bool OnInput(InputCode event_type);
    bool OnSignal(int signal_id,int metadata_len, byte* metadata);
};


#endif
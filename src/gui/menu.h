#ifndef MENU_H
#define MENU_H

#include "widget.h"
#include "layout.h"
#include "../structs/data_types.h"
#include "../input.h"

class Menu{
    public:
    bool active;

    Layout    layout;
    StringMap widgets;

    void SetLayout(int height,int width,LayoutOffset pos);
    //virtual ~Menu(){}

    Widget* AddWidget(Widget* w,char* name);
    Widget* AddWidget(Widget* w,char* name,LayoutOffset pos);
    Widget* GetWidget(char* name);
    Widget* RemoveWidget(char* name);

    void Update(int frames);
        virtual void OnUpdate(int frames){};
    void Paint();
        virtual void OnPaint(){}
    bool HandleInput(InputEvent event_type);
        virtual bool OnInput(InputEvent event_type){return false;}
    void HandleResize();
        virtual void OnResize(){}
    bool HandleSignal(int signal_id,int metadata_len, byte* metadata);
        virtual bool OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}
};


#endif
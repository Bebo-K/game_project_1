#ifndef WIDGET_H
#define WIDGET_H

#include "ui_types.h"
#include "../input.h"
#include "../struct/list.h"

namespace UI{
    
class Widget;

class WidgetComponent{
    public:
    virtual void OnActivate(Widget* w){}
    virtual void OnDeactivate(Widget* w){}
    virtual void OnUpdate(Widget* w,int frames){};
    virtual void OnPaint(Widget* w){}
    virtual bool OnInput(Widget* w,Input::Event event_type){return false;}
    virtual void OnResize(Widget* w){}
    virtual bool OnSignal(Widget* w,Signal signal){return false;}
};

class Widget{
    public:
    Layout                  layout;
    char*                   name;
    bool                    active;
    bool                    visible;
    List<WidgetComponent>   components;
    //WidgetContainer sub_widgets;

    Widget();
    Widget(char* widget_name);
    ~Widget();
    void Activate();
    void Deactivate();
    void Update(int frames);
    void Paint();
    bool HandleInput(Input::Event event_type);
    void HandleResize();
    bool HandleSignal(Signal signal);
};

}

#endif
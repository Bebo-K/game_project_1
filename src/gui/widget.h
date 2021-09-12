#ifndef WIDGET_H
#define WIDGET_H

#include "ui_types.h"
#include "layout.h"
#include "../input.h"
#include "../struct/list.h"

namespace UI{
class Widget;
/*
class WidgetContainer;

struct WidgetContainerIterator{
    WidgetContainer*    parent;
    int                 index;
    Widget*             operator*();
    WidgetContainerIterator operator++();
    bool operator==(WidgetContainerIterator& w2);
    bool operator!=(WidgetContainerIterator& w2);
};
class WidgetContainer{
    public:

    WidgetContainer();
    ~WidgetContainer();
    
    Widget* Add(Widget* w,char* name);
    Widget* Add(Widget* w,char* name,Layout pos);
    Widget* Get(char* name);
    //Widget* Find(char* pathname);
    Widget* Remove(char* name);
    void Destroy(char* name);
    //Widget* GetAtPosition(int x,int y);
    void Clear();

    WidgetContainerIterator begin();
    WidgetContainerIterator end();

    private:
    Layout* parent_layout;
    char**      keys;
    Widget**    values;
    int         slots;


    int next(int index);

    friend class WidgetContainerIterator;
};
*/

class WidgetComponent{
    public:
    virtual void OnActivate(Widget* w){}
    virtual void OnDeactivate(Widget* w){}
    virtual void OnUpdate(Widget* w,int frames){};
    virtual void OnPaint(Widget* w){}
    virtual bool OnInput(Widget* w,Input::Event event_type){return false;}
    virtual void OnResize(Widget* w){}
    virtual bool OnSignal(Widget* w,UISignal signal){return false;}
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
    bool HandleSignal(UISignal signal);
};

}

#endif
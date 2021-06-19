#ifndef WIDGET_H
#define WIDGET_H

#include "layout.h"
#include "../input.h"

class Widget;
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

    WidgetContainer(Layout* parent_layout);
    ~WidgetContainer();
    
    Widget* Add(Widget* w,char* name);
    Widget* Add(Widget* w,char* name,LayoutOffset pos);
    Widget* Get(char* name);
    //Widget* Find(char* pathname);
    Widget* Remove(char* name);
    void Destroy(char* name);
    //Widget* GetAtPosition(int x,int y);
    void Clear();

    WidgetContainerIterator begin();
    WidgetContainerIterator end();

    private:
    Layout*     parent_layout;
    char**      keys;
    Widget**    values;
    int         slots;


    int next(int index);

    friend class WidgetContainerIterator;
};


class Widget{
    public:
    Layout          layout;
    char*           name;
    bool            active;
    bool            visible;
    WidgetContainer sub_widgets;

    void SetName(char* widget_name);
    void ParentTo(Widget* parent,char* my_name);
    Widget();
    virtual ~Widget(){Destroy();}
    void Destroy();
        virtual void OnDestroy(){};

    void Activate();
        virtual void OnActivate(){}
    void Deactivate();
        virtual void OnDeactivate(){}
    void Update(int frames);
        virtual void OnUpdate(int frames){};
    void Paint();
        virtual void OnPaint(){}
    bool HandleInput(Input::EventID event_type);
        virtual bool OnInput(Input::EventID event_type){return false;}
    void HandleResize();
        virtual void OnResize(){}
    bool HandleSignal(int signal_id,int metadata_len, byte* metadata);
        virtual bool OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}
};

#endif
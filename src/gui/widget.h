#ifndef WIDGET_H
#define WIDGET_H

#include "ui_types.h"
#include "../input.h"
#include "../struct/list.h"
#include "../gfx/ui_shape.h"
#include "../gfx/ui_sprite.h"
#include "../gfx/ui_text.h"

namespace UI{
class Widget;
enum PressedState{Not_Pressed,Hovering,Pressed,Released};


//Struct containing callbacks for when a widget highlighted by the controller
class WidgetSelectInfo{
    private:
    public:
    bool locked;
    Widget* next_up;
    Widget* next_down;
    Widget* next_left;
    Widget* next_right;
    void (*onHighlightEffect)(Widget*);
    void (*onStopHighlightEffect)(Widget*);
    bool (*onSelectedInput)(Widget* w, Input::Event event_type);
    
    WidgetSelectInfo();
    ~WidgetSelectInfo();
};

//Struct containing callbacks for when a widget is clicked
class WidgetClickInfo{
    public:
    PressedState state;
    void (*onHoverEffect)(Widget*);
    void (*onStopHoverEffect)(Widget*);
    void (*onClickEffect)(Widget*);
    void (*onClickReleaseEffect)(Widget*);
    void (*onClickAction)(Widget*);
    
    WidgetClickInfo();
    ~WidgetClickInfo();
};


class Widget{
    public:
    Layout                  layout;
    char*                   name;
    bool                    active;
    bool                    visible;
    WidgetSelectInfo*       selectable;
    WidgetClickInfo*        clickable;
    List<Sprite>            sprites;
    List<UI_Rect>           rects;
    List<UI_Text>           texts;

    Widget();
    Widget(char* widget_name);

    void Activate();
    void Deactivate();
    void Update(int frames);
    void Paint();
    bool HandleInput(Input::Event event_type);
    void HandleResize();
    bool HandleSignal(EventSignal signal);

    bool IsSelectable();

    virtual void OnUpdate();
    virtual bool OnInput(Input::Event event_type);
    virtual void OnResize();
    virtual bool OnSignal(EventSignal signal);
    virtual ~Widget();
};

}

#endif
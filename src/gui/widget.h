#ifndef WIDGET_H
#define WIDGET_H

#include "layout.h"
#include "../signal.h"
#include "../input.h"
#include "../struct/list.h"
#include "../gfx/ui_shape.h"
#include "../gfx/sprite.h"
#include "../gfx/text.h"

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
    char*                   name;
    Layout                  layout;
    bool                    active;
    bool                    visible;
    WidgetSelectInfo*       selectable;
    WidgetClickInfo*        clickable;
    List<Sprite>            sprites;
    List<UI_Rect>           rects;
    List<DrawableText>      texts;

    Widget();
    Widget(char* widget_name);

    void Activate();
    void Deactivate();
    void Update(int frames);
    void Paint();
    bool HandleInput(Input::Event event_type);
    void HandleResize();
    bool HandleSignal(EventSignal signal);

    void MoveTo(Widget* parent,VerticalOrigin vmode,HorizontalOrigin hmode, point_i offset);

    bool IsSelectable();

    virtual void OnUpdate();
    virtual bool OnInput(Input::Event event_type);
    virtual void OnMove();
    virtual void OnPaint();
    virtual void OnResize();
    virtual bool OnSignal(EventSignal signal);
    virtual ~Widget();
};

}

#endif
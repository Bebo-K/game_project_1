#ifndef COMPONENT_CLICKABLE_H
#define COMPONENT_CLICKABLE_H

#include "../widget.h"

namespace UI{
    
enum PressedState{Not_Pressed,Hovering,Pressed,Released};

class ClickableComponent: public WidgetComponent{
    public:
    PressedState state;
    void (*onHoverEffect)(Widget*);
    void (*onStopHoverEffect)(Widget*);
    void (*onClickEffect)(Widget*);
    void (*onClickReleaseEffect)(Widget*);
    void (*onClickAction)();
    
    ClickableComponent();
    ~ClickableComponent();

    void OnUpdate(Widget* w, int frames);
    bool OnInput(Widget* w, Input::Event event_type);


};


}



#endif
#ifndef BUTTON_H
#define BUTTON_H

#include "../widget.h"

namespace UI{
    
enum PressedState{Not_Pressed,Hovering,Pressed,Released};

class ButtonComponent: public WidgetComponent{
    public:
    PressedState state;
    void (*onClickCallback)();
    
    ButtonComponent(void (*callback)() );

    void OnUpdate(Widget* w, int frames);
    bool OnInput(Widget* w, Input::Event event_type);

};


}



#endif
#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include "../widget.h"

enum PressedState{Not_Pressed,Hovering,Pressed,Released};

class ButtonWidget: public Widget{
    public:
    PressedState state;
    void (*onClickCallback)();

    ButtonWidget(void (*callback)() );
    ~ButtonWidget();

    void OnUpdate(int frames);
    bool OnInput(Input::Event event_type);
};



#endif
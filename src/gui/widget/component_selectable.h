#ifndef COMPONENT_SELECTABLE_H
#define COMPONENT_SELECTABLE_H

#include "../widget.h"

namespace UI{
    
enum PressedState{Not_Pressed,Hovering,Pressed,Released};

class SelectableComponent: public WidgetComponent{
    private:
    bool just_highlighted;
    public:
    bool locked;
    bool highlighted;
    SelectableComponent* next_up;
    SelectableComponent* next_down;
    SelectableComponent* next_left;
    SelectableComponent* next_right;
    void (*onHighlightEffect)(Widget*);
    void (*onStopHighlightEffect)(Widget*);
    void (*onSelectEffect)(Widget*);
    void (*onSelectAction)();
    void (*onCancelEffect)(Widget*);
    void (*onCancelAction)();
    
    SelectableComponent();
    ~SelectableComponent();

    void OnUpdate(Widget* w, int frames);
    bool OnInput(Widget* w, Input::Event event_type);


};


}



#endif
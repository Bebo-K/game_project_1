#include "component_selectable.h"

using namespace UI;

void Action_NO_OP(){}
void Effect_NO_OP(Widget* w){}

SelectableComponent::SelectableComponent(){
    highlighted=false;
    locked=false;
    next_up=nullptr;
    next_down=nullptr;
    next_left=nullptr;
    next_right=nullptr;
    onHighlightEffect=Effect_NO_OP;
    onStopHighlightEffect=Effect_NO_OP;
    onSelectEffect=Effect_NO_OP;
    onSelectAction=Action_NO_OP;
    onCancelEffect=Effect_NO_OP;
    onCancelAction=Action_NO_OP;
}

SelectableComponent::~SelectableComponent(){}
void SelectableComponent::OnUpdate(Widget* w, int frames){
    if(just_highlighted){onHighlightEffect(w);just_highlighted=false;highlighted = true;}
}
bool SelectableComponent::OnInput(Widget* w, Input::Event event_type){
    if(!highlighted)return false;
    bool handled=false;
    if(event_type==Input::Move && !locked){
        Controller::Axis axis = Controller::GetAxis(Controller::Move);
        if(axis.IsUp() && next_up != nullptr){
            next_up->just_highlighted=true;
            highlighted=false;
            onStopHighlightEffect(w);
            handled=true;
        }
        if(axis.IsDown() && next_down != nullptr){
            next_down->just_highlighted=true;
            highlighted=false;
            onStopHighlightEffect(w);
            handled=true;
        }
        if(axis.IsLeft() && next_left != nullptr){
            next_left->just_highlighted=true;
            highlighted=false;
            onStopHighlightEffect(w);
            handled=true;
        }
        if(axis.IsRight() && next_right != nullptr){
            next_right->just_highlighted=true;
            highlighted=false;
            onStopHighlightEffect(w);
            handled=true;
        }
    }
    if(event_type==Input::A){
        onSelectEffect(w);
        onSelectAction();
        handled=true;
    }
    if(event_type==Input::B){
        onCancelEffect(w);
        onCancelAction();
        handled=true;
    }
    return handled;
}

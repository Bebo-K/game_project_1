#include "component_clickable.h"

using namespace UI;

void Action_NO_OP(){}
void Effect_NO_OP(Widget* w){}

ClickableComponent::ClickableComponent(){
    state=PressedState::Not_Pressed;
    onHoverEffect=Effect_NO_OP;
    onStopHoverEffect=Effect_NO_OP;
    onClickEffect=Effect_NO_OP;
    onClickReleaseEffect=Effect_NO_OP;
    onClickAction=Action_NO_OP;
}

ClickableComponent::~ClickableComponent(){}

void ClickableComponent::OnUpdate(Widget* w, int frames){
    if(state == Released){
        point_i cursor = Controller::GetPCCursor();
        rect_i layout_rect = w->layout.GetRect().to_integers();

        state = layout_rect.contains(cursor)?Hovering:Not_Pressed;
    } 
}

bool ClickableComponent::OnInput(Widget* w, Input::Event event_type){
    bool handled=false;
    if(event_type== Input::PC_Cursor||
        event_type== Input::PC_LClick){
        point_i cursor = Controller::GetPCCursor();
        rect_i layout_rect = w->layout.GetRect().to_integers();

        if(layout_rect.contains(cursor)){
            Controller::Button cursor = Controller::GetPCLeftMouse();
            if(cursor.IsDown()){
                if(cursor.IsJustPressed()){
                    onClickEffect(w);
                    state = Pressed;
                    handled=true;
                }
            }
            else{
                if(cursor.IsJustReleased()){
                    if(state == Pressed){
                        onClickReleaseEffect(w);
                        onClickAction();
                        state = Released;
                        handled=true;
                    }
                }
                else{
                    if(state != Hovering){onHoverEffect(w);}
                    state = Hovering;
                }
            }
        }
        else{
        if(state == Hovering){onStopHoverEffect(w);}
           state = Not_Pressed;
        }
    }
    return handled;
}

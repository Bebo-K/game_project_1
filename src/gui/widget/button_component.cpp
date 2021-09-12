#include "button.h"

using namespace UI;

ButtonComponent::ButtonComponent(void (*callback)() ){
    onClickCallback = callback;
}

void ButtonComponent::OnUpdate(Widget* w, int frames){
    if(state == Released){
        point_i cursor = Controller::GetPCCursor();
        rect_i layout_rect = w->layout.GetRect();

        state = layout_rect.contains(cursor)?Hovering:Not_Pressed;
    } 
}

bool ButtonComponent::OnInput(Widget* w, Input::Event event_type){
    bool handled=false;
    if(event_type== Input::PC_Cursor||
        event_type== Input::PC_LClick){
        point_i cursor = Controller::GetPCCursor();
        rect_i layout_rect = w->layout.GetRect();

        if(layout_rect.contains(cursor)){
            Controller::Button cursor = Controller::GetPCLeftMouse();

            if(cursor.IsDown()){
                if(cursor.IsJustPressed()){
                    state = Pressed;
                    handled=true;
                }
            }
            else{
                if(cursor.IsJustReleased()){
                    if(state == Pressed){
                        onClickCallback();
                        state = Released;
                        handled=true;
                    }
                }
                else{
                    state = Hovering;
                }
            }
        }
        else{
           state = Not_Pressed;
        }
    }
    return handled;
}

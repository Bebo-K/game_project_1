#include "button_widget.h"


ButtonWidget::ButtonWidget(void (*callback)() ){
    onClickCallback = callback;
}
ButtonWidget::~ButtonWidget(){}


void ButtonWidget::OnUpdate(int frames){
    if(state == Released){
        point_i cursor = Controller::GetPCCursor();
        rect_i layout_rect = {layout.X,layout.Y,layout.W,layout.H};

        state = layout_rect.contains(cursor)?Hovering:Not_Pressed;
    } 
}

bool ButtonWidget::OnInput(Input::Event event_type){
    bool handled=false;
    if(event_type== Input::PC_Cursor||
        event_type== Input::PC_LClick){
        point_i cursor = Controller::GetPCCursor();
        rect_i layout_rect = {layout.X,layout.Y,layout.W,layout.H};

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

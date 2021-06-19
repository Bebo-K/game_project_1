#include "button_widget.h"


ButtonWidget::ButtonWidget(void (*callback)() ){
    onClickCallback = callback;
}
ButtonWidget::~ButtonWidget(){}


void ButtonWidget::OnUpdate(int frames){
    if(state == Released){
        Input::Axis cursorAxis = Input::Cursor_Axis();
        point_i cursor_pos = {cursorAxis.raw_x,cursorAxis.raw_y};
        rect_i layout_rect = {layout.X,layout.Y,layout.W,layout.H};

        state = layout_rect.contains(cursor_pos)?Hovering:Not_Pressed;
    } 
}

bool ButtonWidget::OnInput(Input::EventID event_type){
    bool handled=false;
    if(event_type== Input::EventID::CursorSelect ||
        event_type== Input::EventID::CursorAxis){
        Input::Axis cursorAxis = Input::Cursor_Axis();
        point_i cursor_pos = {cursorAxis.raw_x,cursorAxis.raw_y};
        rect_i layout_rect = {layout.X,layout.Y,layout.W,layout.H};

        if(layout_rect.contains(cursor_pos)){
            Input::Button cursor = Input::Button_Cursor_Select();

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

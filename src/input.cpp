#include "input.h"
#include "config.h"
#include "log.h"
#include "structs/data_types.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>


bool Input::Button::IsDown(){return (state&1) > 0;}
bool Input::Button::IsUp(){return (state&1) == 0;}
bool Input::Button::IsJustPressed(){return state == 3;}
bool Input::Button::IsJustReleased(){return state == 2;}

const int button_count = 16;
const int button_input_start=3;
const char* button_names[button_count] = {
    "button_a",
    "button_b",
    "button_c",
    "button_d",
    "button_up",
    "button_down",
    "button_left",
    "button_right",
    "button_l1",      
    "button_l2",       
    "button_r1",         
    "button_r2",       
    "button_pause",
    "button_menu",
    "button_cursor",
    "button_toggle_console"
};

const int axis_count=4;
const int axis_binding_count = 16;//axis count * axis_directions
const char* axis_names[4] = {
    "move",
    "camera",
    "cursor",
    "scroll"
};

const char* axis_directions[4] = {
    "up",
    "down",
    "left",
    "right"
};


int                 key_bind_count;
Input::InputBinding* key_binds;

long            state_changed;
Input::Button   any_button;               
Input::Axis     axis_1;                     
Input::Axis     axis_2;                     
Input::Axis     cursor;                     
Input::Axis     scroll;                     
Input::Button   buttons[button_count]; 

text_char*  input_text=null;            
text_char   character_code[2]  = {0,0};

InputCode Input::NextInput(){
    long input_flag = 1;
    for(int i=0; i<32;i++){
        if((state_changed & input_flag) > 0){
            state_changed ^= input_flag;
            return input_flag;}
        input_flag = input_flag << 1;
    }
    return 0;
}

Input::Button Input::AnyButton(){return any_button;}
Input::Axis Input::Axis_1(){return axis_1;}
Input::Axis Input::Axis_2(){return axis_2;}
Input::Button Input::Button_A(){return buttons[0];}
Input::Button Input::Button_B(){return buttons[1];}
Input::Button Input::Button_C(){return buttons[2];}
Input::Button Input::Button_D(){return buttons[3];}
Input::Button Input::Button_Up(){return buttons[4];}
Input::Button Input::Button_Down(){return buttons[5];}
Input::Button Input::Button_Left(){return buttons[6];}
Input::Button Input::Button_Right(){return buttons[7];}
Input::Button Input::Button_L1(){return buttons[8];}
Input::Button Input::Button_L2(){return buttons[9];}
Input::Button Input::Button_R1(){return buttons[10];}
Input::Button Input::Button_R2(){return buttons[11];}
Input::Button Input::Button_Pause(){return buttons[12];}
Input::Button Input::Button_Menu(){return buttons[13];}
Input::Button Input::Cursor_Select(){return buttons[14];}
Input::Button Input::Button_ToggleConsole(){return buttons[15];}
text_char* Input::TextInput(){return input_text;}
Input::Axis Input::Cursor(){return cursor;}
Input::Axis Input::Scroll(){return scroll;}

void Input::Init(){
    state_changed=0;
    any_button.state=0;
    memset(&axis_1,0,sizeof(Axis));
    memset(&axis_2,0,sizeof(Axis));
    memset(&cursor,0,sizeof(Axis));
    memset(&scroll,0,sizeof(Axis));
    memset(&buttons,0,sizeof(Button)*button_count);
    input_text=null;

    key_bind_count=0;
    key_binds=null;
    character_code[0] = 0;
    character_code[1] = 0;

    LoadKeyBindings();
}

void Input::Destroy(){
    free(key_binds);key_binds=null;
}

void Input::LoadKeyBindings(){
    key_bind_count = config::key_binding_count;
    int *i=(int*)calloc(255,sizeof(int));
    key_binds = (InputBinding*)calloc(key_bind_count,sizeof(InputBinding));
    for(int i=0;i< config::key_binding_count;i++){
        StringPair binding = config::key_bindings[i];

        key_binds[i].input_id = GetKeyID(binding.value);
        key_binds[i].event_id = GetEventID(binding.key);
        logger::info("Assigning input %d to event %d\n",key_binds[i].input_id,key_binds[i].event_id);
    }
    free(i);
}

bool HandleAxisKeys(int key_id, bool down,Input::Axis& axis,int start){
    int prev_axis_x = axis.x;
    int prev_axis_y = axis.y;
    switch(key_id-start){
        case 0:/*INVALID: button cannot be assigned to axis*/
        case 1:/*up*/
            if(down){axis.y = 255;}
            if(!down && axis.y > 0){axis.y=0;}
        break;
        case 2:/*down*/
            if(down){axis.y = -255;}
            if(!down && axis.y < 0){axis.y=0;}
        break;
        case 3:/*left*/
            if(down){axis.x = -255;}
            if(!down && axis.x < 0){axis.x=0;}
        break;
        case 4:/*right*/
            if(down){axis.x = 255;}
            if(!down && axis.x > 0){axis.x=0;}
        break;
        default:return false;break;
    }
    axis.dx = axis.x - prev_axis_x;
    axis.dy = axis.y - prev_axis_y;
    return true;
}


int Input::BoundEvent(int input_id){
    for(int i=0;i<key_bind_count;i++){if(key_binds[i].input_id==input_id)return key_binds[i].event_id;}
    return 0;
}

void Input::HandleKey(int key_id, bool down){
    state_changed |= InputEvent::AnyButton;
    any_button.state=down?3:2;

    int event = BoundEvent(key_id);
    if(event <= 0)return;
    if(event < 16){
        int button_id = event-1;
        int button_mask = (InputEvent::Button_A << button_id);
        buttons[button_id].state = down?3:2;
        state_changed |= button_mask;
    }
    else if(event > 100){
        int axis_id = (event-100)/5;
        switch(axis_id){
            case 0:if(HandleAxisKeys(event,down,axis_1,100)){state_changed |= InputEvent::Axis_1;}
            case 1:if(HandleAxisKeys(event,down,axis_2,105)){state_changed |= InputEvent::Axis_2;}
            case 2:if(HandleAxisKeys(event,down,cursor,110)){state_changed |= InputEvent::Axis_Cursor;}
            case 3:if(HandleAxisKeys(event,down,scroll,115)){state_changed |= InputEvent::Axis_Scroll;}
            default:return;//invalid input axis?
        }
    }  
}

void Input::HandleCursor(int pos_x,int pos_y){
    cursor.dx = pos_x - cursor.x;
    cursor.dy = pos_y - cursor.y;
    cursor.x = pos_x;
    cursor.y = pos_y;
    state_changed |= InputEvent::Axis_Cursor;
}

bool SetAxis(Input::Axis& axis,int x,int y){
    int prev_axis_x = axis.x;
    int prev_axis_y = axis.y;

    axis.x = x;
    axis.y = y;
    axis.dx = axis.x - prev_axis_x;
    axis.dy = axis.y - prev_axis_y;
    if(axis.dx ==0 && axis.dy==0){return false;}
    return true;
}
void Input::HandleControlStick(int stick_id,float px, float py){
    int stick_input = BoundEvent(stick_id);

    int x = (int)(px*255.0f);
    int y = (int)(py*255.0f);

    switch(stick_input){
        case 100:if(SetAxis(axis_1,x,y)){state_changed |= InputEvent::Axis_1;}break;
        case 105:if(SetAxis(axis_2,x,y)){state_changed |= InputEvent::Axis_2;} break;
        case 110:if(SetAxis(cursor,x,y)){state_changed |= InputEvent::Axis_Cursor;} break;
        case 115:if(SetAxis(scroll,x,y)){state_changed |= InputEvent::Axis_Scroll;} break;
        default:break;
    }
}

int Input::GetEventID(char* event_name){
    int event_id=0;
    for(int i=0;i<axis_count;i++){
        if(cstr::starts_with(event_name,axis_names[i])){
            event_id = 100 + 5*i;
            for(int j=0; j < 4;j++){
                if(cstr::contains(event_name,axis_directions[j])){
                    event_id += 1+j;
                    break;
                }
            }
        }
    }
    if(event_id ==0){
        for(int i=0;i<button_count;i++){
            if(cstr::starts_with(button_names[i],event_name)){
                event_id = i+1;
            }
        }
    }

    return event_id;
}

void Input::HandleCharacter(int code_point){
    character_code[0]=code_point;
    input_text = character_code;
    state_changed |= InputEvent::Text;
}

void Input::ClearInputText(){
    state_changed ^= InputEvent::Text;//clear text event;
    character_code[0]=0;
    if(input_text != nullptr){
        if(input_text != character_code){free(input_text);}
        input_text = nullptr;
    }
}

//Prepares input for next frame. Call after gamestate/ui update.
void Input::PostUpdate(){
    if(any_button.state == 3){any_button.state=1;}
    if(any_button.state == 2){any_button.state=0;}

    axis_1.dx=0;axis_1.dy=0;
    axis_2.dx=0;axis_2.dy=0;
    cursor.dx=0;cursor.dy=0;
    scroll.dx=0;scroll.dy=0;

    ClearInputText();

    state_changed=0;

    for(int i=0;i< button_count;i++){
        if(buttons[i].state == 3){buttons[i].state=1;}
        if(buttons[i].state == 2){buttons[i].state=0;}
    }
}

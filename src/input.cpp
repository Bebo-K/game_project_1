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

const int button_count = 15;
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
    "button_cursor"
};

const int axis_mapping_count = 16;
const char* axis_mapping_names[axis_mapping_count] = {
    "up",
    "down",
    "left",
    "right",
    "camera_up",
    "camera_down",
    "camera_left",
    "camera_right",
    "cursor_up",
    "cursor_down",
    "cursor_left",
    "cursor_right",
    "scroll_up",
    "scroll_down",
    "scroll_left",
    "scroll_right",
};

int         key_mappings[button_count];
int         axis_mappings[axis_mapping_count];

Input::Button any_button;               bool any_button_changed;
Input::Axis axis_1;                     bool axis_1_changed;
Input::Axis axis_2;                     bool axis_2_changed;
Input::Axis cursor;                     bool cursor_changed;
Input::Axis scroll;                     bool scroll_changed;
Input::Button buttons[button_count];    bool button_changed[button_count];
text_char*  input_text=null;            bool input_text_changed;
text_char   character_code[2]  = {0,0};

bool Input::StateChanged(InputEvent event_type){
    switch (event_type){
    case InputEvent::AnyButton:return any_button_changed;break;
    case InputEvent::Axis_1:return axis_1_changed;break;
    case InputEvent::Axis_2:return axis_2_changed;break;
    case InputEvent::Button_A: return button_changed[0];break;
    case InputEvent::Button_B:return button_changed[1];break;
    case InputEvent::Button_C:return button_changed[2];break;
    case InputEvent::Button_D:return button_changed[3];break;
    case InputEvent::Button_Up: return button_changed[4];break;
    case InputEvent::Button_Down:return button_changed[5];break;
    case InputEvent::Button_Left:return button_changed[6];break;
    case InputEvent::Button_Right:return button_changed[7];break;
    case InputEvent::Button_L1: return button_changed[8];break;
    case InputEvent::Button_L2:return button_changed[9];break;
    case InputEvent::Button_R1:return button_changed[10];break;
    case InputEvent::Button_R2:return button_changed[11];break;
    case InputEvent::Button_Pause:return button_changed[12];break;
    case InputEvent::Button_Menu:return button_changed[13];break;
    case InputEvent::Text:return input_text_changed;break;
    case InputEvent::Cursor_Move:return cursor_changed;break;
    case InputEvent::Cursor_Select:return button_changed[14];break;
    case InputEvent::Scroll:return scroll_changed;break;
    
    default:return false;
    }
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
text_char* Input::TextInput(){return input_text;}
Input::Axis Input::Cursor(){return cursor;}
Input::Axis Input::Scroll(){return scroll;}
Input::Button Input::Cursor_Select(){return buttons[14];}

void Input::Init(){

    any_button.state=0;             any_button_changed=false;
    memset(&axis_1,0,sizeof(Axis)); axis_1_changed = false;
    memset(&axis_2,0,sizeof(Axis)); axis_2_changed = false;
    memset(&cursor,0,sizeof(Axis)); cursor_changed = false;
    memset(&scroll,0,sizeof(Axis)); scroll_changed = false;
    memset(&buttons,0,sizeof(Button)*button_count);
        for(int i=0;i<button_count;i++){button_changed[i]=false;}
    input_text=null;                input_text_changed=false;

    for(int i=0;i<button_count;i++){key_mappings[i]=-1;}
    for(int i=0;i<axis_mapping_count;i++){axis_mappings[i]=-1;}

    character_code[0] = 0;
    character_code[1] = 0;

    char* keyname;
    for(int i=0;i< button_count;i++){
        keyname = (char*)config::key_bindings.Get((char*)button_names[i]);
        if(keyname != null){
            int key_id = GetKeyID(keyname);
            if(key_id >= 0){
                key_mappings[i]=key_id;
            }
        }
    }
    for(int i=0;i< axis_mapping_count;i++){
        keyname = (char*)config::key_bindings.Get((char*)axis_mapping_names[i]);
        if(keyname != null){
            int key_id = GetKeyID(keyname);
            if(key_id >= 0){
                axis_mappings[i]=key_id;
            }
        }
    }
}

bool HandleAxisKeys(int axis_id, bool down,Input::Axis& axis,int start){
    int prev_axis_x = axis.x;
    int prev_axis_y = axis.y;
    switch(axis_id-start){
        case 0:/*up*/
            if(down){axis.y = 255;}
            if(!down && axis.y > 0){axis.y=0;}
        break;
        case 1:/*down*/
            if(down){axis.y = -255;}
            if(!down && axis.y < 0){axis.y=0;}
        break;
        case 2:/*left*/
            if(down){axis.x = -255;}
            if(!down && axis.x < 0){axis.x=0;}
        break;
        case 3:/*right*/
            if(down){axis.x = 255;}
            if(!down && axis.x > 0){axis.x=0;}
        break;
        default:return false;break;
    }
    axis.dx = axis.x - prev_axis_x;
    axis.dy = axis.y - prev_axis_y;
    return true;
}

void Input::HandleKey(int key_id, bool down){
    any_button_changed=true;
    any_button.state=down?3:2;
    for(int i=0;i<button_count;i++){
        if(key_mappings[i] == key_id){
            button_changed[i]=true;
            buttons[i].state=down?3:2;
        }
    }
    for(int i=0;i<axis_mapping_count;i++){
        if(axis_mappings[i] == key_id){
            if(HandleAxisKeys(i,down,axis_1,0)){axis_1_changed=true;}
            if(HandleAxisKeys(i,down,axis_2,4)){axis_2_changed=true;}
            if(HandleAxisKeys(i,down,cursor,8)){cursor_changed=true;}
            if(HandleAxisKeys(i,down,scroll,12)){scroll_changed=true;}
        }
    }    
}

void Input::HandleCursor(int pos_x,int pos_y){
    cursor.dx = pos_x - cursor.x;
    cursor.dy = pos_y - cursor.y;
    cursor.x = pos_x;
    cursor.y = pos_y;
    cursor_changed=true;
}

void Input::HandleCharacter(int code_point){
    character_code[0]=code_point;
    input_text = character_code;
    input_text_changed=true;
}

//Prepares input for next frame. Call after gamestate/ui update.
void Input::PostUpdate(){
    any_button_changed=false;
    if(any_button.state == 3){any_button.state=1;}
    if(any_button.state == 2){any_button.state=0;}

    axis_1.dx=0;axis_1.dy=0;axis_1_changed=false;
    axis_2.dx=0;axis_2.dy=0;axis_2_changed=false;
    cursor.dx=0;cursor.dy=0;cursor_changed=false;
    scroll.dx=0;scroll.dy=0;scroll_changed=false;

    input_text_changed=false;
    character_code[0]=0;
    if(input_text != nullptr){
        if(input_text != character_code){free(input_text);}
        input_text = nullptr;
    }

    for(int i=0;i< button_count;i++){
        button_changed[i]=false;
        if(buttons[i].state == 3){buttons[i].state=1;button_changed[i]=true;}
        if(buttons[i].state == 2){buttons[i].state=0;button_changed[i]=true;}
    }
}

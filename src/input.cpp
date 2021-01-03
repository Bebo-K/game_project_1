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


long                                    state_changed;
Input::Button any_button;               
Input::Axis axis_1;                     
Input::Axis axis_2;                     
Input::Axis cursor;                     
Input::Axis scroll;                     
Input::Button buttons[button_count];    
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
    state_changed |= InputEvent::AnyButton;
    any_button.state=down?3:2;
    for(int i=0;i<button_count;i++){
        if(key_mappings[i] == key_id){
            state_changed |= (1 << (i+button_input_start));
            buttons[i].state=down?3:2;
        }
    }
    for(int i=0;i<axis_mapping_count;i++){
        if(axis_mappings[i] == key_id){
            if(HandleAxisKeys(i,down,axis_1,0)){state_changed |= InputEvent::Axis_1;}
            if(HandleAxisKeys(i,down,axis_2,4)){state_changed |= InputEvent::Axis_2;}
            if(HandleAxisKeys(i,down,cursor,8)){state_changed |= InputEvent::Cursor_Move;}
            if(HandleAxisKeys(i,down,scroll,12)){state_changed |= InputEvent::Cursor_Scroll;}
        }
    }    
}

void Input::HandleCursor(int pos_x,int pos_y){
    cursor.dx = pos_x - cursor.x;
    cursor.dy = pos_y - cursor.y;
    cursor.x = pos_x;
    cursor.y = pos_y;
    state_changed |= InputEvent::Cursor_Move;
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

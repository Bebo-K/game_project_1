#include "input.h"
#include "config.h"
#include "log.h"
#include <stdlib.h>
#include "structs/data_types.h"


const int   button_count=15;
char*       button_names[button_count] = {
    "up",
    "down",
    "left",
    "right",
    "camera_up",
    "camera_down",
    "camera_left",
    "camera_right",
    "confirm",
    "cancel",
    "jump",
    "crouch",
    "center_cam",
    "pause",
    "inventory"
};
Controller::Button keys[button_count];
int         key_mappings[button_count];

bool        any_pressed;
vec2        joy_axis_1;
vec2        joy_axis_2;

int         cursor_x;
int         cursor_dx;
int         cursor_y;
int         cursor_dy;
int         scroll;

char*       paste_data;
int         character_code;

vec2        move_axis;
vec2        cam_axis;

void Input::Init(){
    char* keyname;
    for(int i=0;i< button_count;i++){
        keyname = (char*)config::key_bindings.StrGet(button_names[i]);
        if(keyname != null){
            int key_id = GetKeyID(keyname);
            if(key_id >= 0){
                key_mappings[i]=key_id;
            }
        }
    }
}

void HandleAxisButton(int updated_key_id, bool down,int target_key_id,float* axis){
    if(updated_key_id==target_key_id){
        if(down){*axis = 1.0f;}
        if(!down && *axis > 0.0f){*axis=0.0f;}
    }
}

void HandleAxisKeys(int key_id, bool down,vec2& axis,int start){
    switch(key_id-start){
        case 0:/*up*/
            if(down){axis.y = 1.0f;}
            if(!down && axis.y > 0.0f){axis.y=0.0f;}
        break;
        case 1:/*down*/
            if(down){axis.y = -1.0f;}
            if(!down && axis.y < 0.0f){axis.y=0.0f;}
        break;
        case 2:/*left*/
            if(down){axis.x = -1.0f;}
            if(!down && axis.x < 0.0f){axis.x=0.0f;}
        break;
        case 3:/*right*/
            if(down){axis.x = 1.0f;}
            if(!down && axis.x > 0.0f){axis.x=0.0f;}
        break;
        default:break;
    }
    if(axis.length_sqr() > 0){
        axis.normalize();
    }
}

void Input::HandleKey(int key_id, bool down){
    for(int i=0;i<button_count;i++){
        if(key_mappings[i] == key_id){
            keys[i].state=down?3:2;
            HandleAxisKeys(i,down,move_axis,0);
            HandleAxisKeys(i,down,cam_axis,4);
        }
    }
}

void Input::HandleCursor(int pos_x,int pos_y){
    cursor_dx = pos_x - cursor_x;
    cursor_dy = pos_y - cursor_y;
    cursor_x = pos_x;
    cursor_y = pos_y;
}

void Input::HandleCharacter(int code_point){
    character_code=code_point;
}

//Prepares input for next frame. Call after gamestate/ui update.
void Input::Update(){
    any_pressed=false;

    cursor_x += cursor_dx;
    cursor_dx = 0;
    cursor_y += cursor_dy;
    cursor_dy = 0;
    scroll = 0;

    if(paste_data != nullptr){free(paste_data); paste_data = nullptr;}
    character_code=0;

    for(int i=0;i< button_count;i++){
        if(keys[i].state == 3)keys[i].state=1;
        if(keys[i].state == 2)keys[i].state=0;
    }
}


bool Controller::AnyButtonDown(){return any_pressed;}
vec2 Controller::MoveAxis(){return move_axis;}
vec2 Controller::CameraAxis(){return cam_axis;}
Controller::Button Controller::Confirm(){return keys[8];}
Controller::Button Controller::Cancel(){return keys[9];}
Controller::Button Controller::Jump(){return keys[10];}
Controller::Button Controller::Crouch(){return keys[11];}
Controller::Button Controller::CenterCamera(){return keys[12];}
Controller::Button Controller::Pause(){return keys[13];}
Controller::Button Controller::Inventory(){return keys[14];}

bool Controller::Button::IsDown(){return (state&1) > 0;}
bool Controller::Button::IsUp(){return (state&1) == 0;}
bool Controller::Button::IsJustPressed(){return state == 3;}
bool Controller::Button::IsJustReleased(){return state == 2;}
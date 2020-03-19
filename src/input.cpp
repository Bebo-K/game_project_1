#include "input.h"
#include "config.h"
#include "log.h"
#include <stdlib.h>
#include "data_structs.h"


const int   button_count=11;
char*       button_names[button_count] = {
    "up",
    "down",
    "left",
    "right",
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

void Input::HandleKey(int key_id, bool down){
    if(down){
        logger::info("%s\n", GetKeyName(key_id));
        for(int i=0;i<button_count;i++){
            if(key_mappings[i] == key_id){
                logger::info(" -> %s\n", button_names[i]);
            }
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

void Input::HandleJoystick(int stick_id,float tilt_x,float tilt_y){
    if(stick_id==0){
        //TODO
    }
    else if(stick_id==1){
        //TODO
    }
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
Controller::Button Controller::Confirm(){return keys[4];}
Controller::Button Controller::Cancel(){return keys[5];}
Controller::Button Controller::Jump(){return keys[6];}
Controller::Button Controller::Crouch(){return keys[7];}
Controller::Button Controller::CenterCamera(){return keys[8];}
Controller::Button Controller::Pause(){return keys[9];}
Controller::Button Controller::Inventory(){return keys[10];}
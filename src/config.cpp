#include "config.h"



int config::window_height = 720;
int config::window_width = 1280;
 
int config::ui_height =  360;//
int config::ui_width = 640;//
AssociativeArray config::key_bindings(8);

void config::Init(){
    key_bindings.Add((byte*)"up",(byte*)"w");
    key_bindings.Add((byte*)"left",(byte*)"a");
    key_bindings.Add((byte*)"down",(byte*)"s");
    key_bindings.Add((byte*)"right",(byte*)"d");
    key_bindings.Add((byte*)"camera_up",(byte*)"u");
    key_bindings.Add((byte*)"camera_left",(byte*)"q");
    key_bindings.Add((byte*)"camera_down",(byte*)"j");
    key_bindings.Add((byte*)"camera_right",(byte*)"e");
    key_bindings.Add((byte*)"confirm",(byte*)"n");
    key_bindings.Add((byte*)"cancel",(byte*)"m");
    key_bindings.Add((byte*)"jump",(byte*)"space");
    key_bindings.Add((byte*)"crouch",(byte*)"shift");
    key_bindings.Add((byte*)"center_cam",(byte*)"q");
    key_bindings.Add((byte*)"pause",(byte*)"enter");
    key_bindings.Add((byte*)"inventory",(byte*)"tab");
}

void config::LoadFile(char* filename){


    return;
}
#include "config.h"



int config::window_height = 720;
int config::window_width = 1280;
 
int config::ui_height = 720;// 360;//
int config::ui_width = 1280;//640;//
StringMap config::key_bindings(8);

void config::Init(){
    key_bindings.Add("up",(byte*)"w");
    key_bindings.Add("left",(byte*)"a");
    key_bindings.Add("down",(byte*)"s");
    key_bindings.Add("right",(byte*)"d");
    key_bindings.Add("camera_up",(byte*)"u");
    key_bindings.Add("camera_left",(byte*)"q");
    key_bindings.Add("camera_down",(byte*)"j");
    key_bindings.Add("camera_right",(byte*)"e");
    key_bindings.Add("confirm",(byte*)"n");
    key_bindings.Add("cancel",(byte*)"m");
    key_bindings.Add("jump",(byte*)"space");
    key_bindings.Add("crouch",(byte*)"shift");
    key_bindings.Add("center_cam",(byte*)"q");
    key_bindings.Add("pause",(byte*)"enter");
    key_bindings.Add("inventory",(byte*)"tab");
}

void config::LoadFile(char* filename){


    return;
}
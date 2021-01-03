#include "config.h"



int config::window_height = 720;
int config::window_width = 1280;
 
int config::ui_height = 720;// 360;//
int config::ui_width = 1280;//640;//
StringMap config::key_bindings(8);

char config::show_console;
char config::show_fps_counter;

void config::Init(){
    key_bindings.Add("up",(byte*)"w");
    key_bindings.Add("left",(byte*)"a");
    key_bindings.Add("down",(byte*)"s");
    key_bindings.Add("right",(byte*)"d");
    key_bindings.Add("button_a",(byte*)"n");
    key_bindings.Add("button_b",(byte*)"m");
    key_bindings.Add("button_c",(byte*)"j");
    key_bindings.Add("button_d",(byte*)"k");
    key_bindings.Add("button_l1",(byte*)"q");
    key_bindings.Add("button_r1",(byte*)"e");
    key_bindings.Add("pause",(byte*)"enter");
    key_bindings.Add("menu",(byte*)"tab");
    key_bindings.Add("button_toggle_console",(byte*)"p");
    show_console=false;
    show_fps_counter=true;
}

void config::LoadFile(char* filename){


    return;
}
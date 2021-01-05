#include "config.h"
#include <stdlib.h>


int config::window_height = 720;
int config::window_width = 1280;
 
int config::ui_height = 720;// 360;//
int config::ui_width = 1280;//640;//

int config::key_binding_count = 0;
StringPair* config::key_bindings = null;

char config::show_console;
char config::show_fps_counter;

void config::Init(){
    key_binding_count = 26;
    key_bindings = (StringPair*)calloc(key_binding_count,sizeof(StringPair));
        key_bindings[0].key="move.up";key_bindings[0].value="w";
        key_bindings[1].key="move.down";key_bindings[1].value="s";
        key_bindings[2].key="move.left";key_bindings[2].value="a";
        key_bindings[3].key="move.right";key_bindings[3].value="d";

        key_bindings[4].key="button_a";key_bindings[4].value="n";
        key_bindings[5].key="button_b";key_bindings[5].value="m";
        key_bindings[6].key="button_c";key_bindings[6].value="j";
        key_bindings[7].key="button_d";key_bindings[7].value="k";

        key_bindings[8].key="button_l1";key_bindings[8].value="q";
        key_bindings[9].key="button_r1";key_bindings[9].value="e";

        key_bindings[10].key="pause";key_bindings[10].value="enter";
        key_bindings[11].key="menu";key_bindings[11].value="tab";
        
        key_bindings[12].key="button_toggle_console";key_bindings[12].value="p";

        
        key_bindings[13].key="move";key_bindings[13].value="gamepad_lstick";
        key_bindings[14].key="camera";key_bindings[14].value="gamepad_rstick";

        key_bindings[15].key="move.up";key_bindings[15].value="gamepad_d_up";
        key_bindings[16].key="move.down";key_bindings[16].value="gamepad_d_down";
        key_bindings[17].key="move.left";key_bindings[17].value="gamepad_d_left";
        key_bindings[18].key="move.right";key_bindings[18].value="gamepad_d_right";

        key_bindings[19].key="button_a";key_bindings[19].value="gamepad_a";
        key_bindings[20].key="button_b";key_bindings[20].value="gamepad_b";
        key_bindings[21].key="button_c";key_bindings[21].value="gamepad_x";
        key_bindings[22].key="button_d";key_bindings[22].value="gamepad_y";

        key_bindings[23].key="button_l1";key_bindings[23].value="gamepad_lshoulder";
        key_bindings[24].key="button_r1";key_bindings[24].value="gamepad_rshoulder";

        key_bindings[25].key="pause";key_bindings[25].value="gamepad_start";
        key_bindings[26].key="menu";key_bindings[26].value="gamepad_back";


    show_console=false;
    show_fps_counter=true;
}

void config::Destroy(){
    free(config::key_bindings);
}

void config::LoadFile(char* filename){


    return;
}
#ifndef CONFIG_H
#define CONFIG_H

#include "structs/data_types.h"



struct StringPair{
    char* key;
    char* value;
};

namespace config{

    extern int window_width;
    extern int window_height;
    extern int ui_width;
    extern int ui_height;
    extern int key_binding_count;
    extern StringPair* key_bindings;
    extern char show_console;
    extern char show_fps_counter;

    void Init();
    void LoadFile(char* filename);
    void LoadKeyLayout(char* layoutfile);
    void Destroy();
};


#endif
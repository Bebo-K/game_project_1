#ifndef CONFIG_H
#define CONFIG_H

#include "structs/data_types.h"

namespace config{

    extern int window_width;
    extern int window_height;
    extern int ui_width;
    extern int ui_height;
    extern StringMap key_bindings;

    void Init();
    void LoadFile(char* filename);
};


#endif
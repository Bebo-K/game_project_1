#ifndef CONFIG_H
#define CONFIG_H

#include "data_structs.h"

namespace config{

    extern int window_width;
    extern int window_height;
    extern int ui_width;
    extern int ui_height;
    extern AssociativeArray key_bindings;

    void Init();
    void LoadFile(char* filename);
};


#endif
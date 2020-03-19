#ifndef CONFIG_H
#define CONFIG_H

#include "data_structs.h"

class config{
    public:

    static int window_width;
    static int window_height;
    static int ui_width;
    static int ui_height;
    static AssociativeArray key_bindings;

    static void    LoadFile(char* filename);
};


#endif
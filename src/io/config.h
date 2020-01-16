#ifndef CONFIG_H
#define CONFIG_H

class config{
    public:

    static int window_width;
    static int window_height;
    static int ui_width;
    static int ui_height;
    

    static void    LoadFile(char* filename);
};


#endif
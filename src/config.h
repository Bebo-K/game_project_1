#ifndef CONFIG_H
#define CONFIG_H

#include "struct/data_types.h"



struct StringPair{
    char* key;
    char* value;
};

class ConfigMap{
    private:
    int         entry_slots;
    public:
    StringPair* entries;
    int         entry_count;

    ConfigMap();
    ~ConfigMap();

    void Add(char* key, char* value);
    char* Get(char* key);
    void Set(char* key, char* value);
    void Remove(char* key);
    void LoadFromFile(char* filename);
    void LoadFromTextArray(char** lines,int line_count);

    StringPair* begin();
    StringPair* end();
};

namespace config{
    enum ConfigID{
        id_window_height=0,
        id_window_width=1,
        id_ui_height=2,
        id_ui_width=3,
        id_show_console=4,
        id_show_fps_counter=5,
        id_debug_mode=6
    };

    extern int window_width;
    extern int window_height;
    extern int ui_width;
    extern int ui_height;
    extern int key_binding_count;
    extern char show_console;
    extern char show_fps_counter;
    extern char debug_mode;

    void Init();
    void LoadFile(char* filename);
    void LoadKeyLayout(char* layoutfile);
    void Destroy();
};


#endif
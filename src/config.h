#ifndef CONFIG_H
#define CONFIG_H

#include "struct/list.h"





namespace config{
    enum ConfigEntryType{INT,FLOAT,BOOL,STRING,WSTRING};

    extern int window_height;
    extern int window_width;
    extern int ui_height;
    extern int ui_width;
    extern int network_resend_max;
    extern int network_resend_interval;
    extern int network_ping_interval;
    extern int network_packet_expiry;
    extern int network_timeout;
    extern bool show_console;
    extern bool show_fps_counter;
    extern bool debug_mode;
    extern bool debug_net;
    extern wchar_t* save_directory;

    extern int key_binding_count;


    void Init();
    void SetConfig(char* name,char* value);
    void LoadFile(char* filename);
    void LoadKeyLayout(char* layoutfile);
    void Destroy();
};

namespace server_config{
    extern char* save_name;
    extern int   player_count;
    extern unsigned short default_port;
}


struct ConfigEntry{
    char* name;
    config::ConfigEntryType type;
    void* primitive;


    ConfigEntry(char* entry_name,config::ConfigEntryType entry_type,void* entry_data);
    ~ConfigEntry();
};


extern List<ConfigEntry> config_entries;

#endif
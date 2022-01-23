#include "config.h"
#include "struct/data_types.h"
#include "struct/str.h"
#include "io/file.h"
#include "os.h"
#include <stdlib.h>


List<ConfigEntry> global_config_entries(10);

int config::window_width = 1280;
int config::window_height = 720;
int config::network_resend_max = 64;
int config::network_resend_interval = 100;
int config::network_ping_interval   = 2000;
int config::network_packet_expiry   = 10000;//10 s
int config::network_timeout         = 30000;

bool config::show_console=false;
bool config::show_fps_counter=false;
bool config::debug_mode=false;
bool config::debug_net=true;

wchar_t* config::save_directory=L"/saves";

char* server_config::save_name="server";
int server_config::player_count=1;
unsigned short server_config::default_port=8380;


ConfigEntry::ConfigEntry(char* entry_name,config::ConfigEntryType entry_type,void* entry_data){
    name = cstr::new_copy(entry_name);
    type = entry_type;
    primitive = entry_data;
}
ConfigEntry::~ConfigEntry(){
    free(name);name=nullptr;
    primitive=nullptr;
}

void config::Init(){
    if(build_game_folder_path()){save_directory = get_games_folder_path();}
    global_config_entries.Add(new ConfigEntry("window_width",INT,&window_width));
    global_config_entries.Add(new ConfigEntry("window_height",INT,&window_height));
    global_config_entries.Add(new ConfigEntry("show_console",BOOL,&show_console));
    global_config_entries.Add(new ConfigEntry("show_fps_counter",BOOL,&show_fps_counter));
    global_config_entries.Add(new ConfigEntry("debug_mode",BOOL,&debug_mode));
    global_config_entries.Add(new ConfigEntry("debug_net",BOOL,&debug_net));
    global_config_entries.Add(new ConfigEntry("save_directory",WSTRING,&save_directory));
    global_config_entries.Add(new ConfigEntry("network_resend_max",INT,&network_resend_max));
    global_config_entries.Add(new ConfigEntry("network_resend_interval",INT,&network_resend_interval));
    global_config_entries.Add(new ConfigEntry("network_ping_interval",INT,&network_ping_interval));
    global_config_entries.Add(new ConfigEntry("network_packet_expiry",INT,&network_packet_expiry));
    global_config_entries.Add(new ConfigEntry("network_timeout",INT,&network_timeout));
}

void config::Destroy(){}

void config::SetConfig(char* name,char* value){
    for(ConfigEntry* entry :global_config_entries){
        if(cstr::compare(entry->name,name)){
            switch (entry->type)
            {
            case INT:*((int*)entry->primitive) = cstr::integer_from_string(value);break;
            case FLOAT:*((float*)entry->primitive) = cstr::float_from_string(value);break;
            case BOOL:*((bool*)entry->primitive) = cstr::bool_from_string(value);break;
            case STRING:*((char**)entry->primitive) = cstr::new_copy(value);break;
            case WSTRING:*((wchar_t**)entry->primitive) = wstr::from_cstr(value);break;
            default:break;
            }
        }
    }
}

void config::LoadFile(char* filename){
    FileReader config_file(filename);

    for(byte* line=config_file.ReadLine();line != null;line=config_file.ReadLine()){
        char* key = cstr::substr_before((char*)line,'=');
        char* value = cstr::substr_after((char*)line,'=');
        if(key!=nullptr && value!=nullptr){SetConfig(key,value);}
        free(key);
        free(value);
    }
}



    



#include <game_project_1/config.hpp>
#include <game_project_1/types/data_types.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/io/file.hpp>
#include <game_project_1/os.hpp>
#include <stdlib.h>


List<ConfigEntry> global_config_entries(10);



int config::framerate = 60;
int config::max_drawrate = 60;
int config::tickrate = 30;
float config::frame_interval = 1.0f/60;
float config::draw_interval = 1.0f/60;
float config::tick_interval = 1.0f/30;

int config::window_width = 1280;
int config::window_height = 720;
int config::network_resend_max = 64;
int config::network_resend_interval = 100;
int config::network_ping_interval   = 2000;
int config::network_packet_expiry   = 10000;//10 s
int config::network_timeout         = 30000;

bool config::show_console=false;
bool config::show_fps_counter=false;
bool config::debug_mode=true;
bool config::debug_net=true;

wchar_t* config::save_directory=L"/saves";
wchar_t* config::player_persona=L"Chowzang";


wchar* server_config::server_name=L"my server";
wchar* server_config::server_description=L"a server";
char* server_config::save_name="server";
bool server_config::local_only=true;
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
    if(OS::BuildGameFolderPath()){save_directory = OS::GetGameFolderPath();}
    global_config_entries.Add(new ConfigEntry("update_rate",INT,&framerate));
    global_config_entries.Add(new ConfigEntry("framerate",INT,&max_drawrate));
    global_config_entries.Add(new ConfigEntry("tickrate",INT,&tickrate));
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
    global_config_entries.Add(new ConfigEntry("player_persona",WSTRING,&player_persona));
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
            if(cstr::compare(entry->name,"update_rate")){frame_interval = 1.0f/framerate;}
            if(cstr::compare(entry->name,"framerate")){draw_interval = 1.0f/max_drawrate;}
            if(cstr::compare(entry->name,"tickrate")){tick_interval = 1.0f/tickrate;}
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



    



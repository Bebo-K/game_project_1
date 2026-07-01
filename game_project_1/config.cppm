export module Config;

import String;
import Memory;
import Primitives;
import File;
import OS;
import List;
//import <stdlib>;


typedef byte ConfigEntryType;
constexpr ConfigEntryType INT=0;
constexpr ConfigEntryType FLOAT=0;
constexpr ConfigEntryType BOOL=0;
constexpr ConfigEntryType STRING=0;
constexpr ConfigEntryType WSTRING=0;
constexpr ConfigEntryType CSTRING=0;

namespace config{
    struct ConfigEntry{
        char* name;
        ConfigEntryType type;
        void* primitive;

        ConfigEntry(const char* entry_name,ConfigEntryType entry_type,void* entry_data){
            name = cstr::new_copy(entry_name);
            type = entry_type;
            primitive = entry_data;
        }
        ~ConfigEntry(){
            Free(name);name=nullptr;
            primitive=nullptr;
        }
    };
    List<ConfigEntry> global_config_entries(10);

    export int framerate = 60;
    export int tickrate = 60;
    export float frame_interval = 1.0f/60;
    export float tick_interval = 1.0f/60;

    export int window_width = 1280;
    export int window_height = 720;
    export int network_resend_max = 64;
    export int network_resend_interval = 100;
    export int network_ping_interval   = 2000;
    export int network_packet_expiry   = 10000;//10 s
    export int network_timeout         = 30000;

    export bool enable_dev_console=true;
    export bool show_console=false;
    export bool show_fps_counter=false;
    export bool debug_mode=true;
    export bool debug_net=true;

    export wchar* save_directory= Translate("/saves");
    export wchar* player_persona= Translate("Chowzang");

    export char* key_layout_filename = "dat/misc/en_us_keyids.txt";

    export int key_binding_count;


    export void Init(){
        if(OS::BuildGameFolderPath()){save_directory = OS::GetGameFolderPath();}
        global_config_entries.Add(new ConfigEntry("update_rate",INT,&tickrate));
        global_config_entries.Add(new ConfigEntry("framerate",INT,&framerate));
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
        global_config_entries.Add(new ConfigEntry("key_layout_filename",CSTRING,&key_layout_filename));
    }

    export void Destroy(){}

    export void SetConfig(char* name,char* value){
        for(ConfigEntry* entry :global_config_entries){
            if(cstr::compare(entry->name,name)){
                switch (entry->type)
                {
                case INT:*static_cast<int *>(entry->primitive) = cstr::integer_from_string(value);break;
                case FLOAT:*static_cast<float *>(entry->primitive) = cstr::float_from_string(value);break;
                case BOOL:*static_cast<bool *>(entry->primitive) = cstr::bool_from_string(value);break;
                case STRING:*static_cast<char **>(entry->primitive) = cstr::new_copy(value);break;
                case WSTRING:*static_cast<wchar_t **>(entry->primitive) = wstr::from_cstr(value);break;
                case CSTRING:*static_cast<char *>(entry->primitive) = cstr::new_copy(value);break;
                default:break;
                }
                if(cstr::compare(entry->name,"update_rate")){tick_interval = 1.0f/tickrate;}
                if(cstr::compare(entry->name,"framerate")){frame_interval = 1.0f/framerate;}
            }
        }
    }

    export void LoadFile(char* filename){
        FileReader config_file(filename);

        for(byte* line=config_file.ReadLine();line != null;line=config_file.ReadLine()){
            char* key = cstr::substr_before((char*)line,'=');
            char* value = cstr::substr_after((char*)line,'=');
            if(key!=nullptr && value!=nullptr){SetConfig(key,value);}
            Free(key);
            Free(value);
        }
    }

};

namespace server_config{
    export wchar* server_name = Translate("my server");
    export wchar* server_description = Translate("a server");
    export char* save_name= TranslateC("server");
    export bool local_only=true;
    export int player_count=1;
    export unsigned short default_port=8380;
}



















    



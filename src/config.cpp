#include "config.h"
#include "io/file.h"
#include <stdlib.h>

int config_key_count = 7;
char* config_keys[] = {
    "window_height",
    "window_width",
    "ui_height",
    "ui_width",
    "show_console",
    "show_fps_counter",
    "debug_mode"
};

char* boolean_values[] = {"false","true"};


int config::window_height = 720;
int config::window_width = 1280;
 
int config::ui_height = 720;// 360;//
int config::ui_width = 1280;//640;//

char config::show_console;
char config::show_fps_counter;
char config::debug_mode;


ConfigMap::ConfigMap(){
    entry_count=0;
    entry_slots=1;
    entries = (StringPair*)calloc(entry_slots,sizeof(StringPair));
}
ConfigMap::~ConfigMap(){
    for(int i=0;i<entry_count;i++){
        if(entries[i].key != nullptr){free(entries[i].key);}
        if(entries[i].value != nullptr){free(entries[i].value);}
    }
    free(entries);
}

void ConfigMap::Add(char* key, char* value){
    if(entry_count < entry_slots){
        entries[entry_count].key= cstr::new_copy(key);
        entries[entry_count].value= cstr::new_copy(value);
    }
    else{
        StringPair* new_entries = (StringPair*)calloc(entry_slots*2,sizeof(StringPair));
        memcpy(new_entries,entries,sizeof(StringPair)*entry_slots);
        free(entries);
        entries=new_entries;
        entry_slots*=2;
        entries[entry_count].key= cstr::new_copy(key);
        entries[entry_count].value= cstr::new_copy(value);
    }
    entry_count++;
}
char* ConfigMap::Get(char* key){
    for(int i=0;i<entry_count;i++){
        if(entries[i].key != nullptr && cstr::compare(entries[i].key,key)){
            return entries[i].value;
        }
    }
    return null;
}
void ConfigMap::Set(char* key, char* value){
    for(int i=0;i<entry_count;i++){
        if(entries[i].key != nullptr && cstr::compare(entries[i].key,key)){
            if(entries[i].value)free(entries[i].value);
            entries[i].value=cstr::new_copy(value);
        }
    }
}
void ConfigMap::Remove(char* key){
    for(int i=0;i<entry_count;i++){
        if(entries[i].key != nullptr && cstr::compare(entries[i].key,key)){
            if(i < entry_count){
                if(entries[i].key)free(entries[i].key);
                if(entries[i].value)free(entries[i].value);
                memcpy(&entries[i],&entries[entry_count-1],sizeof(StringPair));
                entry_count--;
            }
            else if(entry_count>0){
                entry_count--;
                if(entries[i].key)free(entries[i].key);
                if(entries[i].value)free(entries[i].value);
            }
        }
    }
}
void ConfigMap::LoadFromFile(char* filename){
    FileReader layout_file(filename);

    for(byte* line=layout_file.ReadLine();line != null;line=layout_file.ReadLine()){
        char* key = cstr::substr_before((char*)line,'=');
        char* value = cstr::substr_after((char*)line,'=');
        if(key!=nullptr && value!=nullptr && Get(key)==nullptr){
            Add(key,value);
        }
        free(key);
        free(value);
    }
}
void ConfigMap::LoadFromTextArray(char** lines,int line_count){
    for(int i=0;i<line_count;i++){
        char* line = lines[i];
        char* key = cstr::substr_before((char*)line,'=');
        char* value = cstr::substr_after((char*)line,'=');
        if(key!=nullptr && value!=nullptr && Get(key)==nullptr){
            Add(key,value);
        }
        free(key);
        free(value);
    }
}

StringPair* ConfigMap::begin(){return entries;}
StringPair* ConfigMap::end(){return &entries[entry_count];}

void config::Init(){
    show_console=false;
    show_fps_counter=true;
    debug_mode=false;
}

void config::Destroy(){}

void config::LoadFile(char* filename){
    ConfigMap configFile;
    configFile.LoadFromFile(filename);

    int config_key_id=-1;
    for(StringPair entry:configFile){
        config_key_id=-1;
        for(int i=0;i<config_key_count;i++){
            if(cstr::compare(entry.key,config_keys[i])){config_key_id=i;}
        }
        switch (config_key_id)
        {
            case config::id_window_height: window_height = cstr::integer_from_string(entry.value);
            case config::id_window_width: window_width = cstr::integer_from_string(entry.value);
            case config::id_ui_height: ui_height = cstr::integer_from_string(entry.value);
            case config::id_ui_width: ui_width = cstr::integer_from_string(entry.value);
            case config::id_show_console: show_console = cstr::bool_from_string(entry.value);
            case config::id_show_fps_counter: show_fps_counter = cstr::bool_from_string(entry.value);
            case config::id_debug_mode: debug_mode = cstr::bool_from_string(entry.value);
            default:break;
        }
    }
}


    



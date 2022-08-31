#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <game_project_1/types/map.hpp>


namespace Areas{
    const int save_start_id=1;
    const int save_start_entrance=0;
    const int error_room=1;
    const int error_entrance=0;

    extern char* error_room_map;
    extern Map<int,char*> area_map_names;

    char* GetMapName(int area_id);
    void RegisterMapName(int area_id,char* map_name);
};



#endif
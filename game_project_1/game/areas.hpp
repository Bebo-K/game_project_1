#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <game_project_1/types/map.hpp>

namespace Areas{
    extern Map<int,char*> map_names;
    const int error_room =0;

    bool ValidArea(int area_id);
    char* GetMap(int area_id);
    char* ErrorRoom();
    void RegisterMap(int area_id,char* filename);
};





#endif
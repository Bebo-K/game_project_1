#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <stdlib.h>
#include <game_project_1/types/str.hpp>

struct PlayerInfo{
    wchar* player_name;
    wchar* character_name;
    int    player_entity_id;
    int    player_entity_scene;
    bool   connected;

    PlayerInfo();
    void Clear();
};






#endif

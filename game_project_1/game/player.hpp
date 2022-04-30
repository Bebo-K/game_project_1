#ifndef PLAYER_H
#define PLAYER_H

#include <game_project_1/types/str.hpp>

struct Player{
    int    state; const static int DISCONNECTED=0,CONNECTING=1,LOADING_INTO_ZONE=2,PLAYING=3,AFK=4,DISCONNECTING=-1;
    wchar* name;
    wchar* character_name;
    int    save_id;//ID of PlayerSave object
    int    save_entity_id;//Global ID of SaveEntity object
    int    entity_id;//Scene-wide ID of ingame entity if it exists (0 otherwise)
    int    entity_scene;//Area ID of ingame entity
    
    Player();
    ~Player();

    void Clear();
};





#endif
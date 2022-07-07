#ifndef PLAYER_H
#define PLAYER_H

#include <game_project_1/types/str.hpp>
#include <game_project_1/game/savefile.hpp>

struct Player{
    wchar*      persona;
    wchar*      character_name;
    int         entity_id;
    int         entity_scene;
    SavePlayer* save;
    
    Player();
    ~Player();

    bool Active();
    void Clear();
};





#endif
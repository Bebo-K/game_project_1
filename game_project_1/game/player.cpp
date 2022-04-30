#include <stdlib.h>
#include <game_project_1/game/player.hpp>


Player::Player(){
    state = DISCONNECTED;
    name = nullptr;
    character_name = nullptr;
    save_id=0;
    save_entity_id=0;
    entity_id=0;
    entity_scene=0;
}
Player::~Player(){
    Clear();
}

void Player::Clear(){
    state = DISCONNECTED;
    if(name != nullptr){free(name); name = nullptr;}
    if(character_name != nullptr){free(character_name); character_name = nullptr;}
    save_id=0;
    save_entity_id=0;
    entity_id=0;
    entity_scene=0;
}
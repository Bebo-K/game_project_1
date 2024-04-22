#include <stdlib.h>
#include <game_project_1/core/player.hpp>


Player::Player(){
    persona = nullptr;
    character_name = nullptr;
    save=nullptr;
    entity_id=0;
    entity_scene=0;
}
Player::~Player(){
    Clear();
}

bool Player::Active(){
    return (entity_id != 0 && entity_scene != 0);
}

void Player::Clear(){
    DEALLOCATE(persona)
    DEALLOCATE(character_name)
    save=nullptr;
    entity_id=0;
    entity_scene=0;
}
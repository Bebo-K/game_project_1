#include "player_info.h"



PlayerInfo::PlayerInfo(){
        player_name=nullptr;
        character_name=nullptr;
        player_entity_id=-1;
        player_entity_scene=-1;
}
void PlayerInfo::Clear(){
    if(player_name!=nullptr){free(player_name);player_name=nullptr;}
    if(character_name!=nullptr){free(character_name);character_name=nullptr;}
    player_entity_id=-1;
    player_entity_scene=-1;
}
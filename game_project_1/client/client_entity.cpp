#include <game_project_1/client/client_entity.hpp>

ClientEntity::ClientEntity(){
    id =0;
    entity_class_id =0;
    name=nullptr;
    x=y=z=0;
    velocity={0,0,0};
    scale ={1,1,1};
    rotation = {0,0,0};

    
    models=null;
    sprites=null;
    phys_data=null;
    colliders=null;
    movement=null;
    player_data=null;
    camera_target=null;
    state=null;
    anim_state=null;
    unit_data=null;
    npc_data=null;
}
ClientEntity::~ClientEntity(){
    if(name != nullptr){free(name);name=nullptr;}
    if(models != nullptr){delete models;models=nullptr;}
    if(sprites != nullptr){delete sprites;sprites=nullptr;}
    if(phys_data != nullptr){delete phys_data;phys_data=nullptr;}
    if(colliders != nullptr){delete colliders;colliders=nullptr;}
    if(movement != nullptr){delete movement;movement=nullptr;}
    if(state != nullptr){delete state;state=nullptr;}
    //if(animations != nullptr){delete animations;animations=nullptr;}
    if(player_data != nullptr){delete player_data;player_data=nullptr;}
    if(camera_target != nullptr){delete camera_target;camera_target=nullptr;}
    if(unit_data != nullptr){delete unit_data;unit_data=nullptr;}
    if(npc_data != nullptr){delete  npc_data;npc_data=nullptr;}
}
vec3 ClientEntity::GetPos(){return {x,y,z};}


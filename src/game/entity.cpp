#include "entity.h"
#include <stdlib.h>


Entity::Entity(){
    eid =0;
    type =0;
    name=nullptr;
    x=y=z=0;
    velocity={0,0,0};
    scale ={1,1,1};
    rotation = {0,0,0,1};

    
    models=nullptr;
    sprites=nullptr;
    colliders=nullptr;
    movement=nullptr;
    state=nullptr;
    //animations=nullptr;
    player_data=nullptr;
    camera_target=nullptr;
    unit_data=nullptr;
    npc_data=nullptr;
}

void Entity::SetName(char* name){
    name = cstr::new_copy(name);
}

vec3 Entity::GetPos(int ms){
    float seconds = ms/1000.0f;
    return {x+(velocity.x*seconds),y+(velocity.y*seconds),z+(velocity.z*seconds)};
}

Entity::~Entity(){
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


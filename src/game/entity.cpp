#include "entity.h"
#include <stdlib.h>


Entity::Entity(){
    instance_id =0;
    unique_id =0;
    type_id =0;
    name=nullptr;
    x=y=z=0;
    scale ={1,1,1};
    rotation = {0,0,0,1};

    velocity=nullptr;
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
    if(models != nullptr){free(models);models=nullptr;}
    if(sprites != nullptr){free(sprites);sprites=nullptr;}
    if(phys_data != nullptr){free(phys_data);phys_data=nullptr;}
    if(colliders != nullptr){free(colliders);colliders=nullptr;}
    if(movement != nullptr){free(movement);movement=nullptr;}
    if(state != nullptr){free(state);state=nullptr;}
    //if(animations != nullptr){free(animations);animations=nullptr;}
    if(player_data != nullptr){free(player_data);player_data=nullptr;}
    if(camera_target != nullptr){free(camera_target);camera_target=nullptr;}
    if(unit_data != nullptr){free(unit_data);unit_data=nullptr;}
    if(npc_data != nullptr){free(npc_data);npc_data=nullptr;}
}


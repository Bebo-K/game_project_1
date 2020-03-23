#include "entity.h"



Entity::Entity(){
    unique_id=-1;
    type_id=-1;
    name=nullptr;
    x=y=z=0;
    scale={1,1,1};
    rotation={0,0,0,1};

/*
    models=nullptr;
    sprites=nullptr;
    phys=nullptr;
    colliders=nullptr;
    movement=nullptr;
    state=nullptr;
    anims=nullptr;
    player_stats=nullptr;
    cam_target=nullptr;
    unit_data=nullptr;
    npc_stats=nullptr;
*/
}

Entity::~Entity(){
    if(name != nullptr){delete name;name=nullptr;}
    /*
    if(models!=nullptr){delete models;models=nullptr;}
    if(sprites!=nullptr){delete sprites;sprites=nullptr;}
    if(phys!=nullptr){delete phys;phys=nullptr;}
    if(colliders!=nullptr){delete colliders;colliders=nullptr;}
    if(movement!=nullptr){delete movement;movement=nullptr;}
    if(state!=nullptr){delete state;state=nullptr;}
    if(anims!=nullptr){delete anims;anims=nullptr;}
    if(player_stats!=nullptr){delete player_stats;player_stats=nullptr;}
    if(cam_target!=nullptr){delete cam_target;cam_target=nullptr;}
    if(unit_data!=nullptr){delete unit_data;unit_data=nullptr;}
    if(npc_stats!=nullptr){delete npc_stats;npc_stats=nullptr;}
    */
}
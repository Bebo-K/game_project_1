#ifndef ENTITY_H
#define ENTITY_H

#include "../gfx/types.h"

struct Entity{
    int     unique_id;
    int     type_id;
    char*   name;
    float   x,y,z;
    vec3    scale;
    quaternion rotation;

    Entity();
    ~Entity();

/*
    ModelSet				models;
    SpriteSet			sprites;
    PhysConstants		phys;
    ColliderSet			colliders;
    Movement				movement;
    StateMachine			state;
    StateAnimations		anims;
    PlayerControlStats	player_stats;
    CameraTargetcam_target;
    Unit            unit_data;
    NPCControlStats	npc_stats;
*/
};




#endif





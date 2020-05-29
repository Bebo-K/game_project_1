#ifndef ENTITY_H
#define ENTITY_H

#include "components/component.h"
#include "components/gfx_components.h"
#include "components/phys_components.h"
#include "components/logic_components.h"
#include "components/anim_components.h"
#include "components/state.h"


struct Entity{
    int     eid;
    int     type;
    char*   name;
    float   x,y,z;
    float   turn;//euler y rotation.
    vec3    velocity;
    vec3    scale;
    vec3   rotation;

    //components
    ModelSet*       models;
    SpriteSet*      sprites;
    PhysicsData*    phys_data;
    ColliderSet*    colliders;
    MovementData*   movement;
    PlayerData*     player_data;
    CameraTarget*   camera_target;
    State*          state;
    AnimationState* anim_state;
    UnitData*       unit_data;
    NPCData*        npc_data;

    Entity();
    void SetName(char* name);
    vec3 GetPos(int ms);

    ~Entity();
};







#endif





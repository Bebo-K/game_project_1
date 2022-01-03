#ifndef ENTITY_H
#define ENTITY_H

#include "component/component.h"
#include "component/gfx_components.h"
#include "component/phys_components.h"
#include "component/logic_components.h"
#include "component/anim_components.h"
#include "component/state.h"


struct Entity{
    int     eid;
    int     type;
    wchar*   name;
    float   x,y,z;
    float   turn;//euler y rotation.
    vec3    velocity;
    vec3    scale;
    vec3    rotation;

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
    void SetName(wchar* name);
    vec3 GetPos(int ms);

    ~Entity();
};







#endif





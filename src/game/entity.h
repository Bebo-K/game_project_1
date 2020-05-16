#ifndef ENTITY_H
#define ENTITY_H

#include "components/component.h"
#include "components/gfx_components.h"
#include "components/phys_components.h"
#include "components/logic_components.h"
#include "components/state_machine.h"


struct Entity{
    int     eid;
    int     type;
    char*   name;
    float   x,y,z;
    vec3    velocity;
    vec3    scale;
    quaternion rotation;

    //components
    ModelSet*       models;
    SpriteSet*      sprites;
    PhysicsData*    phys_data;
    ColliderSet*    colliders;
    MovementData*   movement;
    StateMachine*   state;
    PlayerData*     player_data;
    CameraTarget*   camera_target;
    UnitData*       unit_data;
    NPCData*        npc_data;

    Entity();
    void SetName(char* name);
    vec3 GetPos(int ms);

    ~Entity();
};







#endif





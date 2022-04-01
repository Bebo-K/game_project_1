#ifndef CLIENT_ENTITY_H
#define CLIENT_ENTITY_H

#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/component/gfx_components.hpp>
#include <game_project_1/component/anim_components.hpp>
#include <game_project_1/component/phys_components.hpp>
#include <game_project_1/component/logic_components.hpp>
#include <game_project_1/component/state.hpp>


class ClientEntity{
    public:
    int     id;
    int     entity_class_id;
    wchar*  name;
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

    ClientEntity();
    ~ClientEntity();

    vec3 GetPos();
};






#endif
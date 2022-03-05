#ifndef ENTITY_H
#define ENTITY_H

#include <game_project_1/game/game_constants.hpp>
#include <game_project_1/component/component.hpp>
#include <game_project_1/component/gfx_components.hpp>
#include <game_project_1/component/phys_components.hpp>
#include <game_project_1/component/logic_components.hpp>
#include <game_project_1/component/anim_components.hpp>
#include <game_project_1/component/state.hpp>


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

    int SerializedLength();
    void Serialize(void* data);

    ~Entity();
};


class EntityClass{
    GameConstants::EntityClassID id;

    virtual Entity* BuildClientEntity();
    virtual Entity* BuildServerEntity();

    
    static void Register(EntityClass* eclass);
    static Entity* BuildClient(GameConstants::EntityClassID class_id);
    static Entity* BuildServer(GameConstants::EntityClassID class_id);
};







#endif





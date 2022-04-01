#ifndef SERVER_ENTITY_H
#define SERVER_ENTITY_H

#include <game_project_1/game/game_constants.hpp>
#include <game_project_1/component/component.hpp>
#include <game_project_1/component/phys_components.hpp>
#include <game_project_1/component/logic_components.hpp>
#include <game_project_1/component/anim_components.hpp>
#include <game_project_1/component/state.hpp>


struct ServerEntity{
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
    PhysBody*       phys_data;
    ColliderSet*    colliders;
    MovementData*   movement;
    PlayerData*     player_data;
    State*          state;
    AnimationState* anim_state;
    UnitData*       unit_data;
    NPCData*        npc_data;

    ServerEntity();
    ~ServerEntity();
    vec3 GetPos();

    int DeltaLength();
    void SerializeDelta(byte* data);

    int SerializedLength();
    void Serialize(byte* data);
};

/*
class EntityClass{
    GameConstants::EntityClassID id;

    virtual Entity* BuildClientEntity();
    virtual Entity* BuildServerEntity();

    
    static void Register(EntityClass* eclass);
    static Entity* BuildClient(GameConstants::EntityClassID class_id);
    static Entity* BuildServer(GameConstants::EntityClassID class_id);
};
*/

#endif
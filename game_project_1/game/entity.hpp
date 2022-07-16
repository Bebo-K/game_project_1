#ifndef ENTITY_H
#define ENTITY_H

#include <game_project_1/game/game_constants.hpp>
#include <game_project_1/component/gfx_components.hpp>
#include <game_project_1/component/phys_components.hpp>
#include <game_project_1/component/logic_components.hpp>
#include <game_project_1/component/anim_components.hpp>
#include <game_project_1/component/state.hpp>
#include <game_project_1/io/serializer.hpp>


namespace ComponentChunk{
    enum ID{
        POSITION=0,
        MOVE=1,
        STATE=2,
        PHYS=3,
        COLLIDERS=4,
        IDS=5,
        STATS=6,
        EQUIP=7,  
        INVENTORY=8,
        CHARACTER=9,
    };
    typedef int Mask;
    const int COMPONENT_COUNT=10;
    const Mask BASIC_COMPONENTS= (1) | (1 << IDS);
};

class BaseEntity{
    public:
    int     id;
    int     entity_class_id;
    int     last_update[ComponentChunk::COMPONENT_COUNT];
    wchar*  name;
    float   x,y,z;
    vec3    velocity;
    vec3    scale;
    vec3    rotation;

    //nullable
    PhysBody*       phys_data;
    ColliderSet*    colliders;
    MovementData*   movement;
    State*          state;
    StatBlock*      stats;
    Equip*          equip;
    Inventory*      inventory;
    Character*      char_data;

    BaseEntity(int id);
    virtual ~BaseEntity();
    void Clear();
    vec3 GetPos();
    Location GetLocation();
    float GetTurnAngle();

    ComponentChunk::Mask AllExistingComponents();//Mask needed to create all non-null components

    int ChunkLength(ComponentChunk::ID ChunkID);
    void WriteChunk(ComponentChunk::ID ChunkID,Serializer& dat);
    void ReadChunk(ComponentChunk::ID ChunkID,Deserializer& dat);

    void Serialize(ComponentChunk::Mask delta_mask,Serializer& dat);
    void Deserialize(Deserializer& dat,int timestamp);
    int SerializedLength(ComponentChunk::Mask delta_mask);
    void Skip(Deserializer& dat);

    void CopyFrom(BaseEntity* e);
};

class ClientEntity: public BaseEntity{
    public:
    //client-side components
    ModelSet*       models;
    SpriteSet*      sprites;
    AnimationState* anim_state;

    ClientEntity(int entity_id);
    ~ClientEntity();

    vec3 GetPos();
};

struct ServerEntity: public BaseEntity{
    public:
    int  delta_mask;//mask of entity chunks updated since last delta was sent out
    int  spawn_mode;
        //If >0: just spawned this frame, spawn_mode is the spawn type ID for clients to use
        //If <0: despawning after this frame, (-spawn_mode) is the despawn type ID for clients to use
    bool just_deleted;//Entities persist until the end of the frame they're deleted on, then a "Delete Entity" packet is sent and it's removed.
    int player_id;//0 if not a player. Helpful to cache as players control what scenes stay loaded

    vec3 lastupdate_position;//position as sent out from last delta
    vec3 lastupdate_velocity;//velocity as ^

    Persistance*    persist;
 //   NPCControllerState*     npc_state;


    ServerEntity(int entity_id);
    ~ServerEntity();
};


#endif





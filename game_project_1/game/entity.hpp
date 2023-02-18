#ifndef ENTITY_H
#define ENTITY_H

#include <game_project_1/component/gfx_components.hpp>
#include <game_project_1/component/phys_components.hpp>
#include <game_project_1/component/logic_components.hpp>
#include <game_project_1/component/anim_components.hpp>
#include <game_project_1/component/state_components.hpp>
#include <game_project_1/io/serializer.hpp>


typedef int EntityClass;

class BaseEntity{
    public:
    int                 id;
    //Considered the "Identity" component, ID=0, not nullable
    EntityClass         type;
    wchar*              name;

    //Considered the "Position" component, ID=1, not nullable
    float               x,y,z;
    vec3                velocity;
    vec3                scale;
    vec3                rotation;

    //Rest of the components, nullable
    PhysicsProperties*  phys_props;         //ID = 2
    MoveProperties*     move_props;         //ID = 3
    //ActionProperties*   action_props;     //ID = 4
    PhysicsState*       phys_state;         //ID = 5
    MovementState*      move_state;         //ID = 6
    ActionState*        action_state;       //ID = 7
    ColliderSet*        colliders;          //ID = 8
    StatBlock*          stats;              //ID = 9
    Equip*              equip;              //ID = 10
    Inventory*          inventory;         //ID = 11
    Character*          char_data;          //ID = 12


    //metadata
    const static int    COMPONENT_COUNT = 13;
    int                 last_update[COMPONENT_COUNT];

    BaseEntity(int id);
    virtual ~BaseEntity();
    void Duplicate(BaseEntity* copy);

    bool HasComponent(int component_id);
    int  ComponentSize(int component_id);//Serialized length of component
    void WriteComponent(int component_id,Serializer& dat);
    void ReadComponent(int component_id,Deserializer& dat);
    void Clear();

    vec3 GetPos();
    Location GetLocation();
    float GetTurnAngle();
};

class ClientEntity: public BaseEntity{
    public:
    ModelSet*       models;
    SpriteSet*      sprites;
    AnimationState* anim_state;

    ClientEntity(int entity_id);
    ~ClientEntity();
};

struct ServerEntity: public BaseEntity{
    public:
    //State tracking
    int                 delta_mask;//mask of entity chunks updated since last delta was sent out
    int                 spawn_mode;
        //If >0: just spawned this frame, spawn_mode is the spawn type ID for clients to use
        //If <0: despawning after this frame, (-spawn_mode) is the despawn type ID for clients to use
    int                 player_id;//0 if not a player. Helpful to cache as players control what scenes stay loaded
    vec3                lastupdate_position;//position as sent out from last delta
    vec3                lastupdate_velocity;//velocity as ^

    //Components
    Persistance*        persist;
    NPCControllerState* npc_state;

    void MarkComponentUpdated(int component_id);
    ServerEntity(int entity_id);
    ~ServerEntity();
};


#endif





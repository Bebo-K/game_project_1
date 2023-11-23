#ifndef ENTITY_H
#define ENTITY_H

#include <game_project_1/types/str.hpp>
#include <game_project_1/component/component.hpp>
#include <game_project_1/component/all_components.hpp>

class ComponentMask;
class ClientComponentMask;
class ServerComponentMask;

class Entity{
    private:
    const static int    component_slots = 13;
    Component*          components[component_slots];
    inline int Id(int slot){return slot+1;}
    inline int Slot(int id){return id-1;}
    void Add(int slot);
    
    protected:
    const static int    component_count = component_slots + 1;
    template<typename T>
    int IdOf(){logger::exception("Entity.IdOf -> Could not get type ID of invalid component");return -1;}

    friend ComponentMask;
    friend ClientComponentMask;
    friend ServerComponentMask;
    
    public:
    int                 id;

    float               x,y,z;
    vec3                scale;
    vec3                rotation;
    vec3                velocity;

    Entity(int id);
    ~Entity();

    vec3 GetPos();
    Location GetLocation();
    void SetPos(vec3 pos);
    void SetLocation(Location loc);

    template<typename T>
    inline T* Get(){return (T*)components[Slot(IdOf<T>())];}
    template <typename T>
    bool Has(){return components[Slot(IdOf<T>())]!=null;}
    template<typename T>
    inline void Set(T* obj){
        int slot = Slot(IdOf<T>());
        if(components[slot] != nullptr){delete components[slot];}
        components[slot]=obj;
    }
    template<typename T>
    inline T* GetOrAdd(){
        int slot = Slot(IdOf<T>());
        if(components[slot] == nullptr){Add(slot);} return (T*)components[slot];}
    template<typename T>
    inline T* Remove(){
        int slot = Slot(IdOf<T>());
        if(components[slot] != nullptr){delete components[slot];components[slot]=nullptr;}}

    void Clear();
    void CloneTo(Entity* copy);

    bitmask AllExistingComponents();
    bool HasComponents(bitmask id_mask);
    bool HasAll(ComponentMask mask);
    int  SerializedLength(bitmask id_mask);
    void Write(Serializer& dat, bitmask id_mask);
    void Read(Deserializer& dat, int timestamp);
    static void SkipRead(Deserializer& dat);
};

template<> int Entity::IdOf<Identity>();
template<> int Entity::IdOf<PhysicsProperties>();
template<> int Entity::IdOf<MovementProperties>();
//template<> int Entity::IdOf<ActionProperties>();
template<> int Entity::IdOf<PhysicsState>();
template<> int Entity::IdOf<MovementState>();
template<> int Entity::IdOf<ActionState>();
template<> int Entity::IdOf<ColliderSet>();
template<> int Entity::IdOf<StatBlock>();
template<> int Entity::IdOf<Equip>();
template<> int Entity::IdOf<Inventory>();
template<> int Entity::IdOf<CharacterInfo>();
template<> int Entity::IdOf<Interactable>();

class ComponentMask{
    static Entity dummy;
    bitmask ids;
    public: 
    ComponentMask();
    ComponentMask(bitmask val);
    template<typename T>
    inline ComponentMask With(){ids.and_with(dummy.IdOf<T>());return ComponentMask(ids);}
    inline bitmask Mask(){return ids;}
};



#endif





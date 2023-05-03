#ifndef ENTITY_H
#define ENTITY_H

#include <game_project_1/types/str.hpp>
#include <game_project_1/component/component.hpp>
#include <game_project_1/component/component_ids.hpp>


class Entity{
    private:
    int                 pos_last_updated;

    Component**         components;
    int                 component_count;

    public:
    int                 id;

    float               x,y,z;
    vec3                velocity;
    vec3                scale;
    vec3                rotation;

    Entity(int id);
    ~Entity();

    vec3 GetPos();
    void SetPos(vec3 pos);
    Location GetLocation();

    void Clear();
    void CloneTo(Entity* copy);

    void NewComponent(int component_id);
    void DeleteComponent(int component_id);
    bool HasComponent(int component_id);
    bool HasComponents(bitmask mask);

    bitmask AllExistingComponents();
    int  SerializedLength(bitmask mask);
    void Write(Serializer& dat, bitmask mask);
    void Read(Deserializer& dat, int timestamp);

    template <typename T>
    T* Get(){return (T*)components[SharedComponent::TypeID<T>];}

    template <typename T>
    T* Has(){return components[SharedComponent::TypeID<T>]!=null;}

    template <typename T>
    void Add(T* comp){
        int typeID = SharedComponent::TypeID<T>;
        DeleteComponent(typeID);
        components[typeID]=(Component*)comp;
    }

    template <typename T>
    void Remove(){DeleteComponent(SharedComponent::TypeID<T>);}
};


class ClientEntity: public Entity{
    public:
    Component** cli_components;
    int         cli_component_count;  

    ClientEntity(int id);
    ~ClientEntity();

    void Clear();
    void CloneTo(ClientEntity* copy);
    
    void    NewClientComponent(int component_id);
    void    DeleteClientComponent(int component_id);
    bool    HasClientComponent(int component_id);
    bool    HasClientComponents(bitmask mask);

    template <typename T>
    T* ClientGet(){return (T*)cli_components[ClientComponent::TypeID<T>];}

    template <typename T>
    T* Has(){return Entity::Has<T>() || cli_components[ClientComponent::TypeID<T>]!=null;}

    template <typename T>
    void ClientAdd(T* comp){
        int typeID = ClientComponent::TypeID<T>;
        DeleteClientComponent(typeID);
        cli_components[typeID]=(Component*)comp;
    }

    template <typename T>
    void ClientRemove(){DeleteClientComponent(ClientComponent::TypeID<T>);}
};

struct ServerEntity: public Entity{
    public:

    //position when the last delta for this entity was sent out
    vec3    last_position;
    vec3    last_velocity;
    vec3    last_scale;
    vec3    last_rotation;

    int     player_id;
    inline bool IsPlayerOwned(){return player_id >= 0;}

    Component** svr_components;
    int         svr_component_count;

    //mask of components updated since last delta was sent out, bitshifted by 1. Index 0 is for position.
    bitmask changed_components;
    bool    save_to_scene;


    ServerEntity(int entity_id);
    ~ServerEntity();
    
    void Clear();
    void CloneTo(ServerEntity* copy);
    void NewServerComponent(int component_id);
    void DeleteServerComponent(int component_id);
    bool HasServerComponent(int component_id);
    bool HasServerComponents(bitmask mask);

    template <typename T>
    T* ServerGet(){return (T*)svr_components[ServerComponent::TypeID<T>];}

    template <typename T>
    T* Has(){return Entity::Has<T>() || svr_components[ServerComponent::TypeID<T>]!=null;}

    template <typename T>
    void ServerAdd(T* comp){
        int typeID = ServerComponent::TypeID<T>;
        DeleteServerComponent(typeID);
        svr_components[typeID]=(Component*)comp;
    }

    template <typename T>
    void ServerRemove(){DeleteServerComponent(ServerComponent::TypeID<T>);}

    inline void MarkMoved(){changed_components.set(0);}
    template <typename T>
    void MarkChanged(){changed_components.set(SharedComponent::TypeID<T> +1);}
    inline void ClearDelta(){changed_components.clear();}
};


#endif





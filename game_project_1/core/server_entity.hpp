#ifndef SERVER_ENTITY_H
#define SERVER_ENTITY_H

#include <game_project_1/core/entity.hpp>


struct ServerEntity: public Entity{
    private:
    const static int    svr_component_slots = 3;
    Component*          svr_components[svr_component_slots];

    template<typename T>
    int IdOf(){return Entity::IdOf<T>();}
    int SvrSlot(int cid){return -cid - 1;}
    void Add(int cid);

    friend class ServerComponentMask;
    
    public:
     //position when the last delta for this entity was sent out
    vec3    last_position;
    vec3    last_velocity;
    vec3    last_scale;
    vec3    last_rotation;
    bitmask changed_component_ids;//since last server frame

    template<typename T>
    inline T* Get(){
        int cid = IdOf<T>(),slot = SvrSlot(cid);
        return(cid >= 0)?Entity::Get<T>():(T*)svr_components[slot];
    }
    template <typename T>
    bool Has(){
        int cid = IdOf<T>(),slot = SvrSlot(cid);
        return(cid > 0)?Entity::Has<T>():svr_components[slot]!=null;
    }
    template<typename T>
    inline void Set(T* obj){
        int cid = IdOf<T>(),slot = SvrSlot(cid);
        if(cid >= 0) return Entity::Set<T>(obj);
        if(svr_components[slot] != nullptr){delete svr_components[slot];}
        svr_components[slot]=obj;
    }
    template<typename T>
    inline T* GetOrAdd(){
        int cid = IdOf<T>(),slot = SvrSlot(cid);
        if(cid >= 0){return Entity::GetOrAdd<T>();}
        if(svr_components[slot] == nullptr){Add(id);} return (T*)svr_components[slot];}
    template<typename T>
    inline void Remove(){
        int cid = IdOf<T>(),slot = SvrSlot(cid);
        if(cid >= 0){Entity::Remove<T>();return;}
        if(svr_components[slot] != nullptr){delete svr_components[slot];svr_components[slot]=nullptr;}}
    
    inline void MarkMoved(){changed_component_ids.set(0);}
    template <typename T>
    void MarkChanged(){changed_component_ids.set(Entity::IdOf<T>());}
    inline void ClearDelta(){changed_component_ids.clearAll();}

    ServerComponentMask ComponentMask();
    bool HasAll(ServerComponentMask mask);

    ServerEntity(int cid);
    ~ServerEntity();

    void Clear();
    void CloneTo(ServerEntity* copy);
};

template<> int ServerEntity::IdOf<PlayerOwner>();
template<> int ServerEntity::IdOf<Persistence>();
template<> int ServerEntity::IdOf<NPCProperties>();
template<> int ServerEntity::IdOf<NPCState>();

class ServerComponentMask{
    static ServerEntity dummy;
    bitmask ids;
    public: 
    ServerComponentMask();
    ServerComponentMask(bitmask val);
    template<typename T>
    inline ServerComponentMask With(){
        int cid = dummy.IdOf<T>();
        if(cid < 0){cid = -cid;cid += dummy.component_slots;}
        ids.set(cid);
        return ServerComponentMask(ids);
    }
        
    template<typename T>
    inline ServerComponentMask Without(){
        int cid = dummy.IdOf<T>();
        if(cid < 0){cid = -cid;cid += dummy.component_slots;}
        ids.clear(cid);
        return ServerComponentMask(ids);
    }

    inline bitmask Mask(){return ids;}
};

/*
struct ServerEntity: public Entity{
    public:



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
    bool Has(){return Entity::Has<T>() || (svr_components[ServerComponent::TypeID<T>]!=null);}

    template <typename T>
    void ServerAdd(T* comp){
        int typeID = ServerComponent::TypeID<T>;
        DeleteServerComponent(typeID);
        svr_components[typeID]=(Component*)comp;
    }

    template <typename T>
    void ServerRemove(){DeleteServerComponent(ServerComponent::TypeID<T>);}


};

*/



#endif
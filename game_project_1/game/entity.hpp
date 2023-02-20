#ifndef ENTITY_H
#define ENTITY_H

#include <game_project_1/component/component.hpp>


class SharedEntity{
    private:
    //Every entity has an identity + position so they're stored locally. Components[0 + 1] should always point to these.
    static const int LOCAL_COMPONENTS = 2;
    Identity p_identity;
    Position p_position;
    public:
    int                 id;
    const static int    COMPONENT_COUNT = 13;
    SharedComponent*    components[COMPONENT_COUNT];  

    SharedEntity(int id);
    virtual ~SharedEntity();
    void Clear();
    //void Duplicate(SharedEntity* copy);

    int GetType();
    vec3 GetPos();
    Location GetLocation();
    float GetTurnAngle();

    template <typename T>
    T* Get(){
        int component = T::ComponentID;
        if(component < 0 || component > COMPONENT_COUNT){
            logger::exception("Invalid component ID access (entity: %d, component: %d)",id,component);
        }
        return (T*)components[T::ComponentID];
    }

    void    NewComponent(int component_id);
    void    AddComponent(SharedComponent* c);
    void    DeleteComponent(int component_id);
    bitmask AllExistingComponents();
    bool    HasComponent(int component_id);
    bool    HasComponents(bitmask mask);
};

class ClientEntity: public SharedEntity{
    public:
    const static int    CLIENT_COMPONENT_COUNT = 3;
    ClientComponent*    cli_components[CLIENT_COMPONENT_COUNT];  

    ClientEntity(int id);
    virtual ~ClientEntity();
    void Clear();

    template <typename T>
    T* ClientGet(){
        int component = T::ClientComponentID;
        if(component < 0 || component > CLIENT_COMPONENT_COUNT){
            logger::exception("Invalid client component ID access (entity: %d, component: %d)",id,component);
        }
        return (T*)components[T::ClientComponentID];
    }

    void    NewClientComponent(int component_id);
    void    AddClientComponent(ClientComponent* c);
    void    DeleteClientComponent(int component_id);
    bool    HasClientComponent(int component_id);
};

struct ServerEntity: public SharedEntity{
    private:
    static const int SVR_LOCAL_COMPONENTS = 1;
    //Every server entity should have state tracking so it's stored locally. svr_components[0] should always point to this.
    ServerDelta p_delta;
    public:
    const static int    SERVER_COMPONENT_COUNT = 3;
    ClientComponent*    svr_components[SERVER_COMPONENT_COUNT];  

    //State tracking
    int                 spawn_mode;
        //If >0: just spawned this frame, spawn_mode is the spawn type ID for clients to use
        //If <0: despawning after this frame, (-spawn_mode) is the despawn type ID for clients to use
    int                 player_id;//0 if not a player. Helpful to cache as players control what scenes stay loaded

    ServerEntity(int entity_id);
    ~ServerEntity();
    void Clear();

    template <typename T>
    T* ServerGet(){
        int component = T::ServerComponentID;
        if(component < 0 || component > SERVER_COMPONENT_COUNT){
            logger::exception("Invalid server component ID access (entity: %d, component: %d)",id,component);
        }
        return (T*)components[T::ServerComponentID];
    }

    void    NewServerComponent(int component_id);
    void    AddServerComponent(ServerComponent* c);
    void    DeleteServerComponent(int component_id);
    bool    HasServerComponent(int component_id);
    void    MarkComponentUpdated(int component_id);
};


#endif





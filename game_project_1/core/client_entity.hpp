#ifndef CLIENT_ENTITY_H
#define CLIENT_ENTITY_H

#include <game_project_1/core/entity.hpp>


class ClientEntity: public Entity{
    private:
    const static int    cli_component_slots = 3;
    Component*          cli_components[cli_component_slots];

    template<typename T>
    int IdOf(){return Entity::IdOf<T>();}
    int CliSlot(int cid){return -cid - 1;}
    void Add(int id);

    friend ClientComponentMask;
    
    public:

    template<typename T>
    inline T* Get(){
        int cid = IdOf<T>(), slot = CliSlot(cid);
        return(cid >= 0)?Entity::Get<T>():(T*)cli_components[slot];
    }
    template <typename T>
    bool Has(){
        int cid = IdOf<T>(), slot = CliSlot(cid);
        return(cid > 0)?Entity::Has<T>():cli_components[slot]!=null;
    }
    template<typename T>
    inline void Set(T* obj){
        int cid = IdOf<T>(), slot = CliSlot(cid);
        if(cid >= 0) return Entity::Set<T>(obj);
        if(cli_components[slot] != nullptr){delete cli_components[slot];}
        cli_components[slot]=obj;
    }
    template<typename T>
    inline T* GetOrAdd(){
        int cid = IdOf<T>(), slot = CliSlot(cid);
        if(cid >= 0){return Entity::GetOrAdd<T>();}
        if(cli_components[-id] == nullptr){Add(id);} return (T*)cli_components[-id];}
    template<typename T>
    inline void Remove(){
        int cid = IdOf<T>(), slot = CliSlot(cid);
        if(cid >= 0){Entity::Remove<T>();return;}
        if(cli_components[slot] != nullptr){delete cli_components[slot];cli_components[slot]=nullptr;}}

    ClientComponentMask ComponentMask();
    bool HasAll(ClientComponentMask mask);

    ClientEntity(int id);
    ~ClientEntity();

    void Clear();
    void CloneTo(ClientEntity* copy);
};

template<> int ClientEntity::IdOf<AnimationState>();
template<> int ClientEntity::IdOf<ModelSet>();
template<> int ClientEntity::IdOf<SpriteSet>();
template<> int ClientEntity::IdOf<HitBoxes>();

class ClientComponentMask{
    static ClientEntity dummy;
    bitmask ids;
    public: 
    ClientComponentMask();
    ClientComponentMask(bitmask val);
    template<typename T>
    inline ClientComponentMask With(){
        int cid = dummy.IdOf<T>();
        if(cid < 0){cid = -cid;cid += dummy.component_slots;}
        ids.set(cid);return ClientComponentMask(ids);}
    inline bitmask Mask(){return ids;}
};


#endif





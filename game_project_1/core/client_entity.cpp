#include <game_project_1/core/client_entity.hpp>
#include <game_project_1/component/all_components.hpp>

ClientEntity ClientComponentMask::dummy = ClientEntity(-1);
ClientComponentMask::ClientComponentMask():ids(0){}
ClientComponentMask::ClientComponentMask(bitmask val):ids(0){ids=val;}

template<> int ClientEntity::IdOf<AnimationState>(){return -1;}
template<> int ClientEntity::IdOf<ModelSet>(){return -2;}
template<> int ClientEntity::IdOf<SpriteSet>(){return -3;}
template<> int ClientEntity::IdOf<HitBoxes>(){return -4;}

void ClientEntity::Add(int cid){
    switch(cid){
        case -1: cli_components[-cid]= new AnimationState(); break;
        case -2: cli_components[-cid]= new ModelSet(); break;
        case -3: cli_components[-cid]= new SpriteSet(); break;
        case -4: cli_components[-cid]= new HitBoxes(); break;
        default: 
            logger::warn("Could not add component ID %d to client entity ID %d, id is invalid",-cid,id);
        break;
    }
}

ClientEntity::ClientEntity(int id):Entity(id){
    for(int i=0;i<cli_component_slots;i++){cli_components[i]=null;}
}
ClientEntity::~ClientEntity(){
    Clear();
}

ClientComponentMask ClientEntity::ComponentMask(){
    bitmask mask = AllExistingComponents();
    for(int i=0;i<cli_component_slots;i++){
        if(cli_components[i] != null){mask.set(i + component_count);}
    }
    return ClientComponentMask(mask);
}
bool ClientEntity::HasAll(ClientComponentMask mask){
    bitmask id_mask = mask.Mask();
    if(!HasComponents(id_mask))return false;
    for(int i=0;i<cli_component_slots;i++){
        if(id_mask.get_bit(i + component_count) && cli_components[i] == null)return false;
    }
    return true;
}

void ClientEntity::Clear(){
    Entity::Clear();
    for(int i=0;i<cli_component_slots;i++){
        if(cli_components[i]!=null){delete cli_components[i]; cli_components[i]=null;};
    }
}

void ClientEntity::CloneTo(ClientEntity* copy){
    Entity::CloneTo((Entity*)copy);
    for(int i=0;i<cli_component_slots;i++){
        if(cli_components[i]!=null){copy->cli_components[i]=cli_components[i]->Clone();};
    }
}



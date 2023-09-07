#include <game_project_1/core/server_entity.hpp>
#include <game_project_1/component/all_components.hpp>

ServerEntity ServerComponentMask::dummy = ServerEntity(-1);
ServerComponentMask::ServerComponentMask():ids(0){}
ServerComponentMask::ServerComponentMask(bitmask val):ids(0){ids=val;}

template<> int ServerEntity::IdOf<PlayerOwner>(){return -1;}
template<> int ServerEntity::IdOf<Persistence>(){return -2;}
template<> int ServerEntity::IdOf<NPCProperties>(){return -3;}
template<> int ServerEntity::IdOf<NPCState>(){return -4;}

void ServerEntity::Add(int cid){
    switch(cid){
        case -1: svr_components[-cid]= new PlayerOwner(); break;
        case -2: svr_components[-cid]= new Persistence(); break;
        case -3: svr_components[-cid]= new NPCProperties(); break;
        case -4: svr_components[-cid]= new NPCState(); break;
        default: 
            logger::warn("Could not add component ID %d to server entity ID %d, id is invalid",-cid,id);
        break;
    }
}

ServerEntity::ServerEntity(int id):Entity(id),changed_component_ids(0){
    for(int i=0;i<svr_component_slots;i++){svr_components[i]=null;}
}

ServerEntity::~ServerEntity(){
    Clear();
}

ServerComponentMask ServerEntity::ComponentMask(){
    bitmask mask = AllExistingComponents();
    for(int i=0;i<svr_component_slots;i++){
        if(svr_components[i] != null){mask.set(i + component_count);}
    }
    return ServerComponentMask(mask);
}

bool ServerEntity::HasAll(ServerComponentMask mask){
    bitmask id_mask = mask.Mask();
    if(!HasComponents(id_mask))return false;
    for(int i=0;i<svr_component_slots;i++){
        if(id_mask.get_bit(i + component_count) && svr_components[i] == null)return false;
    }
    return true;
}

void ServerEntity::Clear(){
    Entity::Clear();
    changed_component_ids = bitmask::none;
    for(int i=0;i<svr_component_slots;i++){
        if(svr_components[i]!=null){delete svr_components[i]; svr_components[i]=null;};
    }
}

void ServerEntity::CloneTo(ServerEntity* copy){
    Entity::CloneTo((Entity*)copy);
    for(int i=0;i<svr_component_slots;i++){
        if(svr_components[i]!=null){copy->svr_components[i]=svr_components[i]->Clone();};
    }
    
    copy->changed_component_ids=changed_component_ids;
    copy->last_position = last_position;
    copy->last_rotation = last_rotation;
    copy->last_velocity = last_velocity;
    copy->last_scale = last_scale;
}


/*
void ServerEntity::NewServerComponent(int component_id){
    DeleteServerComponent(component_id);
    svr_components[component_id] = ServerComponent::NewInstance<component_id>();
}
void ServerEntity::DeleteServerComponent(int component_id){
    if(component_id < 1 || component_id > svr_component_count){
        logger::exception("Invalid ServerEntity component index %d",component_id);
    }
    if(svr_components[component_id] != null){delete svr_components[component_id];}
}
bool ServerEntity::HasServerComponent(int component_id){
    return (svr_components[component_id] != null);
}
bool ServerEntity::HasServerComponents(bitmask mask){
    for(int i=0;i<svr_component_count;i++){if(mask.get_bit(i) && svr_components[i] == null){return false;}}
    return true;
}
*/
#include <game_project_1/game/entity.hpp>

SharedEntity::SharedEntity(int entity_id):p_identity(),p_position(){
    for(int i=0;i<COMPONENT_COUNT;i++){components[i]=null;}
    components[0] = &p_identity;
    components[1] = &p_position;
    id =entity_id;
}
SharedEntity::~SharedEntity(){
    Clear();
}
void SharedEntity::Clear(){
    id=0;
    p_identity.Clear();
    p_position.Reset();
    for(int i=LOCAL_COMPONENTS;i<COMPONENT_COUNT;i++){if(components[i] != null){delete components[i];components[i] = null;}}
}

int SharedEntity::GetType(){return p_identity.type;}
vec3 SharedEntity::GetPos(){return {p_position.x,p_position.y,p_position.z};}
Location SharedEntity::GetLocation(){
    return {{p_position.x,p_position.y,p_position.z},p_position.rotation,p_position.scale};
}
float SharedEntity::GetTurnAngle(){return p_position.rotation.y;}

void SharedEntity::NewComponent(int component_id){
    if(component_id < LOCAL_COMPONENTS || component_id > COMPONENT_COUNT){
        logger::exception("Invalid component index for NewComponent (entity: %d, component: %d)",id,component_id);
    }
    if(components[component_id] != null){
        logger::warn("NewComponent overwrites an existing component (entity: %d, component: %d)",id,component_id);
        delete components[component_id];
    }
    components[component_id] = SharedComponent::NewInstance(component_id);
}
void SharedEntity::AddComponent(SharedComponent* c){
    int component_id = c->ID();
    if(component_id < LOCAL_COMPONENTS || component_id > COMPONENT_COUNT){
        logger::exception("Invalid component index for add (entity: %d, component: %d)",id,component_id);
    }
    if(components[component_id] != null){
        logger::warn("AddComponent overwrites an existing component (entity: %d, component: %d)",id,component_id);
        delete components[component_id];
    }
    components[component_id] = c;
}
void SharedEntity::DeleteComponent(int component_id){
    if(component_id < LOCAL_COMPONENTS || component_id > COMPONENT_COUNT){
        logger::exception("Invalid component index for delete (entity: %d, component: %d)",id,component_id);
    }
    if(components[component_id] != null){
        delete components[component_id];
        components[component_id] = null;
    }
    else{
        logger::warn("Deleting a component that does not exist (entity: %d, component: %d)",id,component_id);
    }
}
bitmask SharedEntity::AllExistingComponents(){
    bitmask mask(0);
    for(int i=0;i<COMPONENT_COUNT;i++){
        if(components[i] != null){mask.set(i);}
    }
    return mask;
}
bool SharedEntity::HasComponent(int component_id){
    if(component_id < 0 || component_id > COMPONENT_COUNT){
        logger::exception("Invalid component index for HasComponent (entity: %d, component: %d)",id,component_id);
    }
    return components[i] != null;
}
bool SharedEntity::HasComponents(bitmask mask){
    bitmask mask(0);
    for(int i=0;i<COMPONENT_COUNT;i++){
        if(mask.get_bit(i) && components[i] == null){return false;}
    }
    return true;
}



ClientEntity::ClientEntity(int entity_id):SharedEntity(entity_id){
    for(int i=0;i<CLIENT_COMPONENT_COUNT;i++){cli_components[i]=null;}
}
ClientEntity::~ClientEntity(){
    Clear();
}
void ClientEntity::Clear(){
    SharedEntity::Clear();
    for(int i=0;i<CLIENT_COMPONENT_COUNT;i++){
        if(cli_components[i] != null){delete cli_components[i];cli_components[i] = null;}
    }
}

void ClientEntity::NewClientComponent(int component_id){
    if(component_id < 0 || component_id > CLIENT_COMPONENT_COUNT){
        logger::exception("Invalid component index for NewClientComponent (entity: %d, component: %d)",id,component_id);
    }
    if(cli_components[component_id] != null){
        logger::warn("NewClientComponent overwrites an existing component (entity: %d, component: %d)",id,component_id);
        delete cli_components[component_id];
    }
    cli_components[component_id] = ClientComponent::NewInstance(component_id);
}
void ClientEntity::AddClientComponent(ClientComponent* c){
    int component_id = c->ID();
    if(component_id < 0 || component_id > CLIENT_COMPONENT_COUNT){
        logger::exception("Invalid component index for AddClientComponent (entity: %d, component: %d)",id,component_id);
    }
    if(cli_components[component_id] != null){
        logger::warn("AddClientComponent overwrites an existing component (entity: %d, component: %d)",id,component_id);
        delete cli_components[component_id];
    }
    cli_components[component_id] = c;
}
void ClientEntity::DeleteClientComponent(int component_id){
    if(component_id < 0 || component_id > CLIENT_COMPONENT_COUNT){
        logger::exception("Invalid component index for client delete (entity: %d, component: %d)",id,component_id);
    }
    if(cli_components[component_id] != null){
        delete cli_components[component_id];
        cli_components[component_id] = null;
    }
    else{
        logger::warn("Deleting a client component that does not exist (entity: %d, component: %d)",id,component_id);
    }
}
bool ClientEntity::HasClientComponent(int component_id){
    if(component_id < 0 || component_id > CLIENT_COMPONENT_COUNT){
        logger::exception("Invalid component index for HasServerComponent (entity: %d, component: %d)",id,component_id);
    }
    return cli_components[i] != null;
}



ServerEntity::ServerEntity(int entity_id):SharedEntity(entity_id),p_delta(){
    for(int i=SVR_LOCAL_COMPONENTS;i<SERVER_COMPONENT_COUNT;i++){svr_components[i]=null;}
    svr_components[0]=&p_delta;
    player_id = 0;
    spawn_mode = 0;
}
ServerEntity::~ServerEntity(){
    Clear();
}
ServerEntity::Clear(){
    SharedEntity::Clear();
    p_delta.Clear();
    for(int i=SVR_LOCAL_COMPONENTS;i<SERVER_COMPONENT_COUNT;i++){
        if(svr_components[i] != null){delete svr_components[i];svr_components[i] = null;}
    }
}

void ServerEntity::MarkComponentUpdated(int component_id){
    if(component_id < 0 || component_id > COMPONENT_COUNT){
        logger::exception("Invalid component index for MarkComponentUpdated (entity: %d, component: %d)",id,component_id);
    }
    p_delta.changed_components.set(component_id);
}

void ServerEntity::NewServerComponent(int component_id){
    if(component_id < SVR_LOCAL_COMPONENTS || component_id > SERVER_COMPONENT_COUNT){
        logger::exception("Invalid component index for NewServerComponent (entity: %d, component: %d)",id,component_id);
    }
    if(svr_components[component_id] != null){
        logger::warn("NewServerComponent overwrites an existing component (entity: %d, component: %d)",id,component_id);
        delete svr_components[component_id];
    }
    svr_components[component_id] = ServerComponent::NewInstance(component_id);
}
void ServerEntity::AddServerComponent(ServerComponent* c){
    int component_id = c->ID();
    if(component_id < SVR_LOCAL_COMPONENTS || component_id > SERVER_COMPONENT_COUNT){
        logger::exception("Invalid component index for AddServerComponent (entity: %d, component: %d)",id,component_id);
    }
    if(svr_components[component_id] != null){
        logger::warn("AddServerComponent overwrites an existing component (entity: %d, component: %d)",id,component_id);
        delete svr_components[component_id];
    }
    svr_components[component_id] = c;
}
void ServerEntity::DeleteServerComponent(int component_id){
    if(component_id < SVR_LOCAL_COMPONENTS || component_id > SERVER_COMPONENT_COUNT){
        logger::exception("Invalid component index for server delete (entity: %d, component: %d)",id,component_id);
    }
    if(svr_components[component_id] != null){
        delete svr_components[component_id];
        svr_components[component_id] = null;
    }
    else{
        logger::warn("Deleting a server component that does not exist (entity: %d, component: %d)",id,component_id);
    }
}
bool ServerEntity::HasServerComponent(int component_id){
    if(component_id < 0 || component_id > SERVER_COMPONENT_COUNT){
        logger::exception("Invalid component index for HasServerComponent (entity: %d, component: %d)",id,component_id);
    }
    return svr_components[i] != null;
}
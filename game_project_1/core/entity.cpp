#include <game_project_1/core/entity.hpp>


Entity::Entity(int id){
    this->id = id;
    component_count = SharedComponent::Count;
    components = new Component*[component_count];
    for(int i=0;i<component_count;i++){components[i]=null;}
}

Entity::~Entity(){
    Clear();
    delete components;components=null;
}

void Entity::Clear(){
    id=0;
    x=y=z=0;
    rotation = vec3::zero();
    scale = {1,1,1};
    velocity = vec3::zero();
    for(int i=0;i<component_count;i++){
        if(components[i]!=null){
            delete components[i]; 
            components[i]=null;
        }
    }
}
void Entity::CloneTo(Entity* copy){
    copy->id = id;
    copy->SetPos({x,y,z});
    copy->rotation = rotation;
    copy->scale = scale;
    copy->velocity = velocity;
    for(int i=0;i<component_count;i++){
        if(components[i]!=null){copy->components[i]=components[i]->Clone();};
    }
}
void Entity::NewComponent(int component_id){
    DeleteComponent(component_id);
    components[component_id]=SharedComponent::NewInstance<component_id>();
}
void Entity::DeleteComponent(int component_id){
    if(component_id < 0 || component_id > component_count){logger::exception("Invalid Entity component index %d",component_id);}
    if(components[component_id] != null){delete components[component_id];}
}
bitmask Entity::AllExistingComponents(){
    bitmask mask(0);
    for(int i=0;i<component_count;i++){if(components[i] != null){mask.set(i);}}
    return mask;
}
bool Entity::HasComponent(int componentID){
    return (components[componentID]!=null);
}
bool Entity::HasComponents(bitmask mask){
    for(int i=0;i<component_count;i++){if(mask.get_bit(i) && components[i] == null){return false;}}
    return true;
}

int Entity::SerializedLength(bitmask mask){
    int length = sizeof(bitmask);
    if(mask.get_bit(0)){ length += sizeof(float)*3 + sizeof(vec3)*3; }
    for(int i=0;i<component_count;i++){
        if(mask.get_bit(i+1) && components[i] != null){length += (sizeof(int) + components[i]->SerializedLength());}
    }
    return length;
}
void Entity::Write(Serializer& dat, bitmask mask){
    dat.PutInt(sizeof(bitmask));
    if(mask.get_bit(0)){ dat.PutVec3({x,y,z});dat.PutVec3(rotation);dat.PutVec3(scale);dat.PutVec3(velocity);}
    for(int i=0;i<component_count;i++){
        if(mask.get_bit(i+1) && components[i] != null){components[i]->Write(dat);}
    }
}
void Entity::Read(Deserializer& dat, int timestamp){
    bitmask read_mask(dat.GetInt());
    if(read_mask.get_bit(0) ){
        vec3 pos = dat.GetVec3(); x=pos.x; y=pos.y; z=pos.z;
        rotation = dat.GetVec3();
        scale = dat.GetVec3();
        velocity = dat.GetVec3();
    }
    for(int i=0;i<component_count;i++){
        if(read_mask.get_bit(i+1)){
            if(components[i]==null){NewComponent(i);}
            if(components[i]->last_updated < timestamp){
                components[i]->Read(dat);
                components[i]->MarkUpdated(timestamp);
            }
            else{
                Component* temp = components[i]->Clone();
                temp->Read(dat);
                delete temp;
            }
        }
    }
}




ClientEntity::ClientEntity(int id):Entity(id){
    cli_component_count = ClientComponent::Count;
    cli_components = new Component*[cli_component_count];
    for(int i=0;i<cli_component_count;i++){cli_components[i]=null;}
}
ClientEntity::~ClientEntity(){
    Clear();
    delete cli_components;cli_components=null;
}

void ClientEntity::Clear(){
    Entity::Clear();
    for(int i=0;i<cli_component_count;i++){
        if(cli_components[i]!=null){delete cli_components[i]; cli_components[i]=null;};
    }
}
void ClientEntity::CloneTo(ClientEntity* copy){
    Entity::CloneTo((Entity*)copy);
    for(int i=0;i<cli_component_count;i++){
        if(cli_components[i]!=null){copy->cli_components[i]=cli_components[i]->Clone();};
    }
}
void ClientEntity::NewClientComponent(int component_id){
    DeleteClientComponent(component_id);
    cli_components[component_id] = ClientComponent::NewInstance<component_id>();
}
void ClientEntity::DeleteClientComponent(int component_id){
    if(component_id < 0 || component_id > cli_component_count){
        logger::exception("Invalid ClientEntity component index %d",component_id);
    }
    if(cli_components[component_id] != null){delete cli_components[component_id];}
}
bool ClientEntity::HasClientComponent(int component_id){
    return (cli_components[component_id] != null);
}
bool ClientEntity::HasClientComponents(bitmask mask){
    for(int i=0;i<cli_component_count;i++){if(mask.get_bit(i) && cli_components[i] == null){return false;}}
    return true;
}




ServerEntity::ServerEntity(int id):Entity(id){
    svr_component_count = ServerComponent::Count;
    svr_components = new Component*[svr_component_count];
    for(int i=0;i<svr_component_count;i++){svr_components[i]=null;}
    changed_components.val=0;
    save_to_scene=false;
}
ServerEntity::~ServerEntity(){
    Clear();
    delete svr_components;svr_components=null;
}

void ServerEntity::Clear(){
    Entity::Clear();
    delta.Clear();
    for(int i=1;i<svr_component_count;i++){
        if(svr_components[i]!=null){delete svr_components[i]; svr_components[i]=null;};
    }
}
void ServerEntity::CloneTo(ServerEntity* copy){
    Entity::CloneTo((Entity*)copy);
    copy->delta=delta;
    for(int i=1;i<svr_component_count;i++){
        if(svr_components[i]!=null){copy->svr_components[i]=svr_components[i]->Clone();};
    }
}
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
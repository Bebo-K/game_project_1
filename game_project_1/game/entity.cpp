#include <game_project_1/game/entity.hpp>


vec3 BaseEntity::GetPos(){return {x,y,z};}
Location BaseEntity::GetLocation(){
    return {{x,y,z},rotation,scale};
}

float BaseEntity::GetTurnAngle(){return rotation.y;}

BaseEntity::BaseEntity(int entity_id){
    id =entity_id;
    type =0;
    name=nullptr;
    x=y=z=0;
    velocity={0,0,0};
    scale ={1,1,1};
    rotation = {0,0,0};

    phys_props = null;
    move_props = null;
    //action_props = null;
    phys_state = null;
    move_state = null;
    action_state = null;
    colliders = null;
    stats = null;
    equip = null;
    inventory = null;
    char_data = null;

    for(int i=0;i<COMPONENT_COUNT;i++){last_update[i]=0;}
}
BaseEntity::~BaseEntity(){
    Clear();
}

void BaseEntity::Duplicate(BaseEntity* copy){
    Clear();
    type = copy->type;
    name= wstr::new_copy(copy->name);
    x=copy->x; y=copy->y; z=copy->z;
    velocity= copy->velocity;
    scale = copy->scale;
    rotation = copy->rotation;

    if(copy->phys_props)phys_props = new PhysicsProperties(copy->phys_props);
    if(copy->move_props)move_props = new MoveProperties(copy->move_props);
    //if(copy->phys_props)to->action_props = new ActionProperties(copy->action_props);
    if(copy->phys_state)phys_state = new PhysicsState(copy->phys_state);
    if(copy->move_state)move_state = new MovementState(copy->move_state);
    if(copy->action_state)action_state = new ActionState(copy->action_state);
    if(copy->colliders)colliders = new ColliderSet(copy->colliders);
    if(copy->stats)stats = new StatBlock(copy->stats);
    if(copy->equip)equip = new Equip(copy->equip);
    if(copy->inventory)inventory = new Inventory(copy->inventory);
    if(copy->char_data)char_data = new Character(copy->char_data);

    for(int i=0;i<COMPONENT_COUNT;i++){last_update[i]=copy->last_update[i];}
}

void BaseEntity::Clear(){
    id = 0;
    type = 0;
    if(name != nullptr){free(name);name=nullptr;}
    x=y=z=0;
    velocity={0,0,0};
    scale ={1,1,1};
    rotation = {0,0,0};

    if(phys_props != nullptr){free(phys_props);phys_props=nullptr;}
    if(move_props  != nullptr){free(move_props);move_props=nullptr;}
    //if(move_props != nullptr){free(move_props);move_props=nullptr;}
    if(phys_state != nullptr){free(phys_state);phys_state=nullptr;}
    if(move_state != nullptr){free(move_state);move_state=nullptr;}
    if(action_state != nullptr){free(action_state);action_state=nullptr;}
    if(colliders != nullptr){free(colliders);colliders=nullptr;}
    if(stats != nullptr){free(stats);stats=nullptr;}
    if(equip != nullptr){free(equip);equip=nullptr;}
    if(inventory != nullptr){free(inventory);inventory=nullptr;}
    if(char_data != nullptr){free(char_data);char_data=nullptr;}

    for(int i=0;i<COMPONENT_COUNT;i++){last_update[i]=0;}
}

bool BaseEntity::HasComponent(int component_id){
    switch (component_id){
        case  0:
        case  1:{return true;}
        case  2:{return phys_props != null;}
        case  3:{return move_props != null;} 
        case  4:{return false;} 
        case  5:{return phys_state != null;} 
        case  6:{return move_state != null;} 
        case  7:{return action_state != null;} 
        case  8:{return colliders != null;} 
        case  9:{return stats != null;} 
        case 10:{return equip != null;} 
        case 11:{return inventory != null;} 
        case 12:{return char_data != null;}  
        default: return false;
    }
}

int BaseEntity::ComponentSize(int component_id){
    switch (component_id){
        case  0:{return sizeof(int) + (wstr::len(name)+1)*sizeof(wchar);}
        case  1:{return sizeof(vec3)*4;}
        case  2:{return phys_props->SerializedLength();}
        case  3:{return move_props->SerializedLength();} 
        case  4:{return 0;} 
        case  5:{return phys_state->SerializedLength();} 
        case  6:{return move_state->SerializedLength();} 
        case  7:{return action_state->SerializedLength();} 
        case  8:{return colliders->SerializedLength();} 
        case  9:{return stats->SerializedLength();} 
        case 10:{return equip->SerializedLength();} 
        case 11:{return inventory->SerializedLength();} 
        case 12:{return char_data->SerializedLength();}  
        default: return 0;
    }
}

void BaseEntity::WriteComponent(int component_id,Serializer& dat){
    switch (component_id){
    case  0:{
        dat.PutInt(type);
        dat.PutWString(name);
        break;}
    case  1:{
        dat.PutFloat(x);dat.PutFloat(y);dat.PutFloat(z);
        dat.PutFloat(rotation.x);dat.PutFloat(rotation.y);dat.PutFloat(rotation.z);
        dat.PutFloat(scale.x);dat.PutFloat(scale.y);dat.PutFloat(scale.z);
        dat.PutFloat(velocity.x);dat.PutFloat(velocity.y);dat.PutFloat(velocity.z);
        break;}
    case  2:{phys_props->Write(dat);break;}
    case  3:{move_props->Write(dat);break;} 
    case  4:{break;} 
    case  5:{phys_state->Write(dat);break;} 
    case  6:{move_state->Write(dat);break;} 
    case  7:{action_state->Write(dat);break;} 
    case  8:{colliders->Write(dat);break;} 
    case  9:{stats->Write(dat);break;} 
    case 10:{equip->Write(dat);break;} 
    case 11:{inventory->Write(dat);break;} 
    case 12:{char_data->Write(dat);break;}  
    default: break;
    }
}

void BaseEntity::ReadComponent(int component_id,Deserializer& dat){
    switch (component_id){
    case  0:{
        type=dat.GetInt(); 
        name=dat.GetWString();
        break;}
    case  1:{
        x=dat.GetFloat();y=dat.GetFloat();z=dat.GetFloat();
        rotation={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        scale={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        velocity={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        break;}
    case  2:{if(!phys_props){phys_props = new PhysicsProperties();} phys_props->Read(dat);break;}
    case  3:{if(!move_props){move_props = new MoveProperties();} move_props->Read(dat);break;} 
    case  4:{break;} 
    case  5:{if(!phys_state){phys_state = new PhysicsState();} phys_state->Read(dat);break;} 
    case  6:{if(!move_state){move_state = new MovementState();} move_state->Read(dat);break;} 
    case  7:{if(!action_state){action_state = new ActionState();} action_state->Read(dat);break;} 
    case  8:{if(!colliders){colliders = new ColliderSet();} colliders->Read(dat);break;} 
    case  9:{if(!stats){stats = new StatBlock();} stats->Read(dat);break;} 
    case 10:{if(!equip){equip = new Equip();} equip->Read(dat);break;} 
    case 11:{if(!inventory){inventory = new Inventory();} inventory->Read(dat);break;} 
    case 12:{if(!char_data){char_data = new Character();} char_data->Read(dat);break;}  
    default: break;
    }
}



ClientEntity::ClientEntity(int entity_id):BaseEntity(entity_id){
    models=null;
    sprites=null;
    anim_state=null;
}
ClientEntity::~ClientEntity(){
    if(models != null){delete models;models=null;}
    if(sprites != null){delete sprites;sprites=null;}
    if(anim_state != null){delete anim_state;anim_state=null;}
}

ServerEntity::ServerEntity(int entity_id):BaseEntity(entity_id){
    delta_mask=0;
    player_id = 0;
    lastupdate_position = {x,y,z};
    lastupdate_velocity = velocity;
    persist=null;
}
ServerEntity::~ServerEntity(){}
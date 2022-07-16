#include <game_project_1/game/entity.hpp>

using namespace ComponentChunk;

vec3 BaseEntity::GetPos(){return {x,y,z};}
Location BaseEntity::GetLocation(){
    return {{x,y,z},rotation,scale};
}

float BaseEntity::GetTurnAngle(){return rotation.y;}

BaseEntity::BaseEntity(int entity_id){
    id =entity_id;
    entity_class_id =0;
    for(int i=0;i<COMPONENT_COUNT;i++){last_update[i]=0;}
    name=nullptr;
    x=y=z=0;
    velocity={0,0,0};
    scale ={1,1,1};
    rotation = {0,0,0};
  
    phys_data=null;
    colliders=null;
    movement=null;
    state=null;
    stats=null;
    equip=null;
    inventory=null;
    char_data=null;
}
BaseEntity::~BaseEntity(){
    Clear();
}

void BaseEntity::Clear(){
    if(name != nullptr){free(name);name=nullptr;}
    if(phys_data != nullptr){delete phys_data;phys_data=nullptr;}
    if(colliders != nullptr){delete colliders;colliders=nullptr;}
    if(movement != nullptr){delete movement;movement=nullptr;}
    if(state != nullptr){delete state;state=nullptr;}
    if(stats != nullptr){delete stats;stats=nullptr;}
    if(equip != nullptr){delete equip;equip=nullptr;}
    if(inventory != nullptr){delete inventory;inventory=nullptr;}
    if(char_data != nullptr){delete char_data;char_data=nullptr;}
}

int BaseEntity::SerializedLength(Mask delta_mask){
    Mask write_mask = delta_mask & AllExistingComponents();
    Mask chunk_mask = 1;
    int length = sizeof(Mask);
    for(int i=0;i<COMPONENT_COUNT;i++){
        if(chunk_mask & write_mask){
            length += sizeof(int) + ChunkLength((ID)i);
        }
        chunk_mask = chunk_mask << 1;
    }
    return length;
}
void BaseEntity::Serialize(Mask delta_mask,Serializer& dat){
    Mask write_mask = delta_mask & AllExistingComponents();
    Mask chunk_mask = 1;
    dat.PutInt(write_mask);
    for(int i=0;i<COMPONENT_COUNT;i++){
        if(chunk_mask & write_mask){
            int chunk_len = ChunkLength((ID)i);
            dat.PutInt(chunk_len);
            int start_place = dat.place;
            WriteChunk((ID)i,dat);
            if(start_place+chunk_len != dat.place){
                logger::exception("Serialization failed for entity ID %d, expected %d, was %d\n",id,chunk_len,dat.place-start_place);
            }
        }
        chunk_mask = chunk_mask << 1;
    }
}
void BaseEntity::Deserialize(Deserializer& dat,int timestamp){
    Mask read_mask = dat.GetInt();
    Mask chunk_mask = 1;
    for(int i=0;i<COMPONENT_COUNT;i++){
        if(chunk_mask & read_mask){
            if(last_update[i] <= timestamp){
                int chunk_len = dat.GetInt();//chunk_length
                int start_place = dat.place;
                ReadChunk((ID)i,dat);
                if(dat.place-start_place != chunk_len){
                    logger::exception("Deserialization failed for entity ID %d, expected %d, was %d\n",id,chunk_len,dat.place-start_place); 
                }
                last_update[i] = timestamp;
            }
            else{
                dat.place+=dat.GetInt();
            }
        }
        chunk_mask = chunk_mask << 1;
    }
}

void BaseEntity::Skip(Deserializer& dat){//discard all serialized chunks for this entity
    Mask read_mask = dat.GetInt();
    Mask chunk_mask = 1;
    for(int i=0;i<COMPONENT_COUNT;i++){
        if((chunk_mask & read_mask)){dat.place+=dat.GetInt();}
        chunk_mask = chunk_mask << 1;
    }
}

int BaseEntity::ChunkLength(ID ChunkID){
    switch (ChunkID){
    case POSITION:return sizeof(vec3)*4 + 1;
    case MOVE:return movement->SerializedLength();
    case STATE:return state->SerializedLength();
    case PHYS:return phys_data->SerializedLength();
    case COLLIDERS:return colliders->SerializedLength();
    case IDS:return sizeof(int) + (wstr::len(name)+1)*sizeof(wchar);
    case STATS:return stats->SerializedLength();
    case EQUIP:return equip->SerializedLength();
    case INVENTORY:return inventory->SerializedLength();
    case CHARACTER:return char_data->SerializedLength();
    default: break;
    }
    return 0;
}
void BaseEntity::WriteChunk(ID ChunkID,Serializer& dat){
    switch (ChunkID){
    case POSITION:{
        dat.PutFloat(x);dat.PutFloat(y);dat.PutFloat(z);
        dat.PutFloat(rotation.x);dat.PutFloat(rotation.y);dat.PutFloat(rotation.z);
        dat.PutFloat(scale.x);dat.PutFloat(scale.y);dat.PutFloat(scale.z);
        dat.PutFloat(velocity.x);dat.PutFloat(velocity.y);dat.PutFloat(velocity.z);
        byte is_midair=0;
        if(phys_data != null){is_midair = phys_data->is_midair?1:0;}
        dat.PutByte(is_midair);
        break;}
    case MOVE:{
        movement->Write(dat);
        break;}
    case STATE:{
        state->Write(dat);
        break;}
    case PHYS:{
        phys_data->Write(dat);
        break;}
    case COLLIDERS:{
        colliders->Write(dat);
        break;}
    case IDS:{
        dat.PutInt(entity_class_id);
        dat.PutWString(name);
        break;}
    case STATS:{
        stats->Write(dat);
        break;}
    case EQUIP:{
        equip->Write(dat);
        break;}
    case INVENTORY:{
        inventory->Write(dat);
        break;}
    case CHARACTER:{
        char_data->Write(dat);
        break;}
    default: break;
    }
}
void BaseEntity::ReadChunk(ID ChunkID,Deserializer& dat){
    switch (ChunkID){
    case POSITION:{
        x=dat.GetFloat();y=dat.GetFloat();z=dat.GetFloat();
        rotation={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        scale={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        velocity={dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
        bool midair = (dat.GetByte() > 0);
        if(phys_data != null){phys_data->is_midair = midair;}
        break;}
    case MOVE:{
        if(movement == null){movement = new MovementData();}
        movement->Read(dat);
        break;}
    case STATE:{
        if(state == null){state = new State();}
        state->Read(dat);
        break;}
    case PHYS:{
        if(phys_data == null){phys_data = new PhysBody();}
        phys_data->Read(dat);
        break;}
    case COLLIDERS:{
        if(colliders == null){colliders = new ColliderSet();}
        colliders->Read(dat);
        break;}
    case IDS:{
        entity_class_id=dat.GetInt(); 
        name=dat.GetWString();
        break;}
    case STATS:{
        if(stats == null){stats = new StatBlock();}
        stats->Read(dat);
        break;}
    case EQUIP:{
        if(equip== null){equip = new Equip();}
        equip->Read(dat);
        break;}
    case INVENTORY:{
        if(inventory == null){inventory = new Inventory();}
        inventory->Read(dat);
        break;}
    case CHARACTER:{
        if(char_data == null){char_data = new Character();}
        char_data->Read(dat);
        break;}
    default: break;
    }
}

Mask BaseEntity::AllExistingComponents(){//Mask needed to create all non-null components
    int mask = BASIC_COMPONENTS;
    if(movement != null){mask |=  1 << MOVE;}
    if(state != null){mask |= 1 << STATE;}
    if(phys_data != null){mask |= 1 << PHYS;}
    if(colliders != null){mask |= 1 << COLLIDERS;}
    if(stats != null){mask |= 1 << STATS;}
    if(equip != null){mask |= 1 << EQUIP;}
    if(inventory != null){mask |= 1 << INVENTORY;}
    if(char_data != null){mask |= 1 << CHARACTER;}
    return mask;
}

void BaseEntity::CopyFrom(BaseEntity* e){//Deep entity copy
    Clear();
    id=e->id;
    entity_class_id=e->entity_class_id;
    for(int i=0;i<COMPONENT_COUNT;i++){last_update[i]=e->last_update[i];}
    name = wstr::new_copy(e->name);
    x=e->x;y=e->y;z=e->z;
    velocity=e->velocity;
    scale=e->scale;
    rotation=e->rotation;

    if(e->phys_data != null){
        phys_data = new PhysBody();
        phys_data->Copy(e->phys_data);
    }
    if(e->colliders != null){
        colliders = new ColliderSet();
        colliders->Copy(e->colliders);
    }
    if(e->movement != null){
        movement = new MovementData();
        movement->Copy(e->movement);
    }
    if(e->state != null){
        state = new State();
        state->Copy(e->state);
    }
    if(e->stats != null){
        stats = new StatBlock();
        stats->Copy(e->stats);
    }
    if(e->equip != null){
        equip = new Equip();
        equip->Copy(e->equip);
    }
    if(e->inventory != null){
        inventory = new Inventory();
        inventory->Copy(e->inventory);
    }
    if(e->char_data != null){
        char_data = new Character();
        char_data->Copy(e->char_data);
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
vec3 ClientEntity::GetPos(){return {x,y,z};}
ServerEntity::ServerEntity(int entity_id):BaseEntity(entity_id){
    delta_mask=0;
    player_id = 0;
    lastupdate_position = {x,y,z};
    lastupdate_velocity = velocity;
    persist=null;
}
ServerEntity::~ServerEntity(){}



#include <game_project_1/core/entity.hpp>

ComponentMask::ComponentMask():ids(0){}
ComponentMask::ComponentMask(bitmask val):ids(0){ids=val;}
Entity ComponentMask::dummy = Entity(-1);

template<> int Entity::IdOf<Identity>()               {return 1;}
template<> int Entity::IdOf<PhysicsProperties>()      {return 2;}
template<> int Entity::IdOf<MovementProperties>()     {return 3;}
//template<> int Entity::IdOf<ActionProperties>()     {return 4;}
template<> int Entity::IdOf<PhysicsState>()           {return 5;}
template<> int Entity::IdOf<MovementState>()          {return 6;}
template<> int Entity::IdOf<ActionState>()            {return 7;}
template<> int Entity::IdOf<ColliderSet>()            {return 8;}
template<> int Entity::IdOf<StatBlock>()              {return 9;}
template<> int Entity::IdOf<Equip>()                  {return 10;}
template<> int Entity::IdOf<Inventory>()              {return 11;}
template<> int Entity::IdOf<CharacterInfo>()          {return 12;}
template<> int Entity::IdOf<Interactable>()           {return 13;}

void Entity::Add(int slot){
    switch(slot){
        case 0: components[slot] = new Identity();break;
        case 1: components[slot] = new PhysicsProperties();break;
        case 2: components[slot] = new MovementProperties();break;
        //case 3: components[slot] = new ActionProperties();break;
        case 4: components[slot] = new PhysicsState();break;
        case 5: components[slot] = new MovementState();break;
        case 6: components[slot] = new ActionState();break;
        case 7: components[slot] = new ColliderSet(this);break;
        case 8: components[slot] = new StatBlock();break;
        case 9: components[slot] = new Equip();break;
        case 10: components[slot] = new Inventory();break;
        case 11: components[slot] = new CharacterInfo();break;
        case 12: components[slot] = new Interactable();break;
        default: 
            logger::warn("Could not add component ID %d to entity ID %d, id is invalid\n",Id(slot),id);
        break;
    }
}

Entity::Entity(int eid){
    id=eid;
    x=0;y=0;z=0;
    rotation={0,0,0};
    scale={1,1,1};
    for(int i=0;i<component_slots;i++){components[i]=nullptr;}
}
Entity::~Entity(){Clear();}

void Entity::Clear(){
    for(int i=0;i<component_slots;i++){
        if(components[i]!=nullptr){
            delete components[i];
            components[i] = nullptr;
        }
    }
}

void Entity::CloneTo(Entity* copy){
    copy->id = id;
    copy->x = x;
    copy->y = y;    
    copy->z = z;
    copy->rotation = rotation;
    copy->scale = scale;
    copy->velocity = velocity;
    for(int i=0;i<component_slots;i++){
        if(components[i]!=null){copy->components[i]=components[i]->Clone(ComponentParentContext{copy});};
    }
}

bitmask Entity::AllExistingComponents(){
    bitmask id_mask(1);
    for(int slot=0;slot<component_slots;slot++){if(components[slot] != null){id_mask.set(Id(slot));}}
    return id_mask;
}

bool Entity::HasComponents(bitmask id_mask){
    for(int slot=0;slot<component_slots;slot++){if(id_mask.get_bit(Id(slot)) && components[slot] == null){return false;}}
    return true;
}

bool Entity::HasAll(ComponentMask mask){return HasComponents(mask.Mask());}

int  Entity::SerializedLength(bitmask mask){
    int length = sizeof(bitmask);
    if(mask.get_bit(0)){ length += Transform::SerializedLength() + sizeof(vec3);}
    for(int slot=0;slot<component_slots;slot++){
        if(mask.get_bit(Id(slot)) && components[slot] != null){length += (sizeof(int) + components[slot]->SerializedLength());}
    }
    return length;
}

void Entity::Write(Serializer& dat, bitmask id_mask){
    bitmask write_mask = AllExistingComponents();write_mask.and_with(id_mask);
    dat.PutInt(write_mask.val);
    if(id_mask.get_bit(0)){
        dat.WriteTransform(this);
        dat.PutVec3(velocity);
    }
    for(int slot=0;slot<component_slots;slot++){
        if(id_mask.get_bit(Id(slot)) && components[slot] != null){components[slot]->Write(dat);}
    }
}

void Entity::Read(Deserializer& dat, int timestamp){
    bitmask read_mask(dat.GetInt());
    if(read_mask.get_bit(0) ){
        dat.ReadTransform(this);
        velocity=dat.GetVec3();
    }
    for(int slot=0;slot<component_slots;slot++){
        if(read_mask.get_bit(Id(slot))){
            if(components[slot]==null){Add(slot);}
            if(components[slot]->last_updated < timestamp){
                components[slot]->Read(dat);
                components[slot]->MarkUpdated(timestamp);
            }
            else{
                Component* skip = components[slot]->Clone(ComponentParentContext{this});
                skip->Read(dat);
                delete skip;
            }
        }
    }
}

void Entity::SkipRead(Deserializer& dat){
    //TODO: less lazy memory mgmt
    Entity temp(-1);
    temp.Read(dat,0);
}

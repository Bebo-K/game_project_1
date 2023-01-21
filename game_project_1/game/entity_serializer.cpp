#include <game_project_1/game/entity_serializer.hpp>

using namespace EntitySerializer;


void EntitySerializer::Serialize(BaseEntity* e,ComponentMask delta_mask,Serializer& dat){
    ComponentMask write_mask = delta_mask & EntitySerializer::AllExistingComponents(e);
    ComponentMask chunk_mask = 1;
    dat.PutInt(write_mask);
    for(int i=0;i<BaseEntity::COMPONENT_COUNT;i++){
        if(chunk_mask & write_mask){
            int len = e->ComponentSize(i);
            dat.PutInt(len);
            int start_place = dat.place;
            e->WriteComponent(i,dat);
            if(start_place+len != dat.place){
                logger::exception("Serialization failed for entity ID %d, expected %d, was %d\n",e->id,len,dat.place-start_place);
            }
        }
        chunk_mask = chunk_mask << 1;
    }
}

void EntitySerializer::Deserialize(BaseEntity* e,Deserializer& dat,int timestamp){
    ComponentMask read_mask = dat.GetInt();
    ComponentMask chunk_mask = 1;
    for(int i=0;i<BaseEntity::COMPONENT_COUNT;i++){
        if(chunk_mask & read_mask){
            if(e->last_update[i] <= timestamp){
                int len = dat.GetInt();//chunk_length
                int start_place = dat.place;
                e->ReadComponent(i,dat);
                if(dat.place-start_place != len){
                    logger::exception("Deserialization failed for entity ID %d, expected %d, was %d\n",e->id,len,dat.place-start_place); 
                }
                e->last_update[i] = timestamp;
            }
            else{//skip, it's older data
                dat.place+=dat.GetInt();
            }
        }
        chunk_mask = chunk_mask << 1;
    } 
}

ComponentMask EntitySerializer::AllExistingComponents(BaseEntity* e){
    ComponentMask mask = GUARENTEED_COMPONENTS;
    
    if(e->phys_props != nullptr){mask |= (1 << 2);}
    if(e->move_props  != nullptr){mask |= (1 << 3);}
    //if(move_props != nullptr){mask |= (1 << 4);}
    if(e->phys_state != nullptr){mask |= (1 << 5);}
    if(e->move_state != nullptr){mask |= (1 << 6);}
    if(e->action_state != nullptr){mask |= (1 << 7);}
    if(e->colliders != nullptr){mask |= (1 << 8);}
    if(e->stats != nullptr){mask |= (1 << 9);}
    if(e->equip != nullptr){mask |= (1 << 10);}
    if(e->inventory != nullptr){mask |= (1 << 11);}
    if(e->char_data != nullptr){mask |= (1 << 12);}
    
    return mask;
}

int EntitySerializer::SerializedLength(BaseEntity* e,ComponentMask mask){
    int length = sizeof(ComponentMask);
    ComponentMask write_mask = mask & EntitySerializer::AllExistingComponents(e);
    ComponentMask chunk_mask = 1;
    for(int i=0;i<BaseEntity::COMPONENT_COUNT;i++){
        if(chunk_mask & write_mask){ length += sizeof(int) + e->ComponentSize(i);}
        chunk_mask = chunk_mask << 1;
    }
    return length;
}

void EntitySerializer::Skip(Deserializer& dat){
    ComponentMask read_mask = dat.GetInt();
    ComponentMask chunk_mask = 1;
    for(int i=0;i<BaseEntity::COMPONENT_COUNT;i++){
        if((chunk_mask & read_mask)){dat.place+=dat.GetInt();}
        chunk_mask = chunk_mask << 1;
    }
}
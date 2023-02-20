#include <game_project_1/game/entity_serializer.hpp>

using namespace EntitySerializer;


void EntitySerializer::Serialize(SharedEntity* e,bitmask delta_mask,Serializer& dat){
    bitmask write_mask = delta_mask;
    write_mask.val &= EntitySerializer::AllExistingComponents(e).val;
    dat.PutInt(write_mask.val);
    for(int i=0;i<SharedEntity::COMPONENT_COUNT;i++){
        if(write_mask.get_bit(i)){
            int chunk_len = e->components[i]->SerializedLength();
            int start_place = dat.place;
            dat.PutInt(chunk_len);
            e->components[i]->Write(dat);
            if(start_place+chunk_len != dat.place){
                logger::exception("Serialization failed for entity ID %d, expected %d, was %d\n",e->id,len,dat.place-start_place);
            }
        }
    }
}

void EntitySerializer::Deserialize(SharedEntity* e,Deserializer& dat,int timestamp){
    bitmask components(dat.GetInt());
    for(int i=0;i<SharedEntity::COMPONENT_COUNT;i++){
        if(components.get_bit(i)){
            if(e->components[i] == null){e->NewComponent(i);}
            if(e->components[i]->last_update <= timestamp){
                int len = dat.GetInt();//chunk_length
                int start_place = dat.place;
                e->components[i]->Read(dat);
                e->components[i]->last_update = timestamp;
                if(dat.place-start_place != len){
                    logger::exception("Deserialization failed for entity ID %d, expected %d, was %d\n",e->id,len,dat.place-start_place); 
                }
            }
            else{//skip, it's older data
                dat.place+=dat.GetInt();
            }
        }
    }
}

int EntitySerializer::SerializedLength(SharedEntity* e,bitmask mask){
    int length = sizeof(bitmask);
    for(int i=0;i<SharedEntity::COMPONENT_COUNT;i++){
        if(mask.get_bit(i) && e->components[i] != null){length += (sizeof(int) + e->components[i]->SerializedLength());}
    }
    return length;
}

void EntitySerializer::Skip(Deserializer& dat){
    bitmask components(dat.GetInt());
    for(int i=0;i<SharedEntity::COMPONENT_COUNT;i++){
        if(components.get_bit(i)){
            dat.place += dat.GetInt();
        }
    }
}
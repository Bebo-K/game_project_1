#ifndef ENTITY_SERIALIZER_H
#define ENTITY_SERIALIZER_H


#include <game_project_1/game/entity.hpp>


typedef int ComponentMask;

namespace EntitySerializer{
    const static ComponentMask GUARENTEED_COMPONENTS = 3;//components that exist for every entity, Identity(0) and Position(1)

    ComponentMask AllExistingComponents(BaseEntity* e);//Mask of all non-null components for an entity
    void Serialize(BaseEntity* e, ComponentMask delta_mask, Serializer& dat);
    void Deserialize(BaseEntity* e, Deserializer& dat, int timestamp);

    int SerializedLength(BaseEntity* e, ComponentMask mask);
    int ComponentLength(BaseEntity* e, int component_id);

    
    void WriteComponent(BaseEntity* e, Serializer& dat, int component_id);
    void ReadComponent(BaseEntity* e, Deserializer& dat, int component_id);

    void Skip(Deserializer& dat);
    void Copy(BaseEntity* from, BaseEntity* to);
};





#endif
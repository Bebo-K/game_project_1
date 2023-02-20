#ifndef ENTITY_SERIALIZER_H
#define ENTITY_SERIALIZER_H


#include <game_project_1/game/entity.hpp>

namespace EntitySerializer{
    void Serialize(SharedEntity* e, bitmask delta_mask, Serializer& dat);
    void Deserialize(SharedEntity* e, Deserializer& dat, int timestamp);
    int SerializedLength(SharedEntity* e, bitmask mask);
    void Skip(Deserializer& dat);
};





#endif
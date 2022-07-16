#ifndef CLIENT_SERIALIZER_H
#define CLIENT_SERIALIZER_H

#include "game_project_1/client/client.hpp"
#include "game_project_1/net/payload.hpp"
#include "game_project_1/game/entity.hpp"

namespace ClientSerializer{

    void Init(Client* c);
    void Free();
    Payload ClientDelta(ClientEntity* entity);

    void DeserializeNewScene(ClientScene* scene, Payload payload);
    void DeserializeServerDelta(ClientScene* scene, Payload payload);
    void DeserializeSpawnEntities(ClientScene* scene, Payload payload);
    void DeserializeDespawnEntities(ClientScene* scene, Payload payload);


};



#endif
#ifndef SERVER_SERIALIZER_H
#define SERVER_SERIALIZER_H

#include "game_project_1/net/payload.hpp"
#include "game_project_1/server/server_scene.hpp"
#include "game_project_1/game/player.hpp"

namespace ServerSerializer{

    Payload FullScene(Player* player_for,ServerScene* scene);

    Payload SceneNewEntities(ServerScene* scene);
    Payload SceneDeletedEntities(ServerScene* scene);
    Payload SceneDelta(ServerScene* scene);

    void DeserializeClientDelta(Player* player_from,ServerScene* scene,Payload p);
};


#endif

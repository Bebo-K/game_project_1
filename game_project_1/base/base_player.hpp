#ifndef BASE_PAK_PLAYER_H
#define BASE_PAK_PLAYER_H

#include <game_project_1/game/entity.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

namespace EntityClass{
    const int Humanoid = 1;

};

void ClientInit_BasePak1();
void ServerInit_BasePak1();

void HumanoidClientBuilder(ClientEntity* entity, ClientScene* scene);
void HumanoidServerBuilder(ServerEntity* entity, ServerScene* scene);


#endif
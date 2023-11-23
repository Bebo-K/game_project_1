#ifndef INTERACT_H
#define INTERACT_H


#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

namespace Interact{
	
    ClientEntity* ClientPollInteract(ClientEntity* e,ClientScene* s);
    //bool ServerVerifyInteract(ServerEntity* e,ServerEntity* e2,ServerScene* s);

    void ClientTryInteract(ClientEntity* e,ClientEntity* e2,ClientScene* s);
    void ServerTryInteract(ServerEntity* e,ServerEntity* e2,ServerScene* s);

};



#endif
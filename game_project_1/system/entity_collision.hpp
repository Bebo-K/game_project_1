#ifndef ENTITY_COLLISION_H
#define ENTITY_COLLISION_H

#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_signal.hpp>

namespace EntityCollision{

	void ClientFrameUpdate(ClientEntity* e,ClientScene* s, float delta);
	void ServerFrameUpdate(ServerEntity* e,ServerScene* s, float delta);

};



#endif
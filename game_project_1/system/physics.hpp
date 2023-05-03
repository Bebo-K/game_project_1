#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <game_project_1/core/entity.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

namespace Physics{

	extern float GRAVITY_TERMINAL;
	extern float GRAVITY_ACCEL;

	void ClientFrame(ClientEntity* e,ClientScene* s,float delta);
	void ServerFrame(ServerEntity* e,ServerScene* s,float delta);
};



#endif
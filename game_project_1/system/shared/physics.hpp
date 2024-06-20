#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <game_project_1/types/timestep.hpp>
#include <game_project_1/core/client_entity.hpp>
#include <game_project_1/core/server_entity.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

namespace Physics{

	extern float GRAVITY_TERMINAL;
	extern float GRAVITY_ACCEL;

	void ClientUpdate(ClientEntity* e,ClientScene* s,Timestep delta);
	void ServerUpdate(ServerEntity* e,ServerScene* s,Timestep delta);
};



#endif
#ifndef LEVEL_COLLISION_H
#define LEVEL_COLLISION_H

#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/component/shared/identity.hpp>


namespace LevelCollision {

	typedef void (*ClientHandler)(ClientEntity*,CollisionResult,ClientScene*);
	typedef void (*ServerHandler)(ServerEntity*,CollisionResult,ServerScene*);

    const int	VELOCITY_STEPS	= 4;
	const bool ENFORCE_BOUNDS 	= true;//for debug
	
	//void ClientInit();
	//void ServerInit();

	void ClientFrame(ClientEntity* e,ClientScene* s,float delta);
	void ServerFrame(ServerEntity* e,ServerScene* s,float delta);
	
	void RunCollisionStep(Entity* e, List<MeshCollider>* meshes,float step_delta,CollisionResult* list);
	vec3 HandleSolidStepCollisions(Entity* e, vec3 step_movement,CollisionResult* list);
	
	void RegisterClientLevelCollisionHandler(CollisionType coll_type, ClientHandler client_callback);
	void RegisterServerLevelCollisionHandler(CollisionType coll_type, ServerHandler server_callback);
};


#endif
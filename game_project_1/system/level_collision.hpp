#ifndef LEVEL_COLLISION_H
#define LEVEL_COLLISION_H

#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

namespace LevelCollision {

	typedef void (*ClientHandlerCallback)(ClientEntity*,CollisionResult,ClientScene*);
	typedef void (*ServerHandlerCallback)(ServerEntity*,CollisionResult,ServerScene*);

    const int	VELOCITY_STEPS	= 4;
	const bool ENFORCE_BOUNDS 	= true;//for debug
	
	//void ClientInit();
	//void ServerInit();

	void ClientFrame(ClientEntity* e,ClientScene* s,float delta);
	void ServerFrame(ServerEntity* e,ServerScene* s,float delta);

	void BaseFrame(BaseEntity* e,Array<MeshCollider>* meshes,float delta,bool server);
	
	void RunCollisionStep(BaseEntity* e, Array<MeshCollider> meshes,float step_delta,CollisionResult* list);
	vec3 HandleSolidStepCollisions(BaseEntity* e, vec3 step_movement,CollisionResult* list);
	
	void RegisterClientEntityClassCallbacks(EntityClass type,ClientHandlerCallback client_callback);
	void RegisterServerEntityClassCallbacks(EntityClass type,ServerHandlerCallback server_callback);
};


#endif
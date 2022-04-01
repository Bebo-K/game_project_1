#ifndef LEVEL_COLLISION_H
#define LEVEL_COLLISION_H

#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>

namespace LevelCollision {

    const int	VELOCITY_STEPS	= 4;
	const bool ENFORCE_BOUNDS 	= true;//for debug
	
	struct StepInfo{
		vec3 shunt;
		vec3 movement;
		vec3 velocity;
		CollisionList* floor_entry;//so we don't shunt too much for overlapping floors

		StepInfo(vec3 initial_movement);
	};
	
	void ClientFrameUpdate(ClientEntity* e,ClientScene* s, float delta);
	void ServerFrameUpdate(ServerEntity* e,ServerScene* s, float delta);
	
	CollisionList* RunCollisionStep(PhysBody* b, CollisionMesh* meshes,int mesh_count,float step_delta);
	void HandleSolidStepCollisions(PhysBody* b, CollisionMesh* meshes,int mesh_count,StepInfo* step, CollisionList* collisions);

	void HandleCollisionResults(PhysBody* b, CollisionMesh* meshes,int mesh_count, CollisionList* collisions);

	CollisionList* GrabCollisionSlot();
	void FreeCollisionSlots();
};


#endif
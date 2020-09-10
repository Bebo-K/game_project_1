#ifndef LEVEL_COLLISION_H
#define LEVEL_COLLISION_H

#include "../../phys/collision_types.h"
#include "../scene.h"

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
	
	void FrameUpdate(Entity* e,Scene* scene, float delta);
	
	CollisionList* RunCollisionStep(Entity* e, Scene* s,float step_delta);
	void HandleSolidStepCollisions(Entity* e,Scene* s,StepInfo* step, CollisionList* collisions);

	void HandleCollisionResults(Entity* e,Scene* s, CollisionList* collisions);

	CollisionList* GrabCollisionSlot();
	void FreeCollisionSlots();
		
};


#endif
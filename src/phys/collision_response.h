#ifndef COLLISION_RESPONSE_H
#define COLLISION_RESPONSE_H

#include "../game/scene.h"
#include "collision_types.h"

namespace CollisionResponse
{

void HandleCollision(Entity* e,Scene* s, CollisionList* collision);

void HandleSolid(Entity* e,Scene* s,CollisionList* collision);

void HandleWater(Entity* e,Scene* s,CollisionList* collision);

void HandleDamage(Entity* e,Scene* s,CollisionList* collision);

void HandleDeath(Entity* e,Scene* s,CollisionList* collision);

void HandleExit(Entity* e,Scene* s,CollisionList* collision);

void HandleTrigger(Entity* e,Scene* s,CollisionList* collision);

};







#endif
#ifndef COLLISION_RESPONSE_H
#define COLLISION_RESPONSE_H

#include <game_project_1/game/scene.hpp>
#include <game_project_1/phys/collision_types.hpp>

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
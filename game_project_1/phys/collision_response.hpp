#ifndef COLLISION_RESPONSE_H
#define COLLISION_RESPONSE_H

#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/server/server_scene.hpp>
#include <game_project_1/phys/collision_types.hpp>

namespace ClientCollisionResponse
{

void HandleCollision(ClientEntity* e,ClientScene* s, CollisionList* collision);
void HandleSolid(ClientEntity* e,ClientScene* s,CollisionList* collision);
void HandleWater(ClientEntity* e,ClientScene* s,CollisionList* collision);
void HandleDamage(ClientEntity* e,ClientScene* s,CollisionList* collision);
void HandleDeath(ClientEntity* e,ClientScene* s,CollisionList* collision);
void HandleExit(ClientEntity* e,ClientScene* s,CollisionList* collision);
void HandleTrigger(ClientEntity* e,ClientScene* s,CollisionList* collision);
void HandleEntity(ClientEntity* e1,ClientEntity* e2,ClientScene* s,CapsuleCollider* c1,CapsuleCollider* c2,vec3 point);

};

namespace ServerCollisionResponse
{

void HandleCollision(ServerEntity* e,ServerScene* s, CollisionList* collision);
void HandleSolid(ServerEntity* e,ServerScene* s,CollisionList* collision);
void HandleWater(ServerEntity* e,ServerScene* s,CollisionList* collision);
void HandleDamage(ServerEntity* e,ServerScene* s,CollisionList* collision);
void HandleDeath(ServerEntity* e,ServerScene* s,CollisionList* collision);
void HandleExit(ServerEntity* e,ServerScene* s,CollisionList* collision);
void HandleTrigger(ServerEntity* e,ServerScene* s,CollisionList* collision);
void HandleEntity(ServerEntity* e1,ServerEntity* e2,ServerScene* s,CapsuleCollider* c1,CapsuleCollider* c2,vec3 point);

};







#endif